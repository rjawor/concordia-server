#ifndef LOGGER_HDR
#define LOGGER_HDR

#include <string>
#include <sstream>

class Logger {
public:
    /*! Constructor.
    */
    Logger();
    /*! Destructor.
    */
    virtual ~Logger();
    
    static void log(std::string message);
private:
    static int initialized;
};

#endif
