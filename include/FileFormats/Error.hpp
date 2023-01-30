#pragma once 

#include "Defs.hpp"

#include <variant>

namespace FileFormats
{

class Error
{
  public:
    explicit Error(int code) : m_code{ code } {}
    explicit Error(const char* message) : m_message{ message } {}

    virtual ~Error()
    {
      if (m_message == nullptr)
        delete[] m_message;
    }

    template <typename... Args>
    static Error FromFormatStr(const char* format, Args... args)
    {
      int bufsize = std::snprintf(nullptr, 0, format, args...);

      char* buf = new char[bufsize];
      std::snprintf(buf, bufsize, format, args...);

      return Error{buf};
    }
  
    bool IsCode()
    {
      return m_message != nullptr;
    }
  
    int GetCode()
    {
      return m_code;
    }
  
    const char* GetMessage()
    {
      return m_message;
    }
  
  private:
    int m_code;
    const char* m_message{ nullptr };
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
