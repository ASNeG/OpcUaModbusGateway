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

#ifndef __OpcUaModbusGateway_OpcUaModbusServerInterface_h__
#define __OpcUaModbusGateway_OpcUaModbusServerInterface_h__

#include <vector>

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/CustomerObjectType/ModbusServerInterface.h"
#include "OpcUaModbusGateway/Application/ModbusTCPServer.h"
#include "OpcUaModbusGateway/Application/ModbusServerModel.h"
#include "OpcUaModbusGateway/OpcUa/OpcUaModbusServerRegister.h"

namespace OpcUaModbusGateway
{
   
   class OpcUaModbusServerInterface
   : public ModbusServerInterface
   , public boost::enable_shared_from_this<OpcUaModbusServerInterface>
   {
     public:
       using SPtr = boost::shared_ptr<OpcUaModbusServerInterface>;
       using Vec = std::vector<SPtr>;
   
        OpcUaModbusServerInterface(void);
        virtual ~OpcUaModbusServerInterface(void);

		bool init(
			ModbusTCPServerConfig::SPtr modbusTCPServerConfig,
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
			const std::string& namespaceName
		);

    	bool addToOpcUaModel(
    		uint32_t parentId,
    		const OpcUaStackCore::OpcUaNodeId& referenceTypeNodeId
    	);
    	bool deleteFromOpcUaModel(void);

      private:
        ModbusTCPServer::SPtr modbusTCPServer_ = nullptr;
        ModbusTCPServerConfig::SPtr modbusTCPServerConfig_;
        OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
        ModbusServerModel::SPtr modbusServerModel_ = nullptr;

        OpcUaModbusServerRegister modbusServerRegister_;

        std::string namespaceName_ = "";
        uint32_t namespaceIndex_ = 0;
   };

}

#endif
