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
#include "OpcUaModbusGateway/Util/TypeConverter.h"

using namespace OpcUaStackCore;

namespace OpcUaModbusGateway
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class RegisterConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	std::map<RegisterConfig::ModbusType, std::string> RegisterConfig::modbusTypeMap_ = {
		{RegisterConfig::ModbusType::None, "None"},
		{RegisterConfig::ModbusType::Bool, "Bool"},
		{RegisterConfig::ModbusType::UInt16, "UInt16"}
	};

	std::map<RegisterConfig::ModbusAccess, std::string> RegisterConfig::modbusAccessMap_ = {
		{RegisterConfig::ModbusAccess::None, "None"},
		{RegisterConfig::ModbusAccess::Read, "Read"},
		{RegisterConfig::ModbusAccess::Write, "Write"},
		{RegisterConfig::ModbusAccess::ReadWrite, "ReadWrite"}
	};

	std::map<RegisterConfig::ModbusAppl, std::string> RegisterConfig::modbusApplMap_ = {
		{RegisterConfig::ModbusAppl::None, "None"},
		{RegisterConfig::ModbusAppl::Slave, "Slave"},
		{RegisterConfig::ModbusAppl::Master, "Master"}
	};


	RegisterConfig::RegisterConfig(
		ModbusType modbusType,
		ModbusAccess modbusAccess,
		ModbusAppl modbusAppl
	)
	: modbusType_(modbusType)
	, modbusAccess_(modbusAccess)
	, modbusAppl_(modbusAppl)
	{
	}

	RegisterConfig::~RegisterConfig(void)
	{
	}

	std::string
	RegisterConfig::toString(ModbusType type)
	{
		auto it = modbusTypeMap_.find(type);
		return it == modbusTypeMap_.end() ? "Unknown" : it->second;
	}

	RegisterConfig::ModbusType
	RegisterConfig::toType(const std::string& modbusType)
	{
		for (auto it : modbusTypeMap_) {
			if (it.second == modbusType) return it.first;
		}
		return ModbusType::None;
	}

	std::string
	RegisterConfig::toString(ModbusAccess access)
	{
		auto it = modbusAccessMap_.find(access);
		return it == modbusAccessMap_.end() ? "Unknown" : it->second;
	}

	RegisterConfig::ModbusAccess
	RegisterConfig::toAccess(const std::string& modbusAccess)
	{
		for (auto it : modbusAccessMap_) {
			if (it.second == modbusAccess) return it.first;
		}
		return ModbusAccess::None;
	}

	std::string
	RegisterConfig::toString(ModbusAppl appl)
	{
		auto it = modbusApplMap_.find(appl);
		return it == modbusApplMap_.end() ? "Unknown" : it->second;
	}

	RegisterConfig::ModbusAppl
	RegisterConfig::toAppl(const std::string& modbusAppl)
	{
		for (auto it : modbusApplMap_) {
			if (it.second == modbusAppl) return it.first;
		}
		return ModbusAppl::None;
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
			unit_ = "Digit";
		}

		// Get type attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Type", type_);
		if (rc == false) {
			type_ = "Boolean";
			if (modbusType_ == RegisterConfig::ModbusType::UInt16) type_ = "UInt16";
		}
		// Check if type exist in opc ua model
		rc = TypeConverter::checkType(type_);
		if (rc == false) {
			Log(Error, "unknown type found in register configuration")
				.parameter("TypeName", type_);
			return false;
		}
		modbusTypeString_ = modbusType_ == RegisterConfig::ModbusType::UInt16 ? "UInt16" : "Boolean";
		opcUaTypeString_ = type_;
		// Check conversion support
		rc = TypeConverter::canConversion(modbusTypeString_, opcUaTypeString_);
		if (!rc) {
			Log(Error, "cannot convert modbus variable type to opc ua variable type")
				.parameter("ModbusType", modbusTypeString_)
				.parameter("OpcUaType", opcUaTypeString_);
			return false;
		}
		rc = TypeConverter::canConversion(opcUaTypeString_, modbusTypeString_);
		if (!rc) {
			Log(Error, "cannot convert opc ua variable type to modbus variable type")
				.parameter("OpcUaType", opcUaTypeString_)
				.parameter("ModbusType", modbusTypeString_);
			return false;
		}

		// Get a_ attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.A", a_);
		if (rc == false) {
		    a_ = 0.0;
		}

		// Get b_ attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.B", b_);
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

	RegisterConfig::ModbusType
	RegisterConfig::modbusType(void)
	{
		return modbusType_;
	}

	std::string&
	RegisterConfig::opcUaTypeString(void)
	{
		return opcUaTypeString_;
	}

	RegisterConfig::ModbusAccess
	RegisterConfig::modbusAccess(void)
	{
		return modbusAccess_;
	}

	RegisterConfig::ModbusAppl
	RegisterConfig::modbusAppl(void)
	{
		return modbusAppl_;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class RegisterGroupConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	std::map<RegisterGroupConfig::ModbusGroupType, std::string> RegisterGroupConfig::modbusGroupTypeMap_ = {
		{RegisterGroupConfig::ModbusGroupType::None, "None"},
		{RegisterGroupConfig::ModbusGroupType::Coil, "Coil"},
		{RegisterGroupConfig::ModbusGroupType::Input, "Input"},
		{RegisterGroupConfig::ModbusGroupType::InputRegister, "InputRegister"},
		{RegisterGroupConfig::ModbusGroupType::HoldingRegister, "HoldingRegister"}
	};


	RegisterGroupConfig::RegisterGroupConfig(
		RegisterConfig::ModbusAppl modbusAppl
	)
	: modbusAppl_(modbusAppl)
	{
	}

	RegisterGroupConfig::~RegisterGroupConfig(void)
	{
	}

	std::string
	RegisterGroupConfig::toString(ModbusGroupType modbusGroupType)
	{
		auto it = modbusGroupTypeMap_.find(modbusGroupType);
		return it == modbusGroupTypeMap_.end() ? "Unknown" : it->second;
	}

	RegisterGroupConfig::ModbusGroupType
	RegisterGroupConfig::toType(const std::string& type)
	{
		for (auto it : modbusGroupTypeMap_) {
			if (it.second == type) return it.first;
		}
		return ModbusGroupType::None;
	}

	RegisterGroupConfig::ModbusGroupType
	RegisterGroupConfig::type(void)
	{
		return modbusGroupType_;
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
		modbusGroupType_ = toType(group);
		if (modbusGroupType_ == ModbusGroupType::None) {
			Log(Error, "invalid group type in register group configuration")
				.parameter("Attribute", "Group")
				.parameter("GroupType", group);
			return false;
		}

		auto modbusType = RegisterConfig::ModbusType::Bool;
		if (modbusGroupType_ == ModbusGroupType::InputRegister) modbusType = RegisterConfig::ModbusType::UInt16;
		if (modbusGroupType_ == ModbusGroupType::HoldingRegister) modbusType = RegisterConfig::ModbusType::UInt16;

		auto modbusAccess = RegisterConfig::ModbusAccess::ReadWrite;
		if (modbusAppl_ == RegisterConfig::ModbusAppl::Master) {
			if (modbusGroupType_ == ModbusGroupType::InputRegister) modbusAccess = RegisterConfig::ModbusAccess::Read;
			if (modbusGroupType_ == ModbusGroupType::Input) modbusAccess = RegisterConfig::ModbusAccess::Read;
		}

		// Get name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.GroupName", groupName_);
		if (rc == false) {
			Log(Error, "attribute not found in register group configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get read interval attribute from configuration
		if (modbusAppl_ == RegisterConfig::ModbusAppl::Slave) {
			readInterval_ = 0;
		}
		else {
			rc = config.getConfigParameter("<xmlattr>.ReadInterval", readInterval_);
			if (rc == false) {
				Log(Error, "attribute not found in register group configuration")
					.parameter("Attribute", "readInterval")
					.parameter("GroupName", groupName_);
				return false;
			}
		}

		// Get write interval attribute from configuration
		if (modbusAppl_ == RegisterConfig::ModbusAppl::Slave) {
			writeInterval_ = 0;
		}
		else {
			rc = config.getConfigParameter("<xmlattr>.WriteInterval", writeInterval_);
			if (rc == false) {
				Log(Error, "attribute not found in register group configuration")
					.parameter("Attribute", "WriteInterval")
					.parameter("GroupName", groupName_);
				return false;
			}
		}

		// Find input register entries in configuration
		std::vector<Config> configVec;
		config.getChilds("Register", configVec);
		if (configVec.size() != 0) {
			// parse input register entries
			for (auto configEntry: configVec) {
				auto registerConfig = std::make_shared<RegisterConfig>(modbusType, modbusAccess, modbusAppl_);

				// parse register entry
				rc = registerConfig->parse(configEntry);
				if (rc == false) {
					Log(Error, "parse register entry error")
						.parameter("GroupName", groupName_);
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
	RegisterGroupConfig::writeInterval(void)
	{
		return writeInterval_;
	}

	uint32_t
	RegisterGroupConfig::readInterval(void)
	{
		return readInterval_;
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

	RegisterGroupConfig::Vec&
	ModbusTCPClientConfig::registerGroupConfigVec(RegisterGroupConfig::ModbusGroupType modbusGroupType)
	{
		auto it = registerGroupMap_.find(modbusGroupType);
		if (it == registerGroupMap_.end()) {
			registerGroupMap_.insert(std::make_pair(modbusGroupType, RegisterGroupConfig::Vec()));
			auto it = registerGroupMap_.find(modbusGroupType);
			return it->second;
		}
		return it->second;
	}

	std::vector<RegisterGroupConfig::ModbusGroupType>
	ModbusTCPClientConfig::registerGroupTypes(void)
	{
		std::vector<RegisterGroupConfig::ModbusGroupType> vec;
		for (auto it : registerGroupMap_) {
			vec.push_back(it.first);
		}
		return vec;
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

		// Find register group entries in configuration
		configVec.clear();
		config.getChilds("RegisterGroup", configVec);
		if (configVec.size() != 0) {

			for (auto configEntry : configVec) {
				// create register group config
				auto registerGroup = std::make_shared<RegisterGroupConfig>(RegisterConfig::ModbusAppl::Master);

				// Parse register group config
				rc = registerGroup->parse(configEntry);
				if (!rc) {
					Log(Error, "parse register group entry error");
					return false;
				}

				// Add new register group to map
				registerGroupConfigVec(registerGroup->type()).push_back(registerGroup);
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
	// class ModbusTCPServerConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ModbusTCPServerConfig::ModbusTCPServerConfig(void)
	{
	}

	ModbusTCPServerConfig::~ModbusTCPServerConfig(void)
	{
	}

	RegisterGroupConfig::Vec&
	ModbusTCPServerConfig::registerGroupConfigVec(RegisterGroupConfig::ModbusGroupType modbusGroupType)
	{
		auto it = registerGroupMap_.find(modbusGroupType);
		if (it == registerGroupMap_.end()) {
			registerGroupMap_.insert(std::make_pair(modbusGroupType, RegisterGroupConfig::Vec()));
			auto it = registerGroupMap_.find(modbusGroupType);
			return it->second;
		}
		return it->second;
	}

	std::vector<RegisterGroupConfig::ModbusGroupType>
	ModbusTCPServerConfig::registerGroupTypes(void)
	{
		std::vector<RegisterGroupConfig::ModbusGroupType> vec;
		for (auto it : registerGroupMap_) {
			vec.push_back(it.first);
		}
		return vec;
	}

	bool
	ModbusTCPServerConfig::parse(Config& config)
	{
		bool rc = true;
		std::vector<Config> configVec;

		// Get modbus tcp server name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.Name", name_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp server configuration")
				.parameter("Attribute", "Name");
			return false;
		}

		// Get ip address attribute from configuration
		rc = config.getConfigParameter("IPAddress", ipAddress_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp server configuration")
				.parameter("IPAddress", ipAddress_)
				.parameter("Attribute", "IPAddress");
			return false;
		}

		// Get port attribute from configuration
		rc = config.getConfigParameter("Port", port_);
		if (rc == false) {
			Log(Error, "attribute not found in modbus tcp server configuration")
				.parameter("Port", port_)
				.parameter("Attribute", "Port");
			return false;
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

		// Find register group entries in configuration
		configVec.clear();
		config.getChilds("RegisterGroup", configVec);
		if (configVec.size() != 0) {

			for (auto configEntry : configVec) {
				// create register group config
				auto registerGroup = std::make_shared<RegisterGroupConfig>(RegisterConfig::ModbusAppl::Slave);

				// Parse register group config
				rc = registerGroup->parse(configEntry);
				if (!rc) {
					Log(Error, "parse register group entry error");
					return false;
				}

				// Add new register group to map
				registerGroupConfigVec(registerGroup->type()).push_back(registerGroup);
			}
		}

		return true;
	}

	std::string
	ModbusTCPServerConfig::name(void)
	{
		return name_;
	}

	std::string
	ModbusTCPServerConfig::ipAddress(void)
	{
		return ipAddress_;
	}

	uint32_t
	ModbusTCPServerConfig::port(void)
	{
		return port_;
	}

	uint32_t
	ModbusTCPServerConfig::sendTimeout(void)
	{
		return sendTimeout_;
	}

	uint32_t
	ModbusTCPServerConfig::recvTimeout(void)
	{
		return recvTimeout_;
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

		// Find modbus tcp server entries in configuration
		configVec.clear();
		config.getChilds("OpcUaModbusGateway.ModbusTCPServer", configVec);
		if (configVec.size() == 0) {
			Log(Error, "modbus server configuration not found in configuration file")
				.parameter("ConfigurationFile", fileName)
				.parameter("Attribute", "OpcUaModbusGateway.ModbusTCPServer");
			return false;
		}

		// parse modbus server entries
		for (auto configEntry: configVec) {
			auto modbusTCPServerConfig = std::make_shared<ModbusTCPServerConfig>();

			// parse modbus tcp server entry
			rc = modbusTCPServerConfig->parse(configEntry);
			if (rc == false) {
				Log(Error, "parse modbus tcp server entry error")
					.parameter("ConfigurationFile", fileName)
					.parameter("Attribute", "OpcUaModbusGateway.ModbusTCPServer");
				return false;
			}

			// add file system entry to file system entry vector
			modbusTCPServerConfigVec_.push_back(modbusTCPServerConfig);
		}

		return true;
	}

	ModbusTCPClientConfig::Vec&
	ModbusGatewayConfig::modbusTCPClientConfig(void)
	{
		return modbusTCPClientConfigVec_;
	}

	ModbusTCPServerConfig::Vec&
	ModbusGatewayConfig::modbusTCPServerConfig(void)
	{
		return modbusTCPServerConfigVec_;
	}

}
