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
		modbusTCPServerConfig_ = modbusTCPServerConfig;

		// Add own log handler to modbus tcp client
		logHandler_ = std::make_shared<LogDefault>("modbus server message");
		modbusTCPServer_.logHandler(logHandler_);

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
					return acceptNewConnection(ctx, client);
				}
		);

		return true;
	}

	ModbusTCP::TCPServerHandler::SPtr
	ModbusTCPServerImpl::acceptNewConnection(
		asio::io_context& ctx,
		asio::ip::tcp::socket& client
	)
	{
		ModbusTCP::TCPServerModel::SPtr tcpServerModel = nullptr;
		connectionId_++;
		uint32_t connectionId = connectionId_;

		Log(Debug, "tcp modbus server receives connection request")
		    .parameter("Client", client.remote_endpoint())
			.parameter("ConnectionId", connectionId);

		if (modbusServerModel_ == nullptr) {
			Log(Error, "tcp modbus server error, because modbus server model empty")
				.parameter("Client", client.remote_endpoint());
			return tcpServerModel;
		}

		// Create tcp server model
		tcpServerModel = std::make_shared<ModbusTCP::TCPServerModel>(ctx);
		tcpServerModel->recvTimeout(modbusTCPServerConfig_->recvTimeout());
		tcpServerModel->sendTimeout(modbusTCPServerConfig_->sendTimeout());
		tcpServerModel->addModbusModel(modbusServerModel_);
		tcpServerModel->stateCallback(
			[this, &client, connectionId](ModbusTCP::TCPServerState state) {
				serverStateCallback(state, client, connectionId);
			}
		);

		// Add tcp server model to map
		tcpServerModelMap_.insert(std::make_pair(connectionId, tcpServerModel));

		Log(Error, "tcp modbus server accepted connection from client")
			.parameter("Client", client.remote_endpoint());
		return tcpServerModel;
	}

	bool
	ModbusTCPServerImpl::close(void)
	{
		// Close all existing tco server models
		for (auto tcpServerModel : tcpServerModelMap_) {
			tcpServerModel.second->disconnect();
		}

		// Close server endpoint
		modbusTCPServer_.close();

		return true;
	}

	void
	ModbusTCPServerImpl::serverStateCallback(
		ModbusTCP::TCPServerState serverState,
		asio::ip::tcp::socket& client,
		uint32_t connectionId
	)
	{
		// Check if connection is down
		if (serverState == ModbusTCP::TCPServerState::Down) {
			// Remove connection from tcp server model map
			tcpServerModelMap_.erase(connectionId);
		}

		// Check if stated changed
		if (serverState == modbusTCPServerState_) {
			return;
		}

		auto newStateString = ModbusTCP::TCPServerHandler::tcpServerStateToString(serverState);
		auto oldStateString = ModbusTCP::TCPServerHandler::tcpServerStateToString(modbusTCPServerState_);

		Log(Debug, "change server state")
			.parameter("ConnectionId", connectionId)
			.parameter("OldState", oldStateString)
			.parameter("NewState", newStateString);

		modbusTCPServerState_ = serverState;
		if (stateCallback_ == nullptr) {
			return;
		}

		stateCallback_(newStateString);
	}


}

