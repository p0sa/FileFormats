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

static ErrorOr<void> readConst(std::istream& stream, ClassInfo& info)
{
  TRY(Read<BigEndian>(stream, info.NameIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, FieldrefInfo& info)
{
  TRY(Read<BigEndian>(stream, info.ClassIndex, info.NameAndTypeIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, MethodrefInfo& info)
{
  TRY(Read<BigEndian>(stream, info.ClassIndex, info.NameAndTypeIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, InterfaceMethodrefInfo& info)
{
  TRY(Read<BigEndian>(stream, info.ClassIndex, info.NameAndTypeIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, StringInfo& info)
{
  TRY(Read<BigEndian>(stream, info.StringIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, IntegerInfo& info)
{
  TRY(Read<BigEndian>(stream, info.Bytes));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, FloatInfo& info)
{
  TRY(Read<BigEndian>(stream, info.Bytes));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, LongInfo& info)
{
  TRY(Read<BigEndian>(stream, info.HighBytes, info.LowBytes));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, DoubleInfo& info)
{
  TRY(Read<BigEndian>(stream, info.HighBytes, info.LowBytes));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, NameAndTypeInfo& info)
{
  TRY(Read<BigEndian>(stream, info.NameIndex, info.DescriptorIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, UTF8Info& info)
{
  U16 len;
  TRY(Read<BigEndian>(stream, len));

  info.String = std::string(len, '\0');
  stream.read(&info.String[0], len);

  if (stream.bad())
    return Error::FromFormatStr("failed to read UTF8 const string. Stream badbit set after read. (streampos = 0x%X)", stream.tellg());

  return {};
}

static ErrorOr<void> readConst(std::istream& stream, MethodHandleInfo& info)
{
  TRY(Read<BigEndian>(stream, info.ReferenceKind, info.ReferenceIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, MethodTypeInfo& info)
{
  TRY(Read<BigEndian>(stream, info.DescriptorIndex));
  return {};
}

static ErrorOr<void> readConst(std::istream& stream, InvokeDynamicInfo& info)
{
  TRY(Read<BigEndian>(stream, info.BootstrapMethodAttrIndex, info.NameAndTypeIndex));
  return {};
}

template <typename CPInfoT>
static ErrorOr< std::unique_ptr<CPInfo> > parseConstT(std::istream& stream)
{
  CPInfoT* pInfo = new CPInfoT{};
  auto err = readConst(stream, *pInfo);

  if (err.IsError())
    return err.GetError();

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

  return Error::FromFormatStr("ParseConstant encountered unknown tag: 0x%X (streampos = 0x%X)", static_cast<U8>(type), static_cast<size_t>(stream.tellg()));
}
