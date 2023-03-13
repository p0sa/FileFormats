#pragma once

#include "../Defs.hpp"
#include "../Error.hpp"

#include <string_view>
#include <vector>

namespace FileFormats::JVM
{

//TODO: enum class
enum OpCode : U8
{
  OP_NOP            = 0X00,
  OP_ACONST_NULL    = 0X01,
  OP_ICONST_M1      = 0X02,
  OP_ICONST_0       = 0X03,
  OP_ICONST_1       = 0X04,
  OP_ICONST_2       = 0X05,
  OP_ICONST_3       = 0X06,
  OP_ICONST_4       = 0X07,
  OP_ICONST_5       = 0X08,
  OP_LCONST_0       = 0X09,
  OP_LCONST_1       = 0X0A,
  OP_FCONST_0       = 0X0B,
  OP_FCONST_1       = 0X0C,
  OP_FCONST_2       = 0X0D,
  OP_DCONST_0       = 0X0E,
  OP_DCONST_1       = 0X0F,
  OP_BIPUSH         = 0X10,
  OP_SIPUSH         = 0X11,
  OP_LDC            = 0X12,
  OP_LDC_W          = 0X13,
  OP_LDC2_W         = 0X14,
  OP_ILOAD          = 0X15,
  OP_LLOAD          = 0X16,
  OP_FLOAD          = 0X17,
  OP_DLOAD          = 0X18,
  OP_ALOAD          = 0X19,
  OP_ILOAD_0        = 0X1A,
  OP_ILOAD_1        = 0X1B,
  OP_ILOAD_2        = 0X1C,
  OP_ILOAD_3        = 0X1D,
  OP_LLOAD_0        = 0X1E,
  OP_LLOAD_1        = 0X1F,
  OP_LLOAD_2        = 0X20,
  OP_LLOAD_3        = 0X21,
  OP_FLOAD_0        = 0X22,
  OP_FLOAD_1        = 0X23,
  OP_FLOAD_2        = 0X24,
  OP_FLOAD_3        = 0X25,
  OP_DLOAD_0        = 0X26,
  OP_DLOAD_1        = 0X27,
  OP_DLOAD_2        = 0X28,
  OP_DLOAD_3        = 0X29,
  OP_ALOAD_0        = 0X2A,
  OP_ALOAD_1        = 0X2B,
  OP_ALOAD_2        = 0X2C,
  OP_ALOAD_3        = 0X2D,
  OP_IALOAD         = 0X2E,
  OP_LALOAD         = 0X2F,
  OP_FALOAD         = 0X30,
  OP_DALOAD         = 0X31,
  OP_AALOAD         = 0X32,
  OP_BALOAD         = 0X33,
  OP_CALOAD         = 0X34,
  OP_SALOAD         = 0X35,
  OP_ISTORE         = 0X36,
  OP_LSTORE         = 0X37,
  OP_FSTORE         = 0X38,
  OP_DSTORE         = 0X39,
  OP_ASTORE         = 0X3A,
  OP_ISTORE_0       = 0X3B,
  OP_ISTORE_1       = 0X3C,
  OP_ISTORE_2       = 0X3D,
  OP_ISTORE_3       = 0X3E,
  OP_LSTORE_0       = 0X3F,
  OP_LSTORE_1       = 0X40,
  OP_LSTORE_2       = 0X41,
  OP_LSTORE_3       = 0X42,
  OP_FSTORE_0       = 0X43,
  OP_FSTORE_1       = 0X44,
  OP_FSTORE_2       = 0X45,
  OP_FSTORE_3       = 0X46,
  OP_DSTORE_0       = 0X47,
  OP_DSTORE_1       = 0X48,
  OP_DSTORE_2       = 0X49,
  OP_DSTORE_3       = 0X4A,
  OP_ASTORE_0       = 0X4B,
  OP_ASTORE_1       = 0X4C,
  OP_ASTORE_2       = 0X4D,
  OP_ASTORE_3       = 0X4E,
  OP_IASTORE        = 0X4F,
  OP_LASTORE        = 0X50,
  OP_FASTORE        = 0X51,
  OP_DASTORE        = 0X52,
  OP_AASTORE        = 0X53,
  OP_BASTORE        = 0X54,
  OP_CASTORE        = 0X55,
  OP_SASTORE        = 0X56,
  OP_POP            = 0X57,
  OP_POP2           = 0X58,
  OP_DUP            = 0X59,
  OP_DUP_X1         = 0X5A,
  OP_DUP_X2         = 0X5B,
  OP_DUP2           = 0X5C,
  OP_DUP2_X1        = 0X5D,
  OP_DUP2_X2        = 0X5E,
  OP_SWAP           = 0X5F,
  OP_IADD           = 0X60,
  OP_LADD           = 0X61,
  OP_FADD           = 0X62,
  OP_DADD           = 0X63,
  OP_ISUB           = 0X64,
  OP_LSUB           = 0X65,
  OP_FSUB           = 0X66,
  OP_DSUB           = 0X67,
  OP_IMUL           = 0X68,
  OP_LMUL           = 0X69,
  OP_FMUL           = 0X6A,
  OP_DMUL           = 0X6B,
  OP_IDIV           = 0X6C,
  OP_LDIV           = 0X6D,
  OP_FDIV           = 0X6E,
  OP_DDIV           = 0X6F,
  OP_IREM           = 0X70,
  OP_LREM           = 0X71,
  OP_FREM           = 0X72,
  OP_DREM           = 0X73,
  OP_INEG           = 0X74,
  OP_LNEG           = 0X75,
  OP_FNEG           = 0X76,
  OP_DNEG           = 0X77,
  OP_ISHL           = 0X78,
  OP_LSHL           = 0X79,
  OP_ISHR           = 0X7A,
  OP_LSHR           = 0X7B,
  OP_IUSHR          = 0X7C,
  OP_LUSHR          = 0X7D,
  OP_IAND           = 0X7E,
  OP_LAND           = 0X7F,
  OP_IOR            = 0X80,
  OP_LOR            = 0X81,
  OP_IXOR           = 0X82,
  OP_LXOR           = 0X83,
  OP_IINC           = 0X84,
  OP_I2L            = 0X85,
  OP_I2F            = 0X86,
  OP_I2D            = 0X87,
  OP_L2I            = 0X88,
  OP_L2F            = 0X89,
  OP_L2D            = 0X8A,
  OP_F2I            = 0X8B,
  OP_F2L            = 0X8C,
  OP_F2D            = 0X8D,
  OP_D2I            = 0X8E,
  OP_D2L            = 0X8F,
  OP_D2F            = 0X90,
  OP_I2B            = 0X91,
  OP_I2C            = 0X92,
  OP_I2S            = 0X93,
  OP_LCMP           = 0X94,
  OP_FCMPL          = 0X95,
  OP_FCMPG          = 0X96,
  OP_DCMPL          = 0X97,
  OP_DCMPG          = 0X98,
  OP_IFEQ           = 0X99,
  OP_IFNE           = 0X9A,
  OP_IFLT           = 0X9B,
  OP_IFGE           = 0X9C,
  OP_IFGT           = 0X9D,
  OP_IFLE           = 0X9E,
  OP_IF_ICMPEQ      = 0X9F,
  OP_IF_ICMPNE      = 0XA0,
  OP_IF_ICMPLT      = 0XA1,
  OP_IF_ICMPGE      = 0XA2,
  OP_IF_ICMPGT      = 0XA3,
  OP_IF_ICMPLE      = 0XA4,
  OP_IF_ACMPEQ      = 0XA5,
  OP_IF_ACMPNE      = 0XA6,
  OP_GOTO           = 0XA7,
  OP_JSR            = 0XA8,
  OP_RET            = 0XA9,
  OP_TABLESWITCH    = 0XAA,
  OP_LOOKUPSWITCH   = 0XAB,
  OP_IRETURN        = 0XAC,
  OP_LRETURN        = 0XAD,
  OP_FRETURN        = 0XAE,
  OP_DRETURN        = 0XAF,
  OP_ARETURN        = 0XB0,
  OP_RETURN         = 0XB1,
  OP_GETSTATIC      = 0XB2,
  OP_PUTSTATIC      = 0XB3,
  OP_GETFIELD       = 0XB4,
  OP_PUTFIELD       = 0XB5,
  OP_INVOKEVIRTUAL  = 0XB6,
  OP_INVOKESPECIAL  = 0XB7,
  OP_INVOKESTATIC   = 0XB8,
  OP_INVOKEINTERFACE= 0XB9,
  OP_INVOKEDYNAMIC  = 0XBA,
  OP_NEW            = 0XBB,
  OP_NEWARRAY       = 0XBC,
  OP_ANEWARRAY      = 0XBD,
  OP_ARRAYLENGTH    = 0XBE,
  OP_ATHROW         = 0XBF,
  OP_CHECKCAST      = 0XC0,
  OP_INSTANCEOF     = 0XC1,
  OP_MONITORENTER   = 0XC2,
  OP_MONITOREXIT    = 0XC3,
  OP_WIDE           = 0XC4,
  OP_MULTIANEWARRAY = 0XC5,
  OP_IFNULL         = 0XC6,
  OP_IFNONNULL      = 0XC7,
  OP_GOTO_W         = 0XC8,
  OP_JSR_W          = 0XC9,
  OP_BREAKPOINT     = 0XCA,
  OP_IMPDEP1        = 0XFE,
  OP_IMPDEP2        = 0XFF,
};

ErrorOr<std::string_view> GetOpCodeMnemonic(U8);

struct Instruction
{
  U8 OpCode;

