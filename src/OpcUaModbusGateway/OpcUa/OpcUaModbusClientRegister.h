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

#ifndef __OpcUaModbusGateway_OpcUaModbusClientRegister_h__
#define __OpcUaModbusGateway_OpcUaModbusClientRegister_h__

#include <string>

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/OpcUa/OpcUaModbusClientGroup.h"

namespace OpcUaModbusGateway
{

	class OpcUaModbusClientRegister
	{
	  public:
		OpcUaModbusClientRegister(void);
		~OpcUaModbusClientRegister(void);

		bool startup(
			const std::string& namespaceName,
			ModbusTCPClientConfig::SPtr modbusTCPClientConfig,
			OpcUaStackServer::ApplicationIf* OpcUaModbusClientIf,
			OpcUaStackCore::OpcUaNodeId& rootNodeId,
			ModbusTCPClient::SPtr modbusTCPClient
		);
		bool shutdown(void);

	  private:
		std::string namespaceName_ = "";
		ModbusTCPClientConfig::SPtr modbusTCPClientConfig_;
		OpcUaStackServer::ApplicationIf* applicationIf_ = nullptr;
		OpcUaStackCore::OpcUaNodeId rootNodeId_;

		OpcUaModbusClientGroup::Vec modbusClientGroupVec_;
	};

}

#endif
