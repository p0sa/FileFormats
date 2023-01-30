#include "FileFormats/JVM/ClassFileParser.hpp"

#include "Util/IO.hpp"
#include "Util/Error.hpp"

#include <iostream>

using namespace FileFormats;
using namespace JVM;

ErrorOr<ClassFile> ClassFileParser::ParseClassFile(std::istream& stream)
{
  ClassFile cf;

  TRY(Read<BigEndian>(stream,
                      cf.Magic,
                      cf.MinorVersion,
                      cf.MajorVersion));

  auto errOrCP = ClassFileParser::ParseConstantPool(stream);

  if ( errOrCP.IsError() )
    return errOrCP.GetError();

  cf.ConstPool = errOrCP.Get();

  TRY(Read<BigEndian>(stream,
                      cf.AccessFlags,
                      cf.ThisClass,
                      cf.SuperClass));


  return cf;
}

ErrorOr<ConstantPool> ClassFileParser::ParseConstantPool(std::istream& stream)
{
  ConstantPool cp;

  U16 count;
  TRY(Read<BigEndian>(stream, count));

  cp.Reserve(count);

  //count = number of constants + 1
  for(U16 i = 0; i < count-1; i++)
  {
    auto errOrCPInfo = ClassFileParser::ParseConstant(stream);

    if (errOrCPInfo.IsError())
      return errOrCPInfo.GetError();

    auto cpInfo = errOrCPInfo.Release();

    CPInfo::Type type = cpInfo->GetType();

    cp.Add( std::move(cpInfo) ); 

    //Long & Double constants require the next index into the constant pool
    //after them be invalid.
    if(type == CPInfo::Type::Long || type == CPInfo::Type::Double)
    {
      cp.Add(nullptr);
      i++;
    }

  }

  return cp;
}

std::istream& operator >>(std::istream& stream, ClassInfo& info)
{
  Read<BigEndian>(stream, info.NameIndex);

  return stream;
}

std::istream& operator >>(std::istream& stream, FieldrefInfo& info)
{
  Read<BigEndian>(stream, info.ClassIndex, info.NameAndTypeIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, MethodrefInfo& info)
{
  Read<BigEndian>(stream, info.ClassIndex, info.NameAndTypeIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, InterfaceMethodrefInfo& info)
{
  Read<BigEndian>(stream, info.ClassIndex, info.NameAndTypeIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, StringInfo& info)
{
  Read<BigEndian>(stream, info.StringIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, IntegerInfo& info)
{
  Read<BigEndian>(stream, info.Bytes);
  return stream;
}

std::istream& operator >>(std::istream& stream, FloatInfo& info)
{
  Read<BigEndian>(stream, info.Bytes);
  return stream;
}

std::istream& operator >>(std::istream& stream, LongInfo& info)
{
  Read<BigEndian>(stream, info.HighBytes, info.LowBytes);
  return stream;
}

std::istream& operator >>(std::istream& stream, DoubleInfo& info)
{
  Read<BigEndian>(stream, info.HighBytes, info.LowBytes);
  return stream;
}

std::istream& operator >>(std::istream& stream, NameAndTypeInfo& info)
{
  Read<BigEndian>(stream, info.NameIndex, info.DescriptorIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, UTF8Info& info)
{
  U16 len;
  Read<BigEndian>(stream, len);

  info.String = std::string(len, '\0');
  stream.read(&info.String[0], len);

  return stream;
}

std::istream& operator >>(std::istream& stream, MethodHandleInfo& info)
{
  Read<BigEndian>(stream, info.ReferenceKind, info.ReferenceIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, MethodTypeInfo& info)
{
  Read<BigEndian>(stream, info.DescriptorIndex);
  return stream;
}

std::istream& operator >>(std::istream& stream, InvokeDynamicInfo& info)
{
  Read<BigEndian>(stream, info.BootstrapMethodAttrIndex, info.NameAndTypeIndex);
  return stream;
}

template <typename CPInfoT>
static ErrorOr< std::unique_ptr<CPInfo> > parseConstT(std::istream& stream)
{
  CPInfoT* pInfo = new CPInfoT{};
  stream >> *pInfo;

  //TODO: rewrite stream operators to regular funcitons so we can return the actual Error caused during parsing of T
  if (stream.bad())
    return Error{"Failed to parse T"};

  return std::unique_ptr<CPInfo>(pInfo);
}

ErrorOr< std::unique_ptr<CPInfo> > ClassFileParser::ParseConstant(std::istream& stream)
{
  CPInfo::Type type = static_cast<CPInfo::Type>(stream.get());

  switch(type)
  {
    case CPInfo::Type::Class:       return parseConstT<ClassInfo>(stream);
    case CPInfo::Type::Fieldref:    return parseConstT<FieldrefInfo>(stream);
    case CPInfo::Type::Methodref:   return parseConstT<MethodrefInfo>(stream);
    case CPInfo::Type::InterfaceMethodref: return parseConstT<InterfaceMethodrefInfo>(stream);
    case CPInfo::Type::String:      return parseConstT<StringInfo>(stream);
    case CPInfo::Type::Integer:     return parseConstT<IntegerInfo>(stream);
    case CPInfo::Type::Float:       return parseConstT<FloatInfo>(stream);
    case CPInfo::Type::Long:        return parseConstT<LongInfo>(stream);
    case CPInfo::Type::Double:      return parseConstT<DoubleInfo>(stream);
    case CPInfo::Type::NameAndType: return parseConstT<NameAndTypeInfo>(stream);
    case CPInfo::Type::UTF8:        return parseConstT<UTF8Info>(stream);
    case CPInfo::Type::MethodHandle:  return parseConstT<MethodHandleInfo>(stream);
    case CPInfo::Type::MethodType:    return parseConstT<MethodTypeInfo>(stream);
    case CPInfo::Type::InvokeDynamic: return parseConstT<InvokeDynamicInfo>(stream);
  }

  return Error::FromFormatStr("ParseConstant encountered unknown tag: 0x%X (streampos: %u)", static_cast<U8>(type), static_cast<size_t>(stream.tellg()));
}
