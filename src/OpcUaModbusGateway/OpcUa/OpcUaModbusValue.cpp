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

#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateVariableInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"

#include "OpcUaModbusGateway/OpcUa/OpcUaModbusValue.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaModbusGateway
{

	uint32_t OpcUaModbusValue::id_ = 11000;

	OpcUaModbusValue::OpcUaModbusValue(void)
	{
	}

	OpcUaModbusValue::~OpcUaModbusValue(void)
	{
	}

	bool
	OpcUaModbusValue::startup(
		const std::string& namespaceName,
		uint32_t namespaceIndex,
		RegisterConfig::SPtr registerConfig,
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		OpcUaStackCore::OpcUaNodeId& rootNodeId
	)
	{
		// Set parameter
		namespaceName_ = namespaceName;
		namespaceIndex_ = namespaceIndex;
		registerConfig_ = registerConfig;
		applicationServiceIf_ = applicationServiceIf;
		rootNodeId_ = rootNodeId;

		// create a new modbus value object instance in opc ua information model
		auto analogValue = boost::make_shared<AnalogValue>();
		Object::SPtr obj = analogValue;
		CreateVariableInstance createVariableInstance(
			namespaceName_,									// namespace name of the object instance
			OpcUaLocalizedText("", registerConfig->name()), // display name of the object instance
			rootNodeId,										// parent node of the object instance
			OpcUaNodeId((uint32_t)OpcUaId_HasComponent),	// reference type between object and variable instance
			obj
		);
		id_++;
		valueNodeId_.set((uint32_t)id_, namespaceIndex_);
		createVariableInstance.addNodeId("Variable", valueNodeId_);
		if (!createVariableInstance.query(applicationServiceIf_)) {
			Log(Error, "create register value object error")
				.parameter("DisplayName", registerConfig->name());
			return false;
		}
		analogValueVec_.push_back(analogValue);

		return true;
	}

	bool
	OpcUaModbusValue::shutdown(void)
	{
		// remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(valueNodeId_);

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete value node instance error")
				.parameter("ValueNodeId", valueNodeId_)
				.parameter("ValueName", registerConfig_->name());
			return false;
		}

		return true;
	}

	void
	OpcUaModbusValue::getWriteDataValue(OpcUaStackCore::OpcUaDataValue& writeDataValue)
	{
		writeDataValue = writeDataValue_;
	}


}

