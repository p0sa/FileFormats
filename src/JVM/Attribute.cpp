#include "FileFormats/JVM/Attribute.hpp"

#include <map>
#include <cassert>

using namespace FileFormats;
using namespace JVM;

static std::map<AttributeInfo::Type, std::string_view> typeNames =
{
  {AttributeInfo::Type::ConstantValue, "ConstantValue"},

  {AttributeInfo::Type::Raw, "_Raw"}
};

ErrorOr<std::string_view> AttributeInfo::GetTypeName(AttributeInfo::Type type)
{
  auto itr = typeNames.find(type);

  if (itr != typeNames.end())
    return std::get<1>(*itr);

  return Error::FromFormatStr("AttributeInfo::GetTypeName called with unknown type: 0x%X", type);
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
  auto errOrName = AttributeInfo::GetTypeName(this->GetType());

  //should never error for derived types of AttributeInfo as they should be tied to a known type always
  assert(errOrName.IsError() == false);

  return errOrName.Get();
}

AttributeInfo::Type AttributeInfo::GetType()
{
  return m_type;
}

