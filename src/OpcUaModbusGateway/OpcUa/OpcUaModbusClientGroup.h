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

#ifndef __OpcUaModbusGateway_OpcUaModbusClientGroup_h__
#define __OpcUaModbusGateway_OpcUaModbusClientGroup_h__

#include <string>

#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaModbusGateway/Application/ModbusTCPClient.h"
#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"
#include "OpcUaModbusGateway/OpcUa/OpcUaModbusValue.h"

namespace OpcUaModbusGateway
{

	class RegisterJob
	{
	  public:
		using SPtr = std::shared_ptr<RegisterJob>;
		using Vec = std::vector<SPtr>;

		uint16_t actAddress_ = 0;
		uint16_t startingAddress_ = 0;
		std::vector<OpcUaModbusValue::SPtr> modbusValueVec_;
	};


	class OpcUaModbusClientGroup
	{
	  public:
		using SPtr = std::shared_ptr<OpcUaModbusClientGroup>;
		using Vec = std::vector<SPtr>;

		OpcUaModbusClientGroup(void);
		~OpcUaModbusClientGroup(void);

		void maxRegisterInRequest(uint32_t maxRegisterInRequest);
		bool startup(
			const std::string& namespaceName,
			uint32_t namespaceIndex,
			RegisterGroupConfig::SPtr registerGroupConfig,
			OpcUaStackServer::ApplicationIf* applicationIf_,
			OpcUaStackCore::OpcUaNodeId& rootNodeId,
			ModbusTCPClient::SPtr modbusTCPClient
		);
		bool shutdown(void);

	  private:
		static uint32_t id_;
		std::string namespaceName_ = "";
		uint32_t namespaceIndex_ = 0;
		RegisterGroupConfig::SPtr registerGroupConfig_;
		OpcUaStackServer::ApplicationIf* applicationIf_ = nullptr;
		OpcUaStackCore::OpcUaNodeId groupNodeId_;
		OpcUaStackCore::OpcUaNodeId rootNodeId_;

		OpcUaModbusValue::Vec modbusValueVec_;

		uint32_t maxRegisterInRequest_ = 20;
		ModbusTCPClient::SPtr modbusTCPClient_ = nullptr;
		OpcUaStackCore::IOThread::SPtr ioThread_ = nullptr;
		OpcUaStackCore::SlotTimerElement::SPtr readSlotTimer_ = nullptr;
		OpcUaStackCore::SlotTimerElement::SPtr writeSlotTimer_ = nullptr;

		RegisterJob::Vec readRegisterJobs_;

		void initReadJobs(void);
		void readCoil(void);
		void readLoop(void);
		void writeLoop(void);
	};

}

#endif
