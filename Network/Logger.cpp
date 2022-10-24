#include "pch.h"
#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Log(const std::string& _log, LOG_TYPE _type)
{
	std::string log;
	switch (_type)
	{
	case INFO:
		log = "LOG INFO - \n";
		break;
	case LOG_ERROR:
		log = "LOG ERROR - \n";
		break;
	}

	log += _log;
	log += "\n";

	std::cout << log;
}


