#pragma once

#include "../Defs.hpp"
#include "ConstantPool.hpp"

namespace FileFormats::JVM
{

struct FieldMethodInfo
{
  U16 AccessFlags;
  U16 NameIndex;
  U16 DescriptorIndex;
  //TODO: implement attribute
  //std::vector<Attribute::Info*> Attributes;
};

struct ClassFile 
{
  U32 Magic;
  U16 MinorVersion;
  U16 MajorVersion;
  ConstantPool ConstPool;
  U16 AccessFlags;
  U16 ThisClass;
  U16 SuperClass;
  std::vector<U16> Interfaces;
  std::vector<FieldMethodInfo> Fields;
  std::vector<FieldMethodInfo> Methods;
  //TODO: implement attribute
  //std::vector< Attribute::Info* > Attributes;
};


} //namespace FileFormats::JVM
