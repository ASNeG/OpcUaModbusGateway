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

#include "OpcUaModbusGateway/Application/ModbusTCPClient.h"

namespace OpcUaModbusGateway
{

	ModbusTCPClient::ModbusTCPClient(void)
	{
	}

	ModbusTCPClient::~ModbusTCPClient(void)
	{
	}

	bool
	ModbusTCPClient::connect(const std::string& address, uint32_t port)
	{
		bool rc = true;

		// Create client endpoint
		rc = modbusTCPClient_.getEndpoint(address, port, serverEndpoint_);
		if (!rc) {
			return false;
		}

		// Connect to modbus tcp server
		modbusTCPClient_.connect(
			serverEndpoint_,
			[this](ModbusTCP::TCPClientState clientState) {
				clientConnectionHandler(clientState);
		});
	}

	bool
	ModbusTCPClient::disconnect(void)
	{
		modbusTCPClient_.disconnect();
		return true;
	}

	void
	ModbusTCPClient::clientConnectionHandler(
		ModbusTCP::TCPClientState clientState
	)
	{
		modbusTCPClientState_ = clientState;
	}

}

