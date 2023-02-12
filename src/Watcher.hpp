#ifndef WATCHER_HPP
#define WATCHER_HPP
#include "Logger.hpp"
#include <string>
#include <utility>
#include "MySTL.hpp"
#include <type_traits>

template<class T>
class Watcher : public T
{
public:
    Watcher (const char* name = nullptr) : T()
    {
        if(name == nullptr) 
        {
            _debugInfo.originalName = "TMP#";
            _debugInfo.originalName += std::to_string(_tmpId++);
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
    explicit Watcher(U&& u, const char* name = nullptr) : //FIXME: Not only single element constructor
        T(my::forward<U>(u))
    {
        if(name == nullptr) 
        {
            _debugInfo.originalName = "TMP#";
            _debugInfo.originalName += std::to_string(_tmpId++);
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
            _debugInfo.originalName += std::to_string(_tmpId++);
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
        _debugInfo.originalName += std::to_string(_tmpId++);
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
        _debugInfo.originalName += std::to_string(_tmpId++);
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
        _debugInfo.history = "Copy of \\\"" + other._debugInfo.history + "\\\"";
        _debugInfo.originalName = _debugInfo.history;
        _debugInfo.address = this;
        _debugInfo.parentId = other._debugInfo.nodeId;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreationCopy(_debugInfo);
    }

    Watcher& operator=(const Watcher& other)
    {
        T::operator=(other);
        _debugInfo.history = "Copy of \\\"" + other._debugInfo.history + "\\\"";
        _debugInfo.parentId = other._debugInfo.nodeId;
        if constexpr (Printable<T>)
        {
            _debugInfo.value.str("");
            _debugInfo.value << *this;
        }
        LOG.logAssigmentCopy(_debugInfo);
        return *this;
    }

    Watcher(Watcher<T>&& other) : T(my::move(other))
    {
        static_assert(std::is_rvalue_reference_v<decltype(other)>);
        _debugInfo.history = "Move of \\\"" + other._debugInfo.history + "\\\"";
        _debugInfo.originalName = _debugInfo.history;
        _debugInfo.address = this;
        _debugInfo.parentId = other._debugInfo.nodeId;
        if constexpr (Printable<T>)
        {
            _debugInfo.value << *this;
        }
        LOG.logCreationMove(_debugInfo);
    }

    Watcher& operator=(Watcher&& other)
    {
        T::operator=(my::move(other));
        _debugInfo.history = "Move of \\\"" + other._debugInfo.history + "\\\"";
        _debugInfo.parentId = other._debugInfo.nodeId;
        if constexpr (Printable<T>)
        {
            _debugInfo.value.str("");
            _debugInfo.value << *this;
        }
        LOG.logAssigmentMove(_debugInfo);
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
    static uint64_t _tmpId;
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

template<class T>
uint64_t Watcher<T>::_tmpId = 0;
#endif /* WATCHER_HPP */
