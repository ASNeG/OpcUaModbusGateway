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

#include "OpcUaStackCore/BuildInTypes/OpcUaTypeConversion.h"

#include "OpcUaModbusGateway/Util/TypeConverter.h"

using namespace OpcUaStackCore;

namespace OpcUaModbusGateway
{

	TypeConverter::TypeConverter(void)
	{
	}

	TypeConverter::~TypeConverter(void)
	{
	}

	bool
	TypeConverter::checkType(
		const std::string& typeString
	)
	{
		// Check if type string in opc ua exist
		auto type = OpcUaBuildInTypeMap::string2BuildInType(typeString);
		if (type == OpcUaBuildInType_Unknown) return false;
		return true;
	}

	OpcUaBuildInType
	TypeConverter::getType(
		const std::string& typeString)
	{
		return OpcUaBuildInTypeMap::string2BuildInType(typeString);
	}

	bool
	TypeConverter::canConversion(
		const std::string& fromTypeString,
		const std::string& toTypeString
	)
	{
		if (!checkType(fromTypeString)) return false;
		if (!checkType(toTypeString)) return false;
		auto fromType = getType(fromTypeString);
		auto toType = getType(toTypeString);

		OpcUaTypeConversion typeConv;
		auto conversionType = typeConv.conversionType(fromType, toType);
		if (conversionType == 'X') return false;
		return true;
	}

	bool
	TypeConverter::conversion(
		OpcUaVariant& sourceVariant,
		OpcUaBuildInType targetType,
		OpcUaVariant& targetVariant
	)
	{
		OpcUaTypeConversion typeConv;
		return typeConv.conversion(sourceVariant, targetType, targetVariant);
		return true;
	}

}
