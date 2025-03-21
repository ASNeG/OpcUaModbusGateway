/*
    VariableTypeClass: ModbusClientInterface

    Generated Source Code - please do not change this source code

    VariableTypeCodeGenerator Version:
        OpcUaStackCore - 4.0.0

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#include "OpcUaModbusGateway/CustomerObjectType/ModbusClientInterface.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{
    
    ModbusClientInterface::ModbusClientInterface(void)
    : ObjectBase()
    , address_Variable_(boost::make_shared<ServerVariable>("Address_Variable"))
    , modbusConnectionState_Variable_(boost::make_shared<ServerVariable>("ModbusConnectionState_Variable"))
    , readCoils_Method_(boost::make_shared<ServerMethod>("ReadCoils_Method"))
    , readDiscreteInputs_Method_(boost::make_shared<ServerMethod>("ReadDiscreteInputs_Method"))
    , readInputRegisters_Method_(boost::make_shared<ServerMethod>("ReadInputRegisters_Method"))
    , readMultiHoldingRegisters_Method_(boost::make_shared<ServerMethod>("ReadMultiHoldingRegisters_Method"))
    , writeMultipleCoils_Method_(boost::make_shared<ServerMethod>("WriteMultipleCoils_Method"))
    , writeMultipleHoldingRegisters_Method_(boost::make_shared<ServerMethod>("WriteMultipleHoldingRegisters_Method"))
    , writeSingleCoil_Method_(boost::make_shared<ServerMethod>("WriteSingleCoil_Method"))
    , writeSingleHoldingRegister_Method_(boost::make_shared<ServerMethod>("WriteSingleHoldingRegister_Method"))
    {
        objectTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        objectTypeNodeId((OpcUaUInt32)1003);
        setServerVariable(address_Variable_);
        setServerVariable(modbusConnectionState_Variable_);
        setServerMethod(readCoils_Method_);
        setServerMethod(readDiscreteInputs_Method_);
        setServerMethod(readInputRegisters_Method_);
        setServerMethod(readMultiHoldingRegisters_Method_);
        setServerMethod(writeMultipleCoils_Method_);
        setServerMethod(writeMultipleHoldingRegisters_Method_);
        setServerMethod(writeSingleCoil_Method_);
        setServerMethod(writeSingleHoldingRegister_Method_);
        readCoils_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadCoils_Method, this, boost::placeholders::_1));
        readDiscreteInputs_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadDiscreteInputs_Method, this, boost::placeholders::_1));
        readInputRegisters_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadInputRegisters_Method, this, boost::placeholders::_1));
        readMultiHoldingRegisters_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadMultiHoldingRegisters_Method, this, boost::placeholders::_1));
        writeMultipleCoils_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteMultipleCoils_Method, this, boost::placeholders::_1));
        writeMultipleHoldingRegisters_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteMultipleHoldingRegisters_Method, this, boost::placeholders::_1));
        writeSingleCoil_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteSingleCoil_Method, this, boost::placeholders::_1));
        writeSingleHoldingRegister_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteSingleHoldingRegister_Method, this, boost::placeholders::_1));
    }
    
    ModbusClientInterface::ModbusClientInterface(const ModbusClientInterface& value)
    : ObjectBase()
    , address_Variable_(boost::make_shared<ServerVariable>("Address_Variable"))
    , modbusConnectionState_Variable_(boost::make_shared<ServerVariable>("ModbusConnectionState_Variable"))
    , readCoils_Method_(boost::make_shared<ServerMethod>("ReadCoils_Method"))
    , readDiscreteInputs_Method_(boost::make_shared<ServerMethod>("ReadDiscreteInputs_Method"))
    , readInputRegisters_Method_(boost::make_shared<ServerMethod>("ReadInputRegisters_Method"))
    , readMultiHoldingRegisters_Method_(boost::make_shared<ServerMethod>("ReadMultiHoldingRegisters_Method"))
    , writeMultipleCoils_Method_(boost::make_shared<ServerMethod>("WriteMultipleCoils_Method"))
    , writeMultipleHoldingRegisters_Method_(boost::make_shared<ServerMethod>("WriteMultipleHoldingRegisters_Method"))
    , writeSingleCoil_Method_(boost::make_shared<ServerMethod>("WriteSingleCoil_Method"))
    , writeSingleHoldingRegister_Method_(boost::make_shared<ServerMethod>("WriteSingleHoldingRegister_Method"))
    {
        objectTypeNamespaceName("http://ASNEG.de/OpcUaModbusGateway/");
        objectTypeNodeId((OpcUaUInt32)1003);
        setServerVariable(address_Variable_);
        setServerVariable(modbusConnectionState_Variable_);
        setServerMethod(readCoils_Method_);
        setServerMethod(readDiscreteInputs_Method_);
        setServerMethod(readInputRegisters_Method_);
        setServerMethod(readMultiHoldingRegisters_Method_);
        setServerMethod(writeMultipleCoils_Method_);
        setServerMethod(writeMultipleHoldingRegisters_Method_);
        setServerMethod(writeSingleCoil_Method_);
        setServerMethod(writeSingleHoldingRegister_Method_);
        readCoils_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadCoils_Method, this, boost::placeholders::_1));
        readDiscreteInputs_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadDiscreteInputs_Method, this, boost::placeholders::_1));
        readInputRegisters_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadInputRegisters_Method, this, boost::placeholders::_1));
        readMultiHoldingRegisters_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_ReadMultiHoldingRegisters_Method, this, boost::placeholders::_1));
        writeMultipleCoils_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteMultipleCoils_Method, this, boost::placeholders::_1));
        writeMultipleHoldingRegisters_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteMultipleHoldingRegisters_Method, this, boost::placeholders::_1));
        writeSingleCoil_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteSingleCoil_Method, this, boost::placeholders::_1));
        writeSingleHoldingRegister_Method_->registerMethod(boost::bind(&ModbusClientInterface::call_WriteSingleHoldingRegister_Method, this, boost::placeholders::_1));
    }
    
    ModbusClientInterface::~ModbusClientInterface(void)
    {
    }

    ServerVariable::SPtr&
    ModbusClientInterface::address_Variable(void)
    {
        return address_Variable_;
    }

    ServerVariable::SPtr&
    ModbusClientInterface::modbusConnectionState_Variable(void)
    {
        return modbusConnectionState_Variable_;
    }

    void
    ModbusClientInterface::address_Variable(ServerVariable::SPtr& serverVariable)
    {
        address_Variable_ = serverVariable;
    }

    void
    ModbusClientInterface::modbusConnectionState_Variable(ServerVariable::SPtr& serverVariable)
    {
        modbusConnectionState_Variable_ = serverVariable;
    }

    bool
    ModbusClientInterface::get_Address_Variable(OpcUaDataValue& dataValue)
    {
        return address_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusClientInterface::get_ModbusConnectionState_Variable(OpcUaDataValue& dataValue)
    {
        return modbusConnectionState_Variable_->getDataValue(dataValue);
    }

    bool
    ModbusClientInterface::set_Address_Variable(const OpcUaDataValue& dataValue)
    {
        return address_Variable_->setDataValue(dataValue);
    }

    bool
    ModbusClientInterface::set_ModbusConnectionState_Variable(const OpcUaDataValue& dataValue)
    {
        return modbusConnectionState_Variable_->setDataValue(dataValue);
    }

    void
    ModbusClientInterface::call_ReadCoils_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_ReadDiscreteInputs_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_ReadInputRegisters_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_ReadMultiHoldingRegisters_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_WriteMultipleCoils_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_WriteMultipleHoldingRegisters_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_WriteSingleCoil_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

    void
    ModbusClientInterface::call_WriteSingleHoldingRegister_Method(ApplicationMethodContext* applicationMethodContext)
    {
        applicationMethodContext->statusCode_ = BadNotSupported;
    }

}
