#pragma once 

#include "Defs.hpp"

#include <string>
#include <variant>

#include <iostream>

namespace FileFormats
{

class Error
{
  public:
    template <typename... Args>
    static Error FromFormatStr(const char* format, Args... args)
    {
      int bufsize = std::snprintf(nullptr, 0, format, args...);

      std::unique_ptr<char[]> buf{ new char[bufsize+1] };
      std::snprintf(buf.get(), bufsize+1, format, args...);

      return Error{ std::string{buf.get(), buf.get()+bufsize}};
    }

    static Error FromLiteralStr(const char* msg)
    {
      return Error{msg};
    }

    static Error FromErrCode(int code)
    {
      return Error{code};
    }
  
    bool IsCode()
    {
      return m_message.empty();
    }
  
    int GetCode()
    {
      return m_code;
    }
  
    std::string GetMessage()
    {
      return m_message;
    }

  protected:
    Error(int code) : m_code{ code } {}
    Error(std::string message) : m_message{message} {}
  
  private:
    int m_code;
    std::string m_message;
};

template <typename T>
class ErrorOr
{
  public:
    ErrorOr(Error err) : m_errorOrT { err } {}
    ErrorOr(T const& t) : m_errorOrT { t } {}
    ErrorOr(T&& t) : m_errorOrT { std::move(t) } {}
  
    bool IsError()
    {
      return std::holds_alternative<Error>(m_errorOrT);
    }
  
    Error GetError()
    {
      return std::get<Error>(m_errorOrT);
    }
  
    T Get()
    {
      return std::get<T>(m_errorOrT);
    }
  
    T&& Release()
    {
      return std::move(std::get<T>(m_errorOrT));
    }
  
  private:
  std::variant<Error, T> m_errorOrT;

};

template <>
class ErrorOr<void> : public ErrorOr<std::monostate>
{
  public:
    ErrorOr(Error error) : ErrorOr<std::monostate>{error} {}
    ErrorOr() : ErrorOr<std::monostate>{ {}  } {}
};

} //namespace FileFormats
