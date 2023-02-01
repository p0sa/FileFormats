#pragma once

#include "FileFormats/Defs.hpp"
#include "FileFormats/Error.hpp"

using namespace FileFormats;

enum ByteOrder
{
  LittleEndian,
  BigEndian
};

constexpr ByteOrder GetHostByteOrder()
{
  const U16 data{ 0xAABB };
  const void* addr = static_cast<const void*>(&data);
  const unsigned char* leastSigByte = static_cast<const unsigned char*>(addr);

  return *leastSigByte == 0xAA ? ByteOrder::BigEndian : ByteOrder::LittleEndian;
}

template <typename T>
void SwapByteOrder(T& t)
{
  char* bytes = reinterpret_cast<char*>(&t);
  size_t len = sizeof(t);

  char tmp;
  for (size_t i = 0; i < (len / 2); i++)
  {
    tmp = bytes[i];
    bytes[i] = bytes[len - 1 - i];
    bytes[len - 1 - i] = tmp;
  }

}

template <ByteOrder Order = LittleEndian, typename T>
ErrorOr<void> Read(std::istream& stream, T& t)
{
  stream.read(reinterpret_cast<char*>(&t), sizeof(T));

  if (stream.bad())
    return Error::FromFormatStr("Read() failed: streams badbit got set after read. (steampos = 0x%X) (T = %s) (sizeof T = %u)", stream.tellg(), typeid(T).name(), sizeof(T));

  if (Order != GetHostByteOrder())
    SwapByteOrder(t);

  return {};
}

template <ByteOrder Order = LittleEndian, typename... Args>
ErrorOr<void> Read(std::istream& stream, Args&... args)
{
  return (Read<Order>(stream, args), ...);
}
