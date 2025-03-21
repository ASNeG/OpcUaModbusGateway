/*
   Copyright 2025 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/make_shared.hpp>

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackServer/ServiceSetApplication/BrowsePathToNodeId.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateVariableInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateNodeInstance.h"

#include "OpcUaModbusGateway/OpcUa/OpcUaModbusClientGroup.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

	uint32_t OpcUaModbusClientGroup::id_ = 10000;

	OpcUaModbusClientGroup::OpcUaModbusClientGroup(void)
	{
	}

	OpcUaModbusClientGroup::~OpcUaModbusClientGroup(void)
	{
	}

	void
	OpcUaModbusClientGroup::maxRegisterInRequest(uint32_t maxRegisterInRequest)
	{
		maxRegisterInRequest_ = maxRegisterInRequest;
	}

	bool
	OpcUaModbusClientGroup::startup(
		const std::string& namespaceName,
		uint32_t namespaceIndex,
		RegisterGroupConfig::SPtr registerGroupConfig,
		OpcUaStackServer::ApplicationIf* applicationIf,
		OpcUaStackCore::OpcUaNodeId& rootNodeId,
		ModbusTCPClient::SPtr modbusTCPClient
	)
	{
		bool rc = true;

		ioThread_ = applicationIf->applicationThreadPool();
		namespaceName_ = namespaceName;
		namespaceIndex_ = namespaceIndex;
		registerGroupConfig_ = registerGroupConfig;
		rootNodeId_ = rootNodeId;
		applicationIf_ = applicationIf;
		modbusTCPClient_ = modbusTCPClient;

		// Create group folder instance
		id_++;
		groupNodeId_.set((uint32_t)id_, namespaceIndex_);
		CreateNodeInstance createNodeInstance(
			registerGroupConfig_->groupName(),				// name
			NodeClass::EnumObject,							// node class
			rootNodeId,										// parent node id
			groupNodeId_,										// node id
			OpcUaLocalizedText("en", registerGroupConfig_->groupName()),// display name
			OpcUaQualifiedName(registerGroupConfig_->groupName(), 1),	// browse name
			OpcUaNodeId((uint32_t)OpcUaId_Organizes),		// reference type id
			OpcUaNodeId((uint32_t)OpcUaId_BaseObjectType)   // type node id
		);

		if (!createNodeInstance.query(&applicationIf_->service())) {
			Log(Error, "create register group node instance error")
				.parameter("DisplayName", registerGroupConfig_->groupName());
			return false;
		}

		// Create register group instances
		for (auto registerConfig : registerGroupConfig_->registerConfigVec()) {
			auto value = std::make_shared<OpcUaModbusValue>();
			auto rc =  value->startup(
			   	namespaceName,
				rootNodeId_.namespaceIndex(),
				registerConfig,
				&applicationIf_->service(),
				groupNodeId_,
				nullptr,
				registerGroupConfig->type()
			);
			if (!rc) {
			   	Log(Error, "create opc ua register value error")
			   		.parameter("Name", registerConfig->name());
			   	return true;
			}

			// Register write callbacks
			rc = value->useWriteDataValue();
			if (!rc) {
			   	Log(Error, "register write callback error")
			   		.parameter("Name", registerConfig->name());
			   	return true;
			}

			modbusValueVec_.push_back(value);
		}

		// Init all read jobs
		initReadJobs();

		// Startup write timer
		writeSlotTimer_ = boost::make_shared<SlotTimerElement>();
		writeSlotTimer_->timeoutCallback(boost::bind(&OpcUaModbusClientGroup::writeLoop, this));
		writeSlotTimer_->expireTime(boost::posix_time::microsec_clock::local_time(), registerGroupConfig->writeInterval());
		ioThread_->slotTimer()->start(writeSlotTimer_);

		// Startup read timer
		readSlotTimer_ = boost::make_shared<SlotTimerElement>();
		readSlotTimer_->timeoutCallback(boost::bind(&OpcUaModbusClientGroup::readLoop, this));
		readSlotTimer_->expireTime(boost::posix_time::microsec_clock::local_time(), registerGroupConfig->readInterval());
		ioThread_->slotTimer()->start(readSlotTimer_);

		return true;
	}

	bool
	OpcUaModbusClientGroup::shutdown(void)
	{
		// Stop read timer
		ioThread_->slotTimer()->stop(readSlotTimer_);
		readSlotTimer_.reset();

		// Stop write timer
		ioThread_->slotTimer()->stop(writeSlotTimer_);
		writeSlotTimer_.reset();

		// Shutdown modbus values
		for (auto modbusValue : modbusValueVec_) {
			auto rc = modbusValue->shutdown();
			if (!rc) {
				Log(Error, "delete value node instance error");
				return false;
			}
		}

		// Remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(groupNodeId_);

		if (!deleteNodeInstance.query(&applicationIf_->service())) {
			Log(Error, "delete group node instance error")
				.parameter("GroupNodeId", groupNodeId_)
				.parameter("Name", registerGroupConfig_->groupName());
			return false;
		}

		return true;
	}

	void
	OpcUaModbusClientGroup::initReadJobs(void)
	{
		// Sort modbus values by address
		OpcUaModbusValue::Vec modbusValueVec = modbusValueVec_;
		std::sort(
			modbusValueVec.begin(),
			modbusValueVec.end(),
			[](OpcUaModbusValue::SPtr& a, OpcUaModbusValue::SPtr& b) {
				return a->registerConfig()->address() < b->registerConfig()->address();
			}
		);

		// Create read jobs
		RegisterJob::SPtr registerJob = nullptr;
		for (auto modbusValue : modbusValueVec) {

			// Check maximum number of register in read request
			if (registerJob != nullptr && registerJob->modbusValueVec_.size() >= maxRegisterInRequest_) {
				registerJob = nullptr;
			}

			// Create new job
			if (registerJob == nullptr) {
				registerJob = std::make_shared<RegisterJob>();
				registerJob->startingAddress_ = modbusValue->registerConfig()->address();
				registerJob->actAddress_ = modbusValue->registerConfig()->address();
				registerJob->modbusValueVec_.push_back(modbusValue);
				readRegisterJobs_.push_back(registerJob);
			}
			else {
				// Add modbus value to existing job - The registers must be connected
				// in ascending order
				if (registerJob->actAddress_ + 1 == modbusValue->registerConfig()->address()) {
					registerJob->actAddress_ = modbusValue->registerConfig()->address();
					registerJob->modbusValueVec_.push_back(modbusValue);
				}

				// Create new job - There is a gap in the list of registers.
				else {
					registerJob = std::make_shared<RegisterJob>();
					registerJob->startingAddress_ = modbusValue->registerConfig()->address();
					registerJob->actAddress_ = modbusValue->registerConfig()->address();
					registerJob->modbusValueVec_.push_back(modbusValue);
					readRegisterJobs_.push_back(registerJob);
				}
			}
		}
	}

	void
	OpcUaModbusClientGroup::readCoils(void)
	{
		// Don't block this function

		for (auto job : readRegisterJobs_) {
			modbusTCPClient_->readCoils( // asynchronous call
				job->startingAddress_,
				job->modbusValueVec_.size(),
				[this, job](uint32_t errorCode, std::vector<bool>& coilStatus) {
					if (errorCode != 0) {
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(BadCommunicationError, false);
						}
					}
					else {
						uint16_t idx = 0;
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(Success, coilStatus[idx]);
							idx++;
						}
					}
				}
			);
		}
	}

	void
	OpcUaModbusClientGroup::readInputs(void)
	{
		// Don't block this function

		for (auto job : readRegisterJobs_) {
			modbusTCPClient_->readDiscreteInputs( // asynchronous call
				job->startingAddress_,
				job->modbusValueVec_.size(),
				[this, job](uint32_t errorCode, std::vector<bool>& inputStatus) {
					if (errorCode != 0) {
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(BadCommunicationError, false);
						}
					}
					else {
						uint16_t idx = 0;
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(Success, inputStatus[idx]);
							idx++;
						}
					}
				}
			);
		}
	}

	void
	OpcUaModbusClientGroup::readHoldingRegisters(void)
	{
		// Don't block this function

		for (auto job : readRegisterJobs_) {
			modbusTCPClient_->readHoldingRegisters( // asynchronous call
				job->startingAddress_,
				job->modbusValueVec_.size(),
				[this, job](uint32_t errorCode, std::vector<uint16_t>& holdingRegisters) {
					if (errorCode != 0) {
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(BadCommunicationError, false);
						}
					}
					else {
						uint16_t idx = 0;
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(Success, holdingRegisters[idx]);
							idx++;
						}
					}
				}
			);
		}
	}

	void
	OpcUaModbusClientGroup::readInputRegisters(void)
	{
		// Don't block this function

		for (auto job : readRegisterJobs_) {
			modbusTCPClient_->readInputRegisters( // asynchronous call
				job->startingAddress_,
				job->modbusValueVec_.size(),
				[this, job](uint32_t errorCode, std::vector<uint16_t>& inputRegisters) {
					if (errorCode != 0) {
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(BadCommunicationError, false);
						}
					}
					else {
						uint16_t idx = 0;
						for (auto modbusValue : job->modbusValueVec_) {
							modbusValue->setDataValue(Success, inputRegisters[idx]);
							idx++;
						}
					}
				}
			);
		}
	}

	void
	OpcUaModbusClientGroup::readLoop(void)
	{
		// Check modbus type
		switch(registerGroupConfig_->type()) {
			case RegisterGroupConfig::ModbusGroupType::Coil:
				readCoils();
				break;
			case RegisterGroupConfig::ModbusGroupType::Input:
				readInputs();
				break;
			case RegisterGroupConfig::ModbusGroupType::HoldingRegister:
				readHoldingRegisters();
				break;
			case RegisterGroupConfig::ModbusGroupType::InputRegister:
				readInputRegisters();
				break;
		}
	}

	void
	OpcUaModbusClientGroup::writeCoils(void)
	{
		// Don't block this function

		bool rc;
		bool value;
		for (auto modbusValue : modbusValueVec_) {
			// Get write value if exist
			rc = modbusValue->getWriteDataValue(value);
			if (!rc) {
				continue;
			}

			// Send write coil to modbus slave
			uint16_t address = modbusValue->registerConfig()->address();
			modbusTCPClient_->writeSingleCoil( // asynchronous call
				address,
				value,
				[this, address](uint32_t errorCode) {
					if (errorCode != 0) {
						Log(Error, "write single coil error")
							.parameter("GroupName", registerGroupConfig_->groupName())
							.parameter("Address", address)
							.parameter("ErrorCode", errorCode);
					}
				}
			);
		}
	}

	void
	OpcUaModbusClientGroup::writeHoldingRegisters(void)
	{
		// Don't block this function

		bool rc;
		uint16_t value;
		for (auto modbusValue : modbusValueVec_) {
			// Get write value if exist
			rc = modbusValue->getWriteDataValue(value);
			if (!rc) {
				continue;
			}

			// Send write holding registers to modbus slave
			uint16_t address = modbusValue->registerConfig()->address();
			modbusTCPClient_->writeSingleHoldingRegister( // asynchronous call
				address,
				value,
				[this, address](uint32_t errorCode) {
					if (errorCode != 0) {
						Log(Error, "write single holding register error")
							.parameter("GroupName", registerGroupConfig_->groupName())
							.parameter("Address", address)
							.parameter("ErrorCode", errorCode);
					}
				}
			);
		}
	}

	void
	OpcUaModbusClientGroup::writeLoop(void)
	{
		// Check modbus type
		switch(registerGroupConfig_->type()) {
			case RegisterGroupConfig::ModbusGroupType::Coil:
				writeCoils();
				break;
			case RegisterGroupConfig::ModbusGroupType::HoldingRegister:
				writeHoldingRegisters();
				break;
		}
	}

}

