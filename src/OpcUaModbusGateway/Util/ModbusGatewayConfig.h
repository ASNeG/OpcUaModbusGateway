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

	enum class RegisterValueType
	{
		Bool,
		UInt16
	};

	std::string toString(RegisterValueType registerValueType);

	enum class RegisterGroupType
	{
		Coil,
		Input,
		InputRegister,
		HoldingRegister
	};

	std::string toString(RegisterGroupType registerGroupType);

	class CoilConfig
	{
	  public:
		using SPtr = std::shared_ptr<CoilConfig>;
		using Vec = std::vector<CoilConfig::SPtr>;

		CoilConfig(void);
		~CoilConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		uint16_t address(void);
		std::string name(void);

	  private:
		uint16_t address_ = 0;
		std::string name_ = "";
	};


	class CoilsConfig {
	  public:
		using SPtr = std::shared_ptr<CoilsConfig>;
		using Vec = std::vector<CoilsConfig::SPtr>;

		CoilsConfig(void);
		~CoilsConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string groupName(void);
		uint32_t interval(void);
		CoilConfig::Vec& coilConfigVec(void);

	  private:
		std::string groupName_ = "";
		uint32_t interval_ = 1000;
		CoilConfig::Vec coilConfigVec_;
	};

	class InputConfig
	{
	  public:
		using SPtr = std::shared_ptr<InputConfig>;
		using Vec = std::vector<InputConfig::SPtr>;

		InputConfig(void);
		~InputConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		uint16_t address(void);
		std::string name(void);

	  private:
		uint16_t address_ = 0;
		std::string name_ = "";
	};


	class InputsConfig {
	  public:
		using SPtr = std::shared_ptr<InputsConfig>;
		using Vec = std::vector<InputsConfig::SPtr>;

		InputsConfig(void);
		~InputsConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string groupName(void);
		uint32_t interval(void);
		InputConfig::Vec& inputConfigVec(void);

	  private:
		std::string groupName_ = "";
		uint32_t interval_ = 1000;
		InputConfig::Vec inputConfigVec_;
	};


	class HoldingRegisterConfig
	{
	  public:
		using SPtr = std::shared_ptr<HoldingRegisterConfig>;
		using Vec = std::vector<HoldingRegisterConfig::SPtr>;

		HoldingRegisterConfig(void);
		~HoldingRegisterConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		uint16_t address(void);
		std::string name(void);
		std::string unit(void);
		std::string type(void);
		double a(void);
		double b(void);

	  private:
		uint16_t address_ = 0;
		std::string name_ = "";
		std::string unit_ = "";
		std::string type_ = "";
		double a_ = 0.0;
		double b_ = 0.0;
	};


	class HoldingRegistersConfig {
	  public:
		using SPtr = std::shared_ptr<HoldingRegistersConfig>;
		using Vec = std::vector<HoldingRegistersConfig::SPtr>;

		HoldingRegistersConfig(void);
		~HoldingRegistersConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string groupName(void);
		uint32_t interval(void);
		HoldingRegisterConfig::Vec& holdingRegisterConfigVec(void);

	  private:
		std::string groupName_ = "";
		uint32_t interval_ = 1000;
		HoldingRegisterConfig::Vec holdingRegisterConfigVec_;
	};

	class RegisterConfig
	{
	  public:
		using SPtr = std::shared_ptr<RegisterConfig>;
		using Vec = std::vector<RegisterConfig::SPtr>;

		RegisterConfig(void);
		~RegisterConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		uint16_t address(void);
		std::string name(void);
		std::string unit(void);
		std::string type(void);
		double a(void);
		double b(void);

	  private:
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

		enum class Type
		{
			None,
			Coil,
			Input,
			InputRegister,
			HoldingRegister
		};

		RegisterGroupConfig(void);
		~RegisterGroupConfig(void);

		static std::map<Type, std::string> typeMap_;
		static std::string toString(Type type);
		static Type toType(const std::string& type);

		Type type(void);
		bool parse(OpcUaStackCore::Config& config);
		std::string groupName(void);
		uint32_t interval(void);
		RegisterConfig::Vec& registerConfigVec(void);

	  private:
		Type type_ = Type::None;
		std::string groupName_ = "";
		uint32_t interval_ = 1000;
		RegisterConfig::Vec registerConfigVec_;
	};

	class ModbusTCPClientConfig
	{
	  public:
		using SPtr = std::shared_ptr<ModbusTCPClientConfig>;
		using Vec = std::vector<SPtr>;

		ModbusTCPClientConfig(void);
		~ModbusTCPClientConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string name(void);
		std::string ipAddress(void);
		uint32_t port(void);
		uint8_t slaveId(void);
		RegisterGroupConfig::Vec& registerGroupConfigVec(RegisterGroupConfig::Type type);

		uint32_t connectTimeout(void);
		uint32_t reconnectTimeout(void);
		uint32_t sendTimeout(void);
		uint32_t recvTimeout(void);
		uint32_t queryTimeout(void);

	  private:
		// Mandatory attributes
		std::string name_ = "";
		std::string ipAddress_ = "127.0.0.1";
		uint32_t port_ = 123;
		uint8_t slaveId_ = 0;
		std::map<RegisterGroupConfig::Type, RegisterGroupConfig::Vec> registerGroupMap_;

		// Optional attributes
		uint32_t connectTimeout_ = 1000; 		/* 1000 milliseconds */
		uint32_t reconnectTimeout_ = 5000;		/* 5000 milliseconds */
		uint32_t sendTimeout_ = 1000;			/* 1000 milliseconds */
		uint32_t recvTimeout_ = 3000;			/* 3000 milliseconds */
		uint32_t queryTimeout_ = 3000;			/* 3000 milliseconds */
	};


	class ModbusGatewayConfig
	{
	  public:
		ModbusGatewayConfig(void);
		virtual ~ModbusGatewayConfig(void);

		bool parse(const std::string& fileName);
		ModbusTCPClientConfig::Vec& modbusTCPClientConfig(void);

	  private:
		ModbusTCPClientConfig::Vec modbusTCPClientConfigVec_;
	};

}

#endif
