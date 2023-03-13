#include "FileFormats/JVM/Instruction.hpp"

#include "../Util/Macros.hpp"


#include <map>
#include <tuple>
#include <cassert>

using namespace FileFormats;
using namespace JVM;
using namespace Instructions;

using namespace std::literals;

static std::map<U8, std::string_view> OpCodeNames
{
  NAMED_TUPLE(OP_NOP           ), 
  NAMED_TUPLE(OP_ACONST_NULL   ), 
  NAMED_TUPLE(OP_ICONST_M1     ), 
  NAMED_TUPLE(OP_ICONST_0      ), 
  NAMED_TUPLE(OP_ICONST_1      ), 
  NAMED_TUPLE(OP_ICONST_2      ), 
  NAMED_TUPLE(OP_ICONST_3      ), 
  NAMED_TUPLE(OP_ICONST_4      ), 
  NAMED_TUPLE(OP_ICONST_5      ), 
  NAMED_TUPLE(OP_LCONST_0      ), 
  NAMED_TUPLE(OP_LCONST_1      ), 
  NAMED_TUPLE(OP_FCONST_0      ), 
  NAMED_TUPLE(OP_FCONST_1      ), 
  NAMED_TUPLE(OP_FCONST_2      ), 
  NAMED_TUPLE(OP_DCONST_0      ), 
  NAMED_TUPLE(OP_DCONST_1      ), 
  NAMED_TUPLE(OP_BIPUSH        ), 
  NAMED_TUPLE(OP_SIPUSH        ), 
  NAMED_TUPLE(OP_LDC           ), 
  NAMED_TUPLE(OP_LDC_W         ), 
  NAMED_TUPLE(OP_LDC2_W        ), 
  NAMED_TUPLE(OP_ILOAD         ), 
  NAMED_TUPLE(OP_LLOAD         ), 
  NAMED_TUPLE(OP_FLOAD         ), 
  NAMED_TUPLE(OP_DLOAD         ), 
  NAMED_TUPLE(OP_ALOAD         ), 
  NAMED_TUPLE(OP_ILOAD_0       ), 
  NAMED_TUPLE(OP_ILOAD_1       ), 
  NAMED_TUPLE(OP_ILOAD_2       ), 
  NAMED_TUPLE(OP_ILOAD_3       ), 
  NAMED_TUPLE(OP_LLOAD_0       ), 
  NAMED_TUPLE(OP_LLOAD_1       ), 
  NAMED_TUPLE(OP_LLOAD_2       ), 
  NAMED_TUPLE(OP_LLOAD_3       ), 
  NAMED_TUPLE(OP_FLOAD_0       ), 
  NAMED_TUPLE(OP_FLOAD_1       ), 
  NAMED_TUPLE(OP_FLOAD_2       ), 
  NAMED_TUPLE(OP_FLOAD_3       ), 
  NAMED_TUPLE(OP_DLOAD_0       ), 
  NAMED_TUPLE(OP_DLOAD_1       ), 
  NAMED_TUPLE(OP_DLOAD_2       ), 
  NAMED_TUPLE(OP_DLOAD_3       ), 
  NAMED_TUPLE(OP_ALOAD_0       ), 
  NAMED_TUPLE(OP_ALOAD_1       ), 
  NAMED_TUPLE(OP_ALOAD_2       ), 
  NAMED_TUPLE(OP_ALOAD_3       ), 
  NAMED_TUPLE(OP_IALOAD        ), 
  NAMED_TUPLE(OP_LALOAD        ), 
  NAMED_TUPLE(OP_FALOAD        ), 
  NAMED_TUPLE(OP_DALOAD        ), 
  NAMED_TUPLE(OP_AALOAD        ), 
  NAMED_TUPLE(OP_BALOAD        ), 
  NAMED_TUPLE(OP_CALOAD        ), 
  NAMED_TUPLE(OP_SALOAD        ), 
  NAMED_TUPLE(OP_ISTORE        ), 
  NAMED_TUPLE(OP_LSTORE        ), 
  NAMED_TUPLE(OP_FSTORE        ), 
  NAMED_TUPLE(OP_DSTORE        ), 
  NAMED_TUPLE(OP_ASTORE        ), 
  NAMED_TUPLE(OP_ISTORE_0      ), 
  NAMED_TUPLE(OP_ISTORE_1      ), 
  NAMED_TUPLE(OP_ISTORE_2      ), 
  NAMED_TUPLE(OP_ISTORE_3      ), 
  NAMED_TUPLE(OP_LSTORE_0      ), 
  NAMED_TUPLE(OP_LSTORE_1      ), 
  NAMED_TUPLE(OP_LSTORE_2      ), 
  NAMED_TUPLE(OP_LSTORE_3      ), 
  NAMED_TUPLE(OP_FSTORE_0      ), 
  NAMED_TUPLE(OP_FSTORE_1      ), 
  NAMED_TUPLE(OP_FSTORE_2      ), 
  NAMED_TUPLE(OP_FSTORE_3      ), 
  NAMED_TUPLE(OP_DSTORE_0      ), 
  NAMED_TUPLE(OP_DSTORE_1      ), 
  NAMED_TUPLE(OP_DSTORE_2      ), 
  NAMED_TUPLE(OP_DSTORE_3      ), 
  NAMED_TUPLE(OP_ASTORE_0      ), 
  NAMED_TUPLE(OP_ASTORE_1      ), 
  NAMED_TUPLE(OP_ASTORE_2      ), 
  NAMED_TUPLE(OP_ASTORE_3      ), 
  NAMED_TUPLE(OP_IASTORE       ), 
  NAMED_TUPLE(OP_LASTORE       ), 
  NAMED_TUPLE(OP_FASTORE       ), 
  NAMED_TUPLE(OP_DASTORE       ), 
  NAMED_TUPLE(OP_AASTORE       ), 
  NAMED_TUPLE(OP_BASTORE       ), 
  NAMED_TUPLE(OP_CASTORE       ), 
  NAMED_TUPLE(OP_SASTORE       ), 
  NAMED_TUPLE(OP_POP           ), 
  NAMED_TUPLE(OP_POP2          ), 
  NAMED_TUPLE(OP_DUP           ), 
  NAMED_TUPLE(OP_DUP_X1        ), 
  NAMED_TUPLE(OP_DUP_X2        ), 
  NAMED_TUPLE(OP_DUP2          ), 
  NAMED_TUPLE(OP_DUP2_X1       ), 
  NAMED_TUPLE(OP_DUP2_X2       ), 
  NAMED_TUPLE(OP_SWAP          ), 
  NAMED_TUPLE(OP_IADD          ), 
  NAMED_TUPLE(OP_LADD          ), 
  NAMED_TUPLE(OP_FADD          ), 
  NAMED_TUPLE(OP_DADD          ), 
  NAMED_TUPLE(OP_ISUB          ), 
  NAMED_TUPLE(OP_LSUB          ), 
  NAMED_TUPLE(OP_FSUB          ), 
  NAMED_TUPLE(OP_DSUB          ), 
  NAMED_TUPLE(OP_IMUL          ), 
  NAMED_TUPLE(OP_LMUL          ), 
  NAMED_TUPLE(OP_FMUL          ), 
  NAMED_TUPLE(OP_DMUL          ), 
  NAMED_TUPLE(OP_IDIV          ), 
  NAMED_TUPLE(OP_LDIV          ), 
  NAMED_TUPLE(OP_FDIV          ), 
  NAMED_TUPLE(OP_DDIV          ), 
  NAMED_TUPLE(OP_IREM          ), 
  NAMED_TUPLE(OP_LREM          ), 
  NAMED_TUPLE(OP_FREM          ), 
  NAMED_TUPLE(OP_DREM          ), 
  NAMED_TUPLE(OP_INEG          ), 
  NAMED_TUPLE(OP_LNEG          ), 
  NAMED_TUPLE(OP_FNEG          ), 
  NAMED_TUPLE(OP_DNEG          ), 
  NAMED_TUPLE(OP_ISHL          ), 
  NAMED_TUPLE(OP_LSHL          ), 
  NAMED_TUPLE(OP_ISHR          ), 
  NAMED_TUPLE(OP_LSHR          ), 
  NAMED_TUPLE(OP_IUSHR         ), 
  NAMED_TUPLE(OP_LUSHR         ), 
  NAMED_TUPLE(OP_IAND          ), 
  NAMED_TUPLE(OP_LAND          ), 
  NAMED_TUPLE(OP_IOR           ), 
  NAMED_TUPLE(OP_LOR           ), 
  NAMED_TUPLE(OP_IXOR          ), 
  NAMED_TUPLE(OP_LXOR          ), 
  NAMED_TUPLE(OP_IINC          ), 
  NAMED_TUPLE(OP_I2L           ), 
  NAMED_TUPLE(OP_I2F           ), 
  NAMED_TUPLE(OP_I2D           ), 
  NAMED_TUPLE(OP_L2I           ), 
  NAMED_TUPLE(OP_L2F           ), 
  NAMED_TUPLE(OP_L2D           ), 
  NAMED_TUPLE(OP_F2I           ), 
  NAMED_TUPLE(OP_F2L           ), 
  NAMED_TUPLE(OP_F2D           ), 
  NAMED_TUPLE(OP_D2I           ), 
  NAMED_TUPLE(OP_D2L           ), 
  NAMED_TUPLE(OP_D2F           ), 
  NAMED_TUPLE(OP_I2B           ), 
  NAMED_TUPLE(OP_I2C           ), 
  NAMED_TUPLE(OP_I2S           ), 
  NAMED_TUPLE(OP_LCMP          ), 
  NAMED_TUPLE(OP_FCMPL         ), 
  NAMED_TUPLE(OP_FCMPG         ), 
  NAMED_TUPLE(OP_DCMPL         ), 
  NAMED_TUPLE(OP_DCMPG         ), 
  NAMED_TUPLE(OP_IFEQ          ), 
  NAMED_TUPLE(OP_IFNE          ), 
  NAMED_TUPLE(OP_IFLT          ), 
  NAMED_TUPLE(OP_IFGE          ), 
  NAMED_TUPLE(OP_IFGT          ), 
  NAMED_TUPLE(OP_IFLE          ), 
  NAMED_TUPLE(OP_IF_ICMPEQ     ), 
  NAMED_TUPLE(OP_IF_ICMPNE     ), 
  NAMED_TUPLE(OP_IF_ICMPLT     ), 
  NAMED_TUPLE(OP_IF_ICMPGE     ), 
  NAMED_TUPLE(OP_IF_ICMPGT     ), 
  NAMED_TUPLE(OP_IF_ICMPLE     ), 
  NAMED_TUPLE(OP_IF_ACMPEQ     ), 
  NAMED_TUPLE(OP_IF_ACMPNE     ), 
  NAMED_TUPLE(OP_GOTO          ), 
  NAMED_TUPLE(OP_JSR           ), 
  NAMED_TUPLE(OP_RET           ), 
  NAMED_TUPLE(OP_TABLESWITCH   ), 
  NAMED_TUPLE(OP_LOOKUPSWITCH  ), 
  NAMED_TUPLE(OP_IRETURN       ), 
  NAMED_TUPLE(OP_LRETURN       ), 
  NAMED_TUPLE(OP_FRETURN       ), 
  NAMED_TUPLE(OP_DRETURN       ), 
  NAMED_TUPLE(OP_ARETURN       ), 
  NAMED_TUPLE(OP_RETURN        ), 
  NAMED_TUPLE(OP_GETSTATIC     ), 
  NAMED_TUPLE(OP_PUTSTATIC     ), 
  NAMED_TUPLE(OP_GETFIELD      ), 
  NAMED_TUPLE(OP_PUTFIELD      ), 
  NAMED_TUPLE(OP_INVOKEVIRTUAL ), 
  NAMED_TUPLE(OP_INVOKESPECIAL ), 
  NAMED_TUPLE(OP_INVOKESTATIC  ), 
  NAMED_TUPLE(OP_INVOKEINTERFACE),
  NAMED_TUPLE(OP_INVOKEDYNAMIC ), 
  NAMED_TUPLE(OP_NEW           ), 
  NAMED_TUPLE(OP_NEWARRAY      ), 
  NAMED_TUPLE(OP_ANEWARRAY     ), 
  NAMED_TUPLE(OP_ARRAYLENGTH   ), 
  NAMED_TUPLE(OP_ATHROW        ), 
  NAMED_TUPLE(OP_CHECKCAST     ), 
  NAMED_TUPLE(OP_INSTANCEOF    ), 
  NAMED_TUPLE(OP_MONITORENTER  ), 
  NAMED_TUPLE(OP_MONITOREXIT   ), 
  NAMED_TUPLE(OP_WIDE          ), 
  NAMED_TUPLE(OP_MULTIANEWARRAY), 
  NAMED_TUPLE(OP_IFNULL        ), 
  NAMED_TUPLE(OP_IFNONNULL     ), 
  NAMED_TUPLE(OP_GOTO_W        ), 
  NAMED_TUPLE(OP_JSR_W         ), 
  NAMED_TUPLE(OP_BREAKPOINT    ), 
  NAMED_TUPLE(OP_IMPDEP1       ), 
  NAMED_TUPLE(OP_IMPDEP2       ), 
};

