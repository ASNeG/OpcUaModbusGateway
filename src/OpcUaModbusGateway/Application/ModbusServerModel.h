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

#ifndef __OpcUaModbusGateway_ModbusServerModel_h__
#define __OpcUaModbusGateway_ModbusServerModel_h__

#include <memory>
#include <vector>

namespace OpcUaModbusGateway
{

	class ModbusServerModel
	{
	  public:
		using SPtr = std::shared_ptr<ModbusServerModel>;
		using Vec = std::vector<SPtr>;

		ModbusServerModel(void);
		~ModbusServerModel(void);

	  private:
	};

}

#endif
