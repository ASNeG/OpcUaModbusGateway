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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/ConfigXml.h"

#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaModbusGateway
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// enum functions
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	std::string toString(RegisterValueType registerValueType)
	{
		std::map<RegisterValueType, std::string> map = {
			{RegisterValueType::Bool, "Bool"},
			{RegisterValueType::UInt16, "UInt16"}
		};
		auto it = map.find(registerValueType);
		return it == map.end() ? "Unknown" : it->second;

	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class CoilConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	CoilConfig::CoilConfig(void)
	{
	}

	CoilConfig::~CoilConfig(void)
	{
	}

	bool
	CoilConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Name", name_);
		if (rc == false) {
			Log(Error, "attribute not found in coil configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get address attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Address", address_);
		if (rc == false) {
			Log(Error, "attribute not found in coil configuration")
				.parameter("Attribute", "Address");
			return false;
		}

		return true;
	}

	uint16_t
	CoilConfig::address(void)
	{
		return address_;
	}

	std::string
	CoilConfig::name(void)
	{
		return name_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class CoilsConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	CoilsConfig::CoilsConfig(void)
	{
	}

	CoilsConfig::~CoilsConfig(void)
	{
	}

	bool
	CoilsConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.GroupName", groupName_);
		if (rc == false) {
			Log(Error, "attribute not found in coils configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get interval attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Interval", interval_);
		if (rc == false) {
			Log(Error, "attribute not found in coils configuration")
				.parameter("Interval", interval_)
				.parameter("Attribute", "Interval");
			return false;
		}

		// Find coil entries in configuration
		std::vector<Config> configVec;
		config.getChilds("Coil", configVec);
		if (configVec.size() != 0) {
			// parse coil entries
			for (auto configEntry: configVec) {
				auto coilConfig = std::make_shared<CoilConfig>();

				// parse coil entry
				rc = coilConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse coil entry error");
					return false;
				}

				// add coils configuration entry to map
				coilConfigVec_.push_back(coilConfig);
			}
		}

		return true;
	}

	std::string
	CoilsConfig::groupName(void)
	{
		return groupName_;
	}

	uint32_t
	CoilsConfig::interval(void)
	{
		return interval_;
	}

	CoilConfig::Vec&
	CoilsConfig::coilConfigVec(void)
	{
		return coilConfigVec_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class InputConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	InputConfig::InputConfig(void)
	{
	}

	InputConfig::~InputConfig(void)
	{
	}

	bool
	InputConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Name", name_);
		if (rc == false) {
			Log(Error, "attribute not found in coil configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get address attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Address", address_);
		if (rc == false) {
			Log(Error, "attribute not found in coil configuration")
				.parameter("Attribute", "Address");
			return false;
		}

		return true;
	}

	uint16_t
	InputConfig::address(void)
	{
		return address_;
	}

	std::string
	InputConfig::name(void)
	{
		return name_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class InputsConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	InputsConfig::InputsConfig(void)
	{
	}

	InputsConfig::~InputsConfig(void)
	{
	}

	bool
	InputsConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.GroupName", groupName_);
		if (rc == false) {
			Log(Error, "attribute not found in inputs configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get interval attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Interval", interval_);
		if (rc == false) {
			Log(Error, "attribute not found in inputs configuration")
				.parameter("Interval", interval_)
				.parameter("Attribute", "Interval");
			return false;
		}

		// Find inputs entries in configuration
		std::vector<Config> configVec;
		config.getChilds("Input", configVec);
		if (configVec.size() != 0) {
			// parse coil entries
			for (auto configEntry: configVec) {
				auto inputConfig = std::make_shared<InputConfig>();

				// parse input entry
				rc = inputConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse input entry error");
					return false;
				}

				// add input configuration entry to map
				inputConfigVec_.push_back(inputConfig);
			}
		}

		return true;
	}

	std::string
	InputsConfig::groupName(void)
	{
		return groupName_;
	}

	uint32_t
	InputsConfig::interval(void)
	{
		return interval_;
	}

	InputConfig::Vec&
	InputsConfig::inputConfigVec(void)
	{
		return inputConfigVec_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HoldingRegisterConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HoldingRegisterConfig::HoldingRegisterConfig(void)
	{
	}

	HoldingRegisterConfig::~HoldingRegisterConfig(void)
	{
	}

	bool
	HoldingRegisterConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Name", name_);
		if (rc == false) {
			Log(Error, "attribute not found in holding register configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get address attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Address", address_);
		if (rc == false) {
			Log(Error, "attribute not found in holding register configuration")
				.parameter("Attribute", "Address");
			return false;
		}

		// Get unit attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Unit", unit_);
		if (rc == false) {
			Log(Error, "attribute not found in holding register configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get type attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", type_);
		if (rc == false) {
			Log(Error, "attribute not found in holding register configuration")
				.parameter("Attribute", "Type");
			return false;
		}

		// Get a_ attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", a_);
		if (rc == false) {
		    a_ = 0.0;
		}

		// Get b_ attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", b_);
		if (rc == false) {
		    b_ = 1.0;
		}

		return true;
	}

	uint16_t
	HoldingRegisterConfig::address(void)
	{
		return address_;
	}

	std::string
	HoldingRegisterConfig::name(void)
	{
		return name_;
	}

	std::string
	HoldingRegisterConfig::unit(void)
	{
		return unit_;
	}

	std::string
	HoldingRegisterConfig::type(void)
	{
		return type_;
	}

	double
	HoldingRegisterConfig::a(void)
	{
		return a_;
	}

	double
	HoldingRegisterConfig::b(void)
	{
		return b_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HoldingRegistersConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HoldingRegistersConfig::HoldingRegistersConfig(void)
	{
	}

	HoldingRegistersConfig::~HoldingRegistersConfig(void)
	{
	}

	bool
	HoldingRegistersConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.GroupName", groupName_);
		if (rc == false) {
			Log(Error, "attribute not found in holding register configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get interval attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Interval", interval_);
		if (rc == false) {
			Log(Error, "attribute not found in holding register configuration")
				.parameter("Interval", interval_)
				.parameter("Attribute", "Interval");
			return false;
		}

		// Find inputs entries in configuration
		std::vector<Config> configVec;
		config.getChilds("HoldingRegister", configVec);
		if (configVec.size() != 0) {
			// parse holding register entries
			for (auto configEntry: configVec) {
				auto holdingRegisterConfig = std::make_shared<HoldingRegisterConfig>();

				// parse holding register entry
				rc = holdingRegisterConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse holding register entry error");
					return false;
				}

				// add holding register configuration entry to map
				holdingRegisterConfigVec_.push_back(holdingRegisterConfig);
			}
		}

		return true;
	}

	std::string
	HoldingRegistersConfig::groupName(void)
	{
		return groupName_;
	}

	uint32_t
	HoldingRegistersConfig::interval(void)
	{
		return interval_;
	}

	HoldingRegisterConfig::Vec&
	HoldingRegistersConfig::holdingRegisterConfigVec(void)
	{
		return holdingRegisterConfigVec_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class RegisterConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	RegisterConfig::RegisterConfig(void)
	{
	}

	RegisterConfig::~RegisterConfig(void)
	{
	}

	bool
	RegisterConfig::parse(Config& config)
	{
		bool rc = true;

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Name", name_);
		if (rc == false) {
			Log(Error, "attribute not found in register configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get address attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Address", address_);
		if (rc == false) {
			Log(Error, "attribute not found in register configuration")
				.parameter("Attribute", "Address")
				.parameter("Register", name_);
			return false;
		}

		// Get unit attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Unit", unit_);
		if (rc == false) {
			Log(Error, "attribute not found in input register configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get type attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", type_);
		if (rc == false) {
			Log(Error, "attribute not found in register configuration")
				.parameter("Attribute", "Type")
				.parameter("Register", name_);
			return false;
		}

		// Get a_ attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", a_);
		if (rc == false) {
		    a_ = 0.0;
		}

		// Get b_ attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", b_);
		if (rc == false) {
		    b_ = 1.0;
		}

		return true;
	}

	uint16_t
	RegisterConfig::address(void)
	{
		return address_;
	}

	std::string
	RegisterConfig::name(void)
	{
		return name_;
	}

	std::string
	RegisterConfig::unit(void)
	{
		return unit_;
	}

	std::string
	RegisterConfig::type(void)
	{
		return type_;
	}

	double
	RegisterConfig::a(void)
	{
		return a_;
	}

	double
	RegisterConfig::b(void)
	{
		return b_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class RegisterGroupConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	std::map<RegisterGroupType, std::string> RegisterGroupConfig::typeMap_ = {
		{RegisterGroupType::None, "None"},
		{RegisterGroupType::Coil, "Coil"},
		{RegisterGroupType::Input, "Input"},
		{RegisterGroupType::InputRegister, "InputRegister"},
		{RegisterGroupType::HoldingRegister, "HoldingRegister"}
	};

	RegisterGroupConfig::RegisterGroupConfig(void)
	{
	}

	RegisterGroupConfig::~RegisterGroupConfig(void)
	{
	}

	std::string
	RegisterGroupConfig::toString(Type type)
	{
		auto it = typeMap_.find(type);
		return it == typeMap_.end() ? "Unknown" : it->second;
	}

	Type
	RegisterGroupConfig::toType(const std::string& type)
	{
		for (auto it : typeMap_) {
			if (it->second == type) return it->first;
		}
		return Type::None;
	}

	bool
	RegisterGroupConfig::parse(Config& config)
	{
		bool rc = true;

		// Get register group from configuration
		std::string group = "";
		rc = config.getConfigParameter("<xmlattr>.Group", group);
		if (rc == false) {
			Log(Error, "attribute not found in register group configuration")
				.parameter("Attribute", "Group");
			return false;
		}
		type_ = toType(group);
		if (type_ == Type::None) {
			Log(Error, "invalid group type in register group configuration")
				.parameter("Attribute", "Group")
				.parameter("GroupType", group);
			return false;
		}

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.GroupName", groupName_);
		if (rc == false) {
			Log(Error, "attribute not found in register group configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get interval attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Interval", interval_);
		if (rc == false) {
			Log(Error, "attribute not found in register group configuration")
				.parameter("Interval", interval_)
				.parameter("Attribute", "Interval");
			return false;
		}

		// Find input register entries in configuration
		std::vector<Config> configVec;
		config.getChilds("Register", configVec);
		if (configVec.size() != 0) {
			// parse input register entries
			for (auto configEntry: configVec) {
				auto registerConfig = std::make_shared<RegisterConfig>();

				// parse register entry
				rc = registerConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse register group entry error");
					return false;
				}

				// add register configuration entry to map
				registerConfigVec_.push_back(registerConfig);
			}
		}

		return true;
	}

	std::string
	RegisterGroupConfig::groupName(void)
	{
		return groupName_;
	}

	uint32_t
	RegisterGroupConfig::interval(void)
	{
		return interval_;
	}

	RegisterConfig::Vec&
	RegisterGroupConfig::registerConfigVec(void)
	{
		return registerConfigVec_;
	}



	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class ModbusTCPClientConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ModbusTCPClientConfig::ModbusTCPClientConfig(void)
	{
	}

	ModbusTCPClientConfig::~ModbusTCPClientConfig(void)
	{
	}

	bool
	ModbusTCPClientConfig::parse(Config& config)
	{
		bool rc = true;
		std::vector<Config> configVec;

		// Get modbus tcp client name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Name", name_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp client configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get ip address attribute from configuration
		rc = config.getConfigParameter("IPAddress", ipAddress_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp client configuration")
				.parameter("IPAddress", ipAddress_)
				.parameter("Attribute", "IPAddress");
			return false;
		}

		// Get port attribute from configuration
		rc = config.getConfigParameter("Port", port_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp client configuration")
				.parameter("Port", port_)
				.parameter("Attribute", "Port");
			return false;
		}

		// Get slave id attribute from configuration
		rc = config.getConfigParameter("SlaveId", slaveId_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp client configuration")
				.parameter("SlaveId", slaveId_)
				.parameter("Attribute", "SlaveId");
			return false;
		}

		// Get connect timeout from configuration
		rc = config.getConfigParameter("ConnectTimeout", connectTimeout_);
		if (rc == false) {
			Log(Debug, "attribute not found in modbus tcp client configuration, use default")
				.parameter("ConnectTimeout", connectTimeout_)
				.parameter("Attribute", "ConnectTimeout");
		}

		// Get reconnect timeout from configuration
		rc = config.getConfigParameter("ReconnectTimeout", reconnectTimeout_);
		if (rc == false) {
			Log(Debug, "attribute not found in modbus tcp client configuration, use default")
				.parameter("ReconnectTimeout", reconnectTimeout_)
				.parameter("Attribute", "ReconnectTimeout");
		}

		// Get send timeout from configuration
		rc = config.getConfigParameter("SendTimeout", sendTimeout_);
		if (rc == false) {
			Log(Debug, "attribute not found in modbus tcp client configuration, use default")
				.parameter("SendTimeout", sendTimeout_)
				.parameter("Attribute", "SendTimeout");
		}

		// Get recv timeout from configuration
		rc = config.getConfigParameter("RecvTimeout", recvTimeout_);
		if (rc == false) {
			Log(Debug, "attribute not found in modbus tcp client configuration, use default")
				.parameter("RecvTimeout", recvTimeout_)
				.parameter("Attribute", "RecvTimeout");
		}

		// Get query timeout from configuration
		rc = config.getConfigParameter("QueryTimeout", queryTimeout_);
		if (rc == false) {
			Log(Debug, "attribute not found in modbus tcp client configuration, use default")
				.parameter("QueryTimeout", queryTimeout_)
				.parameter("Attribute", "queryTimeout");
		}

		// Find coil entries in configuration
		configVec.clear();
		config.getChilds("Coils", configVec);
		if (configVec.size() != 0) {
			// parse modbus client entries
			for (auto configEntry: configVec) {
				auto coilsConfig = std::make_shared<CoilsConfig>();

				// parse coils entry
				rc = coilsConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse coils entry error");
					return false;
				}

				// add coils configuration entry to map
				coilsConfigVec_.push_back(coilsConfig);
			}
		}

		// Find input entries in configuration
		configVec.clear();
		config.getChilds("Inputs", configVec);
		if (configVec.size() != 0) {
			// parse modbus client entries
			for (auto configEntry: configVec) {
				auto inputsConfig = std::make_shared<InputsConfig>();

				// parse inputs entry
				rc = inputsConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse inputs entry error");
					return false;
				}

				// add inputs configuration entry to map
				inputsConfigVec_.push_back(inputsConfig);
			}
		}

		// Find holding registers entries in configuration
		configVec.clear();
		config.getChilds("HoldingRegisters", configVec);
		if (configVec.size() != 0) {
			// parse modbus client entries
			for (auto configEntry: configVec) {
				auto holdingRegistersConfig = std::make_shared<HoldingRegistersConfig>();

				// parse holding registers entry
				rc = holdingRegistersConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse holding registers entry error");
					return false;
				}

				// add holding registers configuration entry to map
				holdingRegistersConfigVec_.push_back(holdingRegistersConfig);
			}
		}


		// Find input registers entries in configuration
		configVec.clear();
		config.getChilds("InputRegisters", configVec);
		if (configVec.size() != 0) {
			// parse modbus client entries
			for (auto configEntry: configVec) {
				auto inputRegistersConfig = std::make_shared<InputRegistersConfig>();

				// parse input registers entry
				rc = inputRegistersConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse input registers entry error");
					return false;
				}

				// add input registers configuration entry to map
				inputRegistersConfigVec_.push_back(inputRegistersConfig);
			}
		}

		return true;
	}

	std::string
	ModbusTCPClientConfig::name(void)
	{
		return name_;
	}

	std::string
	ModbusTCPClientConfig::ipAddress(void)
	{
		return ipAddress_;
	}

	uint32_t
	ModbusTCPClientConfig::port(void)
	{
		return port_;
	}

	uint8_t
	ModbusTCPClientConfig::slaveId(void)
	{
		return slaveId_;
	}

	CoilsConfig::Vec&
	ModbusTCPClientConfig::coilsConfigVec(void)
	{
		return coilsConfigVec_;
	}

	InputsConfig::Vec&
	ModbusTCPClientConfig::inputsConfigVec(void)
	{
		return inputsConfigVec_;
	}

	HoldingRegistersConfig::Vec&
	ModbusTCPClientConfig::holdingRegistersConfigVec(void)
	{
		return holdingRegistersConfigVec_;
	}

	InputRegistersConfig::Vec&
	ModbusTCPClientConfig::inputRegistersConfigVec(void)
	{
		return inputRegistersConfigVec_;
	}

	uint32_t
	ModbusTCPClientConfig::connectTimeout(void)
	{
		return connectTimeout_;
	}

	uint32_t
	ModbusTCPClientConfig::reconnectTimeout(void)
	{
		return reconnectTimeout_;
	}

	uint32_t
	ModbusTCPClientConfig::sendTimeout(void)
	{
		return sendTimeout_;
	}

	uint32_t
	ModbusTCPClientConfig::recvTimeout(void)
	{
		return recvTimeout_;
	}

	uint32_t
	ModbusTCPClientConfig::queryTimeout(void)
	{
		return queryTimeout_;
	}


	// ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    //
    // class ModbusGatewayConfig
    //
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
	ModbusGatewayConfig::ModbusGatewayConfig(void)
	{
	}

	ModbusGatewayConfig::~ModbusGatewayConfig(void)
	{
	}

	bool
	ModbusGatewayConfig::parse(const std::string& fileName)
	{
		bool rc = true;

		// Read configuration file
		Config config;
		ConfigXml configXml;
		rc = configXml.parse(fileName, &config);
		if (rc == false) {
			Log(Error, "read configuration file error")
				.parameter("ConfigurationFile", fileName)
				.parameter("Reason", configXml.errorMessage());
			return false;
		}

		// Find modbus tcp client entries in configuration
		std::vector<Config> configVec;
		config.getChilds("OpcUaModbusGateway.ModbusTCPClient", configVec);
		if (configVec.size() == 0) {
			Log(Error, "modbus client configuration not found in configuration file")
				.parameter("ConfigurationFile", fileName)
				.parameter("Attribute", "OpcUaModbusGateway.ModbusTCPClient");
			return false;
		}

		// parse modbus client entries
		for (auto configEntry: configVec) {
			auto modbusTCPClientConfig = std::make_shared<ModbusTCPClientConfig>();

			// parse modbus tcp client entry
			rc = modbusTCPClientConfig->parse(configEntry);
			if (rc == false) {
				Log(Error, "parse modbus tcp client entry error")
					.parameter("ConfigurationFile", fileName)
					.parameter("Attribute", "OpcUaModbusGateway.ModbusTCPClient");
				return false;
			}

			// add file system entry to file system entry vector
			modbusTCPClientConfigVec_.push_back(modbusTCPClientConfig);
		}

		return true;
	}

	ModbusTCPClientConfig::Vec&
	ModbusGatewayConfig::modbusTCPClientConfig(void)
	{
		return modbusTCPClientConfigVec_;
	}

}
