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

	bool
	OpcUaModbusClientGroup::startup(
		const std::string& namespaceName,
		uint32_t namespaceIndex,
		RegisterGroupConfig::SPtr registerGroupConfig,
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		OpcUaStackCore::OpcUaNodeId& rootNodeId,
		ModbusTCPClient::SPtr modbusTCPClient
	)
	{
		bool rc = true;

		namespaceName_ = namespaceName;
		namespaceIndex_ = namespaceIndex;
		registerGroupConfig_ = registerGroupConfig;
		rootNodeId_ = rootNodeId;
		applicationServiceIf_ = applicationServiceIf;
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

		if (!createNodeInstance.query(applicationServiceIf_)) {
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
				applicationServiceIf_,
				groupNodeId_,
				nullptr,
				registerGroupConfig->type()
			);
			if (!rc) {
			   	Log(Error, "create opc ua register value error")
			   		.parameter("Name", registerConfig->name());
			   	return true;
			}

			modbusValueVec_.push_back(value);
		}

		// Startup read timer
#if 0
		slotTimerElement_ = boost::make_shared<SlotTimerElement>();
		slotTimerElement_->timeoutCallback(boost::bind(&OpcUaModbusClientGroup::readLoop, this));
		slotTimerElement_->expireTime(boost::posix_time::microsec_clock::local_time(), 1111);
		ioThread_->slotTimer()->start(slotTimerElement_);
#endif

		return true;
	}

	bool
	OpcUaModbusClientGroup::shutdown(void)
	{
		// Stop read timer

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

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete group node instance error")
				.parameter("GroupNodeId", groupNodeId_)
				.parameter("Name", registerGroupConfig_->groupName());
			return false;
		}

		return true;
	}

	void
	OpcUaModbusClientGroup::readLoop(void)
	{
		;
	}

}

