#pragma once

#include "ClassFile.hpp"
#include "../Error.hpp"

namespace FileFormats::JVM
{

class ClassFileWriter
{
  public:
    static ErrorOr<void> WriteClassFile(std::ostream&, const ClassFile&);
    static ErrorOr<void> WriteConstantPool(std::ostream&, const ConstantPool&);
    static ErrorOr<void> WriteConstant(std::ostream&, const CPInfo&);

    static ErrorOr<void> WriteFieldMethod(std::ostream&, const FieldMethodInfo&);
    static ErrorOr<void> WriteAttribute(std::ostream&, const AttributeInfo&);
};

} //namespace FileFormats::JVM

