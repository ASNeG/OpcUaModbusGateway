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

#include "OpcUaModbusGateway/Application/ModbusTCPServer.h"
#include "OpcUaModbusGateway/Application/ModbusTCPServerImpl.h"

namespace OpcUaModbusGateway
{

	ModbusTCPServer::ModbusTCPServer(void)
	{
		modbusTCPServerImpl_ = new ModbusTCPServerImpl();
	}

	ModbusTCPServer::~ModbusTCPServer(void)
	{
		delete modbusTCPServerImpl_;
	}

	void
	ModbusTCPServer::stateCallback(StateCallback stateCallback)
	{
		modbusTCPServerImpl_->stateCallback(stateCallback);
	}

	void
	ModbusTCPServer::modbusServerModel(ModbusServerModel::SPtr modbusServerModel)
	{
		modbusTCPServerImpl_->modbusServerModel(modbusServerModel);
	}

	bool
	ModbusTCPServer::open(ModbusTCPServerConfig::SPtr& modbusTCPServerConfig)
	{
		return modbusTCPServerImpl_->open(modbusTCPServerConfig);
	}

	bool
	ModbusTCPServer::close(void)
	{
		return modbusTCPServerImpl_->close();
	}

}

