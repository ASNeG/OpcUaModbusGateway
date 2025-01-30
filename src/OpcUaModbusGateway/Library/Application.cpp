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

	// Node Ids
	namespace NodeId {
		const uint32_t modbusServerFolder_ = 5004;
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

			// Create opc ua instance
			auto opcuaModbusClientInterface = boost::make_shared<OpcUaModbusClientInterface>();
			rc = opcuaModbusClientInterface->init(
					modbusTCPClientConfig,
					&applicationIf->service(),
					"http://ASNEG.de/OpcUaModbusGateway/"
			);
			if (rc == false) {
				Log(Error, "init modbus client interface error")
					.parameter("Name", modbusTCPClientConfig->name());
				return false;
			}

			// Add new modbus client object instance to opc ua model
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

		// modbus tcp server object and opc ua model
		for (auto modbusTCPServerConfig : modbusGatewayConfig_.modbusTCPServerConfig()) {
			Log(Debug, "create modbus tcp server")
				.parameter("Name", modbusTCPServerConfig->name())
				.parameter("IPAddress", modbusTCPServerConfig->ipAddress())
				.parameter("Port", modbusTCPServerConfig->port());

			// Create opc ua instance
			auto opcuaModbusServerInterface = boost::make_shared<OpcUaModbusServerInterface>();
			rc = opcuaModbusServerInterface->init(
					modbusTCPServerConfig,
					&applicationIf->service(),
					"http://ASNEG.de/OpcUaModbusGateway/"
			);
			if (rc == false) {
				Log(Error, "init modbus server interface error")
					.parameter("Name", modbusTCPServerConfig->name());
				return false;
			}

			// Add new modbus server object instance to opc ua model
			rc  = opcuaModbusServerInterface->addToOpcUaModel(
				NodeId::modbusServerFolder_,
				OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
			);
			if (rc == false) {
				Log(Error, "add modbus tcp server to opc ua model error")
					.parameter("Name", modbusTCPServerConfig->name());
				return false;
			}

			opcuaModbusServers_.push_back(opcuaModbusServerInterface);
		}

		return true;
	}

	bool
	Application::shutdown(void)
	{
		Log(Debug, "shutdown opc ua modbus gateway server");

		// Cleanup modbus client interfaces
		for (auto opcuaModbusClientInterface : opcuaModbusClients_) {
			opcuaModbusClientInterface->deleteFromOpcUaModel();
		}
		opcuaModbusClients_.clear();

		return true;
	}

}

