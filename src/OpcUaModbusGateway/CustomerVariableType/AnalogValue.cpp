/*
    VariableTypeClass: AnalogValue

    Generated Source Code - please do not change this source code

    VariableTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#include "OpcUaModbusGateway/CustomerVariableType/AnalogValue.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{
    
    AnalogValue::AnalogValue(void)
    : VariableBase()
    , precision_Variable_(boost::make_shared<ServerVariable>("Precision_Variable"))
    , unit_Variable_(boost::make_shared<ServerVariable>("Unit_Variable"))
    , variable_(boost::make_shared<ServerVariable>("Variable"))
    {
        variableTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        variableTypeNodeId((OpcUaUInt32)2003);
        setServerVariable(precision_Variable_);
        setServerVariable(unit_Variable_);
        setServerVariable(variable_);
    }
    
    AnalogValue::AnalogValue(const AnalogValue& value)
    : VariableBase()
    , precision_Variable_(boost::make_shared<ServerVariable>("Precision_Variable"))
    , unit_Variable_(boost::make_shared<ServerVariable>("Unit_Variable"))
    , variable_(boost::make_shared<ServerVariable>("Variable"))
    {
        variableTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        variableTypeNodeId((OpcUaUInt32)2003);
        setServerVariable(precision_Variable_);
        setServerVariable(unit_Variable_);
        setServerVariable(variable_);
    }
    
    AnalogValue::~AnalogValue(void)
    {
    }

    ServerVariable::SPtr&
    AnalogValue::precision_Variable(void)
    {
        return precision_Variable_;
    }

    ServerVariable::SPtr&
    AnalogValue::unit_Variable(void)
    {
        return unit_Variable_;
    }

    ServerVariable::SPtr&
    AnalogValue::variable(void)
    {
        return variable_;
    }

    void
    AnalogValue::precision_Variable(ServerVariable::SPtr& serverVariable)
    {
        precision_Variable_ = serverVariable;
    }

    void
    AnalogValue::unit_Variable(ServerVariable::SPtr& serverVariable)
    {
        unit_Variable_ = serverVariable;
    }

    void
    AnalogValue::variable(ServerVariable::SPtr& serverVariable)
    {
        variable_ = serverVariable;
    }

    bool
    AnalogValue::get_Precision_Variable(OpcUaDataValue& dataValue)
    {
        return precision_Variable_->getDataValue(dataValue);
    }

    bool
    AnalogValue::get_Unit_Variable(OpcUaDataValue& dataValue)
    {
        return unit_Variable_->getDataValue(dataValue);
    }

    bool
    AnalogValue::get_Variable(OpcUaDataValue& dataValue)
    {
        return variable_->getDataValue(dataValue);
    }

    bool
    AnalogValue::set_Precision_Variable(const OpcUaDataValue& dataValue)
    {
        return precision_Variable_->setDataValue(dataValue);
    }

    bool
    AnalogValue::set_Unit_Variable(const OpcUaDataValue& dataValue)
    {
        return unit_Variable_->setDataValue(dataValue);
    }

    bool
    AnalogValue::set_Variable(const OpcUaDataValue& dataValue)
    {
        return variable_->setDataValue(dataValue);
    }

}
