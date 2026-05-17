#pragma once

#include "DelegateInstance.h"
#include <memory>
#include <vector>
#include <mutex>

template <typename RetValType, typename... ArgTypes>
class Delegate
{
public:
    using FuncType = RetValType(ArgTypes...);
    
    template <typename UserClass>
    using MemFuncType = typename MemFuncPtr<false,
            UserClass,
            FuncType
        >::Type;
protected:
    std::shared_ptr<DelegateInstance<FuncType>>instance;

    std::mutex mtx;
public:

    template <typename UserClass>
    inline static Delegate<RetValType, ArgTypes...>
    CreateMember(
        UserClass* InUserObject,
        typename MemFuncPtr<false,
            UserClass,
            FuncType
        >::Type InFunc)
    {
        Delegate<RetValType, ArgTypes...> Result;
        Result.instance.reset(new MemDelegateInstance<false, UserClass, RetValType, ArgTypes...>(InUserObject, InFunc));
        return Result;
    }

    // Create Const Member function
    template <typename UserClass>
    inline static Delegate<RetValType, ArgTypes...>
    CreateMember(
        UserClass* InUserObject,
        typename MemFuncPtr<true,
            UserClass,
            FuncType
        >::Type InFunc)
    {
        Delegate<RetValType, ArgTypes...> Result;
        Result.instance.reset(new MemDelegateInstance<true, UserClass, RetValType, ArgTypes...>(InUserObject, InFunc));
        return Result;
    }

    inline static Delegate<RetValType, ArgTypes...>
    CreateRaw(typename RawFuncPtr<FuncType>::Type InFunc)
    {
        Delegate<RetValType, ArgTypes...> Result;
        Result.instance.reset(new RawDelegateInstance<RetValType, ArgTypes...>(InFunc));
        return Result;
    }

    inline RetValType Execute(ArgTypes... Params)
    {
        std::lock_guard lock(mtx);
        return instance->Execute(Params...);
    }
};

template <typename... ArgTypes>
class MulticastDelegate {
public:
    using FuncType = void(ArgTypes...);

    template <typename UserClass>
    using MemFuncType = typename MemFuncPtr<false,
            UserClass,
            FuncType
        >::Type;
private:
    std::vector<std::shared_ptr<DelegateInstance<FuncType>>> delegates;
    
    std::mutex mtx;
public:

    inline void BindRaw(typename RawFuncPtr<FuncType>::Type InFunc)
    {
        std::lock_guard lock(mtx);
        delegates.push_back(std::make_shared<RawDelegateInstance<void, ArgTypes...>>(InFunc));
    }

    template <typename UserClass>
    inline void BindMember(UserClass* InUserObject,
        typename MemFuncPtr<false,
            UserClass,
            FuncType
        >::Type InFunc)
    {
        std::lock_guard lock(mtx);
        delegates.push_back(std::make_shared<MemDelegateInstance<false, UserClass, void, ArgTypes...>>(InUserObject, InFunc));
    }

    inline void Broadcast(ArgTypes... Params)
    {
        std::lock_guard lock(mtx);
        for(auto delegate : delegates)
        { 
            delegate->Execute(Params...);
        }
    }
};