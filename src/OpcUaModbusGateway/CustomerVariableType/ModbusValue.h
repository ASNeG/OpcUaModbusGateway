/*
    VariableTypeClass: ModbusValue

    Generated Source Code - please do not change this source code

    VariableTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor:     Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaModbusGateway_ModbusValue_h__
#define __OpcUaModbusGateway_ModbusValue_h__

#include "OpcUaStackServer/StandardVariableType/VariableBase.h"

namespace OpcUaModbusGateway
{
   
   class DLLEXPORT ModbusValue
   : public OpcUaStackServer::VariableBase
   {
     public:
       typedef boost::shared_ptr<ModbusValue> SPtr;
       typedef std::vector<ModbusValue::SPtr> Vec;
   
       ModbusValue(void);
       ModbusValue(const ModbusValue& value);
       virtual ~ModbusValue(void);

        //
        // UInt16
        //
        void address_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& address_Variable(void);
        bool get_Address_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_Address_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

        //
        // Double
        //
        void precision_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& precision_Variable(void);
        bool get_Precision_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_Precision_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

        //
        // Double
        //
        void rangeBegin_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& rangeBegin_Variable(void);
        bool get_RangeBegin_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_RangeBegin_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

        //
        // Double
        //
        void rangeEnd_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& rangeEnd_Variable(void);
        bool get_RangeEnd_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_RangeEnd_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

        //
        // String
        //
        void unit_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& unit_Variable(void);
        bool get_Unit_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_Unit_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

        //
        // Variant
        //
        void variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& variable(void);
        bool get_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);
    
      private:
        OpcUaStackServer::ServerVariable::SPtr address_Variable_;
        OpcUaStackServer::ServerVariable::SPtr precision_Variable_;
        OpcUaStackServer::ServerVariable::SPtr rangeBegin_Variable_;
        OpcUaStackServer::ServerVariable::SPtr rangeEnd_Variable_;
        OpcUaStackServer::ServerVariable::SPtr unit_Variable_;
        OpcUaStackServer::ServerVariable::SPtr variable_;
   
   };

}

#endif
