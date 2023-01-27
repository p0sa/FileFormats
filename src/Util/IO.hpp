#pragma once

#include "FileFormats/Defs.hpp"
#include "FileFormats/Error.hpp"


using namespace FileFormats;

enum ByteOrder
{
  LittleEndian,
  BigEndian
};

//TODO: this is only defined in gcc. Add more compiler support or make
//      detection compiler independant.
#define HOST_BYTE_ORDER ( (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) ? \
                                ByteOrder::LittleEndian : ByteOrder::BigEndian)

template <typename T>
void SwapByteOrder(T& t)
{
  char* bytes = reinterpret_cast<char*>(&t);
  size_t len = sizeof(t);

  char tmp;
  for(size_t i = 0; i < (len/2); i++)
  {
    tmp = bytes[i];
    bytes[i] = bytes[len-1-i];
    bytes[len-1-i] = tmp;
  }

}

template <ByteOrder Order=LittleEndian, typename T>
void Read(std::istream& stream, T& t)
{
  stream.read(reinterpret_cast<char*>(&t), sizeof(T));

  if(Order != HOST_BYTE_ORDER)
    SwapByteOrder(t);
}

template <ByteOrder Order=LittleEndian, typename... Args>
void Read(std::istream& stream, Args&... args)
{
  (Read<Order>(stream, args), ...);
}
