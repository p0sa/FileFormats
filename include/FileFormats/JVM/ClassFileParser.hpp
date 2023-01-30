#pragma once

#include "ClassFile.hpp"
#include "../Error.hpp"

namespace FileFormats::JVM
{

class ClassFileParser
{
  public:
    static ErrorOr<ClassFile> ParseClassFile(std::istream&);
    static ErrorOr<ConstantPool> ParseConstantPool(std::istream&);
    static ErrorOr< std::unique_ptr<CPInfo> > ParseConstant(std::istream&);
};


} //namespace FileFormats::JVM
