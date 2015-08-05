#include "logger.hpp"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

Logger::Logger() {
}

Logger::~Logger() {
}

int Logger::initialized = 0;

void Logger::log(std::string message) {
    log4cpp::Category & root = log4cpp::Category::getRoot();
    if (initialized == 0) {
        log4cpp::Appender *appender = new log4cpp::FileAppender("default", "/tmp/concordia-server.log");
	    appender->setLayout(new log4cpp::BasicLayout());

	    root.setPriority(log4cpp::Priority::INFO);
	    root.addAppender(appender);
	    
	    initialized = 1;
	}
	root.info(message);
}


