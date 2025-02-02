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

#ifndef __OpcUaModbusGateway_ModbusTCPServer_h__
#define __OpcUaModbusGateway_ModbusTCPServer_h__

#include <memory>
#include <vector>

#include "OpcUaModbusGateway/Application/ModbusServerModel.h"
#include "OpcUaModbusGateway/Util/ModbusGatewayConfig.h"

namespace OpcUaModbusGateway
{

	class ModbusTCPServerImpl;

	class ModbusTCPServer
	{
	  public:
		using SPtr = std::shared_ptr<ModbusTCPServer>;
		using Vec = std::vector<SPtr>;
		using StateCallback = std::function<void (const std::string&)>;

		ModbusTCPServer(void);
		~ModbusTCPServer(void);

		void stateCallback(StateCallback stateCallback);
		void modbusServerModel(ModbusServerModel::SPtr modbusServerModel);

		bool open(ModbusTCPServerConfig::SPtr& modbusTCPServerConfig);
		bool close(void);

	  private:
		ModbusTCPServerImpl* modbusTCPServerImpl_ = nullptr;
	};

}

#endif
