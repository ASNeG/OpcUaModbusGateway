#!/bin/bash

NODESET="--nodeset ../Config/Opc.Ua.NodeSet2.xml --nodeset ../Config/Opc.Ua.NodeSet.ModbusGateway.xml"
PARAS="--namespaces 1:OpcUaModbusGateway"

OpcUaVariableTypeGenerator4 ${NODESET} ${PARAS} --variabletype ModbusValue
