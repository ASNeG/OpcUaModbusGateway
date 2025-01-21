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
#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"

#include "OpcUaModbusGateway/Library/Application.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

	namespace NodeId {

		const uint32_t modbusClientFolder_ = 5006;
	};

	Application::Application(void)
	{
	}

	Application::~Application(void)
	{
	}

	bool
	Application::startup(
		const std::string& configFileName,
		OpcUaStackServer::ApplicationIf* applicationIf
	)
	{
		bool rc = true;

		Log(Debug, "startup opc ua modbus gateway server");

		// Read modbus gateway configuration file
		rc  = modbusGatewayConfig_.parse(configFileName);
		if (rc == false) {
			Log(Error, "read modbus gateway configuration error")
				.parameter("ConfigFileName", configFileName);
			return false;
		}

		// modbus tcp client object and opc ua model
		for (auto modbusTCPClientConfig : modbusGatewayConfig_.modbusTCPClientConfig()) {
			Log(Debug, "create modbus tcp client")
				.parameter("Name", modbusTCPClientConfig->name())
				.parameter("IPAddress", modbusTCPClientConfig->ipAddress())
				.parameter("Port", modbusTCPClientConfig->port());

			// Add new tcp modbus client instance
			auto tcpModbusClient = std::make_shared<ModbusTCPClient>();
			tcpModbusClient->connectTimeout(modbusTCPClientConfig->connectTimeout());
			tcpModbusClient->reconnectTimeout(modbusTCPClientConfig->reconnectTimeout());
			tcpModbusClient->sendTimeout(modbusTCPClientConfig->sendTimeout());
			tcpModbusClient->recvTimeout(modbusTCPClientConfig->recvTimeout());
			rc = tcpModbusClient->connect(modbusTCPClientConfig);
			if (rc == false) {
				Log(Error, "create modbus tcp module error")
					.parameter("Name", modbusTCPClientConfig->name());
				return false;
			}
			tcpModbusClients_.push_back(tcpModbusClient);


			// Create opc ua instance
			auto opcuaModbusClientInterface = boost::make_shared<OpcUaModbusClientInterface>();
			rc = opcuaModbusClientInterface->init(
					modbusTCPClientConfig,
					tcpModbusClient,
					&applicationIf->service(),
					"http://ASNEG.de/OpcUaModbusGateway/"
			);
			if (rc == false) {
				Log(Error, "init modbus client interface error")
					.parameter("Name", modbusTCPClientConfig->name());
				return false;
			}

			// Add new modbus client object instance to opc ua model
			OpcUaStackCore::OpcUaNodeId documentDataEventTypeNodeId_;

			rc  = opcuaModbusClientInterface->addToOpcUaModel(
				NodeId::modbusClientFolder_,
				OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
			);
			if (rc == false) {
				Log(Error, "add modbus tcp client to opc ua model error")
					.parameter("Name", modbusTCPClientConfig->name());
				return false;
			}

			opcuaModbusClients_.push_back(opcuaModbusClientInterface);
		}


		return true;
	}

	bool
	Application::shutdown(void)
	{
		Log(Debug, "shutdown opc ua modbus gateway server");

		// Cleanup tcp modbus clients
		for (auto tcpModbusCLient : tcpModbusClients_) {
			tcpModbusCLient->disconnect();
		}
		tcpModbusClients_.clear();

		// Cleanup modbus client interfaces
		for (auto opcuaModbusClientInterface : opcuaModbusClients_) {
			opcuaModbusClientInterface->deleteFromOpcUaModel();
		}
		opcuaModbusClients_.clear();

		return true;
	}

}

