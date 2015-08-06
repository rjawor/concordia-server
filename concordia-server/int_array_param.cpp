#include "int_array_param.hpp"

#include <boost/foreach.hpp>
#include <sstream>

IntArrayParam::IntArrayParam(std::vector<int> array) {
    std::stringstream ss;
    ss << "{";
    int index = 0;
    BOOST_FOREACH(int & number, array) {
        ss << number;
        if (index < array.size() - 1) {
            ss << ",";
        }
        index++;
    }
    ss << "}";
    _arrayString = ss.str();    
}

IntArrayParam::~IntArrayParam() {
}

const char * IntArrayParam::getValue() {
    return _arrayString.c_str();
}
    
const int IntArrayParam::getLength() {
    return _arrayString.size();
}

const int IntArrayParam::isBinary() {
    return 0;
}
