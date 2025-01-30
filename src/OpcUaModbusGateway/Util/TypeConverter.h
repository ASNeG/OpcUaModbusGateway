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

#ifndef __OpcUaModbusGateway_TypeConverter_h__
#define __OpcUaModbusGateway_TypeConverter_h__

#include <string>

#include "OpcUaStackCore/BuildInTypes/OpcUaType.h"

namespace OpcUaModbusGateway
{

	class TypeConverter
	{
	  public:
		TypeConverter(void);
		~TypeConverter(void);

		static bool checkType(const std::string& typeString);
		static OpcUaStackCore::OpcUaBuildInType getType(const std::string& typeString);
		static bool canConversion(const std::string& fromTypeString, const std::string& toTypeString);
	};

}

#endif
