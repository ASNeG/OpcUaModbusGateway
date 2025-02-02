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

#include "OpcUaStackCore/Base/Log.h"

#include "ModbusTCP/TCPServerModel.h"
#include "ModbusTCP/TCPServerHandler.h"

#include "OpcUaModbusGateway/Application/ModbusTCPServerImpl.h"
#include "OpcUaModbusGateway/Application/LogDefault.h"

using namespace OpcUaStackCore;

namespace OpcUaModbusGateway
{

	ModbusTCPServerImpl::ModbusTCPServerImpl(void)
	{
	}

	ModbusTCPServerImpl::~ModbusTCPServerImpl(void)
	{
	}

	void
	ModbusTCPServerImpl::stateCallback(StateCallback stateCallback)
	{
		stateCallback_ = stateCallback;
	}

	void
	ModbusTCPServerImpl::modbusServerModel(
		ModbusServerModel::SPtr modbusServerModel
	)
	{
		modbusServerModel_ = modbusServerModel;
	}

	bool
	ModbusTCPServerImpl::open(ModbusTCPServerConfig::SPtr& modbusTCPServerConfig)
	{
		bool rc = true;

		// Add own log handler to modbus tcp client
		Base::LogHandler::SPtr logHandler = std::make_shared<LogDefault>();
		modbusTCPServer_.logHandler(logHandler);

		// Create server endpoint
		rc = modbusTCPServer_.getEndpoint(
			modbusTCPServerConfig->ipAddress(),
			modbusTCPServerConfig->port(),
			serverEndpoint_
		);
		if (!rc) {
			return false;
		}

		// Open server endpoint
		rc = modbusTCPServer_.open(
				serverEndpoint_,
				[this](asio::io_context& ctx, asio::ip::tcp::socket& client) {
					ModbusTCP::TCPServerModel::SPtr tcpServerModel = nullptr;

					Log(Debug, "tcp modbus server receives connection request")
					    .parameter("Client", client.remote_endpoint());

					if (modbusServerModel_ == nullptr) {
						Log(Error, "tcp modbus server error, because modbus server model empty")
							.parameter("Client", client.remote_endpoint());
						return tcpServerModel;
					}

					tcpServerModel->stateCallback(
						[this](ModbusTCP::TCPServerState state) {
							serverStateCallback(state);
						}
					);

					tcpServerModel = std::make_shared<ModbusTCP::TCPServerModel>(ctx);
					tcpServerModel->addModbusModel(modbusServerModel_);
					Log(Error, "tcp modbus server accepted connection from client")
						.parameter("Client", client.remote_endpoint());
					return tcpServerModel;
				}
		);

		return true;
	}

	bool
	ModbusTCPServerImpl::close(void)
	{
		// Close server endpoint
		modbusTCPServer_.close();

		return true;
	}

	void
	ModbusTCPServerImpl::serverStateCallback(ModbusTCP::TCPServerState serverState)
	{
		modbusTCPServerState_ = serverState;
		if (stateCallback_ == nullptr) {
			return;
		}

		auto stateString = ModbusTCP::TCPServerHandler::tcpServerStateToString(serverState);
		stateCallback_(stateString);
	}


}

