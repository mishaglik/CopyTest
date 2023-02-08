#ifndef WATCHER_HPP
#define WATCHER_HPP
#include "Logger.hpp"

template<class T>
class Watcher : public T
{
public:
    template<class... Args>
    Watcher(Args... args, const char* name = nullptr) :
        T(args...)
    {
        if(name == nullptr) 
        {
            _debugInfo.originalName = "TMP#";
            _debugInfo.originalName += std::string(_tmpId++);
        }
        _debugInfo.history = _debugInfo.originalName;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreation(_debugInfo);
    }

    Watcher(const Watcher& other) : T(other)
    {
        _debugInfo.history = "Copy of \"" + other._debugInfo.history + "\"";
        _debugInfo.originalName = _debugInfo.history;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreation(_debugInfo);
    }

    ~Watcher()
    {
        LOG.logDeletion(_debugInfo);
    }
private:
    static uint64_t _tmpId;
    Logger::DebugInfo _debugInfo;
};

#endif /* WATCHER_HPP */
