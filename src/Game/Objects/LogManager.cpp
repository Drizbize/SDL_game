#include <string>
#include "LogManager.h"

namespace game
{
	bool LogManager::init()
	{
		return false;
	}

	void LogManager::addDriver(ILogDriver* driver)
	{
		m_drivers.push_back(*driver);
	}
}