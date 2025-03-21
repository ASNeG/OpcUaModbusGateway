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
#include "OpcUaModbusGateway/Application/LogDefault.h"
#include "OpcUaModbusGateway/Util/Condition.h"

#include "ModbusProt/ReadCoilsPDU.h"
#include "ModbusProt/ReadDiscreteInputsPDU.h"
#include "ModbusProt/ReadInputRegistersPDU.h"
#include "ModbusProt/ReadMultipleHoldingRegistersPDU.h"
#include "ModbusProt/WriteMultipleCoilsPDU.h"
#include "ModbusProt/WriteMultipleHoldingRegistersPDU.h"
#include "ModbusProt/WriteSingleCoilPDU.h"
#include "ModbusProt/WriteSingleHoldingRegisterPDU.h"
#include "ModbusProt/ErrorPDU.h"

namespace OpcUaModbusGateway
{

	ModbusTCPClientImpl::ModbusTCPClientImpl(void)
	{
	}

	ModbusTCPClientImpl::~ModbusTCPClientImpl(void)
	{
	}

	void
	ModbusTCPClientImpl::connectTimeout(uint32_t connectTimeout)
	{
		modbusTCPClient_.connectTimeout(connectTimeout);
	}

	void
	ModbusTCPClientImpl::reconnectTimeout(uint32_t reconnectTimeout)
	{
		modbusTCPClient_.reconnectTimeout(reconnectTimeout);
	}

	void
	ModbusTCPClientImpl::sendTimeout(uint32_t sendTimeout)
	{
		modbusTCPClient_.sendTimeout(sendTimeout);
	}

	void
	ModbusTCPClientImpl::recvTimeout(uint32_t recvTimeout)
	{
		modbusTCPClient_.recvTimeout(recvTimeout);
	}

	void
	ModbusTCPClientImpl::queryTimeout(uint32_t queryTimeout)
	{
		queryTimeout_ = queryTimeout;
	}

	void
	ModbusTCPClientImpl::slaveId(uint8_t slaveId)
	{
		slaveId_ = slaveId;
	}

	void
	ModbusTCPClientImpl::stateCallback(StateCallback stateCallback)
	{
		stateCallback_ = stateCallback;
	}

