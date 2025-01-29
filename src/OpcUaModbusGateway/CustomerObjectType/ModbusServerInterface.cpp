/*
    VariableTypeClass: ModbusServerInterface

    Generated Source Code - please do not change this source code

    VariableTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#include "OpcUaModbusGateway/CustomerObjectType/ModbusServerInterface.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{
    
    ModbusServerInterface::ModbusServerInterface(void)
    : ObjectBase()
    , address_Variable_(boost::make_shared<ServerVariable>("Address_Variable"))
    {
        objectTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        objectTypeNodeId((OpcUaUInt32)1007);
        setServerVariable(address_Variable_);
    }
    
    ModbusServerInterface::ModbusServerInterface(const ModbusServerInterface& value)
    : ObjectBase()
    , address_Variable_(boost::make_shared<ServerVariable>("Address_Variable"))
    {
        objectTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        objectTypeNodeId((OpcUaUInt32)1007);
        setServerVariable(address_Variable_);
    }
    
    ModbusServerInterface::~ModbusServerInterface(void)
    {
    }

    ServerVariable::SPtr&
    ModbusServerInterface::address_Variable(void)
    {
        return address_Variable_;
    }

    void
    ModbusServerInterface::address_Variable(ServerVariable::SPtr& serverVariable)
    {
        address_Variable_ = serverVariable;
    }

    bool
    ModbusServerInterface::get_Address_Variable(OpcUaDataValue& dataValue)
    {
        return address_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusServerInterface::set_Address_Variable(const OpcUaDataValue& dataValue)
    {
        return address_Variable_->setDataValue(dataValue);
    }

}
