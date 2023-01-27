#include "FileFormats/JVM/ConstantPool.hpp"

#include <map>

using namespace FileFormats;
using namespace FileFormats::JVM;

const char* CPInfo::GetTypeName(Type type)
{
  std::map<Type, const char*> typeNames = 
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

  if(itr != typeNames.end())
    return std::get<1>(*itr);

  //TODO: better error handling
  return "UNKNOWN_TAG";
}

const char* CPInfo::GetName() const
{
  return CPInfo::GetTypeName(this->GetType());
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
  return m_pool.size();
}
