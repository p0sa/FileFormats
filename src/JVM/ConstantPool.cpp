#include "FileFormats/JVM/ConstantPool.hpp"

#include <map>
#include <cassert>

using namespace FileFormats;
using namespace FileFormats::JVM;

std::string_view CPInfo::GetTypeName(Type type)
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

  assert(itr != typeNames.end());
  return std::get<1>(*itr);
}

std::string_view CPInfo::GetName() const
{
  return CPInfo::GetTypeName(this->GetType());
}

CPInfo::Type CPInfo::GetType() const
{
  return this->m_type;
}

ConstantPool::ConstantPool(U16 n) 
{
  //constants use 1 based indexing, so we ignore the 0th index
  m_pool.emplace_back( std::unique_ptr<CPInfo>{nullptr} );
}

void ConstantPool::Reserve(U16 n) 
{
  m_pool.reserve(n);
}

std::string_view ConstantPool::GetConstNameOrTypeStr(U16 index) const
{
  auto ptr = m_pool[index].get();

  if(ptr == nullptr)
    return "UNINITIALIZED";

  if(ptr->GetType() == CPInfo::Type::UTF8)
  {
    auto utf8Info = static_cast<UTF8Info*>(ptr);
    return utf8Info->String;
  }

  if(ptr->GetType() == CPInfo::Type::String)
  {
    auto stringInfo = static_cast<StringInfo*>(ptr);
    return this->GetConstNameOrTypeStr(stringInfo->StringIndex);
  }

  if(ptr->GetType() == CPInfo::Type::Class)
  {
    auto classInfo = static_cast<ClassInfo*>(ptr);
    return this->GetConstNameOrTypeStr(classInfo->NameIndex);
  }

  //TODO: implement for more types

  return "";
}

void ConstantPool::Add(std::unique_ptr<CPInfo>&& info) 
{
  m_pool.emplace_back(std::move(info));
}

void ConstantPool::Add(CPInfo* info) 
{
  m_pool.emplace_back( std::unique_ptr<CPInfo>{info} ); 
}

U16 ConstantPool::Count() const
{
  return static_cast<U16>(m_pool.size());
}
