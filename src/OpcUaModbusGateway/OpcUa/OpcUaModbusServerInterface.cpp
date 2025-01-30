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

#include "OpcUaModbusGateway/OpcUa/OpcUaModbusServerInterface.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

    OpcUaModbusServerInterface::OpcUaModbusServerInterface(void)
    : ModbusServerInterface()
    {
    }

    OpcUaModbusServerInterface::~OpcUaModbusServerInterface(void)
    {
    }

	bool
	OpcUaModbusServerInterface::init(
		ModbusTCPServerConfig::SPtr modbusTCPServerConfig,
		ApplicationServiceIf* applicationServiceIf,
		const std::string& namespaceName
	)
	{
		modbusTCPServerConfig_ = modbusTCPServerConfig;
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

		// Create modbus tcp server
		modbusTCPServer_ = std::make_shared<ModbusTCPServer>();

		return true;
	}

	bool
	OpcUaModbusServerInterface::addToOpcUaModel(
		uint32_t parentId,
		const OpcUaNodeId& referenceTypeNodeId
	)
	{
		bool rc  = true;
		OpcUaNodeId parentNodeId(parentId, namespaceIndex_);

		// create a new modbus tcp server object instance in opc ua information model
		Object::SPtr obj = shared_from_this();
		CreateObjectInstance createObjectInstance(
			namespaceName_,									// namespace name of the object instance
			OpcUaLocalizedText("", modbusTCPServerConfig_->name()),// display name of the object instance
			parentNodeId,									// parent node of the object instance
			referenceTypeNodeId,							// reference type between object and variable instance
			obj
		);
		if (!createObjectInstance.query(applicationServiceIf_)) {
			Log(Error, "create object error")
				.parameter("DisplayName", modbusTCPServerConfig_->name());
			return false;
		}

#if 0
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
#endif

		return true;
	}

	bool
	OpcUaModbusServerInterface::deleteFromOpcUaModel(void)
	{
#if 0
		// Disconnect modbus tcp connection
		modbusTCPClient_->disconnect();
		modbusTCPClient_ = nullptr;

		// Delete modbus client register instances from opc ua model
		auto rc = modbusClientRegister_.shutdown();
		if (!rc) {
			Log(Error, "delete client register node instance error");
			return false;
		}

		// remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(nodeId());

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete node instance error")
				.parameter("ModbusCientNodeId", nodeId());
			return false;
		}
#endif
		return true;
	}

#if 0
	void
	OpcUaModbusServerInterface::setModbusConnectionState(
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
#endif

}
