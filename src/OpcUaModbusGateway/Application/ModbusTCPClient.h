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

#ifndef __OpcUaModbusGateway_ModbusTCPClient_h__
#define __OpcUaModbusGateway_ModbusTCPClient_h__

#include "ModbusTCP/TCPClient.h"

namespace OpcUaModbusGateway
{

	class ModbusTCPClient
	{
	  public:
		ModbusTCPClient(void);
		~ModbusTCPClient(void);

		bool connect(const std::string& address, uint32_t port);
		bool disconnect(void);

	  private:
		asio::ip::tcp::endpoint serverEndpoint_;
		ModbusTCP::TCPClient modbusTCPClient_;
		ModbusTCP::TCPClientState modbusTCPClientState_;

		void clientConnectionHandler(ModbusTCP::TCPClientState clientState);
	};

}

#endif
