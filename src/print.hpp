#ifndef PRINT_HPP
#define PRINT_HPP

#include <stdexcept>
#include <utility>
#include "stream.hpp"
#include "MySTL.hpp"
#include "Watcher.hpp"

namespace mgk {

void print(const char* format);

template<typename T, typename ...Ts>
void print(const char* format, T&& arg1, Ts&& ...args)
{
    FUNC_INSPECT
    while (*format != '\0' && *(format) != '%')
    {
        mgk::out << *format;
        format++;
    }
    format++;

    while (1) {
    
        switch (*(format++)) {
            case '\0':
                throw std::runtime_error("Too many print arguments");   
            case 'd':
            case 'f':
            case 'g':
            case 'o':
            case 'p':
            case 's':
            case 'u':
            case 'x':
            case '$':
                mgk::out << arg1;
                print(format, my::forward<Ts>(args)...);
                return;
            case '%':
                mgk::out << '%';
                print(format, my::forward<T>(arg1), my::forward<Ts>(args)...);
                return;
            default:
                break;
        } 
    }
}

}

#endif /* PRINT_HPP */
