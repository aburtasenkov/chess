#pragma once

#include <exception>
#include <string>

class Exception: public std::exception
{
public:
    explicit Exception()
    {   }

    explicit Exception(std::string s)
        :error_msg(s)
    {   }

    virtual ~Exception() noexcept
    {   }

    virtual const char* what() const noexcept
    {
        return error_msg.c_str();
    }
protected:
    std::string error_msg;
};