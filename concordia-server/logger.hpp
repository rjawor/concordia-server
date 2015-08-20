#ifndef LOGGER_HDR
#define LOGGER_HDR

#include <string>
#include <sstream>

#include "log4cpp/Category.hh"


class Logger {
public:
    /*! Constructor.
    */
    Logger();
    /*! Destructor.
    */
    virtual ~Logger();
    
    static void log(std::string message);

    static void logInt(std::string name, int value);

    static void logString(std::string name, std::string value);
private:
    static void _initialize(log4cpp::Category & root);

    static int _initialized;
};

#endif
