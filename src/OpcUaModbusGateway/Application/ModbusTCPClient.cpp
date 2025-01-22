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

	void
	ModbusTCPClient::connectTimeout(uint32_t connectTimeout)
	{
		modbusTCPClientImpl_->connectTimeout(connectTimeout);
	}

	void
	ModbusTCPClient::reconnectTimeout(uint32_t reconnectTimeout)
	{
		modbusTCPClientImpl_->reconnectTimeout(reconnectTimeout);
	}

	void
	ModbusTCPClient::sendTimeout(uint32_t sendTimeout)
	{
		modbusTCPClientImpl_->sendTimeout(sendTimeout);
	}

	void
	ModbusTCPClient::recvTimeout(uint32_t recvTimeout)
	{
		modbusTCPClientImpl_->recvTimeout(recvTimeout);
	}

	void
	ModbusTCPClient::queryTimeout(uint32_t recvTimeout)
	{
		modbusTCPClientImpl_->queryTimeout(recvTimeout);
	}

	void
	ModbusTCPClient::stateCallback(StateCallback stateCallback)
	{
		modbusTCPClientImpl_->stateCallback(stateCallback);
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
		modbusTCPClientImpl_->readCoils(
			startingAddress,
			quantityOfInputs,
			errorCode,
			coilStatus
		);
	}

	void
	ModbusTCPClient::readDiscreteInputs(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<bool>& inputStatus
	)
	{
		modbusTCPClientImpl_->readDiscreteInputs(
			startingAddress,
			quantityOfInputs,
			errorCode,
			inputStatus
		);
	}

	void
	ModbusTCPClient::readInputRegisters(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<uint16_t>& inputRegisters
	)
	{
		modbusTCPClientImpl_->readInputRegisters(
			startingAddress,
			quantityOfInputs,
			errorCode,
			inputRegisters
		);
	}

	void
	ModbusTCPClient::readHoldingRegisters(
		uint16_t startingAddress,
		uint16_t quantityOfInputs,
		uint32_t& errorCode,
		std::vector<uint16_t>& holdingRegisters
	)
	{
		modbusTCPClientImpl_->readHoldingRegisters(
			startingAddress,
			quantityOfInputs,
			errorCode,
			holdingRegisters
		);
	}

	void
	ModbusTCPClient::writeMultipleCoils(
		uint16_t startingAddress,
		std::vector<bool>& coils,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		modbusTCPClientImpl_->writeMultipleCoils(
			startingAddress,
			coils,
			errorCode,
			count
		);
	}

	void
	ModbusTCPClient::writeMultipleHoldingRegisters(
		uint16_t startingAddress,
		std::vector<uint16_t>& holdingRegisters,
		uint32_t& errorCode,
		uint16_t& count
	)
	{
		modbusTCPClientImpl_->writeMultipleHoldingRegisters(
			startingAddress,
			holdingRegisters,
			errorCode,
			count
		);
	}

	void
	ModbusTCPClient::writeSingleCoil(
		uint16_t startingAddress,
		bool value,
		uint32_t& errorCode
	)
	{
		modbusTCPClientImpl_->writeSingleCoil(
			startingAddress,
			value,
			errorCode
		);
	}

	void
	ModbusTCPClient::writeSingleHoldingRegister(
		uint16_t startingAddress,
		uint16_t holdingRegister,
		uint32_t& errorCode
	)
	{
		modbusTCPClientImpl_->writeSingleHoldingRegister(
			startingAddress,
			holdingRegister,
			errorCode
		);
	}

}

