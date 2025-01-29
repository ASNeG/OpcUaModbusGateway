/*
    VariableTypeClass: ModbusValue

    Generated Source Code - please do not change this source code

    VariableTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#include "OpcUaModbusGateway/CustomerVariableType/ModbusValue.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{
    
    ModbusValue::ModbusValue(void)
    : VariableBase()
    , precision_Variable_(boost::make_shared<ServerVariable>("Precision_Variable"))
    , register_Variable_(boost::make_shared<ServerVariable>("Register_Variable"))
    , unit_Variable_(boost::make_shared<ServerVariable>("Unit_Variable"))
    , variable_(boost::make_shared<ServerVariable>("Variable"))
    {
        variableTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        variableTypeNodeId((OpcUaUInt32)2003);
        setServerVariable(precision_Variable_);
        setServerVariable(register_Variable_);
        setServerVariable(unit_Variable_);
        setServerVariable(variable_);
    }
    
    ModbusValue::ModbusValue(const ModbusValue& value)
    : VariableBase()
    , precision_Variable_(boost::make_shared<ServerVariable>("Precision_Variable"))
    , register_Variable_(boost::make_shared<ServerVariable>("Register_Variable"))
    , unit_Variable_(boost::make_shared<ServerVariable>("Unit_Variable"))
    , variable_(boost::make_shared<ServerVariable>("Variable"))
    {
        variableTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        variableTypeNodeId((OpcUaUInt32)2003);
        setServerVariable(precision_Variable_);
        setServerVariable(register_Variable_);
        setServerVariable(unit_Variable_);
        setServerVariable(variable_);
    }
    
    ModbusValue::~ModbusValue(void)
    {
    }

    ServerVariable::SPtr&
    ModbusValue::precision_Variable(void)
    {
        return precision_Variable_;
    }

    ServerVariable::SPtr&
    ModbusValue::register_Variable(void)
    {
        return register_Variable_;
    }

    ServerVariable::SPtr&
    ModbusValue::unit_Variable(void)
    {
        return unit_Variable_;
    }

    ServerVariable::SPtr&
    ModbusValue::variable(void)
    {
        return variable_;
    }

    void
    ModbusValue::precision_Variable(ServerVariable::SPtr& serverVariable)
    {
        precision_Variable_ = serverVariable;
    }

    void
    ModbusValue::register_Variable(ServerVariable::SPtr& serverVariable)
    {
        register_Variable_ = serverVariable;
    }

    void
    ModbusValue::unit_Variable(ServerVariable::SPtr& serverVariable)
    {
        unit_Variable_ = serverVariable;
    }

    void
    ModbusValue::variable(ServerVariable::SPtr& serverVariable)
    {
        variable_ = serverVariable;
    }

    bool
    ModbusValue::get_Precision_Variable(OpcUaDataValue& dataValue)
    {
        return precision_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::get_Register_Variable(OpcUaDataValue& dataValue)
    {
        return register_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::get_Unit_Variable(OpcUaDataValue& dataValue)
    {
        return unit_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::get_Variable(OpcUaDataValue& dataValue)
    {
        return variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::set_Precision_Variable(const OpcUaDataValue& dataValue)
    {
        return precision_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusValue::set_Register_Variable(const OpcUaDataValue& dataValue)
    {
        return register_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusValue::set_Unit_Variable(const OpcUaDataValue& dataValue)
    {
        return unit_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusValue::set_Variable(const OpcUaDataValue& dataValue)
    {
        return variable_->setDataValue(dataValue);
    }

}
