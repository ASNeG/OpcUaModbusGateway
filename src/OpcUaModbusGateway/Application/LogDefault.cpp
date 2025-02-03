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

#include <iostream>
#include <chrono>

#include "OpcUaStackCore/Base/Log.h"

#include "OpcUaModbusGateway/Application/LogDefault.h"

namespace OpcUaModbusGateway
{

	using namespace OpcUaStackCore;

	LogDefault::LogDefault(const std::string& message)
	: Base::LogHandler()
	, message_(message)
	{
	}

	LogDefault::~LogDefault(void)
	{
	}

	void
	LogDefault::message(const std::string& message)
	{
		message_ = message;
	}

	void
	LogDefault::log(Base::LogLevel logLevel, const std::string& message)
	{
		switch (logLevel)
		{
			case Base::LogLevel::Error:
			{
				Log(Error, message_).parameter("Message", message);
				break;
			}
			case Base::LogLevel::Warning:
			{
				Log(Warning, message_).parameter("Message", message);
				break;
			}
			case Base::LogLevel::Info:
			{
				Log(Info, message_).parameter("Message", message);
				break;
			}
			case Base::LogLevel::Debug:
			{
				Log(Debug, message_).parameter("Message", message);
				break;
			}
		}
	}

}
