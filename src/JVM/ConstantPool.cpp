#include "FileFormats/JVM/ConstantPool.hpp"

#include <map>
#include <cassert>

using namespace FileFormats;
using namespace FileFormats::JVM;

ErrorOr<std::string_view> CPInfo::GetTypeName(Type type)
{
  static std::map<Type, std::string_view> typeNames = 
  {
    {Type::Class,              "Class"},
    {Type::Fieldref,           "Fieldref"},
    {Type::Methodref,          "Methodref"},
    {Type::InterfaceMethodref, "InterfaceMethodref"},
    {Type::String,             "String"},
    {Type::Integer,            "Integer"},
    {Type::Float,              "Float"},
    {Type::Long,               "Long"},
    {Type::Double,             "Double"},
    {Type::NameAndType,        "NameAndType"},
    {Type::UTF8,               "UTF8"},
    {Type::MethodHandle,       "MethodHandle"},
    {Type::MethodType,         "MethodType"},
    {Type::InvokeDynamic,      "InvokeDynamic"},
  };

  auto itr = typeNames.find(type);

  if (itr != typeNames.end())
    return std::get<1>(*itr);

  return Error::FromFormatStr("CPInfo::GetTypeName called with unknown type: 0x%X", type);
}

std::string_view CPInfo::GetName() const
{
  auto nameOrErr = CPInfo::GetTypeName(this->GetType());

  //should never be the case for CPInfo derived types as they should be tied to a type 
  assert(nameOrErr.IsError() == false);

  return nameOrErr.Get();
}

CPInfo::Type CPInfo::GetType() const
{
  return this->m_type;
}

ConstantPool::ConstantPool(U16 n) 
: m_pool{nullptr} //constants use 1 based indexing, so we ignore the 0th index
{
  this->Reserve(n);
}

void ConstantPool::Reserve(U16 n) 
{
  m_pool.reserve(n);
}

std::string_view ConstantPool::GetConstNameOrTypeStr(U16 index)
{
  auto ptr = m_pool[index];

  if(ptr == nullptr)
    return "UNINITIALIZED";

  if(ptr->GetType() == CPInfo::Type::UTF8)
  {
    auto utf8Info = std::static_pointer_cast<UTF8Info>(ptr);
    return utf8Info->String;
  }

  if(ptr->GetType() == CPInfo::Type::String)
  {
    auto stringInfo = std::static_pointer_cast<StringInfo>(ptr);
    return this->GetConstNameOrTypeStr(stringInfo->StringIndex);
  }

  if(ptr->GetType() == CPInfo::Type::Class)
  {
    auto classInfo = std::static_pointer_cast<ClassInfo>(ptr);
    return this->GetConstNameOrTypeStr(classInfo->NameIndex);
  }

  //TODO: implement for more types

  return "";
}

void ConstantPool::Add(std::unique_ptr<CPInfo> info) 
{
  this->Add(info.release());
}

void ConstantPool::Add(CPInfo* info) 
{
  m_pool.emplace_back( std::shared_ptr<CPInfo>{info} ); 
}

U16 ConstantPool::Count()
{
  return static_cast<U16>(m_pool.size());
}