  //TODO: get rid of ErrorOr once we replace U8 with OpCode enum
  ErrorOr<std::string_view> GetMnemonic() const;

  //returns total instruction size in bytes (opcode + operand bytes)
  virtual size_t GetLength() const;

  Instruction(U8 opCode) : OpCode{opCode} {}
  virtual ~Instruction() = default;

};

template <U8 OPCODE>
struct NoArgInstruction : public Instruction
{
  NoArgInstruction() : Instruction{OPCODE} {}
};

template <U8 OPCODE, typename FirstT>
struct OneArgInstruction : public Instruction
{
  FirstT FirstArg;

  OneArgInstruction(FirstT first) 
    : Instruction{OPCODE}, FirstArg{first} {}

  virtual size_t GetLength() const override
  {
    return Instruction::GetLength() + sizeof(FirstArg);
  }

};

template <U8 OPCODE, typename FirstT, typename SecondT>
struct TwoArgInstruction : public Instruction
{
  FirstT  FirstArg;
  SecondT SecondArg;

  TwoArgInstruction(FirstT first, SecondT second) 
    : Instruction{OPCODE}, FirstArg{first}, SecondArg{second} {}

  virtual size_t GetLength() const override
  {
    return Instruction::GetLength() + sizeof(FirstArg) + sizeof(SecondArg);
  }
};

//TODO: defined here because an instruction needed it. Should probably be moved
//to a more appropriate place at some point, but currently none exist.
enum class AType : U8
{
  T_BOOLEAN = 4,
  T_CHAR    = 5,
  T_FLOAT   = 6,
  T_DOUBLE  = 7,
  T_BYTE    = 8,
  T_SHORT   = 9,
  T_INT     = 10,
  T_LONG    = 11,
};

namespace Instructions
{
using NOP = NoArgInstruction<OP_NOP>;

using BIPUSH    = OneArgInstruction<OP_BIPUSH, S8>;
using SIPUSH    = OneArgInstruction<OP_SIPUSH, S16>;
using LDC       = OneArgInstruction<OP_LDC,    U8>;
using LDC_W     = OneArgInstruction<OP_LDC_W,  U16>;
using LDC2_W    = OneArgInstruction<OP_LDC2_W, U16>;
using ILOAD     = OneArgInstruction<OP_ILOAD,  U8>;
using LLOAD     = OneArgInstruction<OP_LLOAD,  U8>;
using FLOAD     = OneArgInstruction<OP_FLOAD,  U8>;
using DLOAD     = OneArgInstruction<OP_DLOAD,  U8>;
using ALOAD     = OneArgInstruction<OP_ALOAD,  U8>;
using ISTORE    = OneArgInstruction<OP_ISTORE, U8>;
using LSTORE    = OneArgInstruction<OP_LSTORE, U8>;
using FSTORE    = OneArgInstruction<OP_FSTORE, U8>;
using DSTORE    = OneArgInstruction<OP_DSTORE, U8>;
using ASTORE    = OneArgInstruction<OP_ASTORE, U8>;
using IINC      = TwoArgInstruction<OP_IINC, U8, S8>;
using IFEQ      = OneArgInstruction<OP_IFEQ, S16>;
using IFNE      = OneArgInstruction<OP_IFNE, S16>;
using IFLT      = OneArgInstruction<OP_IFLT, S16>;
using IFGE      = OneArgInstruction<OP_IFGE, S16>;
using IFGT      = OneArgInstruction<OP_IFGT, S16>;
using IFLE      = OneArgInstruction<OP_IFLE, S16>;
using IF_ICMPEQ = OneArgInstruction<OP_IF_ICMPEQ, S16>;
using IF_ICMPNE = OneArgInstruction<OP_IF_ICMPNE, S16>;
using IF_ICMPLT = OneArgInstruction<OP_IF_ICMPLT, S16>;
using IF_ICMPGE = OneArgInstruction<OP_IF_ICMPGE, S16>;
using IF_ICMPGT = OneArgInstruction<OP_IF_ICMPGT, S16>;
using IF_ICMPLE = OneArgInstruction<OP_IF_ICMPLE, S16>;
using IF_ACMPEQ = OneArgInstruction<OP_IF_ACMPEQ, S16>;
using IF_ACMPNE = OneArgInstruction<OP_IF_ACMPNE, S16>;
using GOTO      = OneArgInstruction<OP_GOTO, S16>;
using JSR       = OneArgInstruction<OP_JSR,  S16>;
using RET       = OneArgInstruction<OP_RET,  U8>;

struct TABLESWITCH : public Instruction
{
  S32 Default;
  S32 Low;
  //NOTE: The TABLESWITCH instruction also encodes a S32 "High" operand following 
  //after the "Low" operand. This field is not nescesairy to store in memory as
  //it can be derived from the "Offsets" vector. (as Offsets.size = High - Low + 1)
  std::vector<S32> Offsets;

