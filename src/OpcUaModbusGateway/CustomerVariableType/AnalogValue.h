/*
    VariableTypeClass: AnalogValue

    Generated Source Code - please do not change this source code

    VariableTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor:     Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaModbusGateway_AnalogValue_h__
#define __OpcUaModbusGateway_AnalogValue_h__

#include "OpcUaStackServer/StandardVariableType/VariableBase.h"

namespace OpcUaModbusGateway
{
   
   class AnalogValue
   : public OpcUaStackServer::VariableBase
   {
     public:
       typedef boost::shared_ptr<AnalogValue> SPtr;
       typedef std::vector<AnalogValue::SPtr> Vec;
   
       AnalogValue(void);
       AnalogValue(const AnalogValue& value);
       virtual ~AnalogValue(void);

        //
        // Double
        //
        void precision_Variable(OpcUaStackServer::ServerVariable::SPtr& serverVariable);
        OpcUaStackServer::ServerVariable::SPtr& precision_Variable(void);
        bool get_Precision_Variable(OpcUaStackCore::OpcUaDataValue& dataValue);
        bool set_Precision_Variable(const OpcUaStackCore::OpcUaDataValue& dataValue);

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
        OpcUaStackServer::ServerVariable::SPtr precision_Variable_;
        OpcUaStackServer::ServerVariable::SPtr unit_Variable_;
        OpcUaStackServer::ServerVariable::SPtr variable_;
   
   };

}

#endif
