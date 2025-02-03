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

#ifndef __OpcUaModbusGateway_OpcUaModbusServerRegister_h__
#define __OpcUaModbusGateway_OpcUaModbusServerRegister_h__

#include <string>

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/OpcUa/OpcUaModbusServerGroup.h"
#include "OpcUaModbusGateway/Application/ModbusServerModel.h"

namespace OpcUaModbusGateway
{

	class OpcUaModbusServerRegister
	{
	  public:
		OpcUaModbusServerRegister(void);
		~OpcUaModbusServerRegister(void);

		bool startup(
			const std::string& namespaceName,
			ModbusTCPServerConfig::SPtr modbusTCPServerConfig,
			OpcUaStackServer::ApplicationServiceIf* OpcUaModbusServerRegisterIf,
			OpcUaStackCore::OpcUaNodeId& rootNodeId,
			ModbusServerModel::SPtr modbusServerModel
		);
		bool shutdown(void);

	  private:
		std::string namespaceName_ = "";
		ModbusTCPServerConfig::SPtr modbusTCPServerConfig_;
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
		OpcUaStackCore::OpcUaNodeId rootNodeId_;

		OpcUaModbusServerGroup::Vec modbusServerGroupVec_;
	};

}

#endif
