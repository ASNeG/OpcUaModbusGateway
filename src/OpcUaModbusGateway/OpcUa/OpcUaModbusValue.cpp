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

#include "OpcUaModbusGateway/Util/TypeConverter.h"
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
		OpcUaStackCore::OpcUaNodeId& rootNodeId,
		ModbusServerModel::SPtr modbusServerModel,
		RegisterGroupConfig::ModbusGroupType modbusGroupType
	)
	{
		bool rc = true;
		OpcUaNodeId nodeId;
		OpcUaDataValue dataValue;
		OpcUaDateTime now(boost::posix_time::microsec_clock::universal_time());

		// Set parameter
		namespaceName_ = namespaceName;
		namespaceIndex_ = namespaceIndex;
		registerConfig_ = registerConfig;
		applicationServiceIf_ = applicationServiceIf;
		rootNodeId_ = rootNodeId;
		modbusServerModel_ = modbusServerModel;
		modbusGroupType_ = modbusGroupType;

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

		// Register getter and setter methods
		rc = registerSetterAndGetter();
		if (rc == false) {
			return false;
		}

		return true;
	}

	bool
	OpcUaModbusValue::shutdown(void)
	{
		// Deregister getter and setter methods
		deregisterSetterAndGetter();

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
	OpcUaModbusValue::registerSetterAndGetter(void)
	{
		bool rc = true;

		if (modbusServerModel_ == nullptr) {
			return true;
		}

		switch (modbusGroupType_)
		{
			case RegisterGroupConfig::ModbusGroupType::Coil:
			{
				rc = modbusServerModel_->registerCoils(
					registerConfig_->address(),
					[this](bool value) {
						return setDataValue(value);
					},
					[this](bool& value) {
						return getDataValue(value);
					}
				);
				if (!rc) {
					Log(Error, "register coil in modbus model error")
						.parameter("Name", registerConfig_->name())
						.parameter("Address", registerConfig_->address());
					return false;
				}
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::Input:
			{
				rc = modbusServerModel_->registerInputs(
					registerConfig_->address(),
					[this](bool& value) {
						return getDataValue(value);
					}
				);
				if (!rc) {
					Log(Error, "register input in modbus model error")
						.parameter("Name", registerConfig_->name())
						.parameter("Address", registerConfig_->address());
					return false;
				}
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::HoldingRegister:
			{
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::InputRegister:
			{
				break;
			}
		}

#if 0
		bool registerInputs(
			uint16_t id,
			RegisterEntry::GetBoolCallback getBoolCallback
		);
		bool registerHoldingRegisters(
			uint16_t id,
			RegisterEntry::SetUInt16Callback setUInt16Callback,
			RegisterEntry::GetUInt16Callback getUInt16Callback
		);
		bool registerInputTegisters(
			uint16_t id,
			RegisterEntry::GetUInt16Callback getUInt16Callback
		);

#endif

		return true;
	}

	bool
	OpcUaModbusValue::deregisterSetterAndGetter(void)
	{
		bool rc = true;

		if (modbusServerModel_ == nullptr) {
			return true;
		}

		switch (modbusGroupType_)
		{
			case RegisterGroupConfig::ModbusGroupType::Coil:
			{
				modbusServerModel_->deregisterCoils(registerConfig_->address());
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::Input:
			{
				modbusServerModel_->deregisterInputs(registerConfig_->address());
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::HoldingRegister:
			{
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::InputRegister:
			{
				break;
			}
		}

		return true;
	}

	bool
	OpcUaModbusValue::getDataValue(uint16_t& value)
	{
		// Server: ReadInputRegisters
		// Server: ReadMultipleHoldingRegisters
		auto rc = true;

		// Read modbus variable
		OpcUaDataValue dataValue;
		if (!modbusValue_->get_Variable(dataValue)) {
			return false;
		}

		// Convert modbus variable to uint16
		OpcUaVariant targetVariant;
		rc = TypeConverter::conversion(*dataValue.variant(), OpcUaBuildInType::OpcUaBuildInType_OpcUaInt16, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", dataValue.variant()->variantType())
				.parameter("TargetType", OpcUaBuildInType::OpcUaBuildInType_OpcUaInt16);
			return false;
		}

		// Get variable from variant
		value = targetVariant.get<uint16_t>();
		return true;
	}

	bool
	OpcUaModbusValue::getDataValue(bool& value)
	{
		// Server: ReadCoils
		// Server: ReadDiscreteInputRegisters
		auto rc = true;

		// Read modbus variable
		OpcUaDataValue dataValue;
		if (!modbusValue_->get_Variable(dataValue)) {
			return false;
		}

		// Convert modbus variable to bool
		OpcUaVariant targetVariant;
		rc = TypeConverter::conversion(*dataValue.variant(), OpcUaBuildInType::OpcUaBuildInType_OpcUaBoolean, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", dataValue.variant()->variantType())
				.parameter("TargetType", OpcUaBuildInType::OpcUaBuildInType_OpcUaBoolean);
			return false;
		}

		// Get variable from variant
		value = targetVariant.get<bool>();
		return true;
	}

	bool
	OpcUaModbusValue::setDataValue(uint16_t value)
	{
		// Server: WriteSingleHoldingRegister
		// Server: WriteMultipleHoldingRegisters
		// Client: ReadInputRegisters
		// Client: ReadMultipleHoldingRegisters
		bool rc = true;

		// Convert variable to target data value type
		auto targetType = OpcUaBuildInTypeMap::string2BuildInType(registerConfig_->opcUaTypeString());
		OpcUaVariant sourceVariant(value);
		OpcUaVariant targetVariant;
		rc = TypeConverter::conversion(sourceVariant, targetType, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", OpcUaBuildInType::OpcUaBuildInType_OpcUaUInt16)
				.parameter("TargetType", targetType);
			return false;
		}

		// Set data value
		OpcUaDateTime now(boost::posix_time::microsec_clock::universal_time());
		OpcUaDataValue dataValue;
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->copyFrom(targetVariant);
		modbusValue_->set_Variable(dataValue);
		return true;
	}

	bool
	OpcUaModbusValue::setDataValue(bool value)
	{
		// Server: WriteSingleCoil
		// Server: WriteMultipleCoils
		// Client: ReadCoils
		// Client: ReadDiscreteInputRegisters
		bool rc = true;

		// Convert variable to target data value type
		auto targetType = OpcUaBuildInTypeMap::string2BuildInType(registerConfig_->opcUaTypeString());
		OpcUaVariant sourceVariant(value);
		OpcUaVariant targetVariant;
		rc = TypeConverter::conversion(sourceVariant, targetType, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", OpcUaBuildInType::OpcUaBuildInType_OpcUaBoolean)
				.parameter("TargetType", targetType);
			return false;
		}

		// Set data value
		OpcUaDateTime now(boost::posix_time::microsec_clock::universal_time());
		OpcUaDataValue dataValue;
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->copyFrom(targetVariant);
		modbusValue_->set_Variable(dataValue);
		return true;
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

