#include "string_param.hpp"

StringParam::StringParam(std::string value) : _value(value) {
}

StringParam::~StringParam() {
}

const char * StringParam::getValue() {
    return _value.c_str();
}
    
const int StringParam::getLength() {
    return _value.size();
}

const int StringParam::isBinary() {
    return 0;
}