ErrorOr<std::string_view> FileFormats::JVM::GetOpCodeMnemonic(U8 opCode)
{
  auto itr = OpCodeNames.find(opCode);

  if(itr != OpCodeNames.end())
    return std::get<1>(*itr).substr(2);

  return Error::FromFormatStr("unknown name for opcode: %#04x", opCode);
}

ErrorOr<std::string_view> Instruction::GetMnemonic() const
{
  return GetOpCodeMnemonic(this->OpCode);
}

size_t Instruction::GetLength() const
{
  return sizeof(this->OpCode);
}

size_t TABLESWITCH::GetLength() const 
{
  return Instruction::GetLength() + sizeof(Default) + sizeof(Low) 
    + sizeof(U32)  //"High" operand
    + (sizeof(decltype(Offsets)::value_type) * Offsets.size());
}

size_t TABLESWITCH::GetPaddedLength(U32 codeOffset) const 
{
  return (codeOffset % 4) + this->GetLength();
}

size_t LOOKUPSWITCH::GetLength() const 
{
  return Instruction::GetLength() + sizeof(Default) 
    + sizeof(U32) //"NPairs" operand
    + (sizeof(decltype(Pairs)::value_type) * Pairs.size());
}

size_t LOOKUPSWITCH::GetPaddedLength(U32 codeOffset) const 
{
  return (codeOffset % 4) + this->GetLength();
}

size_t WIDE::GetLength() const 
{
  return Instruction::GetLength() + sizeof(OpCode) + sizeof(Index);
}

size_t WIDE_IINC::GetLength() const 
{
  return Instruction::GetLength() + sizeof(Index) + sizeof(Const);
}
