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

#ifndef __OpcUaModbusGateway_ModbusTCPClientImpl_h__
#define __OpcUaModbusGateway_ModbusTCPClientImpl_h__

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"

#include "ModbusTCP/TCPClient.h"

namespace OpcUaModbusGateway
{

	class ModbusTCPClientImpl
	{
	  public:
		using StateCallback = std::function<void (const std::string&)>;

		using ReadCoilsHandler = std::function<void (uint32_t errorCode, std::vector<bool>& coilStatus)>;
		using ReadDiscreteInputsHandler = std::function<void (uint32_t errorCode, std::vector<bool>& inputStatus)>;
		using ReadInputRegistersHandler = std::function<void (uint32_t errorCode, std::vector<uint16_t>& inputRegisters)>;
		using ReadHoldingRegistersHandler = std::function<void (uint32_t errorCode, std::vector<uint16_t>& holdingRegisters)>;
		using WriteMultipleCoilsHandler = std::function<void (uint32_t errorCode, uint16_t count)>;
		using WriteMultipleHoldingRegistersHandler = std::function<void (uint32_t errorCode, uint16_t count)>;
		using WriteSingleCoilHandler = std::function<void (uint32_t errorCode)>;
		using WriteSingleHoldingRegisterHandler = std::function<void (uint32_t errorCode)>;


		ModbusTCPClientImpl(void);
		~ModbusTCPClientImpl(void);

		bool connect(ModbusTCPClientConfig::SPtr& modbusTCPClientConfig);
		bool disconnect(void);

		void connectTimeout(uint32_t connectTimeout);
		void reconnectTimeout(uint32_t reconnectTimeout);
		void sendTimeout(uint32_t sendTimeout);
		void recvTimeout(uint32_t recvTimeout);
		void queryTimeout(uint32_t queryTimeout);
		void slaveId(uint8_t slaveId);
		void stateCallback(StateCallback stateCallback);

		void readCoils( /* only use for opc ua methods */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<bool>& coilStatus
		);
		void readCoils( /* use for node access */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			ReadCoilsHandler readCoilsHandler
		);

		void readDiscreteInputs( /* only use for opc ua methods */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<bool>& inputStatus
		);
		void readDiscreteInputs( /* use for node access */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			ReadDiscreteInputsHandler readDiscreteInputsHandler
		);

		void readInputRegisters( /* only use for opc ua methods */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<uint16_t>& inputRegisters
		);
		void readInputRegisters( /* use for node access */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			ReadInputRegistersHandler readInputRegistersHandler
		);

		void readHoldingRegisters( /* only use for opc ua methods */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			uint32_t& errorCode,
			std::vector<uint16_t>& holdingRegisters
		);
		void readHoldingRegisters( /* use for node access */
			uint16_t startingAddress,
			uint16_t quantityOfInputs,
			ReadHoldingRegistersHandler readHoldingRegistersHandler
		);

		void writeMultipleCoils( /* only use for opc ua methods */
			uint16_t startingAddress,
			std::vector<bool>& coils,
			uint32_t& errorCode,
			uint16_t& count
		);
		void writeMultipleCoils( /* use for node access */
			uint16_t startingAddress,
			std::vector<bool>& coils,
			WriteMultipleCoilsHandler writeMultipleCoilsHandler
		);

		void writeMultipleHoldingRegisters( /* only use for opc ua methods */
			uint16_t startingAddress,
			std::vector<uint16_t>& holdingRegisters,
			uint32_t& errorCode,
			uint16_t& count
		);
		void writeMultipleHoldingRegisters( /* use for node access */
			uint16_t startingAddress,
			std::vector<uint16_t>& holdingRegisters,
			WriteMultipleHoldingRegistersHandler writeMultipleHoldingRegistersHandler
		);

		void writeSingleCoil( /* only use for opc ua methods */
			uint16_t startingAddress,
			bool value,
			uint32_t& errorCode
		);
		void writeSingleCoil( /* use for node access */
			uint16_t startingAddress,
			bool value,
			WriteSingleCoilHandler writeSingleCoilHandler
		);

		void writeSingleHoldingRegister( /* only use for opc ua methods */
			uint16_t startingAddress,
			uint16_t holdingRegister,
			uint32_t& errorCode
		);
		void writeSingleHoldingRegister( /* use for node access */
			uint16_t startingAddress,
			uint16_t holdingRegister,
			WriteSingleHoldingRegisterHandler writeSingleHoldingRegisterHandler
		);

	  private:
		uint8_t slaveId_ = 0;
		uint32_t queryTimeout_ = 3000;
		StateCallback stateCallback_;

		asio::ip::tcp::endpoint serverEndpoint_;
		ModbusTCP::TCPClient modbusTCPClient_;
		ModbusTCP::TCPClientState modbusTCPClientState_;

		void clientConnectionHandler(ModbusTCP::TCPClientState clientState);

		void readCoilsHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode,
			std::vector<bool>& coilStatus
		);
		void readDiscreteInputsHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode,
			std::vector<bool>& inputStatus
		);
		void readInputRegistersHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode,
			std::vector<uint16_t>& inputRegisters
		);
		void readHoldingRegistersHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode,
			std::vector<uint16_t>& holdingRegisters
		);
		void writeMultipleCoilsHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode,
			uint16_t& count
		);
		void writeMultipleHoldingRegistersHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode,
			uint16_t& count
		);
		void writeSingleCoilHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode
		);
		void writeSingleHoldingRegisterHandleResponse(
			ModbusProt::ModbusError error,
			ModbusProt::ModbusPDU::SPtr& req,
			ModbusProt::ModbusPDU::SPtr& res,
			uint32_t& errorCode
		);

	};
}

#endif
