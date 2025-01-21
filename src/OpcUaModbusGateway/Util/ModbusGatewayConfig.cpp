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
			Log(Error, "file system configuration not found in configuration file")
				.parameter("ConfigurationFile", fileName)
				.parameter("Attribute", "OpcUaModbusGateway.ModbusTCPClient");
			return false;
		}

		// parse modbus client entries
		for (auto configEntry: configVec) {
			auto modbusTCPClientConfig = std::make_shared<ModbusTCPClientConfig>();

			// parse file system entry
			rc = modbusTCPClientConfig->parse(configEntry);
			if (rc == false) {
				Log(Error, "parse file system entry error")
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
