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
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		OpcUaStackCore::OpcUaNodeId& rootNodeId
	)
	{
		bool rc = true;

		namespaceName_ = namespaceName;
		modbusTCPClientConfig_ = modbusTCPClientConfig;
		rootNodeId_ = rootNodeId;
		applicationServiceIf_ = applicationServiceIf;

		// Get register folder nodes
		rc = getRegisterFolderNodeIds();
		if (!rc) {
			return false;
		}

#if 0
		// Create coil nodes
		for (auto coils: modbusTCPClientConfig_->coilsConfigVec()) {
			for (auto coil : coils->coilConfigVec()) {
				auto analogValue = boost::make_shared<AnalogValue>();
				Object::SPtr obj = analogValue;
				CreateVariableInstance createVariableInstance(
					namespaceName_,									// namespace name of the object instance
					OpcUaLocalizedText("", coil->name()),			// display name of the object instance
					coilsFolderNodeId_,								// parent node of the object instance
					OpcUaNodeId((uint32_t)OpcUaId_Organizes),		// reference type between object and variable instance
					obj
				);
				if (!createVariableInstance.query(applicationServiceIf_)) {
					Log(Error, "create variable error")
						.parameter("DisplayName", coil->name());
					return false;
				}

				analogValue_.push_back(analogValue);
			}
		}
#endif

		return true;
	}

	bool
	OpcUaModbusClientRegister::shutdown(void)
	{
		return true;
	}

  	bool
	OpcUaModbusClientRegister::getRegisterFolderNodeIds(void)
   	{

   		// Find coils folder nodeid
   		{
   			BrowsePathToNodeId browsePathToNodeId({
   				BrowseName(rootNodeId_, OpcUaQualifiedName(NodeName::coilsFolder_, rootNodeId_.namespaceIndex()))
   			});
   			if (!browsePathToNodeId.query(applicationServiceIf_, true)) {
   				Log(Error, "call browse path to node id service error (Coils)");
   				return false;
   			}
   			coilsFolderNodeId_ = browsePathToNodeId.nodes()[0];
   		}

		// Find inputs folder nodeid
   		{
   			BrowsePathToNodeId browsePathToNodeId({
   				BrowseName(rootNodeId_, OpcUaQualifiedName(NodeName::inputsFolder_, rootNodeId_.namespaceIndex()))
   			});
   			if (!browsePathToNodeId.query(applicationServiceIf_, true)) {
   				Log(Error, "call browse path to node id service error (Inputs)");
   				return false;
   			}
   			inputsFolderNodeId_ = browsePathToNodeId.nodes()[0];
   		}

		// Find holding registers folder nodeid
   		{
   			BrowsePathToNodeId browsePathToNodeId({
   				BrowseName(rootNodeId_, OpcUaQualifiedName(NodeName::holdingRegistersFolder_, rootNodeId_.namespaceIndex()))
   			});
   			if (!browsePathToNodeId.query(applicationServiceIf_, true)) {
   				Log(Error, "call browse path to node id service error (HoldingRegisters)");
   				return false;
   			}
   			holdingRegistersFolderNodeId_ = browsePathToNodeId.nodes()[0];
   		}

		// Find input registers folder nodeid
   		{
   			BrowsePathToNodeId browsePathToNodeId({
   				BrowseName(rootNodeId_, OpcUaQualifiedName(NodeName::inputRegistersFolder_, rootNodeId_.namespaceIndex()))
   			});
   			if (!browsePathToNodeId.query(applicationServiceIf_, true)) {
   				Log(Error, "call browse path to node id service error (InputRegisters)");
   				return false;
   			}
   			inputRegistersFolderNodeId_ = browsePathToNodeId.nodes()[0];
   		}

   		return true;
   	}

}

