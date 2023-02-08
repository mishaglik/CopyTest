#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <fstream>
#include <string>
#include <sstream>

class Logger
{
public:
    constexpr Logger();
    ~Logger();

    struct DebugInfo
    {
        std::string originalName;
        std::string history;
        std::stringstream value;
        void* address;
        mutable int nodeId = 0;
    };

    void logCreation(const DebugInfo& info);
    void logCreationCopy(const DebugInfo& info);
    void logAssigment(const DebugInfo& info);
    void logDeletion(const DebugInfo& info);

    const char* enterFunction(const char* newName);
    void leaveFunction(const char* oldName);

};

extern Logger LOG;

struct FunctionGuard
{
    const char* oldName = nullptr;
    FunctionGuard(const char* fName)
    {
        oldName = LOG.enterFunction(fName);
    }

    ~FunctionGuard()
    {
        LOG.leaveFunction(oldName);
    }
};

#define FUNC_INSPECT FunctionGuard _fg(__PRETTY_FUNCTION__);
template<class T>
concept Printable = requires(std::stringstream& stream, T t)
{
    stream << t;
};

#endif /* LOGGER_HPP */
