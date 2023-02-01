#pragma once

#include "../Defs.hpp"
#include "../Error.hpp"

#include <string_view>
#include <vector>

namespace FileFormats::JVM
{


struct AttributeInfo
{
  enum class Type
  {
    ConstantValue,

    Raw, //Non standard 
  };

  static ErrorOr<Type> GetType(std::string_view);
  static ErrorOr<std::string_view> GetTypeName(Type);

  std::string_view GetName();
  Type GetType();

  U16 NameIndex;
  virtual U32 GetLength() const = 0;

  protected:
    AttributeInfo(Type type) : m_type{type}{}

  private:
    Type m_type;
};


struct ConstantValueAttribute : public AttributeInfo
{
  ConstantValueAttribute() : AttributeInfo(Type::ConstantValue) {}
  U32 GetLength() const override { return 2;  }

  U16 Index;
};


//Non standard attribute type, used for parsing unknown or unimplemented attributes as a byte array
struct RawAttribute : public AttributeInfo
{
  RawAttribute() : AttributeInfo(Type::Raw) {}
  U32 GetLength() const override { return static_cast<U32>(Bytes.size());  }

  std::vector<U8> Bytes;
};


} //namespace FileFormats::JVM