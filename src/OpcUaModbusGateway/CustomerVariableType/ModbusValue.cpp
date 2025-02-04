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
    , address_Variable_(boost::make_shared<ServerVariable>("Address_Variable"))
    , precision_Variable_(boost::make_shared<ServerVariable>("Precision_Variable"))
    , rangeBegin_Variable_(boost::make_shared<ServerVariable>("RangeBegin_Variable"))
    , rangeEnd_Variable_(boost::make_shared<ServerVariable>("RangeEnd_Variable"))
    , unit_Variable_(boost::make_shared<ServerVariable>("Unit_Variable"))
    , variable_(boost::make_shared<ServerVariable>("Variable"))
    {
        variableTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        variableTypeNodeId((OpcUaUInt32)2003);
        setServerVariable(address_Variable_);
        setServerVariable(precision_Variable_);
        setServerVariable(rangeBegin_Variable_);
        setServerVariable(rangeEnd_Variable_);
        setServerVariable(unit_Variable_);
        setServerVariable(variable_);
    }
    
    ModbusValue::ModbusValue(const ModbusValue& value)
    : VariableBase()
    , address_Variable_(boost::make_shared<ServerVariable>("Address_Variable"))
    , precision_Variable_(boost::make_shared<ServerVariable>("Precision_Variable"))
    , rangeBegin_Variable_(boost::make_shared<ServerVariable>("RangeBegin_Variable"))
    , rangeEnd_Variable_(boost::make_shared<ServerVariable>("RangeEnd_Variable"))
    , unit_Variable_(boost::make_shared<ServerVariable>("Unit_Variable"))
    , variable_(boost::make_shared<ServerVariable>("Variable"))
    {
        variableTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        variableTypeNodeId((OpcUaUInt32)2003);
        setServerVariable(address_Variable_);
        setServerVariable(precision_Variable_);
        setServerVariable(rangeBegin_Variable_);
        setServerVariable(rangeEnd_Variable_);
        setServerVariable(unit_Variable_);
        setServerVariable(variable_);
    }
    
    ModbusValue::~ModbusValue(void)
    {
    }

    ServerVariable::SPtr&
    ModbusValue::address_Variable(void)
    {
        return address_Variable_;
    }

    ServerVariable::SPtr&
    ModbusValue::precision_Variable(void)
    {
        return precision_Variable_;
    }

    ServerVariable::SPtr&
    ModbusValue::rangeBegin_Variable(void)
    {
        return rangeBegin_Variable_;
    }

    ServerVariable::SPtr&
    ModbusValue::rangeEnd_Variable(void)
    {
        return rangeEnd_Variable_;
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
    ModbusValue::address_Variable(ServerVariable::SPtr& serverVariable)
    {
        address_Variable_ = serverVariable;
    }

    void
    ModbusValue::precision_Variable(ServerVariable::SPtr& serverVariable)
    {
        precision_Variable_ = serverVariable;
    }

    void
    ModbusValue::rangeBegin_Variable(ServerVariable::SPtr& serverVariable)
    {
        rangeBegin_Variable_ = serverVariable;
    }

    void
    ModbusValue::rangeEnd_Variable(ServerVariable::SPtr& serverVariable)
    {
        rangeEnd_Variable_ = serverVariable;
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
    ModbusValue::get_Address_Variable(OpcUaDataValue& dataValue)
    {
        return address_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::get_Precision_Variable(OpcUaDataValue& dataValue)
    {
        return precision_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::get_RangeBegin_Variable(OpcUaDataValue& dataValue)
    {
        return rangeBegin_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusValue::get_RangeEnd_Variable(OpcUaDataValue& dataValue)
    {
        return rangeEnd_Variable_->getDataValue(dataValue);
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
    ModbusValue::set_Address_Variable(const OpcUaDataValue& dataValue)
    {
        return address_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusValue::set_Precision_Variable(const OpcUaDataValue& dataValue)
    {
        return precision_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusValue::set_RangeBegin_Variable(const OpcUaDataValue& dataValue)
    {
        return rangeBegin_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusValue::set_RangeEnd_Variable(const OpcUaDataValue& dataValue)
    {
        return rangeEnd_Variable_->setDataValue(dataValue);
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
