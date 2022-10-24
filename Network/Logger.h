#pragma once
#include "Singleton.h"

enum LOG_TYPE
{
	INFO,
	LOG_ERROR
};

class Logger : public Singleton<Logger>
{
	friend class Logger;

private: // member var
	Logger();
	~Logger();
	
public: 
	void Log(const std::string& _log, LOG_TYPE _type = LOG_TYPE::INFO);

};

