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
#include "OpcUaModbusGateway/Application/ModbusTCPClientImpl.h"

namespace OpcUaModbusGateway
{

	ModbusTCPClient::ModbusTCPClient(void)
	{
		modbusTCPClientImpl_ = new ModbusTCPClientImpl();
	}

	ModbusTCPClient::~ModbusTCPClient(void)
	{
		delete modbusTCPClientImpl_;
	}

	bool
	ModbusTCPClient::connect(ModbusTCPClientConfig::SPtr& modbusTCPClientConfig)
	{
		return modbusTCPClientImpl_->connect(modbusTCPClientConfig);
	}

	bool
	ModbusTCPClient::disconnect(void)
	{
		return modbusTCPClientImpl_->disconnect();
	}

}

