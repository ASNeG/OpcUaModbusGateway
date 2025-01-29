#!/bin/bash

NODESET="--nodeset ../Config/Opc.Ua.NodeSet2.xml --nodeset ../Config/Opc.Ua.NodeSet.ModbusGateway.xml"
PARAS="--namespaces 1:OpcUaModbusGateway"

OpcUaObjectTypeGenerator4 ${NODESET} ${PARAS} --objecttype ModbusClientInterface
OpcUaObjectTypeGenerator4 ${NODESET} ${PARAS} --objecttype ModbusServerInterface
