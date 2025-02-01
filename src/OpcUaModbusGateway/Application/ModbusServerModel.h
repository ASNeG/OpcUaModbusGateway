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

#ifndef __OpcUaModbusGateway_ModbusServerModel_h__
#define __OpcUaModbusGateway_ModbusServerModel_h__

#include <memory>
#include <map>

#include "ModbusProt/ModbusModelInterface.h"

namespace OpcUaModbusGateway
{

	class RegisterEntry
	{
	  public:
		using SPtr = std::shared_ptr<RegisterEntry>;
		using Map = std::map<uint16_t, SPtr>;

		using SetBoolCallback = std::function<void (bool value)>;
		using GetBoolCallback = std::function<void (bool& value)>;
		using SetUInt16Callback = std::function<void (uint16_t value)>;
		using GetUInt16Callback = std::function<void (bool& value)>;

		SetBoolCallback setBoolCallback_;
		GetBoolCallback getBoolCallback_;
		SetUInt16Callback setUInt16Callback_;
		GetUInt16Callback getUInt16Callback_;
	};

	class ModbusServerModel
	: public ModbusProt::ModbusModelInterface
	{
	  public:
		using SPtr = std::shared_ptr<ModbusServerModel>;
		using Vec = std::vector<SPtr>;

		ModbusServerModel(void);
		~ModbusServerModel(void);

		bool registerCoils(
			uint16_t id,
			RegisterEntry::SetBoolCallback setBoolCallback,
			RegisterEntry::GetBoolCallback getBoolCallback
		);
		bool registerInputs(
			uint16_t id,
			RegisterEntry::GetBoolCallback getBoolCallback
		);
		bool registerHoldingRegisters(
			uint16_t id,
			RegisterEntry::SetUInt16Callback setUInt16Callback,
			RegisterEntry::GetUInt16Callback getUInt16Callback
		);
		bool registerInputTegisters(
			uint16_t id,
			RegisterEntry::GetUInt16Callback getUInt16Callback
		);

		bool checkType(ModbusProt::MemoryType memoryType) override;
		bool checkAddress(ModbusProt::MemoryType memoryType, uint16_t startAddress, uint16_t numValues) override;
		bool setValue(ModbusProt::MemoryType memoryType, uint16_t startAddress, uint8_t* values, uint16_t numValues) override;
		bool getValue(ModbusProt::MemoryType memoryType, uint16_t startAddress, uint8_t* values, uint16_t numValues) override;

	  private:
		RegisterEntry::Map coilsMap_;
		RegisterEntry::Map inputsMap_;
		RegisterEntry::Map holdingRegistersMap_;
		RegisterEntry::Map inputRegistersMap_;
	};

}

#endif
