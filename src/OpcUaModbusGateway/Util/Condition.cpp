/*
   Copyright 2024-2025 Kai Huebl (kai@huebl-sgh.de)

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

#include "OpcUaModbusGateway/Util/Condition.h"

namespace OpcUaModbusGateway
{

	Condition::Condition(void)
	{
	}

	Condition::~Condition(void)
	{
	}

	void
	Condition::init(void)
	{
		ready_ = false;
	}

	void
	Condition::signal(void)
	{
		{
			std::lock_guard lk(m_);
			ready_ = true;
		}
		cv_.notify_one();
	}

	bool
	Condition::wait(uint32_t timeoutMs)
	{
		std::unique_lock lk(m_);
		if (timeoutMs == 0) {
			cv_.wait(lk, [this]{ return ready_; });
		}
		else {
			while (!ready_) {
				if (cv_.wait_for(lk, std::chrono::milliseconds(timeoutMs)) == std::cv_status::timeout) {
					return false;
				}
			}
		}

        return true;
	}

}
