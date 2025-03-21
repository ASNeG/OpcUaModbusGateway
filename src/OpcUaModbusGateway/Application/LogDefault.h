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

#ifndef __OpcUaModbusGateway_LogDefault_h__
#define __OpcUaModbusGateway_LogDefault_h__

#include "Base/LogHandler.h"

namespace OpcUaModbusGateway
{

	class LogDefault
	: public Base::LogHandler
	{
	  public:
		using SPtr = std::shared_ptr<LogDefault>;

		LogDefault(const std::string& message);
		virtual ~LogDefault(void);

		void message(const std::string& message);

		virtual void log(Base::LogLevel logLevel, const std::string& message) override;

	  private:
		std::string message_ = "modbus client message";
	};
}

#endif
