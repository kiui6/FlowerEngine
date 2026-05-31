#pragma once

#define ENABLE_TRACING

#if defined(ENABLE_TRACING)

#include <vector>
#include <chrono>
#include <deque>

#include <Delegate/Delegate.h>

class Tracer {
    struct TracerScope
    {
        std::string name;
        std::vector<TracerScope> scopes;
        uint64_t start = 0;
        uint64_t end = 0;
    };
    
    TracerScope m_mainScope;
    std::deque<TracerScope*> m_scopeChain;

    Tracer() {
        m_mainScope.name = "Frame";
        m_mainScope.start = std::chrono::steady_clock::now().time_since_epoch().count();
        m_scopeChain.push_back(&m_mainScope);
    }
public:
    static Tracer& Get() {
        static Tracer tracer;
        return tracer;
    }

    inline void PushScope(std::string_view name) {
        TracerScope* hostScope = m_scopeChain.back();
        TracerScope& newScope = hostScope->scopes.emplace_back(TracerScope{std::string(name), {}, 0, 0});
        m_scopeChain.push_back(&newScope);

        newScope.start = std::chrono::steady_clock::now().time_since_epoch().count();
    }

    inline void PopScope() {
        uint64_t nsNow = std::chrono::steady_clock::now().time_since_epoch().count();

        TracerScope* hostScope = m_scopeChain.back();
        hostScope->end = nsNow;
        m_scopeChain.pop_back();
    }

    inline void PopScope(size_t count) {
        for(size_t i = 0; i < count; ++i) {
            PopScope();
        }
    }

    void FlushFrame() {
        m_mainScope.end = std::chrono::steady_clock::now().time_since_epoch().count();

        // TODO: Implement flush to third party consumer. Delegates may be used to allow interfacing via DLL

        m_mainScope.scopes.clear();
        m_scopeChain.clear();
        m_scopeChain.push_back(&m_mainScope);

        m_mainScope.start = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

#   define PUSH_TRACE_SCOPE(Name) Tracer::Get().PushScope(Name)
#   define POP_TRACE_SCOPE(...) Tracer::Get().PopScope( __VA_ARGS__ )
#   define FLUSH_TRACE_FRAME() Tracer::Get().FlushFrame()

#else

#   define PUSH_TRACE_SCOPE(Name)
#   define POP_TRACE_SCOPE() 
#   define FLUSH_TRACE_FRAME()

#endif