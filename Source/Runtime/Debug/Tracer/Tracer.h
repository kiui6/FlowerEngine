#pragma once

#define ENABLE_TRACING

#if defined(ENABLE_TRACING)

#include <vector>
#include <chrono>
#include <deque>

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
        m_mainScope.name = "Application";
        m_mainScope.start = std::chrono::steady_clock::now().time_since_epoch().count();
        m_scopeChain.push_back(&m_mainScope);
    }
public:
    static Tracer& Get() {
        static Tracer tracer;
        return tracer;
    }

    void PushScope(std::string_view name) {
        TracerScope* hostScope = m_scopeChain.back();
        TracerScope& newScope = hostScope->scopes.emplace_back(TracerScope{std::string(name), {}, 0, 0});
        m_scopeChain.push_back(&newScope);

        newScope.start = std::chrono::steady_clock::now().time_since_epoch().count();
    }

    void PopScope() {
        uint64_t nsNow = std::chrono::steady_clock::now().time_since_epoch().count();

        TracerScope* hostScope = m_scopeChain.back();
        hostScope->end = nsNow;
        m_scopeChain.pop_back();
    }
};

#   define PUSH_TRACE_SCOPE(Name) Tracer::Get().PushScope(Name)
#   define POP_TRACE_SCOPE() Tracer::Get().PopScope()

#else

#   define TRACE_PUSH_SCOPE(Name)
#   define TRACE_POP_SCOPE() 

#endif