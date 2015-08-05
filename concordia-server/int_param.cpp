#include "int_param.hpp"

#include <netinet/in.h>
#include <arpa/inet.h>

IntParam::IntParam(int value) {
    _value = htonl((unsigned long int) value);
}

IntParam::~IntParam() {
}

const char * IntParam::getValue() {
    return (const char *) &_value;
}
    
const int IntParam::getLength() {
    return sizeof(_value);
}

const int IntParam::isBinary() {
    return 1;
}
