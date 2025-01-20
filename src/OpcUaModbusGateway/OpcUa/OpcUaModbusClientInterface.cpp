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
		ModbusTCPClient::SPtr modbusTCPClient,
		ApplicationServiceIf* applicationServiceIf,
		const std::string& namespaceName
	)
	{
		modbusTCPClientConfig_ = modbusTCPClientConfig;
		modbusTCPClient_ = modbusTCPClient;
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

		return true;
	}

	bool
	OpcUaModbusClientInterface::addToOpcUaModel(
		uint32_t parentId,
		const OpcUaNodeId& referenceTypeNodeId
	)
	{
		OpcUaNodeId parentNodeId(parentId, namespaceIndex_);

		// create a new object instance in opc ua information model
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

		return true;
	}

	bool
	OpcUaModbusClientInterface::deleteFromOpcUaModel(void)
	{
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
    OpcUaModbusClientInterface::call_ReadCoils_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_ReadDiscreteInputs_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_ReadInputRegisters_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_ReadMultiHoldingRegisters_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_WriteMultipleCoils_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_WriteMultipleHoldingRegisters_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_WriteSingleCoil_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    OpcUaModbusClientInterface::call_WriteSingleHoldingRegister_Method(
    	ApplicationMethodContext* applicationMethodContext
	)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

}
