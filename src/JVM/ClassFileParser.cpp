#include "FileFormats/JVM/ClassFileParser.hpp"

#include "Util/IO.hpp"
#include "Util/Error.hpp"

#include <iostream>
#include <cassert>

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
  VERIFY(errOrCP);

  cf.ConstPool = errOrCP.Release();

  U16 interfacesCount;
  TRY(Read<BigEndian>(stream,
                      cf.AccessFlags,
                      cf.ThisClass,
                      cf.SuperClass,
                      interfacesCount));

  cf.Interfaces.reserve(interfacesCount);
  for (auto i = 0; i < interfacesCount; i++)
  {
    U16 interfaceIndex;
    TRY( Read<BigEndian>(stream, interfaceIndex));
    cf.Interfaces.emplace_back(interfaceIndex);
  }

  U16 fieldsCount;
  TRY(Read<BigEndian>(stream, fieldsCount));

  cf.Fields.reserve(fieldsCount);
  for (auto i = 0; i < fieldsCount; i++)
  {
    auto errOrField = ClassFileParser::ParseFieldMethodInfo(stream, cf.ConstPool);
    VERIFY(errOrField);

    cf.Fields.emplace_back(errOrField.Release());
  }


  U16 methodsCount;
  TRY(Read<BigEndian>(stream, methodsCount));

  cf.Methods.reserve(methodsCount);
  for (auto i = 0; i < methodsCount; i++)
  {
    auto errOrMethod = ClassFileParser::ParseFieldMethodInfo(stream, cf.ConstPool);
    VERIFY(errOrMethod);

    cf.Methods.emplace_back(errOrMethod.Release());
  }

  U16 attributesCount;
  TRY(Read<BigEndian>(stream, attributesCount));

  cf.Attributes.reserve(attributesCount);
  for (auto i = 0; i < attributesCount; i++)
  {
    auto errOrAttr = ClassFileParser::ParseAttribute(stream, cf.ConstPool);
    VERIFY(errOrAttr);

    cf.Attributes.emplace_back(errOrAttr.Release());
  }

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
    VERIFY(errOrCPInfo);

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
  auto errOrConst = readConst(stream, *pInfo);
  VERIFY(errOrConst);

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

  return Error::FromFormatStr("ParseConstant encountered unknown tag: 0x%X (streampos = 0x%X)", static_cast<U8>(type), stream.tellg());
}

ErrorOr<FieldMethodInfo> ClassFileParser::ParseFieldMethodInfo(
    std::istream& stream, const ConstantPool& constPool)
{
  FieldMethodInfo info;

  U16 attributesCount;
  TRY(Read<BigEndian>(stream, info.AccessFlags,
                              info.NameIndex,
                              info.DescriptorIndex,
                              attributesCount));

  info.Attributes.reserve(attributesCount);
  for (auto i = 0; i < attributesCount; i++)
  {
    auto errOrAttr = ClassFileParser::ParseAttribute(stream, constPool);
    VERIFY(errOrAttr);

    info.Attributes.emplace_back(errOrAttr.Release());
  }

  return info;
}


static ErrorOr<void> readAttribute(std::istream& stream, 
    const ConstantPool& constPool, ConstantValueAttribute& attr)
{
  TRY(Read<BigEndian>(stream, attr.Index));
  return {};
}

static ErrorOr<void> readAttribute(std::istream& stream, 
    const ConstantPool& constPool, SourceFileAttribute& attr)
{
  TRY(Read<BigEndian>(stream, attr.SourceFileIndex));
  return {};
}

static ErrorOr<void> readAttribute(std::istream& stream, 
    const ConstantPool& constPool, CodeAttribute& attr)
{
  U32 codeLen;
  TRY(Read<BigEndian>(stream, 
                      attr.MaxStack,
                      attr.MaxLocals,
                      codeLen));


  U32 parsedCodeLen{0};
  while(parsedCodeLen < codeLen)
  {
    auto streampos_before = stream.tellg();

    auto errOrInstr = ClassFileParser::ParseInstruction(stream);
    VERIFY(errOrInstr);

    attr.Code.emplace_back( std::move(errOrInstr.Release()) );

    auto parsed = stream.tellg() - streampos_before;
    assert(parsed > 0);

    parsedCodeLen += parsed;
  }

  if(parsedCodeLen != codeLen)
  {
    return Error::FromFormatStr("Failed parsing code attribute. \"CodeLen\" field parsed as %u but actual parsed codelen is at least %u bytes", static_cast<size_t>(codeLen), static_cast<size_t>(parsedCodeLen));
  }

  //TODO: create a read util function for these sorts of 
  //      "read length, then read array of that length" scenarios
  U16 exceptionTableLen;
  TRY(Read<BigEndian>(stream, exceptionTableLen));

  attr.ExceptionTable.reserve(exceptionTableLen);
  for(auto i = 0; i < exceptionTableLen; i++)
  {
    CodeAttribute::ExceptionHandler handler;
    TRY(Read<BigEndian>(stream, handler.StartPC, 
                                handler.EndPC, 
                                handler.HandlerPC, 
                                handler.CatchType));

    attr.ExceptionTable.emplace_back(handler);
  }

  U16 attributesCount;
  TRY(Read<BigEndian>(stream, attributesCount));

  attr.Attributes.reserve(attributesCount);
  for(auto i = 0; i < attributesCount; i++)
  {
    auto errOrAttr = ClassFileParser::ParseAttribute(stream, constPool);
    VERIFY(errOrAttr);

    attr.Attributes.emplace_back( errOrAttr.Release() );
  }

  return {};
}

