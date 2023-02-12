#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include "DotLogger.hpp"

class Logger
{
public:
    Logger(const char* filename) : dot_(filename) {}
    ~Logger() {}

    struct DebugInfo
    {
        std::string originalName;
        std::string history;
        std::stringstream value;
        void* address;
        mutable uint64_t nodeId = 0;
        uint64_t parentId = 0;
    };

    void logCreation     (const DebugInfo& info);
    void logCreationCopy (const DebugInfo& info);
    void logAssigmentCopy(const DebugInfo& info);
    void logDeletion     (const DebugInfo& info);

    void logCreationOperator(const DebugInfo& infoRes, const DebugInfo& infoLhs, const DebugInfo& infoRhs, const char* operatorName);
    void logCreationOperator(const DebugInfo& infoRes, const DebugInfo& infoLhs, const char* operatorName);

    void enterFunction(const char* newName);
    void leaveFunction();
private:
    DotLogger dot_;
    static uint64_t nodeId;
    std::string makeNodeLabel(const DebugInfo& info);
};

extern Logger LOG;

struct FunctionGuard
{
    // const char* oldName = nullptr;
    FunctionGuard(const char* fName)
    {
        LOG.enterFunction(fName);
    }

    ~FunctionGuard()
    {
        LOG.leaveFunction();
    }
};

#define FUNC_INSPECT FunctionGuard _fg(__PRETTY_FUNCTION__);

template<class T>
concept Printable = requires(std::stringstream& stream, T t)
{
    stream << t;
};

#endif /* LOGGER_HPP */