  TABLESWITCH(S32 def, S32 low, std::vector<S32> offsets) 
    : Instruction{OP_TABLESWITCH}, Default{def}, Low{low}, Offsets{std::move(offsets)} {}

  virtual size_t GetLength() const override;

  size_t GetPaddedLength(U32 codeOffset) const;
};

struct LOOKUPSWITCH  : public Instruction
{
  S32 Default;
  //NOTE: The LOOKUPSWITCH instruction also encodes a S32 "NPairs" operand following
  //after the "Default" operand. This field is not nescesairy to store in memory
  //as it can be derived from the "Pairs" vector. (as Pairs.size = NPairs)
  std::vector<S32> Pairs;

  LOOKUPSWITCH(S32 def, std::vector<S32> pairs) 
    : Instruction{OP_LOOKUPSWITCH}, Default{def}, Pairs{ std::move(pairs) } {}

  virtual size_t GetLength() const override;

  size_t GetPaddedLength(U32 codeOffset) const;
};

using GETSTATIC = OneArgInstruction<OP_GETSTATIC, U16>;
using PUTSTATIC = OneArgInstruction<OP_PUTSTATIC, U16>;
using GETFIELD  = OneArgInstruction<OP_GETFIELD,  U16>;
using PUTFIELD  = OneArgInstruction<OP_PUTFIELD,  U16>;
using INVOKEVIRTUAL   = OneArgInstruction<OP_INVOKEVIRTUAL, U16>;
using INVOKESPECIAL   = OneArgInstruction<OP_INVOKESPECIAL, U16>;
using INVOKESTATIC    = OneArgInstruction<OP_INVOKESTATIC,  U16>;
//NOTE: This instruction has a third U8 operand, but it is always supposed to 
//be 0 so its not nescesairy to store it in memory.
using INVOKEINTERFACE = TwoArgInstruction<OP_INVOKEINTERFACE, U16, U8>;
//NOTE: This instruction has a second & third U8 operand, but they are always 
//supposed to be 0 so it is not nescesairy to store them in memory.
using INVOKEDYNAMIC = OneArgInstruction<OP_INVOKEDYNAMIC, U16>;
using NEW           = OneArgInstruction<OP_NEW, U16>;

using NEWARRAY   = OneArgInstruction<OP_NEWARRAY,   AType>;
using ANEWARRAY  = OneArgInstruction<OP_ANEWARRAY,  U16>;
using CHECKCAST  = OneArgInstruction<OP_CHECKCAST,  U16>;
using INSTANCEOF = OneArgInstruction<OP_INSTANCEOF, U16>;

struct WIDE : public Instruction
{
  U8 OpCode;
  U16 Index;

  //TODO: verify passed opCode is one of the wideable opcodes
  WIDE(U8 opCode, U16 index) : Instruction{OP_WIDE}, OpCode{opCode}, Index{index} {}

  virtual size_t GetLength() const override;
};

struct WIDE_IINC : public Instruction
{
  U16 Index;
  U16 Const;

  WIDE_IINC(U16 index, U16 cnst) : Instruction{OP_WIDE}, Index{index}, Const{cnst} {}

  virtual size_t GetLength() const override;
};

using MULTIANEWARRAY = TwoArgInstruction<OP_MULTIANEWARRAY, U16, U8>;
using IFNULL         = OneArgInstruction<OP_IFNULL,    S16>;
using IFNONNULL      = OneArgInstruction<OP_IFNONNULL, S16>;
using GOTO_W         = OneArgInstruction<OP_GOTO_W,    S32>;
using JSR_W          = OneArgInstruction<OP_JSR_W,     S32>;

} //namespace: Instructions

} //namespace: FileFormats::JVM
