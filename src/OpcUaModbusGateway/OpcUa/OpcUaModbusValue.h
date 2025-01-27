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

#ifndef __OpcUaModbusGateway_OpcUaModbusValue_h__
#define __OpcUaModbusGateway_OpcUaModbusValue_h__

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"

#include "OpcUaModbusGateway/CustomerVariableType/AnalogValue.h"

namespace OpcUaModbusGateway
{

	class OpcUaModbusValue
	{
	  public:
		OpcUaModbusValue(void);
		~OpcUaModbusValue(void);

		bool startup(
			const std::string& namespaceName,
			uint32_t namespaceIndex,
			RegisterConfig::SPtr registerConfig,
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_,
			OpcUaStackCore::OpcUaNodeId& rootNodeId
		);
		bool shutdown(void);

		void getWriteDataValue(OpcUaStackCore::OpcUaDataValue& writeDataValue);

	  private:
		static uint32_t id_;
		std::string namespaceName_ = "";
		uint32_t namespaceIndex_ = 0;
		RegisterConfig::SPtr registerConfig_;
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
		OpcUaStackCore::OpcUaNodeId rootNodeId_;

		AnalogValue::Vec analogValueVec_;
		OpcUaStackCore::OpcUaDataValue writeDataValue_;
	};

}

#endif
