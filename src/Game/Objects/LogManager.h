#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace game
{
    enum LogLevel
    {
        Error = 1,
        Warn,

    };

	class ILogDriver
	{
	public:
		virtual void log(const std::string& log) = 0;
	};

    class ConsoleLogDriver : public ILogDriver
    {
    public:
        void log(const std::string& log) override
        {
            std::cout << log << "\n";
        }
    };

    class FileLogDriver : public ILogDriver
    {
    public:
        void log(const std::string& log) override
        {
            //std::iofile("...");
        }
    };

	class LogManager
	{
	public:
		static bool init();
        void addDriver(ILogDriver* driver);
	private:
		LogManager() {};

        
		std::vector<ILogDriver> m_drivers;
		//bool m_init = false;
	};
}