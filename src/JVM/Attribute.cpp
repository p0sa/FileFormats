#include "FileFormats/JVM/Attribute.hpp"

#include <map>
#include <cassert>

using namespace FileFormats;
using namespace JVM;

static std::map<AttributeInfo::Type, std::string_view> typeNames =
{
  {AttributeInfo::Type::ConstantValue, "ConstantValue"},
  {AttributeInfo::Type::SourceFile,    "SourceFile"},

  {AttributeInfo::Type::Raw, "_Raw"}
};

std::string_view AttributeInfo::GetTypeName(AttributeInfo::Type type)
{
  auto itr = typeNames.find(type);

  assert(itr != typeNames.end());
  return std::get<1>(*itr);
}

ErrorOr<AttributeInfo::Type> AttributeInfo::GetType(std::string_view str)
{
  for (auto itr : typeNames)
  {
    if (str == std::get<1>(itr))
      return std::get<0>(itr);
  }

  return Error::FromFormatStr("AttributeInfo::GetType called with unknown type name \"%s\"", str.data());
}

std::string_view AttributeInfo::GetName()
{
  return AttributeInfo::GetTypeName(this->GetType());
}

AttributeInfo::Type AttributeInfo::GetType()
{
  return m_type;
}

