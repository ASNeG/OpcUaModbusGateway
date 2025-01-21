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

#include "OpcUaModbusGateway/Application/ModbusTCPClient.h"
#include "OpcUaModbusGateway/Application/ModbusTCPClientImpl.h"

namespace OpcUaModbusGateway
{

	ModbusTCPClient::ModbusTCPClient(void)
	{
		modbusTCPClientImpl_ = new ModbusTCPClientImpl();
	}

	ModbusTCPClient::~ModbusTCPClient(void)
	{
		delete modbusTCPClientImpl_;
	}

	bool
	ModbusTCPClient::connect(ModbusTCPClientConfig::SPtr& modbusTCPClientConfig)
	{
		return modbusTCPClientImpl_->connect(modbusTCPClientConfig);
	}

	bool
	ModbusTCPClient::disconnect(void)
	{
		return modbusTCPClientImpl_->disconnect();
	}

	void
	ModbusTCPClient::readCoils(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<bool>& coilStatus
	)
	{
		// FIXME: Test
		errorCode = 0;
		for (uint32_t idx = 0; idx < quantityOfInputs; idx++) {
			coilStatus.push_back(true);
		}
	}

	void
	ModbusTCPClient::readDiscreteInputs(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<bool>& inputStatus
	)
	{
		// FIXME: Test
		errorCode = 0;
		for (uint32_t idx = 0; idx < quantityOfInputs; idx++) {
			inputStatus.push_back(true);
		}
	}

	void
	ModbusTCPClient::readInputRegisters(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<uint16_t>& inputRegisters
	)
	{
		// FIXME: Test
		errorCode = 0;
		for (uint32_t idx = 0; idx < quantityOfInputs; idx++) {
			inputRegisters.push_back((uint16_t)idx);
		}
	}

	void
	ModbusTCPClient::readHoldingRegisters(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<uint16_t>& holdingRegisters
	)
	{
		// FIXME: Test
		errorCode = 0;
		for (uint32_t idx = 0; idx < quantityOfInputs; idx++) {
			holdingRegisters.push_back((uint16_t)idx);
		}
	}

	void
	ModbusTCPClient::writeMultipleCoils(
		uint16_t startingAddress,
		std::vector<bool>& coils,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		// FIXME: Test
		errorCode = 0;
		count = coils.size();
	}

	void
	ModbusTCPClient::writeMultipleHoldingRegisters(
		uint16_t startingAddress,
		std::vector<uint16_t>& holdingRegisters,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		// FIXME: Test
		errorCode = 0;
		count = holdingRegisters.size();
	}

	void
	ModbusTCPClient::writeSingleCoil(
		uint16_t startingAddress,
		bool value,
		uint32_t& errorCode
	)
	{
		// FIXME: Test
		errorCode = 0;
	}


}

