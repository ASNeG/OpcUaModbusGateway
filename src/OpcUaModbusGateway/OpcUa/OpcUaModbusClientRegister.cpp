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

#include <boost/make_shared.hpp>

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackServer/ServiceSetApplication/BrowsePathToNodeId.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateVariableInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"

#include "OpcUaModbusGateway/OpcUa/OpcUaModbusClientRegister.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

	namespace NodeName {
		// Modbus tcp client register nodes
		const std::string coilsFolder_ = "Coils";
		const std::string inputsFolder_ = "Inputs";
		const std::string holdingRegistersFolder_ = "HoldingRegisters";
		const std::string inputRegistersFolder_ = "InputRegisters";
	}


	OpcUaModbusClientRegister::OpcUaModbusClientRegister(void)
	{
	}

	OpcUaModbusClientRegister::~OpcUaModbusClientRegister(void)
	{
	}

	bool
	OpcUaModbusClientRegister::startup(
		const std::string& namespaceName,
		ModbusTCPClientConfig::SPtr modbusTCPClientConfig,
		OpcUaStackServer::ApplicationIf* applicationIf,
		OpcUaStackCore::OpcUaNodeId& rootNodeId,
		ModbusTCPClient::SPtr modbusTCPClient
	)
	{
		bool rc = true;

		namespaceName_ = namespaceName;
		modbusTCPClientConfig_ = modbusTCPClientConfig;
		rootNodeId_ = rootNodeId;
		applicationIf_ = applicationIf;

		// Create register type folder
		for (auto registerGroupType : modbusTCPClientConfig->registerGroupTypes()) {
			// Get root type node id from opc ua information model
			auto displayName = RegisterGroupConfig::toString(registerGroupType)  + "s";
   			BrowsePathToNodeId browsePathToNodeId({
   				BrowseName(rootNodeId_, OpcUaQualifiedName(displayName, rootNodeId_.namespaceIndex()))
   			});
   			if (!browsePathToNodeId.query(&applicationIf_->service(), true)) {
   				Log(Error, "call browse path to node id service error")
   					.parameter("DisplayName", displayName);
   				return false;
   			}
   			auto typeNodeId = browsePathToNodeId.nodes()[0];

   			// Get a list of all register group configurations for actual register type
   			// and create a group instance
   			auto registerGroupConfigs = modbusTCPClientConfig->registerGroupConfigVec(registerGroupType);
   			for (auto registerGroupConfig : registerGroupConfigs) {
   				auto clientGroup = std::make_shared<OpcUaModbusClientGroup>();
   				clientGroup->maxRegisterInRequest(modbusTCPClientConfig->maxNumRegPDU());
   				auto rc =  clientGroup->startup(
   					namespaceName,
					rootNodeId_.namespaceIndex(),
					registerGroupConfig,
					applicationIf_,
					typeNodeId,
					modbusTCPClient
   				);
   				if (!rc) {
   					Log(Error, "create opc ua register group error")
   						.parameter("GroupName", registerGroupConfig->groupName());
   					return true;
   				}

   				modbusClientGroupVec_.push_back(clientGroup);
   			}
		}

		return true;
	}

	bool
	OpcUaModbusClientRegister::shutdown(void)
	{
		// Shutdown modbus client groups
		for (auto clientGroup : modbusClientGroupVec_) {
			auto rc = clientGroup->shutdown();
			if (!rc) {
				Log(Error, "delete client group node instance error");
				return false;
			}
		}

		return true;
	}

}

