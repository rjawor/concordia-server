#include "bool_param.hpp"


BoolParam::BoolParam(bool value):_value(value) {
}

BoolParam::~BoolParam() {
}

const char * BoolParam::getValue() {
    if (_value) {
        return "t";
    } else {
        return "f";
    }
}

const int BoolParam::getLength() {
    return 1;
}

const int BoolParam::isBinary() {
    return 0;
}
