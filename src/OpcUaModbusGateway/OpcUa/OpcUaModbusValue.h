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

#include "OpcUaStackCore/Application/ApplicationWriteContext.h"
#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/CustomerVariableType/ModbusValue.h"
#include "OpcUaModbusGateway/Application/ModbusServerModel.h"

namespace OpcUaModbusGateway
{

	class OpcUaModbusValue
	{
	  public:
		using SPtr = std::shared_ptr<OpcUaModbusValue>;
		using Vec = std::vector<SPtr>;

		OpcUaModbusValue(void);
		~OpcUaModbusValue(void);

		bool startup(
			const std::string& namespaceName,
			uint32_t namespaceIndex,
			RegisterConfig::SPtr registerConfig,
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_,
			OpcUaStackCore::OpcUaNodeId& rootNodeId,
			ModbusServerModel::SPtr modbusServerModel,
			RegisterGroupConfig::ModbusGroupType modbusGroupType
		);
		bool shutdown(void);

		RegisterConfig::SPtr registerConfig(void);

		bool getDataValue(uint16_t& value); 		// Server
		bool getDataValue(bool& value);				// Server
		bool setDataValue( // Client and Server
			OpcUaStackCore::OpcUaStatusCode statusCode,
			uint16_t value
		);
		bool setDataValue( // Client and Server
			OpcUaStackCore::OpcUaStatusCode statusCode,
			bool value
		);

		bool useWriteDataValue(void);				// Client
		bool getWriteDataValue(uint16_t& value); 	// Client
		bool getWriteDataValue(bool& value);		// Client
		void writeValue(OpcUaStackCore::ApplicationWriteContext* applicationWriteContext);

	  private:
		static uint32_t id_;
		std::string namespaceName_ = "";
		uint32_t namespaceIndex_ = 0;
		RegisterConfig::SPtr registerConfig_;
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
		ModbusServerModel::SPtr modbusServerModel_ = nullptr;
		RegisterGroupConfig::ModbusGroupType modbusGroupType_ = RegisterGroupConfig::ModbusGroupType::None;

		OpcUaStackCore::OpcUaNodeId valueNodeId_;
		OpcUaStackCore::OpcUaNodeId rootNodeId_;

		ModbusValue::SPtr modbusValue_;
		OpcUaStackCore::OpcUaDataValue writeDataValue_;

		bool registerSetterAndGetter(void);
		bool deregisterSetterAndGetter(void);

		bool convertModbusToOpcUa(bool sourceValue, OpcUaStackCore::OpcUaVariant& targetVariant);
		bool convertModbusToOpcUa(uint16_t sourceValue, OpcUaStackCore::OpcUaVariant& targetVariant);
		bool convertOpcUaToModbus(OpcUaStackCore::OpcUaVariant& sourceVariant, bool& targetValue);
		bool convertOpcUaToModbus(OpcUaStackCore::OpcUaVariant& sourceVariant, uint16_t& targetValue);
	};

}

#endif
