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

#ifndef __OpcUaModbusGateway_ModbusTCPClientConfig_h__
#define __OpcUaModbusGateway_ModbusTCPClientConfig_h__

#include <string>
#include <vector>

#include "OpcUaStackCore/Base/Config.h"

namespace OpcUaModbusGateway
{

	class RegisterConfig
	{
	  public:
		using SPtr = std::shared_ptr<RegisterConfig>;
		using Vec = std::vector<RegisterConfig::SPtr>;

		enum class ModbusType {
			None,
			Bool,
			UInt16
		};

		enum class ModbusAccess {
			None,
			Read,
			Write,
			ReadWrite
		};

		enum class ModbusAppl {
			None,
			Slave,
			Master
		};

		RegisterConfig(
			ModbusType modbusType,
			ModbusAccess modbusAccess,
			ModbusAppl modbusAppl
		);
		~RegisterConfig(void);

		static std::map<ModbusType, std::string> modbusTypeMap_;
		static std::string toString(ModbusType type);
		static ModbusType toType(const std::string& type);

		static std::map<ModbusAccess, std::string> modbusAccessMap_;
		static std::string toString(ModbusAccess access);
		static ModbusAccess toAccess(const std::string& access);

		static std::map<ModbusAppl, std::string> modbusApplMap_;
		static std::string toString(ModbusAppl modbusAppl);
		static ModbusAppl toAppl(const std::string& appl);


		bool parse(OpcUaStackCore::Config& config);
		uint16_t address(void);
		std::string name(void);
		std::string unit(void);
		std::string type(void);
		double a(void);
		double b(void);
		ModbusType modbusType(void);
		std::string& opcUaTypeString(void);
		ModbusAccess modbusAccess(void);
		ModbusAppl modbusAppl(void);

	  private:
		std::string modbusTypeString_ = "Unknown";
		std::string opcUaTypeString_ = "Unknown";
		ModbusType modbusType_ = ModbusType::None;
		ModbusAccess modbusAccess_ = ModbusAccess::None;
		ModbusAppl modbusAppl_ = ModbusAppl::None;
		uint16_t address_ = 0;
		std::string name_ = "";
		std::string unit_ = "";
		std::string type_ = "";
		double a_ = 0.0;
		double b_ = 0.0;
	};


	class RegisterGroupConfig {
	  public:
		using SPtr = std::shared_ptr<RegisterGroupConfig>;
		using Vec = std::vector<RegisterGroupConfig::SPtr>;

		enum class ModbusGroupType
		{
			None,
			Coil,
			Input,
			InputRegister,
			HoldingRegister
		};

		RegisterGroupConfig(RegisterConfig::ModbusAppl modbusAppl);
		~RegisterGroupConfig(void);

		static std::map<ModbusGroupType, std::string> modbusGroupTypeMap_;
		static std::string toString(ModbusGroupType type);
		static ModbusGroupType toType(const std::string& type);

		ModbusGroupType type(void);
		bool parse(OpcUaStackCore::Config& config);
		std::string groupName(void);
		uint32_t readInterval(void);
		uint32_t writeInterval(void);
		RegisterConfig::Vec& registerConfigVec(void);

	  private:
		ModbusGroupType modbusGroupType_ = ModbusGroupType::None;
		RegisterConfig::ModbusAppl modbusAppl_ = RegisterConfig::ModbusAppl::None;
		std::string groupName_ = "";
		uint32_t readInterval_ = 500;
		uint32_t writeInterval_ = 100;
		RegisterConfig::Vec registerConfigVec_;
	};

	class ModbusTCPClientConfig
	{
	  public:
		using SPtr = std::shared_ptr<ModbusTCPClientConfig>;
		using Vec = std::vector<SPtr>;
		using RegisterGroupMap = std::map<RegisterGroupConfig::ModbusGroupType, RegisterGroupConfig::Vec>;

		ModbusTCPClientConfig(void);
		~ModbusTCPClientConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string name(void);
		std::string ipAddress(void);
		uint32_t port(void);
		uint8_t slaveId(void);
		RegisterGroupConfig::Vec& registerGroupConfigVec(RegisterGroupConfig::ModbusGroupType mosbusGroupType);
		std::vector<RegisterGroupConfig::ModbusGroupType> registerGroupTypes(void);

		uint32_t connectTimeout(void);
		uint32_t reconnectTimeout(void);
		uint32_t sendTimeout(void);
		uint32_t recvTimeout(void);
		uint32_t queryTimeout(void);
		uint32_t maxNumRegPDU(void);

	  private:
		// Mandatory attributes
		std::string name_ = "";
		std::string ipAddress_ = "127.0.0.1";
		uint32_t port_ = 123;
		uint8_t slaveId_ = 0;
		RegisterGroupMap registerGroupMap_;

		// Optional attributes
		uint32_t connectTimeout_ = 1000; 		/* 1000 milliseconds */
		uint32_t reconnectTimeout_ = 5000;		/* 5000 milliseconds */
		uint32_t sendTimeout_ = 1000;			/* 1000 milliseconds */
		uint32_t recvTimeout_ = 3000;			/* 3000 milliseconds */
		uint32_t queryTimeout_ = 3000;			/* 3000 milliseconds */
		uint32_t maxNumRegPDU_ = 10;			/* 10 register per PDU */
	};

	class ModbusTCPServerConfig
	{
	  public:
		using SPtr = std::shared_ptr<ModbusTCPServerConfig>;
		using Vec = std::vector<SPtr>;
		using RegisterGroupMap = std::map<RegisterGroupConfig::ModbusGroupType, RegisterGroupConfig::Vec>;

		ModbusTCPServerConfig(void);
		~ModbusTCPServerConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string name(void);
		std::string ipAddress(void);
		uint32_t port(void);
		uint32_t sendTimeout(void);
		uint32_t recvTimeout(void);
		RegisterGroupConfig::Vec& registerGroupConfigVec(RegisterGroupConfig::ModbusGroupType modbusGroupType);
		std::vector<RegisterGroupConfig::ModbusGroupType> registerGroupTypes(void);

	  private:
		// Mandatory attributes
		std::string name_ = "";
		std::string ipAddress_ = "127.0.0.1";
		uint32_t port_ = 123;
		RegisterGroupMap registerGroupMap_;

		// Optional attributes
		uint32_t sendTimeout_ = 0xFFFFFFFF;
		uint32_t recvTimeout_ = 0xFFFFFFFF;
	};


	class ModbusGatewayConfig
	{
	  public:
		ModbusGatewayConfig(void);
		virtual ~ModbusGatewayConfig(void);

		bool parse(const std::string& fileName);
		ModbusTCPClientConfig::Vec& modbusTCPClientConfig(void);
		ModbusTCPServerConfig::Vec& modbusTCPServerConfig(void);

	  private:
		ModbusTCPClientConfig::Vec modbusTCPClientConfigVec_;
		ModbusTCPServerConfig::Vec modbusTCPServerConfigVec_;
	};

}

#endif
