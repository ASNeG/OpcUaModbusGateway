/*
    ObjectTypeClass: ModbusClientInterface

    Generated Source Code - please do not change this source code

    ObjectTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor:     Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaModbusGateway_ModbusClientInterface_h__
#define __OpcUaModbusGateway_ModbusClientInterface_h__

#include "OpcUaStackServer/StandardVariableType/VariableBase.h"
#include "OpcUaStackServer/StandardObjectType/ObjectBase.h"

namespace OpcUaModbusGateway
{
   
   class DLLEXPORT ModbusClientInterface
   : public OpcUaStackServer::ObjectBase
   {
     public:
       typedef boost::shared_ptr<ModbusClientInterface> SPtr;
       typedef std::vector<ModbusClientInterface::SPtr> Vec;
   
       ModbusClientInterface(void);
       ModbusClientInterface(const ModbusClientInterface& value);
       virtual ~ModbusClientInterface(void);

        //
        // String
        //
        void modbusConnectionState_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& modbusConnectionState_Variable(void);
        bool get_ModbusConnectionState_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_ModbusConnectionState_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

        virtual void call_ReadCoils_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_ReadDiscreteInputs_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_ReadInputRegisters_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_ReadMultiHoldingRegisters_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_WriteMultipleCoils_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_WriteMultipleHoldingRegisters_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_WriteSingleCoil_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);

        virtual void call_WriteSingleHoldingRegister_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext);
    
      private:
        OpcUaStackServer::ServerVariable::SPtr modbusConnectionState_Variable_;
        OpcUaStackServer::ServerMethod::SPtr readCoils_Method_;
        OpcUaStackServer::ServerMethod::SPtr readDiscreteInputs_Method_;
        OpcUaStackServer::ServerMethod::SPtr readInputRegisters_Method_;
        OpcUaStackServer::ServerMethod::SPtr readMultiHoldingRegisters_Method_;
        OpcUaStackServer::ServerMethod::SPtr writeMultipleCoils_Method_;
        OpcUaStackServer::ServerMethod::SPtr writeMultipleHoldingRegisters_Method_;
        OpcUaStackServer::ServerMethod::SPtr writeSingleCoil_Method_;
        OpcUaStackServer::ServerMethod::SPtr writeSingleHoldingRegister_Method_;
   
   };

}

#endif
