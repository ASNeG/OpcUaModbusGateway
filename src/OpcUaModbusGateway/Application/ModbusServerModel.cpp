/*
   Copyright 2025 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <functional>

#include "OpcUaModbusGateway/Application/ModbusServerModel.h"

namespace OpcUaModbusGateway
{

	ModbusServerModel::ModbusServerModel(void)
	{
	}

	ModbusServerModel::~ModbusServerModel(void)
	{
	}

	bool
	ModbusServerModel::registerCoils(
		uint16_t id,
		RegisterEntry::SetBoolCallback setBoolCallback,
		RegisterEntry::GetBoolCallback getBoolCallback
	)
	{
		auto registerEntry = std::make_shared<RegisterEntry>();
		registerEntry->setBoolCallback_ = setBoolCallback;
		registerEntry->getBoolCallback_ = getBoolCallback;
		return coilsMap_.insert_or_assign(id, registerEntry).second;
	}

	bool
	ModbusServerModel::registerInputs(
		uint16_t id,
		RegisterEntry::GetBoolCallback getBoolCallback
	)
	{
		auto registerEntry = std::make_shared<RegisterEntry>();
		registerEntry->getBoolCallback_ = getBoolCallback;
		return inputsMap_.insert_or_assign(id, registerEntry).second;
	}

	bool
	ModbusServerModel::registerHoldingRegisters(
		uint16_t id,
		RegisterEntry::SetUInt16Callback setUInt16Callback,
		RegisterEntry::GetUInt16Callback getUInt16Callback
	)
	{
		auto registerEntry = std::make_shared<RegisterEntry>();
		registerEntry->setUInt16Callback_ = setUInt16Callback;
		registerEntry->getUInt16Callback_ = getUInt16Callback;
		return holdingRegistersMap_.insert_or_assign(id, registerEntry).second;
	}

	bool
	ModbusServerModel::registerInputTegisters(
		uint16_t id,
		RegisterEntry::GetUInt16Callback getUInt16Callback
	)
	{
		auto registerEntry = std::make_shared<RegisterEntry>();
		registerEntry->getUInt16Callback_ = getUInt16Callback;
		return inputRegistersMap_.insert_or_assign(id, registerEntry).second;
	}

}

