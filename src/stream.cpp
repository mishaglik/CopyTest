#include "stream.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <stdexcept>
#include "print.hpp"

namespace mgk {
    ofstream out(stdout);

    void ostream::putString(const char* string, size_t len)
    {
        if(!buffer_)
        {
            write(string, len);
            return;
        }

        if(len > bufferCapacity_ - bufferSize_)
        {
            flush();

            if(len > bufferCapacity_ / 2)
            {
                write(string, len);
                return;
            }
        }

        memcpy(buffer_ + bufferSize_, string, len);
        bufferSize_ += len;
    }

    void ostream::putChar(char c)
    {
        if(!buffer_)
        {
            write(&c, 1);
        }

        if(bufferSize_ == bufferCapacity_)
        {
            flush();
        }

        buffer_[bufferSize_++] = c;
    }


    ostream& ostream::operator<<(bool cond)              
    {
        putString(cond ? "true" : "false", cond ? 4 : 5);
        return *this;
    }

    ostream& ostream::operator<<(char c)              
    {
        putChar(c);
        return *this;
    }

    ostream& ostream::operator<<(uint32_t x)          
    {
        putNumber(x, NumFlags::None);
        return *this;
    }

    ostream& ostream::operator<<(int32_t x)           
    {
        putNumber(x, NumFlags::Signed);
        return *this;
    }

    ostream& ostream::operator<<(int64_t x)           
    {   
        putNumber(x, NumFlags::Signed);
        return *this;
    }

    ostream& ostream::operator<<(uint64_t x)          
    {
        putNumber(x, NumFlags::None);
        return *this;
    }

    ostream& ostream::operator<<(format::hex x)          
    {
        putNumber(x.x, NumFlags::Hex);
        return *this;
    }

    ostream& ostream::operator<<(format::oct x)          
    {
        putNumber(x.x, NumFlags::Oct);
        return *this;
    }


    ostream& ostream::operator<<(std::nullptr_t)    
    {
        putString("nullptr", sizeof("nullptr") - 1);
        return *this;
    }

    ostream& ostream::operator<<(const char* s)       
    {
        size_t len = strlen(s);
        putString(s, len);
        return *this;
    }

    ostream& ostream::operator<<(const void* s)       
    {
        putNumber(reinterpret_cast<uint64_t>(s), NumFlags::Ptr);
        return *this;
    }

    ostream& ostream::operator<<(const std::string& s)
    {
        putString(s.c_str(), s.length());   
        return *this;
    }

    ostream& ostream::operator<<(endl_t)              
    {
        putChar('\n');
        flush();
        return *this;
    }

    void ostream::putNumber(uint64_t number, uint64_t numFlags)
    {
        if(bufferCapacity_ - bufferSize_ < MAX_NUMBER_WIDTH)
        {
            flush();
        }

        char format[10] = "%ju";

        size_t idx = 2;

        #define ADD_SPRINF_FLAG_(name, ch) \
            if(numFlags & NumFlags::name)  \
            {                              \
                format[idx++] = ch;        \
            }                   
        
        ADD_SPRINF_FLAG_(ForcedSign,  '+');
        ADD_SPRINF_FLAG_(SignedSpace, ' ');
        ADD_SPRINF_FLAG_(Prefix,      '#');
        ADD_SPRINF_FLAG_(ZeroFill,    '0');
        ADD_SPRINF_FLAG_(AllignLeft,  '-');
        
        #undef ADD_SPRINF_FLAG_


        if(numFlags & NumFlags::Signed) {
            format[idx++] = 'd';
        } else if (numFlags & NumFlags::Oct) {
            format[idx++] = 'o';
        } else if (numFlags & NumFlags::Hex) {
            format[idx++] = 'x';
        } else {
            format[idx++] = 'u';
        }

        if(numFlags & NumFlags::Ptr)
        {
            format[1] = 'p';
            format[2] = '\0';
            assert(numFlags == NumFlags::Ptr);
        }

        if(buffer_)
        {
            bufferSize_ += sprintf(buffer_ + bufferSize_, format, number); 
        }
        else {
            char tmp[MAX_NUMBER_WIDTH] = {};
            size_t size = sprintf(tmp, format, number);
            putString(tmp, size);
        }
    }

    void ostream::flush()
    {
        if(buffer_ && bufferSize_)
        {
            write(buffer_, bufferSize_);
            bufferSize_ = 0;
        }
    }

