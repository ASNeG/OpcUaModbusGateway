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
	: ModbusProt::ModbusModelInterface()
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

	bool
	ModbusServerModel::checkType(ModbusProt::MemoryType memoryType)
	{
		switch (memoryType)
		{
			case ModbusProt::MemoryType::Coils: return coilsMap_.size() != 0;
			case ModbusProt::MemoryType::Inputs: return inputsMap_.size() != 0;
			case ModbusProt::MemoryType::HoldingRegisters: return holdingRegistersMap_.size() != 0;
			case ModbusProt::MemoryType::InputRegisters: return inputRegistersMap_.size() != 0;
		}
		return false;
	}

	bool
	ModbusServerModel::checkAddress(RegisterEntry::Map& registerMap, uint16_t startAddress, uint16_t numValues)
	{
		for (uint16_t idx = 0; idx < numValues; idx++) {
			auto it = registerMap.find(startAddress + idx);
			if (it == registerMap.end()) return false;
		}
		return true;
	}

	bool
	ModbusServerModel::checkAddress(ModbusProt::MemoryType memoryType, uint16_t startAddress, uint16_t numValues)
	{
		switch (memoryType)
		{
			case ModbusProt::MemoryType::Coils: return checkAddress(coilsMap_, startAddress, numValues);
			case ModbusProt::MemoryType::Inputs: return checkAddress(inputsMap_, startAddress, numValues);
			case ModbusProt::MemoryType::HoldingRegisters: return checkAddress(holdingRegistersMap_, startAddress, numValues);
			case ModbusProt::MemoryType::InputRegisters: return checkAddress(inputRegistersMap_, startAddress, numValues);
		}
		return false;
	}

	bool
	ModbusServerModel::setValueBoolean(RegisterEntry::Map& registerMap, uint16_t startAddress, uint8_t* values, uint16_t numValues)
	{
		return false;
	}

	bool
	ModbusServerModel::setValueUInt16(RegisterEntry::Map& registerMap, uint16_t startAddress, uint8_t* values, uint16_t numValues)
	{
		return false;
	}

	bool
	ModbusServerModel::setValue(ModbusProt::MemoryType memoryType, uint16_t startAddress, uint8_t* values, uint16_t numValues)
	{
		switch (memoryType)
		{
			case ModbusProt::MemoryType::Coils: return setValueBoolean(coilsMap_, startAddress, values, numValues);
			case ModbusProt::MemoryType::Inputs: return setValueBoolean(inputsMap_, startAddress, values, numValues);
			case ModbusProt::MemoryType::HoldingRegisters: return setValueUInt16(holdingRegistersMap_, startAddress, values, numValues);
			case ModbusProt::MemoryType::InputRegisters: return setValueUInt16(inputRegistersMap_, startAddress, values, numValues);
		}
		return false;
	}

	bool
	ModbusServerModel::getValueBoolean(RegisterEntry::Map& registerMap, uint16_t startAddress, uint8_t* values, uint16_t numValues)
	{
		return false;
	}

	bool
	ModbusServerModel::getValueUInt16(RegisterEntry::Map& registerMap, uint16_t startAddress, uint8_t* values, uint16_t numValues)
	{
		return false;
	}

	bool
	ModbusServerModel::getValue(ModbusProt::MemoryType memoryType, uint16_t startAddress, uint8_t* values, uint16_t numValues)
	{
		switch (memoryType)
		{
			case ModbusProt::MemoryType::Coils: return getValueBoolean(coilsMap_, startAddress, values, numValues);
			case ModbusProt::MemoryType::Inputs: return getValueBoolean(inputsMap_, startAddress, values, numValues);
			case ModbusProt::MemoryType::HoldingRegisters: return getValueUInt16(holdingRegistersMap_, startAddress, values, numValues);
			case ModbusProt::MemoryType::InputRegisters: return getValueUInt16(inputRegistersMap_, startAddress, values, numValues);
		}
		return false;
	}


}

