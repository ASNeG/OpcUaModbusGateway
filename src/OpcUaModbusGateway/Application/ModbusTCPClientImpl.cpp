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

#include "OpcUaModbusGateway/Application/ModbusTCPClientImpl.h"

namespace OpcUaModbusGateway
{

	ModbusTCPClientImpl::ModbusTCPClientImpl(void)
	{
	}

	ModbusTCPClientImpl::~ModbusTCPClientImpl(void)
	{
	}

	bool
	ModbusTCPClientImpl::connect(ModbusTCPClientConfig::SPtr& modbusTCPClientConfig)
	{
		bool rc = true;

		// Set reconnect timeout
		modbusTCPClient_.reconnectTimeout(5000);

		// Create client endpoint
		rc = modbusTCPClient_.getEndpoint(
			modbusTCPClientConfig->ipAddress(),
			modbusTCPClientConfig->port(),
			serverEndpoint_
		);
		if (!rc) {
			return false;
		}

		// Connect to modbus tcp server
		modbusTCPClient_.connect(
			serverEndpoint_,
			[this](ModbusTCP::TCPClientState clientState) {
				clientConnectionHandler(clientState);
		});

		return true;
	}

	bool ModbusTCPClientImpl::disconnect(void)
	{
		modbusTCPClient_.disconnect();
		return true;
	}

	void ModbusTCPClientImpl::clientConnectionHandler(
		ModbusTCP::TCPClientState clientState
	)
	{
		modbusTCPClientState_ = clientState;
	}

	void
	ModbusTCPClientImpl::readCoils(
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
	ModbusTCPClientImpl::readDiscreteInputs(
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
	ModbusTCPClientImpl::readInputRegisters(
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
	ModbusTCPClientImpl::readHoldingRegisters(
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
	ModbusTCPClientImpl::writeMultipleCoils(
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
	ModbusTCPClientImpl::writeMultipleHoldingRegisters(
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
	ModbusTCPClientImpl::writeSingleCoil(
		uint16_t startingAddress,
		bool value,
		uint32_t& errorCode
	)
	{
		// FIXME: Test
		errorCode = 0;
	}

	void
	ModbusTCPClientImpl::writeSingleHoldingRegister(
		uint16_t startingAddress,
		uint16_t holdingRegister,
		uint32_t& errorCode
	)
	{
		// FIXME: Test
		errorCode = 0;
	}

}

