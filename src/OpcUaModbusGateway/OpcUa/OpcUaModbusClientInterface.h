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

#ifndef __OpcUaModbusGateway_OpcUaModbusClientInterface_h__
#define __OpcUaModbusGateway_OpcUaModbusClientInterface_h__

#include <vector>

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/CustomerObjectType/ModbusClientInterface.h"
#include "OpcUaModbusGateway/Application/ModbusTCPClient.h"

namespace OpcUaModbusGateway
{
   
   class OpcUaModbusClientInterface
   : public ModbusClientInterface
   , public boost::enable_shared_from_this<OpcUaModbusClientInterface>
   {
     public:
       using SPtr = boost::shared_ptr<OpcUaModbusClientInterface>;
       using Vec = std::vector<SPtr>;
   
        OpcUaModbusClientInterface(void);
        virtual ~OpcUaModbusClientInterface(void);

		bool init(
			ModbusTCPClientConfig::SPtr modbusTCPClientConfig,
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
			const std::string& namespaceName
		);

    	bool addToOpcUaModel(
    		uint32_t parentId,
    		const OpcUaStackCore::OpcUaNodeId& referenceTypeNodeId
    	);
    	bool deleteFromOpcUaModel(void);

    	void setModbusConnectionState(
    		const std::string& setModbusConnectionState
    	);

        virtual void call_ReadCoils_Method(
            OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_ReadDiscreteInputs_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_ReadInputRegisters_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_ReadMultiHoldingRegisters_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_WriteMultipleCoils_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_WriteMultipleHoldingRegisters_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_WriteSingleCoil_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;

        virtual void call_WriteSingleHoldingRegister_Method(
        	OpcUaStackCore::ApplicationMethodContext* applicationMethodContext
		) override;
    
      private:
        ModbusTCPClient::SPtr modbusTCPClient_ = nullptr;
        ModbusTCPClientConfig::SPtr modbusTCPClientConfig_;
        OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;

        std::string namespaceName_ = "";
        uint32_t namespaceIndex_ = 0;

        void createResult(
        	OpcUaStackCore::OpcUaVariantArray::SPtr& outputArguments,
        	uint32_t resultCode
        );
   };

}

#endif
