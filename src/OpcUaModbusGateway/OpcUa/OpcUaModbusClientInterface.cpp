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

#include "OpcUaStackServer/ServiceSetApplication/GetNamespaceInfo.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateObjectInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"

#include "OpcUaModbusGateway/OpcUa/OpcUaModbusClientInterface.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

    OpcUaModbusClientInterface::OpcUaModbusClientInterface(void)
    : ModbusClientInterface()
    {
    }

    OpcUaModbusClientInterface::~OpcUaModbusClientInterface(void)
    {
    }

	bool
	OpcUaModbusClientInterface::init(
		ModbusTCPClientConfig::SPtr modbusTCPClientConfig,
		ApplicationServiceIf* applicationServiceIf,
		const std::string& namespaceName
	)
	{
		modbusTCPClientConfig_ = modbusTCPClientConfig;
		applicationServiceIf_ = applicationServiceIf;
		namespaceName_ = namespaceName;

		// Map namespace name to namespace index
		GetNamespaceInfo getNamespaceInfo;

		// Read Namespace array
		if (!getNamespaceInfo.query(applicationServiceIf)) {
			Log(Error, "get namespace info error")
			    .parameter("NamespaceName", namespaceName);
			return false;
		}

		// map namespace name to namespace index
		namespaceIndex_ = getNamespaceInfo.getNamespaceIndex(namespaceName);
		if (namespaceIndex_ == -1) {
			Log(Error, "namespace name not exist in namespace array")
			    .parameter("NamespaceName", namespaceName);
			return false;
		}

		// Create modbus tcp client
		modbusTCPClient_ = std::make_shared<ModbusTCPClient>();
		modbusTCPClient_->connectTimeout(modbusTCPClientConfig->connectTimeout());
		modbusTCPClient_->reconnectTimeout(modbusTCPClientConfig->reconnectTimeout());
		modbusTCPClient_->sendTimeout(modbusTCPClientConfig->sendTimeout());
		modbusTCPClient_->recvTimeout(modbusTCPClientConfig->recvTimeout());
		modbusTCPClient_->queryTimeout(modbusTCPClientConfig->queryTimeout());
		modbusTCPClient_->slaveId(modbusTCPClientConfig->slaveId());

		return true;
	}

	bool
	OpcUaModbusClientInterface::addToOpcUaModel(
		uint32_t parentId,
		const OpcUaNodeId& referenceTypeNodeId
	)
	{
		bool rc  = true;
		OpcUaNodeId parentNodeId(parentId, namespaceIndex_);

		// create a new modbus tcp client object instance in opc ua information model
		Object::SPtr obj = shared_from_this();
		CreateObjectInstance createObjectInstance(
			namespaceName_,									// namespace name of the object instance
			OpcUaLocalizedText("", modbusTCPClientConfig_->name()),// display name of the object instance
			parentNodeId,									// parent node of the object instance
			referenceTypeNodeId,							// reference type between object and variable instance
			obj
		);
		if (!createObjectInstance.query(applicationServiceIf_)) {
			Log(Error, "create object error")
				.parameter("DisplayName", modbusTCPClientConfig_->name());
			return false;
		}

		// Startup opc ua modbus client register
		OpcUaNodeId rootNodeId = nodeId();
		rc = modbusClientRegister_.startup(
			namespaceName_,
			modbusTCPClientConfig_,
			applicationServiceIf_,
			rootNodeId
		);
		if (rc == false) {
			Log(Error, "create modbus tcp client register error")
				.parameter("Name", modbusTCPClientConfig_->name());
			return false;
		}

		// Set state callback
		modbusTCPClient_->stateCallback(
			[this](const std::string& state) {
				setModbusConnectionState(state);
			}
		);

		// Connect to modbus tcp server
		rc = modbusTCPClient_->connect(modbusTCPClientConfig_);
		if (rc == false) {
			Log(Error, "create modbus tcp module error")
				.parameter("Name", modbusTCPClientConfig_->name());
			return false;
		}

		return true;
	}

	bool
	OpcUaModbusClientInterface::deleteFromOpcUaModel(void)
	{
		// Disconnect modbus tcp connection
		modbusTCPClient_->disconnect();
		modbusTCPClient_ = nullptr;

		// remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(nodeId());

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete node instance error")
				.parameter("ModbusCientNodeId", nodeId());
			return false;
		}
		return true;
	}

	void
	OpcUaModbusClientInterface::setModbusConnectionState(
		const std::string& setModbusConnectionState
	)
	{
		OpcUaDateTime now(boost::posix_time::microsec_clock::local_time());
		OpcUaDataValue dataValue;

		auto value = boost::make_shared<OpcUaString>();
		value->value(setModbusConnectionState);
		dataValue.variant()->variant(value);
		dataValue.statusCode((OpcUaStatusCode)Success);
		dataValue.sourceTimestamp(now);
		dataValue.sourcePicoseconds(0);
		dataValue.serverTimestamp(now);
		dataValue.serverPicoseconds(0);

		set_ModbusConnectionState_Variable(dataValue);
	}

	void
	OpcUaModbusClientInterface::createResult(
		OpcUaVariantArray::SPtr& outputArguments,
		uint32_t resultCode
	)
	{
		OpcUaVariant::SPtr variant;

		// Create output parameter list
		outputArguments->resize(1);

		// Set result code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)resultCode);
		outputArguments->push_back(variant);
	}

    void
    OpcUaModbusClientInterface::call_ReadCoils_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: QuantityOfInputs
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t quantityOfInputs = variant->get<OpcUaUInt16>();

		// Call method
		uint32_t errorCode;
		std::vector<bool> coilStatus;
		modbusTCPClient_->readCoils(startingAddress, quantityOfInputs, errorCode, coilStatus);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(2);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set coil status
		variant = boost::make_shared<OpcUaVariant>();
		for (uint32_t idx = 0; idx < coilStatus.size(); idx++) {
			OpcUaVariantValue value;
			value.variant(coilStatus[idx]);
			variant->pushBack(value);
		}
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_ReadDiscreteInputs_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: QuantityOfInputs
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t quantityOfInputs = variant->get<OpcUaUInt16>();

		// Call method
		uint32_t errorCode;
		std::vector<bool> inputStatus;
		modbusTCPClient_->readDiscreteInputs(startingAddress, quantityOfInputs, errorCode, inputStatus);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(2);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set input status
		variant = boost::make_shared<OpcUaVariant>();
		for (uint32_t idx = 0; idx < inputStatus.size(); idx++) {
			OpcUaVariantValue value;
			value.variant(inputStatus[idx]);
			variant->pushBack(value);
		}
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_ReadInputRegisters_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: QuantityOfInputs
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t quantityOfInputs = variant->get<OpcUaUInt16>();

		// Call method
		uint32_t errorCode;
		std::vector<uint16_t> inputRegisters;
		modbusTCPClient_->readInputRegisters(startingAddress, quantityOfInputs, errorCode, inputRegisters);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(2);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set input status
		variant = boost::make_shared<OpcUaVariant>();
		for (uint32_t idx = 0; idx < inputRegisters.size(); idx++) {
			OpcUaVariantValue value;
			value.variant(inputRegisters[idx]);
			variant->pushBack(value);
		}
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_ReadMultiHoldingRegisters_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: QuantityOfInputs
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t quantityOfInputs = variant->get<OpcUaUInt16>();

		// Call method
		uint32_t errorCode;
		std::vector<uint16_t> holdingRegisters;
		modbusTCPClient_->readHoldingRegisters(startingAddress, quantityOfInputs, errorCode, holdingRegisters);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(2);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set input status
		variant = boost::make_shared<OpcUaVariant>();
		for (uint32_t idx = 0; idx < holdingRegisters.size(); idx++) {
			OpcUaVariantValue value;
			value.variant(holdingRegisters[idx]);
			variant->pushBack(value);
		}
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_WriteMultipleCoils_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: Values
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaBoolean) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		std::vector<bool> values;
		OpcUaVariantValue::Vec variantVec = variant->variant();
		for (uint32_t idx = 0; idx < variantVec.size(); idx++) {
			values.push_back(variantVec[idx].variant<OpcUaBoolean>());
		}

		// Call method
		uint32_t errorCode;
		uint16_t count;
		modbusTCPClient_->writeMultipleCoils(startingAddress, values, errorCode, count);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(3);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set starting address
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)startingAddress);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set count
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)count);
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_WriteMultipleHoldingRegisters_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: HoldingRegisters
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		std::vector<uint16_t> holdingRegisters;
		OpcUaVariantValue::Vec variantVec = variant->variant();
		for (uint32_t idx = 0; idx < variantVec.size(); idx++) {
			holdingRegisters.push_back(variantVec[idx].variant<OpcUaUInt16>());
		}

		// Call method
		uint32_t errorCode;
		uint16_t count;
		modbusTCPClient_->writeMultipleHoldingRegisters(startingAddress, holdingRegisters, errorCode, count);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(3);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set starting address
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)startingAddress);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set count
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)count);
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_WriteSingleCoil_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: Value
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaBoolean) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		bool value = variant->get<OpcUaBoolean>();

		// Call method
		uint32_t errorCode;
		modbusTCPClient_->writeSingleCoil(startingAddress, value, errorCode);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(3);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set starting address
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)startingAddress);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set value
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaBoolean)value);
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

    void
    OpcUaModbusClientInterface::call_WriteSingleHoldingRegister_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
    	OpcUaVariant::SPtr variant;

    	// Check modbus tcp client
		if (modbusTCPClient_ == nullptr) {
			Log(Debug, "modbus tcp client not exist");
				createResult(applicationMethodContext->outputArguments_, 0);
				applicationMethodContext->statusCode_ = BadServiceUnsupported;
			return;
		}

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Debug, "number of input arguments error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: StartingAddress
		applicationMethodContext->inputArguments_->get(0,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [0] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t startingAddress = variant->get<OpcUaUInt16>();

		// Get input parameter [1]: holdingRegister
		applicationMethodContext->inputArguments_->get(1,variant);
		if (variant->variantType() != OpcUaBuildInType_OpcUaUInt16) {
			Log(Debug, "get input argument [1] error");
			createResult(applicationMethodContext->outputArguments_, 0);
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint16_t holdingRegister = variant->get<OpcUaUInt16>();

		// Call method
		uint32_t errorCode;
		modbusTCPClient_->writeSingleHoldingRegister(startingAddress, holdingRegister, errorCode);

		// Create output parameter list
		applicationMethodContext->outputArguments_->resize(3);

		// Set error code
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt32)errorCode);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set starting address
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)startingAddress);
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set value
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant((OpcUaUInt16)holdingRegister);
		applicationMethodContext->outputArguments_->push_back(variant);

        applicationMethodContext->statusCode_ = Success;
    }

}
