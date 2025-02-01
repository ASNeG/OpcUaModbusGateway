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

#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateVariableInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"

#include "OpcUaModbusGateway/OpcUa/OpcUaModbusValue.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

	uint32_t OpcUaModbusValue::id_ = 11000;

	OpcUaModbusValue::OpcUaModbusValue(void)
	{
	}

	OpcUaModbusValue::~OpcUaModbusValue(void)
	{
	}

	bool
	OpcUaModbusValue::startup(
		const std::string& namespaceName,
		uint32_t namespaceIndex,
		RegisterConfig::SPtr registerConfig,
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		OpcUaStackCore::OpcUaNodeId& rootNodeId
	)
	{
		OpcUaNodeId nodeId;
		OpcUaDataValue dataValue;
		OpcUaDateTime now(boost::posix_time::microsec_clock::universal_time());

		// Set parameter
		namespaceName_ = namespaceName;
		namespaceIndex_ = namespaceIndex;
		registerConfig_ = registerConfig;
		applicationServiceIf_ = applicationServiceIf;
		rootNodeId_ = rootNodeId;

		// create a new modbus value object instance in opc ua information model
		modbusValue_ = boost::make_shared<ModbusValue>();
		Object::SPtr obj = modbusValue_;
		CreateVariableInstance createVariableInstance(
			namespaceName_,									// namespace name of the object instance
			OpcUaLocalizedText("", registerConfig->name()), // display name of the object instance
			rootNodeId,										// parent node of the object instance
			OpcUaNodeId((uint32_t)OpcUaId_HasComponent),	// reference type between object and variable instance
			obj
		);
		id_++;
		valueNodeId_.set((uint32_t)id_, namespaceIndex_);
		createVariableInstance.addNodeId("Variable", valueNodeId_);
		if (!createVariableInstance.query(applicationServiceIf_)) {
			Log(Error, "create register value object error")
				.parameter("DisplayName", registerConfig->name());
			return false;
		}

		// Get base class access
		auto sv = modbusValue_->variable(); 		// Get server variable
		auto bnwp = sv->baseNode();					// Get base node weak pointer
		auto bn = bnwp.lock();						// Get base node smart pointer
		if (bn.get() == nullptr) {
			Log(Error, "variable access error")
				.parameter("DisplayName", registerConfig->name());
			return false;
		}

		// Set data type
		auto type = OpcUaBuildInTypeMap::string2BuildInType(registerConfig->opcUaTypeString());
		nodeId.set(type, 0);
		bn->setDataType(nodeId);

		// Set access rights
		uint8_t accessLevel = 0x03;
		if (registerConfig->modbusAccess() == RegisterConfig::ModbusAccess::Read) accessLevel = 0x01;
		bn->setAccessLevel(accessLevel);

		// Set default variable
		OpcUaStatusCode statusCode = Success;
		if (registerConfig->modbusAppl() == RegisterConfig::ModbusAppl::Master) statusCode = BadCommunicationError;
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(statusCode);
		dataValue.variant()->fromString(type, false, "0");
		modbusValue_->set_Variable(dataValue);

		return true;
	}

	bool
	OpcUaModbusValue::shutdown(void)
	{
		// remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(valueNodeId_);

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete value node instance error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name());
			return false;
		}

		return true;
	}

	bool
	OpcUaModbusValue::getDataValue(uint16_t& value)
	{
		// Server: ReadInputRegisters
		// Server: ReadMultipleHoldingRegisters

		// Read modbus variable
		OpcUaDataValue dataValue;
		if (!modbusValue_->get_Variable(dataValue)) return false;

		return true;
	}

	void
	OpcUaModbusValue::getDataValue(bool& value)
	{
		// Server: ReadCoils
		// Server: ReadDiscreteInputRegisters
		// FIXME: TODO
	}

	void
	OpcUaModbusValue::setDataValue(uint16_t value)
	{
		// Server: WriteSingleHoldingRegister
		// Server: WriteMultipleHoldingRegisters
		// Client: ReadInputRegisters
		// Client: ReadMultipleHoldingRegisters
		// FIXME: TODO
	}

	void
	OpcUaModbusValue::setDataValue(bool value)
	{
		// Server: WriteSingleCoil
		// Server: WriteMultipleCoils
		// Client: ReadCoils
		// Client: ReadDiscreteInputRegisters
		// FIXME: TODO
	}

	bool
	OpcUaModbusValue::useWriteDataValue(void)
	{
		// Client: register write callback method
		// FIXME: TODO
		return true;
	}

	void
	OpcUaModbusValue::getWriteDataValue(uint16_t& value)
	{
		// Client: WriteSingleHoldingRegister
		// Client: WriteMultipleHoldingRegisters
		// FIXME: TODO
	}

	void
	OpcUaModbusValue::getWriteDataValue(bool& value)
	{
		// Client: WriteSingleCoil
		// Client: WriteMultipleCoils
		// FIXME: TODO
	}


}

