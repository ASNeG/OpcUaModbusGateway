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
#include "OpcUaModbusGateway/Util/Condition.h"

#include "ModbusProt/ReadCoilsPDU.h"
#include "ModbusProt/ReadDiscreteInputsPDU.h"
#include "ModbusProt/ReadInputRegistersPDU.h"
#include "ModbusProt/ReadMultipleHoldingRegistersPDU.h"
#include "ModbusProt/ErrorPDU.h"

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
		std::cout << "CLIENT STATE: ";
		std::cout << "...." << static_cast<typename std::underlying_type<ModbusTCP::TCPClientState>::type>(clientState) << std::endl;

	}

	void
	ModbusTCPClientImpl::readCoils(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<bool>& coilStatus
	)
	{
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send read coils request
		auto readCoilsReq = std::make_shared<ModbusProt::ReadCoilsReqPDU>();
		readCoilsReq->startingAddress(startingAddress);
		readCoilsReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readCoilsReq;
		modbusTCPClient_.send(0, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(2000)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}
		if (modbusError != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(modbusError) + 100;
			return;
		}

		// Handle error response
		if (modbusRes->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(modbusRes);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readCoilsRes = std::static_pointer_cast<ModbusProt::ReadCoilsResPDU>(modbusRes);
		for (uint32_t idx = 0; idx < readCoilsReq->quantityOfInputs(); idx++) {
			bool b;
			readCoilsRes->getCoilStatus(idx, b);
			coilStatus.push_back(b);
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
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send read discrete inputs request
		auto readDiscreteInputsReq = std::make_shared<ModbusProt::ReadDiscreteInputsReqPDU>();
		readDiscreteInputsReq->startingAddress(startingAddress);
		readDiscreteInputsReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readDiscreteInputsReq;
		modbusTCPClient_.send(0, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(2000)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}
		if (modbusError != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(modbusError) + 100;
			return;
		}

		// Handle error response
		if (modbusRes->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(modbusRes);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readDiscreteInputsRes = std::static_pointer_cast<ModbusProt::ReadDiscreteInputsResPDU>(modbusRes);
		for (uint32_t idx = 0; idx < readDiscreteInputsReq->quantityOfInputs(); idx++) {
			bool b;
			readDiscreteInputsRes->getInputStatus(idx, b);
			inputStatus.push_back(b);
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
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send read input registers request
		auto readInputRegistersReq = std::make_shared<ModbusProt::ReadInputRegistersReqPDU>();
		readInputRegistersReq->startingAddress(startingAddress);
		readInputRegistersReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readInputRegistersReq;
		modbusTCPClient_.send(0, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(2000)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}
		if (modbusError != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(modbusError) + 100;
			return;
		}

		// Handle error response
		if (modbusRes->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(modbusRes);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readInputRegistersRes = std::static_pointer_cast<ModbusProt::ReadInputRegistersResPDU>(modbusRes);
		for (uint32_t idx = 0; idx < readInputRegistersReq->quantityOfInputs(); idx++) {
			uint16_t value;
			readInputRegistersRes->getInputRegisters(idx, value);
			inputRegisters.push_back(value);
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
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send read holding registers request
		auto readHoldingRegistersReq = std::make_shared<ModbusProt::ReadMultipleHoldingRegistersReqPDU>();
		readHoldingRegistersReq->startingAddress(startingAddress);
		readHoldingRegistersReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readHoldingRegistersReq;
		modbusTCPClient_.send(0, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(2000)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}
		if (modbusError != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(modbusError) + 100;
			return;
		}

		// Handle error response
		if (modbusRes->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(modbusRes);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readHoldingRegistersRes = std::static_pointer_cast<ModbusProt::ReadMultipleHoldingRegistersResPDU>(modbusRes);
		for (uint32_t idx = 0; idx < readHoldingRegistersReq->quantityOfInputs(); idx++) {
			uint16_t value;
			readHoldingRegistersRes->getHoldingRegisters(idx, value);
			holdingRegisters.push_back(value);
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

