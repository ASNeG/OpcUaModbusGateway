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


		}

		return true;
	}

	bool
	Application::shutdown(void)
	{
		Log(Debug, "shutdown opc ua modbus gateway server");

		return true;
	}

}