    bool ofstream::open(const char* filename)
    {
        if(file_)
        {
            close();
        }

        file_ = fopen(filename, "w");
        destruct_file = true;
        std::setbuf(file_, nullptr);
        return file_;
    }

    void ofstream::close()
    {
        if(file_ && destruct_file) fclose(file_);
        file_ = nullptr;
        
    }

    void ofstream::write(const void *data, size_t n)
    {
        if(file_)
        {
            fwrite(data, sizeof(char), n,  file_);
        }
    }


    void istream::prepareBlock()
    {
        if(eof_) return;

        if(bufferCur_ == bufferSize_)
        {
            ssize_t red = read(buffer_, bufferCapacity_ - 1);
            if(red == EOF)
            {
                eof_ = true;
                bufferCur_ = bufferSize_ = 0;
                return;
            }
            bufferSize_ = red;
            buffer_[bufferSize_] = 0;
        }
        while (bufferCur_ < bufferSize_ && isspace(buffer_[bufferCur_])) 
        {
            bufferCur_++;
            if(bufferCur_ == bufferSize_)
            {
                ssize_t red = read(buffer_, bufferCapacity_ - 1);
                if(red == EOF)
                {
                    eof_ = true;
                    bufferCur_ = bufferSize_ = 0;
                    return;
                }
                bufferSize_ = red;
                buffer_[bufferSize_] = 0;

            }
        }
        
        while(bufferSize_ < bufferCapacity_)
        {
            for(size_t i = bufferCur_; i < bufferSize_; i++)
            {
                if(isspace(buffer_[i]))
                {
                    return;
                }
            }

            if(bufferCur_ != 0)
            {
                memmove(buffer_, buffer_ + bufferCur_, bufferSize_ - bufferCur_ + 1);
                bufferSize_ -= bufferCur_;
            }

            ssize_t red = read(buffer_ + bufferCur_, bufferCapacity_ - bufferSize_ - 1);
            if(red == eof_) return;
            bufferSize_ += red;
            buffer_[bufferSize_] = 0;
        }
    }


    istream& istream::operator>>(bool& c)       
    {
        prepareBlock();
        if(eof_) return *this;
        if(buffer_[bufferCur_] == '1')
        {
            c = true;
            bufferCur_++;
            return *this;
        }  
        if(buffer_[bufferCur_] == '0')
        {
            c = false;
            bufferCur_++;
            return *this;
        }
        //TODO: read str true/false
        return *this;
    }

    istream& istream::operator>>(char& c)       
    {
        prepareBlock();
        if(eof_) return *this;
        c = buffer_[bufferCur_++];
        return *this;
    }

    istream& istream::operator>>(uint32_t& x)   
    {
        prepareBlock();
        if(eof_) return *this;
        int red = 0;
        if(sscanf(buffer_ + bufferCur_, "%u%n", &x, &red) > 0)
        {
            bufferCur_ += red;
        }
        return *this;
    }

    istream& istream::operator>>(int32_t& x)    
    {
        prepareBlock();
        if(eof_) return *this;
        int red = 0;
        if(sscanf(buffer_ + bufferCur_, "%d%n", &x, &red) > 0)
        {
            bufferCur_ += red;
        } 
        return *this;
    }

    istream& istream::operator>>(int64_t& x)    
    {
        prepareBlock();
        if(eof_) return *this;
        int red = 0;
        if(sscanf(buffer_ + bufferCur_, "%ld%n", &x, &red) > 0)
        {
            bufferCur_ += red;
        }
        return *this;
    }

    istream& istream::operator>>(uint64_t& x)   
    {
        prepareBlock();
        if(eof_) return *this;
        int red = 0;
        if(sscanf(buffer_ + bufferCur_, "%lu%n", &x, &red) > 0)
        {
            bufferCur_ += red;
        }
        return *this;
    }

    istream& istream::operator>>(std::string& s)
    {
        //FIXME: Not implemented;
        s += buffer_ + bufferCur_;
        bufferCur_ = bufferSize_;
        return *this;
    }



    void print(const char* format)
    {
        while (*format) {
            if(*format == '%')
            {
                if(*(++format) != '%')
                {
                    throw std::runtime_error("Too few arguments in print");
                }
            }
            mgk::out << *format;
            format++;
        }
    }
} 