#include "language.hpp"

Language::Language(
   const int id,
   const std::string & code,
   const std::string & name) :
                       _id(id),
                       _code(code),
                       _name(name) {
}

Language::~Language() {
}
