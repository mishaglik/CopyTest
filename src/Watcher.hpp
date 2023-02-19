#ifndef WATCHER_HPP
#define WATCHER_HPP
#include "Logger.hpp"
#include <string>

extern uint64_t TempId;
extern uint64_t Copies;

template<class T>
class Watcher : public T
{
public:
    Watcher (const char* name = nullptr) : T()
    {
        if(name == nullptr) 
        {
            _debugInfo.originalName = "TMP#";
            _debugInfo.originalName += std::to_string(TempId++);
        }
        else {
            _debugInfo.originalName = name;
        }
        _debugInfo.history = _debugInfo.originalName;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreation(_debugInfo);
    }

    template<class U>
    explicit Watcher(U u, const char* name = nullptr) : //FIXME: Not only single element constructor
        T(u)
    {
        if(name == nullptr) 
        {
            _debugInfo.originalName = "TMP#";
            _debugInfo.originalName += std::to_string(TempId++);
        }
        else {
            _debugInfo.originalName = name;
        }
        _debugInfo.history = _debugInfo.originalName;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreation(_debugInfo);
    }
    template<class U, class S>
    explicit Watcher(U u, S s, const char* name = nullptr) : //FIXME: Not only single element constructor
        T(u, s)
    {
        if(name == nullptr) 
        {
            _debugInfo.originalName = "TMP#";
            _debugInfo.originalName += std::to_string(TempId++);
        }
        else {
            _debugInfo.originalName = name;
        }
        _debugInfo.history = _debugInfo.originalName;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreation(_debugInfo);
    }

    Watcher(const T& t, const Watcher& lhs, const Watcher& rhs, const char* operatorName) : T(t)
    {
        _debugInfo.originalName = "TMP#";
        _debugInfo.originalName += std::to_string(TempId++);
        _debugInfo.history = lhs._debugInfo.history + operatorName + rhs._debugInfo.history;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreationOperator(_debugInfo, lhs._debugInfo, rhs._debugInfo, operatorName);
    }

    Watcher(const T& t, const Watcher& lhs, const char* operatorName) : T(t)
    {
        _debugInfo.originalName = "TMP#";
        _debugInfo.originalName += std::to_string(TempId++);
        _debugInfo.history = lhs._debugInfo.history + operatorName;
        _debugInfo.address = this;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreationOperator(_debugInfo, lhs._debugInfo, operatorName);
    }

    Watcher(const Watcher& other) : T(other)
    {
        _debugInfo.history = "<B><FONT COLOR=\"#800000\" POINT-SIZE=\"18\">COPY</FONT></B> of \"" + other._debugInfo.history + "\"";
        _debugInfo.originalName = _debugInfo.history;
        _debugInfo.address = this;
        _debugInfo.parentId = other._debugInfo.nodeId;
        Copies++;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreationCopy(_debugInfo);
    }

    Watcher& operator=(const Watcher& other)
    {
        T::operator=(other);
        _debugInfo.history = "<B><FONT COLOR=\"#800000\" POINT-SIZE=\"18\">COPY</FONT></B> of \"" + other._debugInfo.history + "\"";
        _debugInfo.parentId = other._debugInfo.nodeId;
        Copies++;
        if constexpr (Printable<T>)
        {
            _debugInfo.value.str("");
            _debugInfo.value << *this;
        }
        LOG.logAssigmentCopy(_debugInfo);
        return *this;
    }

    Watcher& operator+=(Watcher other)
    {
        T::operator+=(other);
        return *this;
    }

    ~Watcher()
    {
        LOG.logDeletion(_debugInfo);
    }
private:
    Logger::DebugInfo _debugInfo;

};

template<class T> 
Watcher<T> operator+(const Watcher<T>& lhs, const Watcher<T>& rhs)
{
    FUNC_INSPECT
    return Watcher<T>(static_cast<T>(lhs) + static_cast<T>(rhs), lhs, rhs, "+");
}

template<class T> 
Watcher<T> operator-(const Watcher<T>& lhs, const Watcher<T>& rhs)
{
    FUNC_INSPECT
    return Watcher<T>(static_cast<T>(lhs) - static_cast<T>(rhs), lhs, rhs, "-");
}
template<class T> 
Watcher<T> operator*(const Watcher<T>& lhs, const Watcher<T>& rhs)
{
    FUNC_INSPECT
    return Watcher<T>(static_cast<T>(lhs) * static_cast<T>(rhs), lhs, rhs, "*");
}
template<class T> 
Watcher<T> operator/(const Watcher<T>& lhs, const Watcher<T>& rhs)
{
    FUNC_INSPECT
    return Watcher<T>(static_cast<T>(lhs) / static_cast<T>(rhs), lhs, rhs, "/");
}
template<class T> 
Watcher<T> operator-(const Watcher<T>& lhs)
{
    FUNC_INSPECT
    return Watcher<T>(-static_cast<T>(lhs), lhs, "-@");
}

template<class T> 
bool operator==(const Watcher<T>& lhs, const Watcher<T>& rhs)
{
    FUNC_INSPECT
    return static_cast<T>(lhs) == static_cast<T>(rhs);
}

template<class T> 
bool operator<(const Watcher<T>& lhs, const Watcher<T>& rhs)
{
    FUNC_INSPECT
    return static_cast<T>(lhs) < static_cast<T>(rhs);
}

#endif /* WATCHER_HPP */
