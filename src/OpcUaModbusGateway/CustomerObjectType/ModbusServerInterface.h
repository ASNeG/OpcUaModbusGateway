/*
    ObjectTypeClass: ModbusServerInterface

    Generated Source Code - please do not change this source code

    ObjectTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor:     Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaModbusGateway_ModbusServerInterface_h__
#define __OpcUaModbusGateway_ModbusServerInterface_h__

#include "OpcUaStackServer/StandardVariableType/VariableBase.h"
#include "OpcUaStackServer/StandardObjectType/ObjectBase.h"

namespace OpcUaModbusGateway
{
   
   class DLLEXPORT ModbusServerInterface
   : public OpcUaStackServer::ObjectBase
   {
     public:
       typedef boost::shared_ptr<ModbusServerInterface> SPtr;
       typedef std::vector<ModbusServerInterface::SPtr> Vec;
   
       ModbusServerInterface(void);
       ModbusServerInterface(const ModbusServerInterface& value);
       virtual ~ModbusServerInterface(void);

        //
        // String
        //
        void address_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& address_Variable(void);
        bool get_Address_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_Address_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);
    
      private:
        OpcUaStackServer::ServerVariable::SPtr address_Variable_;
   
   };

}

#endif