	bool
	ModbusTCPClientImpl::connect(ModbusTCPClientConfig::SPtr& modbusTCPClientConfig)
	{
		bool rc = true;

		// Add own log handler to modbus tcp client
		Base::LogHandler::SPtr logHandler = std::make_shared<LogDefault>("modbus client message");
		modbusTCPClient_.logHandler(logHandler);

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
			}
		);

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
		if (stateCallback_ == nullptr) {
			return;
		}

		auto stateString = modbusTCPClient_.tcpClientStateToString(clientState);
		stateCallback_(stateString);
	}

	void
	ModbusTCPClientImpl::readCoilsHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode,
		std::vector<bool>& coilStatus
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readCoilsReq = std::static_pointer_cast<ModbusProt::ReadCoilsReqPDU>(req);
		auto readCoilsRes = std::static_pointer_cast<ModbusProt::ReadCoilsResPDU>(res);
		for (uint32_t idx = 0; idx < readCoilsReq->quantityOfInputs(); idx++) {
			bool b;
			readCoilsRes->getCoilStatus(idx, b);
			coilStatus.push_back(b);
		}
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
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		readCoilsHandleResponse(modbusError, req, modbusRes, errorCode, coilStatus);
	}

	void
	ModbusTCPClientImpl::readCoils(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		ReadCoilsHandler readCoilsHandler
	)
	{
		// Create and send read coils request
		auto readCoilsReq = std::make_shared<ModbusProt::ReadCoilsReqPDU>();
		readCoilsReq->startingAddress(startingAddress);
		readCoilsReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readCoilsReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, readCoilsHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				std::vector<bool> coilStatus;

				// Handle response
				readCoilsHandleResponse(error, req, res, errorCode, coilStatus);
				readCoilsHandler(errorCode, coilStatus);
			}
		);
	}

	void
	ModbusTCPClientImpl::readDiscreteInputsHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode,
		std::vector<bool>& inputStatus
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readDiscreteInputsReq = std::static_pointer_cast<ModbusProt::ReadDiscreteInputsReqPDU>(req);
		auto readDiscreteInputsRes = std::static_pointer_cast<ModbusProt::ReadDiscreteInputsResPDU>(res);
		for (uint32_t idx = 0; idx < readDiscreteInputsReq->quantityOfInputs(); idx++) {
			bool b;
			readDiscreteInputsRes->getInputStatus(idx, b);
			inputStatus.push_back(b);
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
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}
		// Handle response
		readDiscreteInputsHandleResponse(modbusError, req, modbusRes, errorCode, inputStatus);
	}

	void
	ModbusTCPClientImpl::readDiscreteInputs(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		ReadDiscreteInputsHandler readDiscreteInputsHandler
	)
	{
		// Create and send read discrete inputs request
		auto readDiscreteInputsReq = std::make_shared<ModbusProt::ReadDiscreteInputsReqPDU>();
		readDiscreteInputsReq->startingAddress(startingAddress);
		readDiscreteInputsReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readDiscreteInputsReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, readDiscreteInputsHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				std::vector<bool> inputStatus;

				// Handle response
				readDiscreteInputsHandleResponse(error, req, res, errorCode, inputStatus);
				readDiscreteInputsHandler(errorCode, inputStatus);
			}
		);
	}

	void
	ModbusTCPClientImpl::readInputRegistersHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode,
		std::vector<uint16_t>& inputRegisters
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readInputRegistersReq = std::static_pointer_cast<ModbusProt::ReadInputRegistersReqPDU>(req);
		auto readInputRegistersRes = std::static_pointer_cast<ModbusProt::ReadInputRegistersResPDU>(res);
		for (uint32_t idx = 0; idx < readInputRegistersReq->quantityOfInputs(); idx++) {
			uint16_t value;
			readInputRegistersRes->getInputRegisters(idx, value);
			inputRegisters.push_back(value);
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
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		readInputRegistersHandleResponse(modbusError, req, modbusRes, errorCode, inputRegisters);
	}

	void
	ModbusTCPClientImpl::readInputRegisters(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		ReadInputRegistersHandler readInputRegistersHandler
	)
	{
		// Create and send read input registers request
		auto readInputRegistersReq = std::make_shared<ModbusProt::ReadInputRegistersReqPDU>();
		readInputRegistersReq->startingAddress(startingAddress);
		readInputRegistersReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readInputRegistersReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, readInputRegistersHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				std::vector<uint16_t> inputRegisters;

				// Handle response
				readInputRegistersHandleResponse(error, req, res, errorCode, inputRegisters);
				readInputRegistersHandler(errorCode, inputRegisters);
			}
		);
	}

	void
	ModbusTCPClientImpl::readHoldingRegistersHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode,
		std::vector<uint16_t>& holdingRegisters
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto readHoldingRegistersReq = std::static_pointer_cast<ModbusProt::ReadMultipleHoldingRegistersReqPDU>(req);
		auto readHoldingRegistersRes = std::static_pointer_cast<ModbusProt::ReadMultipleHoldingRegistersResPDU>(res);
		for (uint32_t idx = 0; idx < readHoldingRegistersReq->quantityOfInputs(); idx++) {
			uint16_t value;
			readHoldingRegistersRes->getHoldingRegisters(idx, value);
			holdingRegisters.push_back(value);
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
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		readHoldingRegistersHandleResponse(modbusError, req, modbusRes, errorCode, holdingRegisters);
	}

	void
	ModbusTCPClientImpl::readHoldingRegisters(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		ReadHoldingRegistersHandler readHoldingRegistersHandler
	)
	{
		// Create and send read holding registers request
		auto readHoldingRegistersReq = std::make_shared<ModbusProt::ReadMultipleHoldingRegistersReqPDU>();
		readHoldingRegistersReq->startingAddress(startingAddress);
		readHoldingRegistersReq->quantityOfInputs(quantityOfInputs);
		ModbusProt::ModbusPDU::SPtr req = readHoldingRegistersReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, readHoldingRegistersHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				std::vector<uint16_t> holdingRegisters;

				// Handle response
				readHoldingRegistersHandleResponse(error, req, res, errorCode, holdingRegisters);
				readHoldingRegistersHandler(errorCode, holdingRegisters);
			}
		);
	}

	void
	ModbusTCPClientImpl::writeMultipleCoilsHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto writeMultipleCoilsRes = std::static_pointer_cast<ModbusProt::WriteMultipleCoilsResPDU>(res);
		count = writeMultipleCoilsRes->quantityOfOutputs();
	}

	void
	ModbusTCPClientImpl::writeMultipleCoils(
		uint16_t startingAddress,
		std::vector<bool>& coils,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send write multiple coils request
		auto writeMultipleCoilsReq = std::make_shared<ModbusProt::WriteMultipleCoilsReqPDU>();
		writeMultipleCoilsReq->startingAddress(startingAddress);
		writeMultipleCoilsReq->quantityOfOutputs(coils.size());
		for (uint32_t idx = 0; idx < coils.size(); idx++) {
			writeMultipleCoilsReq->setOutputsValue(idx, coils[idx]);
		}
		ModbusProt::ModbusPDU::SPtr req = writeMultipleCoilsReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		writeMultipleCoilsHandleResponse(modbusError, req, modbusRes, errorCode, count);
	}

	void
	ModbusTCPClientImpl::writeMultipleCoils(
		uint16_t startingAddress,
		std::vector<bool>& coils,
		WriteMultipleCoilsHandler writeMultipleCoilsHandler
	)
	{
		// Create and send write multiple coils request
		auto writeMultipleCoilsReq = std::make_shared<ModbusProt::WriteMultipleCoilsReqPDU>();
		writeMultipleCoilsReq->startingAddress(startingAddress);
		writeMultipleCoilsReq->quantityOfOutputs(coils.size());
		for (uint32_t idx = 0; idx < coils.size(); idx++) {
			writeMultipleCoilsReq->setOutputsValue(idx, coils[idx]);
		}
		ModbusProt::ModbusPDU::SPtr req = writeMultipleCoilsReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, writeMultipleCoilsHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				uint16_t count;

				// Handle response
				writeMultipleCoilsHandleResponse(error, req, res, errorCode, count);
				writeMultipleCoilsHandler(errorCode, count);
			}
		);
	}

	void
	ModbusTCPClientImpl::writeMultipleHoldingRegistersHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
		auto writeMultipleHoldingRegistersRes = std::static_pointer_cast<ModbusProt::WriteMultipleHoldingRegistersResPDU>(res);
		count = writeMultipleHoldingRegistersRes->quantityOfRegisters();
	}

	void
	ModbusTCPClientImpl::writeMultipleHoldingRegisters(
		uint16_t startingAddress,
		std::vector<uint16_t>& holdingRegisters,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send write multiple holding registers request
		auto writeMultipleHoldingRegistersReq = std::make_shared<ModbusProt::WriteMultipleHoldingRegistersReqPDU>();
		writeMultipleHoldingRegistersReq->startingAddress(startingAddress);
		writeMultipleHoldingRegistersReq->quantityOfRegisters(holdingRegisters.size());
		for (uint32_t idx = 0; idx < holdingRegisters.size(); idx++) {
			writeMultipleHoldingRegistersReq->setRegistersValue(idx, holdingRegisters[idx]);
		}
		ModbusProt::ModbusPDU::SPtr req = writeMultipleHoldingRegistersReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		writeMultipleHoldingRegistersHandleResponse(modbusError, req, modbusRes, errorCode, count);
	}

	void
	ModbusTCPClientImpl::writeMultipleHoldingRegisters(
		uint16_t startingAddress,
		std::vector<uint16_t>& holdingRegisters,
		WriteMultipleHoldingRegistersHandler writeMultipleHoldingRegistersHandler
	)
	{
		// Create and send write multiple holding registers request
		auto writeMultipleHoldingRegistersReq = std::make_shared<ModbusProt::WriteMultipleHoldingRegistersReqPDU>();
		writeMultipleHoldingRegistersReq->startingAddress(startingAddress);
		writeMultipleHoldingRegistersReq->quantityOfRegisters(holdingRegisters.size());
		for (uint32_t idx = 0; idx < holdingRegisters.size(); idx++) {
			writeMultipleHoldingRegistersReq->setRegistersValue(idx, holdingRegisters[idx]);
		}
		ModbusProt::ModbusPDU::SPtr req = writeMultipleHoldingRegistersReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, writeMultipleHoldingRegistersHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				uint16_t count;

				// Handle response
				writeMultipleHoldingRegistersHandleResponse(error, req, res, errorCode, count);
				writeMultipleHoldingRegistersHandler(errorCode, count);
			}
		);
	}

	void
	ModbusTCPClientImpl::writeSingleCoilHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
	}

	void
	ModbusTCPClientImpl::writeSingleCoil(
		uint16_t startingAddress,
		bool value,
		uint32_t& errorCode
	)
	{
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send write single coil request
		auto writeSingleCoilReq = std::make_shared<ModbusProt::WriteSingleCoilReqPDU>();
		writeSingleCoilReq->address(startingAddress);
		writeSingleCoilReq->value(value);
		ModbusProt::ModbusPDU::SPtr req = writeSingleCoilReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		writeSingleCoilHandleResponse(modbusError, req, modbusRes, errorCode);
	}

	void
	ModbusTCPClientImpl::writeSingleCoil(
		uint16_t startingAddress,
		bool value,
		WriteSingleCoilHandler writeSingleCoilHandler
	)
	{
		// Create and send write single coils request
		auto writeSingleCoilReq = std::make_shared<ModbusProt::WriteSingleCoilReqPDU>();
		writeSingleCoilReq->address(startingAddress);
		writeSingleCoilReq->value(value);
		ModbusProt::ModbusPDU::SPtr req = writeSingleCoilReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, writeSingleCoilHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;
				uint16_t count;

				std::cout << "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;

				// Handle response
				writeSingleCoilHandleResponse(error, req, res, errorCode);
				std::cout << "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
				writeSingleCoilHandler(errorCode);
				std::cout << "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
			}
		);
	}

	void
	ModbusTCPClientImpl::writeSingleHoldingRegisterHandleResponse(
		ModbusProt::ModbusError error,
		ModbusProt::ModbusPDU::SPtr& req,
		ModbusProt::ModbusPDU::SPtr& res,
		uint32_t& errorCode
	)
	{
		if (error != ModbusProt::ModbusError::Ok) {
			errorCode = static_cast<int>(error) + 100;
			return;
		}

		// Handle error response
		if (res->pduType() == ModbusProt::PDUType::Error) {
			auto errorRes = std::static_pointer_cast<ModbusProt::ErrorPDU>(res);
			errorCode = errorRes->exceptionCode();;
			return;
		}

		// Handle response
		errorCode = 0;
	}

	void
	ModbusTCPClientImpl::writeSingleHoldingRegister(
		uint16_t startingAddress,
		uint16_t holdingRegister,
		uint32_t& errorCode
	)
	{
		ModbusProt::ModbusError modbusError;
		ModbusProt::ModbusPDU::SPtr modbusRes;
		Condition responseCondition;

		// Create and send write single holding register request
		auto writeSingleHoldingRegisterReq = std::make_shared<ModbusProt::WriteSingleHoldingRegisterReqPDU>();
		writeSingleHoldingRegisterReq->address(startingAddress);
		writeSingleHoldingRegisterReq->registerValue(holdingRegister);
		ModbusProt::ModbusPDU::SPtr req = writeSingleHoldingRegisterReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, &responseCondition, &modbusRes, &modbusError](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				modbusError = error;
				modbusRes = res;
				responseCondition.signal();
			}
		);

		// Handle error
		if (!responseCondition.wait(queryTimeout_)) {
			errorCode = static_cast<int>(ModbusProt::ModbusError::Timeout) + 100;
			return;
		}

		// Handle response
		writeSingleHoldingRegisterHandleResponse(modbusError, req, modbusRes, errorCode);
	}

	void
	ModbusTCPClientImpl::writeSingleHoldingRegister(
		uint16_t startingAddress,
		uint16_t holdingRegister,
		WriteSingleHoldingRegisterHandler writeSingleHoldingRegisterHandler
	)
	{
		// Create and send write single holding register request
		auto writeSingleHoldingRegisterReq = std::make_shared<ModbusProt::WriteSingleHoldingRegisterReqPDU>();
		writeSingleHoldingRegisterReq->address(startingAddress);
		writeSingleHoldingRegisterReq->registerValue(holdingRegister);
		ModbusProt::ModbusPDU::SPtr req = writeSingleHoldingRegisterReq;
		modbusTCPClient_.send(slaveId_, req,
			[this, writeSingleHoldingRegisterHandler](ModbusProt::ModbusError error, ModbusProt::ModbusPDU::SPtr& req, ModbusProt::ModbusPDU::SPtr& res) {
				uint32_t errorCode;

				// Handle response
				writeSingleHoldingRegisterHandleResponse(error, req, res, errorCode);
				writeSingleHoldingRegisterHandler(errorCode);
			}
		);
	}

}

