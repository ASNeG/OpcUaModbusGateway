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

#ifndef __OpcUaModbusGateway_ModbusTCPClient_h__
#define __OpcUaModbusGateway_ModbusTCPClient_h__

#include <memory>
#include <vector>

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"

namespace OpcUaModbusGateway
{

	class ModbusTCPClientImpl;

	class ModbusTCPClient
	{
	  public:
		using SPtr = std::shared_ptr<ModbusTCPClient>;
		using Vec = std::vector<SPtr>;
		using StateCallback = std::function<void (const std::string&)>;

		ModbusTCPClient(void);
		~ModbusTCPClient(void);

		void connectTimeout(uint32_t connectTimeout);
		void reconnectTimeout(uint32_t reconnectTimeout);
		void sendTimeout(uint32_t sendTimeout);
		void recvTimeout(uint32_t recvTimeout);
		void queryTimeout(uint32_t queryTimeout);
		void stateCallback(StateCallback stateCallback);

		bool connect(ModbusTCPClientConfig::SPtr& modbusTCPClientConfig);
		bool disconnect(void);

		void readCoils(
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<bool>& coilStatus
		);
		void readDiscreteInputs(
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<bool>& inputStatus
		);
		void readInputRegisters(
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<uint16_t>& inputRegisters
		);
		void readHoldingRegisters(
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<uint16_t>& holdingRegisters
		);
		void writeMultipleCoils(
			uint16_t startingAddress,
			std::vector<bool>& coils,
			uint32_t& errorCode,
			uint16_t& count
		);
		void writeMultipleHoldingRegisters(
			uint16_t startingAddress,
			std::vector<uint16_t>& holdingRegisters,
			uint32_t& errorCode,
			uint16_t& count
		);
		void writeSingleCoil(
			uint16_t startingAddress,
			bool value,
			uint32_t& errorCode
		);
		void writeSingleHoldingRegister(
			uint16_t startingAddress,
			uint16_t holdingRegister,
			uint32_t& errorCode
		);

	  private:
		ModbusTCPClientImpl* modbusTCPClientImpl_ = nullptr;
	};

}

#endif
