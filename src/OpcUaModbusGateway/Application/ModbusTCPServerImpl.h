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

#ifndef __OpcUaModbusGateway_ModbusTCPServerImpl_h__
#define __OpcUaModbusGateway_ModbusTCPServerImpl_h__

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/Application/ModbusServerModel.h"

#include "ModbusTCP/TCPServer.h"
#include "ModbusTCP/TCPServerModel.h"

namespace OpcUaModbusGateway
{

	class ModbusTCPServerImpl
	{
	  public:
		using StateCallback = std::function<void (const std::string&)>;

		ModbusTCPServerImpl(void);
		~ModbusTCPServerImpl(void);

		void stateCallback(StateCallback stateCallback);
		void modbusServerModel(ModbusServerModel::SPtr modbusServerModel);

		bool open(ModbusTCPServerConfig::SPtr& modbusTCPServerConfig);
		bool close(void);

	  private:
		StateCallback stateCallback_;
		asio::ip::tcp::endpoint serverEndpoint_;
		ModbusTCP::TCPServerModel::SPtr tcpServerModel_ = nullptr;  // FIXME: Create map.....
		ModbusTCP::TCPServer modbusTCPServer_;
		ModbusTCP::TCPServerState modbusTCPServerState_;
		ModbusServerModel::SPtr modbusServerModel_ = nullptr;
		Base::LogHandler::SPtr logHandler_ = nullptr;

		void serverStateCallback(
			ModbusTCP::TCPServerState modbusTCPServerState,
			asio::ip::tcp::socket& client
		);

	};
}

#endif
