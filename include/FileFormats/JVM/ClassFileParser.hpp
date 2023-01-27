#pragma once

#include "ClassFile.hpp"

namespace FileFormats::JVM
{

class ClassFileParser
{
  public:
    static ClassFile ParseClassFile(std::istream&);
    static ConstantPool ParseConstantPool(std::istream&);
    static std::unique_ptr<CPInfo> ParseConstant(std::istream&);
};


} //namespace FileFormats::JVM
