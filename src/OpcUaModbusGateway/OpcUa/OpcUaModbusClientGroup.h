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

#ifndef __OpcUaModbusGateway_OpcUaModbusClientGroup_h__
#define __OpcUaModbusGateway_OpcUaModbusClientGroup_h__

#include <string>

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/OpcUa/OpcUaModbusValue.h"
#include "OpcUaModbusGateway/CustomerVariableType/AnalogValue.h"

namespace OpcUaModbusGateway
{

	class OpcUaModbusClientGroup
	{
	  public:
		OpcUaModbusClientGroup(void);
		~OpcUaModbusClientGroup(void);

		bool startup(
			const std::string& namespaceName,
			uint32_t namespaceIndex,
			RegisterGroupConfig::SPtr registerGroupConfig,
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_,
			OpcUaStackCore::OpcUaNodeId& rootNodeId
		);
		bool shutdown(void);

	  private:
		static uint32_t id_;
		std::string namespaceName_ = "";
		uint32_t namespaceIndex_ = 0;
		RegisterGroupConfig::SPtr registerGroupConfig_;
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
		OpcUaStackCore::OpcUaNodeId groupNodeId_;
		OpcUaStackCore::OpcUaNodeId rootNodeId_;

		OpcUaModbusValue::Vec modbusValueVec_;
	};

}

#endif
