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
#include "OpcUaStackServer/ServiceSetApplication/ForwardNodeSync.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateVariableInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/RegisterForwardNode.h"

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

		// Set precision value
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->setValue(OpcUaDouble(registerConfig_->b()));
		modbusValue_->set_Precision_Variable(dataValue);

		// Set unit value
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->setValue(OpcUaString(registerConfig_->unit()));
		modbusValue_->set_Unit_Variable(dataValue);

		// Set range begin value
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->setValue(OpcUaDouble(registerConfig_->a()));
		modbusValue_->set_RangeBegin_Variable(dataValue);

		// Set range end value
		double range = 1.0;
		if (registerConfig_->modbusType() == RegisterConfig::ModbusType::UInt16) range = 65535.0;
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->setValue(OpcUaDouble(registerConfig_->a() + registerConfig_->b() * range));
		modbusValue_->set_RangeEnd_Variable(dataValue);

		// Set address value
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(Success);
		dataValue.variant()->setValue((uint16_t)registerConfig_->address());
		modbusValue_->set_Address_Variable(dataValue);

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

	RegisterConfig::SPtr
	OpcUaModbusValue::registerConfig(void)
	{
		return registerConfig_;
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
						return setDataValue(Success, value);
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
				rc = modbusServerModel_->registerHoldingRegisters(
					registerConfig_->address(),
					[this](uint16_t value) {
						return setDataValue(Success, value);
					},
					[this](uint16_t& value) {
						return getDataValue(value);
					}
				);
				if (!rc) {
					Log(Error, "register holding registers in modbus model error")
						.parameter("Name", registerConfig_->name())
						.parameter("Address", registerConfig_->address());
					return false;
				}
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::InputRegister:
			{
				rc = modbusServerModel_->registerInputRegisters(
					registerConfig_->address(),
					[this](uint16_t& value) {
						return getDataValue(value);
					}
				);
				if (!rc) {
					Log(Error, "register input registers in modbus model error")
						.parameter("Name", registerConfig_->name())
						.parameter("Address", registerConfig_->address());
					return false;
				}
				break;
			}
		}
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
				modbusServerModel_->deregisterHoldingRegisters(registerConfig_->address());
				break;
			}
			case RegisterGroupConfig::ModbusGroupType::InputRegister:
			{
				modbusServerModel_->deregisterInputRegisters(registerConfig_->address());
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
			Log(Error, "read data value error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name());
			return false;
		}

		// Convertion opc ua variable to modbus variable
		rc = convertOpcUaToModbus(*dataValue.variant(), value);
		if (!rc) {
			Log(Error, "convert opc ua variable to modbus variable error");
		}

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

		// Convertion opc ua variable to modbus variable
		rc = convertOpcUaToModbus(*dataValue.variant(), value);
		if (!rc) {
			Log(Error, "convert opc ua variable to modbus variable error");
		}

		return true;
	}

	bool
	OpcUaModbusValue::setDataValue(OpcUaStackCore::OpcUaStatusCode statusCode, uint16_t value)
	{
		// Server: WriteSingleHoldingRegister
		// Server: WriteMultipleHoldingRegisters
		// Client: ReadInputRegisters
		// Client: ReadMultipleHoldingRegisters
		bool rc = true;

		// Conversion modbus variable to opc ua variable
		OpcUaStackCore::OpcUaVariant targetVariant;
		rc = convertModbusToOpcUa(value, targetVariant);
		if (!rc) {
			Log(Error, "convert modbus variable to opc ua variable error");
		}

		// Set data value
		OpcUaDateTime now(boost::posix_time::microsec_clock::universal_time());
		OpcUaDataValue dataValue;
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(statusCode);
		dataValue.variant()->copyFrom(targetVariant);
		modbusValue_->set_Variable(dataValue);
		return true;
	}

	bool
	OpcUaModbusValue::setDataValue(OpcUaStatusCode statusCode, bool value)
	{
		// Server: WriteSingleCoil
		// Server: WriteMultipleCoils
		// Client: ReadCoils
		// Client: ReadDiscreteInputRegisters
		bool rc = true;

		// Conversion modbus variable to opc ua variable
		OpcUaStackCore::OpcUaVariant targetVariant;
		rc = convertModbusToOpcUa(value, targetVariant);
		if (!rc) {
			Log(Error, "convert modbus variable to opc ua variable error");
		}

		// Set data value
		OpcUaDateTime now(boost::posix_time::microsec_clock::universal_time());
		OpcUaDataValue dataValue;
		dataValue.serverTimestamp(now);
		dataValue.sourceTimestamp(now);
		dataValue.statusCode(statusCode);
		dataValue.variant()->copyFrom(targetVariant);
		modbusValue_->set_Variable(dataValue);
		return true;
	}

	bool
	OpcUaModbusValue::useWriteDataValue(void)
	{
		// Client: register write callback method
		RegisterForwardNode registerForwardNode(modbusValue_->variable()->nodeId());
		registerForwardNode.setWriteCallback(boost::bind(&OpcUaModbusValue::writeValue, this, boost::placeholders::_1));
		if (!registerForwardNode.query(applicationServiceIf_, true)) {
			Log(Error, "register forward node error")
				.parameter("NodeId", modbusValue_->variable()->nodeId());
			return false;
		}

		return true;
	}

	bool
	OpcUaModbusValue::getWriteDataValue(uint16_t& value)
	{
		// Client: WriteSingleHoldingRegister
		// Client: WriteMultipleHoldingRegisters
		// FIXME: TODO

		return true;
	}

	bool
	OpcUaModbusValue::getWriteDataValue(bool& value)
	{
		// Client: WriteSingleCoil
		// Client: WriteMultipleCoils
		// FIXME: TODO

		return true;
	}

	void
	OpcUaModbusValue::writeValue(ApplicationWriteContext* applicationWriteContext)
	{
		// Check register type
		if (modbusGroupType_ ==  RegisterGroupConfig::ModbusGroupType::Input ||
			modbusGroupType_ ==  RegisterGroupConfig::ModbusGroupType::InputRegister
		) {
			applicationWriteContext->statusCode_ = BadNotWritable;
			return;
		}

		std::cout << "JETZT WURDE DIE VARIABLE GESETZT ..........." << std::endl;
		// applicationWriteContext->statusCode_ = Success;
		// applicationWriteContext->dataValue_.copyTo(*it->second);

		// bool writeDataFlag_ = false;
		// OpcUaStackCore::OpcUaDataValue writeDataValue_;
	}

	bool
	OpcUaModbusValue::convertModbusToOpcUa(bool sourceValue, OpcUaStackCore::OpcUaVariant& targetVariant)
	{
		auto targetType = OpcUaBuildInTypeMap::string2BuildInType(registerConfig_->opcUaTypeString());
		bool calcEnable = registerConfig_->a() != 0.0 || registerConfig_->b() != 1.0;

		// No conversion necessary
		if (targetType == OpcUaBuildInType_OpcUaBoolean && !calcEnable) {
			targetVariant.setValue(sourceValue);
			return true;
		}

		// Convert source value to double value
		double doubleValue = (double)sourceValue;

		// Calculate new target
		if (calcEnable) {
			doubleValue = registerConfig_->a() + registerConfig_->b() * doubleValue;
		}

		// Convert double value to target value
		OpcUaVariant sourceVariant(doubleValue);
		bool rc = TypeConverter::conversion(sourceVariant, targetType, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", OpcUaBuildInType::OpcUaBuildInType_OpcUaBoolean)
				.parameter("TargetType", targetType);
			return false;
		}

		return true;
	}

	bool
	OpcUaModbusValue::convertModbusToOpcUa(uint16_t sourceValue, OpcUaStackCore::OpcUaVariant& targetVariant)
	{
		auto targetType = OpcUaBuildInTypeMap::string2BuildInType(registerConfig_->opcUaTypeString());
		bool calcEnable = registerConfig_->a() != 0.0 || registerConfig_->b() != 1.0;

		// No conversion necessary
		if (targetType == OpcUaBuildInType_OpcUaUInt16 && !calcEnable) {
			targetVariant.setValue(sourceValue);
			return true;
		}

		// Convert source value to double value
		double doubleValue = (double)sourceValue;

		// Calculate new target
		if (calcEnable) {
			doubleValue = registerConfig_->a() + registerConfig_->b() * doubleValue;
		}

		// Convert double value to target values
		OpcUaVariant sourceVariant(doubleValue);
		bool rc = TypeConverter::conversion(sourceVariant, targetType, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", OpcUaBuildInType::OpcUaBuildInType_OpcUaUInt16)
				.parameter("TargetType", targetType);
			return false;
		}

		return true;
	}

	bool
	OpcUaModbusValue::convertOpcUaToModbus(OpcUaStackCore::OpcUaVariant& sourceVariant, bool& targetValue)
	{
		auto sourceType = sourceVariant.variantType();
		bool calcEnable = registerConfig_->a() != 0.0 || registerConfig_->b() != 1.0;

		// No conversion necessary
		if (sourceType == OpcUaBuildInType_OpcUaBoolean && !calcEnable) {
			sourceVariant.getValue(targetValue);
			return true;
		}

		// Convert source value to double
		OpcUaVariant doubleVariant;
		bool rc = TypeConverter::conversion(sourceVariant, OpcUaBuildInType_OpcUaDouble, doubleVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", sourceType)
				.parameter("TargetType", OpcUaBuildInType_OpcUaDouble);
			return false;
		}
		double doubleValue;
		doubleVariant.getValue(doubleValue);


		// Calculate new target
		if (calcEnable) {
			doubleValue = (doubleValue - registerConfig_->a()) / registerConfig_->b();
		}

		// Convert double value to target values
		doubleVariant.setValue(doubleValue);
		OpcUaVariant targetVariant;
		rc = TypeConverter::conversion(doubleVariant, OpcUaBuildInType_OpcUaBoolean, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", OpcUaBuildInType::OpcUaBuildInType_OpcUaDouble)
				.parameter("TargetType", OpcUaBuildInType::OpcUaBuildInType_OpcUaBoolean);
			return false;
		}
		targetVariant.getValue(targetValue);

		return true;
	}

	bool
	OpcUaModbusValue::convertOpcUaToModbus(OpcUaStackCore::OpcUaVariant& sourceVariant, uint16_t& targetValue)
	{
		auto sourceType = sourceVariant.variantType();
		bool calcEnable = registerConfig_->a() != 0.0 || registerConfig_->b() != 1.0;

		// No conversion necessary
		if (sourceType == OpcUaBuildInType_OpcUaUInt16 && !calcEnable) {
			sourceVariant.getValue(targetValue);
			return true;
		}

		// FIXME: Direct conversion if not calcEnable
		// FIXME: Range checks

		// Convert source value to double
		OpcUaVariant doubleVariant;
		bool rc = TypeConverter::conversion(sourceVariant, OpcUaBuildInType_OpcUaDouble, doubleVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", sourceType)
				.parameter("TargetType", OpcUaBuildInType_OpcUaDouble);
			return false;
		}
		double doubleValue;
		doubleVariant.getValue(doubleValue);

		// Calculate new target
		if (calcEnable) {
			doubleValue = (doubleValue - registerConfig_->a()) / registerConfig_->b();
		}

		// Convert double value to target values
		doubleVariant.setValue(doubleValue);
		OpcUaVariant targetVariant;
		rc = TypeConverter::conversion(doubleVariant, OpcUaBuildInType_OpcUaUInt16, targetVariant);
		if (!rc) {
			Log(Error, "value conversion error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name())
				.parameter("SourceType", OpcUaBuildInType::OpcUaBuildInType_OpcUaDouble)
				.parameter("TargetType", OpcUaBuildInType::OpcUaBuildInType_OpcUaUInt16);
			return false;
		}
		targetVariant.getValue(targetValue);

		return true;
	}


}