template <typename AttributeT>
static ErrorOr< std::unique_ptr<AttributeInfo> > parseAttributeT(
    std::istream& stream, const ConstantPool& constPool, U16 nameIndex, U32 len)
{
  AttributeT* attr = new AttributeT();
  attr->NameIndex = nameIndex;

  auto err = readAttribute(stream, constPool, *attr);
  VERIFY(err);

  U32 attrLen = attr->GetLength();
  if(attrLen != len)
  {
    return Error::FromFormatStr("Parsed attribute type \"%.*s\"" 
        " doesnt have the correct length (expected: %u, actual: %u)", 
        static_cast<int>(attr->GetName().size()), attr->GetName().data(), 
        len, attrLen);
  }

  return std::unique_ptr<AttributeInfo>(attr);
}

ErrorOr< std::unique_ptr<AttributeInfo> > ClassFileParser::ParseAttribute(
    std::istream& stream, const ConstantPool& constPool)
{
  U16 nameIndex;
  U32 len;
  TRY(Read<BigEndian>(stream, nameIndex, len));

  auto nameStr = constPool.GetConstNameOrTypeStr(nameIndex);

  auto errOrType = AttributeInfo::GetType(nameStr);

  AttributeInfo::Type type;
  if (errOrType.IsError())
  {
    std::cerr << errOrType.GetError().GetMessage() << '\n';
    type = AttributeInfo::Type::Raw;
  }
  else
    type = errOrType.Get();

  switch (type)
  {
    case AttributeInfo::Type::ConstantValue: 
      return parseAttributeT<ConstantValueAttribute>(stream, constPool, nameIndex, len);
    case AttributeInfo::Type::SourceFile: 
      return parseAttributeT<SourceFileAttribute>(stream, constPool, nameIndex, len);
    case AttributeInfo::Type::Code: 
      return parseAttributeT<CodeAttribute>(stream, constPool, nameIndex, len);
  }

  RawAttribute* attr = new RawAttribute{};
  attr->NameIndex = nameIndex;

  //TODO: add ReadArray<> to util
  attr->Bytes.reserve(len);
  for (unsigned i = 0; i < len; i++)
  {
    U8 byte;
    TRY(Read(stream, byte));

    attr->Bytes.emplace_back(byte);
  }

  return std::unique_ptr<AttributeInfo>(attr);
}


/*
static ErrorOr<Instruction> readInstrWithOperands(std::istream& stream, size_t nOperands, U8 opCode)
{
  Instruction instr{opCode};

  for(size_t i = 0; i < nOperands; i++)
  {
    U8 operandByte;
    TRY(Read<BigEndian>(stream, operandByte));

    instr.OperandBytes.emplace_back(operandByte);
  }

  return std::move(instr);
}
*/

ErrorOr<Instruction> ClassFileParser::ParseInstruction(std::istream& stream)
{
  return Error::FromLiteralStr("PARSE ISNTRUCTION NOT IMPL");
  /*
  U8 opCode;
  TRY(Read<BigEndian>(stream, opCode));

  //undefined opcodes
  if(opCode > 0xCA && opCode < 0xFE)
    return Error::FromFormatStr("failed parsing unknown opcode: %#04x", opCode);


  switch(opCode)
  {
    case LDC:
    case ILOAD:
    case LLOAD:
    case FLOAD:
    case DLOAD:
    case ALOAD:
    case ISTORE:
    case LSTORE:
    case FSTORE:
    case DSTORE:
    case ASTORE:
    case RET:
    case BIPUSH:
    case NEWARRAY:
        return std::move( readInstrWithOperands(stream, 1, opCode) );

    case LDC_W:
    case LDC2_W:
    case GETSTATIC:
    case PUTSTATIC:
    case GETFIELD:
    case PUTFIELD:
    case INVOKEVIRTUAL:
    case INVOKESPECIAL:
    case INVOKESTATIC:
    case NEW:
    case ANEWARRAY:
    case CHECKCAST:
    case INSTANCEOF:
    case IINC:
    case SIPUSH:
    case IFEQ:
    case IFNE:
    case IFLT:
    case IFGE:
    case IFGT:
    case IFLE:
    case IF_ICMPEQ:
    case IF_ICMPNE:
    case IF_ICMPLT:
    case IF_ICMPGE:
    case IF_ICMPGT:
    case IF_ICMPLE:
    case IF_ACMPEQ:
    case IF_ACMPNE:
    case GOTO:
    case JSR:
    case IFNULL:
    case IFNONNULL:
        return std::move( readInstrWithOperands(stream, 2, opCode) );

    case MULTIANEWARRAY:
        return std::move( readInstrWithOperands(stream, 3, opCode) );

    case INVOKEINTERFACE:
    case INVOKEDYNAMIC:
    case GOTO_W:
    case JSR_W:
        return std::move( readInstrWithOperands(stream, 4, opCode) );

    case WIDE: 
    case LOOKUPSWITCH: 
    case TABLESWITCH: 
        return Error::FromLiteralStr("ENCOUNTERED UNIMPLEMENTED OPCODE OPERAND FORMAT");

  };

  return Instruction{opCode};
  */
}
