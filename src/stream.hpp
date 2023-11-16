#ifndef STREAM_HPP
#define STREAM_HPP
#include <bits/types/FILE.h>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <new>
#include <string>

namespace mgk {

    class endl_t {};

    namespace format {
        struct hex
        {
            uint64_t x;
        };

        struct oct
        {
            uint64_t x;
        };
    }

    static const endl_t endl{};

    const size_t MAX_NUMBER_WIDTH = 32;
    const size_t BUFFER_MIN_CAPACITY = MAX_NUMBER_WIDTH;

    class ostream
    {

    protected:
        virtual void write(const void* data, size_t n)  = 0;

    public:
        ostream(size_t capacity = 4096) : bufferCapacity_(capacity)
        {
            if(bufferCapacity_)
            {
                bufferCapacity_ = std::max(capacity, BUFFER_MIN_CAPACITY);
                buffer_ = new(std::nothrow) char[bufferCapacity_];
                if(!buffer_) bufferCapacity_ = 0;
            }
        }

        virtual ~ostream() 
        {
            flush();
            delete [] buffer_;
            bufferSize_ = 0;
            bufferCapacity_ = 0;
        }

        // Non-copyable
        ostream(const ostream& oth)            = delete;
        ostream& operator=(const ostream& oth) = delete;

        // Movable
        ostream(ostream&& oth)            = default;
        ostream& operator=(ostream&& oth) = default;

        ostream& operator<<(bool c)              ;
        ostream& operator<<(char c)              ;
        ostream& operator<<(uint32_t x)          ;
        ostream& operator<<(int32_t x)           ;
        ostream& operator<<(int64_t x)           ;
        ostream& operator<<(uint64_t x)          ;
        ostream& operator<<(format::hex x)       ;
        ostream& operator<<(format::oct x)       ;
        ostream& operator<<(std::nullptr_t x)    ;
        ostream& operator<<(const char* s)       ;
        ostream& operator<<(const void* s)       ;
        ostream& operator<<(const std::string& s);
        ostream& operator<<(endl_t)              ;

        void flush();
    private:
        enum NumFlags : uint64_t
        {
            None        = 0x000,
            Signed      = 0x001,
            ForcedSign  = 0x002,
            Hex         = 0x004,
            Prefix      = 0x008,
            ZeroFill    = 0x010,
            Ptr         = 0x020,
            AllignLeft  = 0x040,
            Oct         = 0x080,
            SignedSpace = 0x100,
        };

        void putChar(char c);
        void putString(const char* string, size_t len);
        void putNumber(uint64_t number, uint64_t numFlags);
        char* buffer_ = nullptr;
        size_t bufferSize_ = 0;
        size_t bufferCapacity_ = 0;
    };

    class ofstream : public ostream
    {
    public:
        ofstream(size_t capacity = 4096) : ostream(capacity) {}
        ofstream(FILE* file, size_t capacity = 4096) : ostream(capacity), file_(file), destruct_file(false) {}
        virtual ~ofstream() override 
        {
            flush();
            close();
        }

        bool open(const char* filename);
        void close();
    private:
        FILE* file_ = nullptr;
        bool destruct_file = true;
        virtual void write(const void *data, size_t n) override;
    };

    extern ofstream out;


    //====================< istream >========================
    class istream
    {

    protected:
        virtual ssize_t read(const void* data, size_t n)  = 0;

    public:
        istream(size_t capacity = 4096) : bufferCapacity_(capacity)
        {
            bufferCapacity_ = std::max(capacity, BUFFER_MIN_CAPACITY);
            buffer_ = new char[bufferCapacity_];
        }

        virtual ~istream() 
        {
            delete [] buffer_;
            bufferSize_ = 0;
            bufferCapacity_ = 0;
        }

        // Non-copyable
        istream(const ostream& oth)            = delete;
        istream& operator=(const ostream& oth) = delete;

        // Movable
        istream(istream&& oth)            = default;
        istream& operator=(istream&& oth) = default;

        istream& operator>>(bool& c)       ;
        istream& operator>>(char& c)       ;
        istream& operator>>(uint32_t& x)   ;
        istream& operator>>(int32_t& x)    ;
        istream& operator>>(int64_t& x)    ;
        istream& operator>>(uint64_t& x)   ;
        istream& operator>>(std::string& s);

        operator bool() {return !eof_;}
    private:
        bool eof_ = false;
        void prepareBlock();
        void putChar(char c);
        void putString(const char* string, size_t len);
        void putNumber(uint64_t number, uint64_t numFlags);
        char* buffer_ = nullptr;
        size_t bufferSize_ = 0;
        size_t bufferCur_ = 0;
        size_t bufferCapacity_ = 0;
    };
}

#endif /* STREAM_HPP */
