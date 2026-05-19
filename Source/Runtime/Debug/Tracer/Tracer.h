#pragma once

#if defined(ENABLE_TRACING) && !defined(NDEBUG)

class Tracer {

public:
    Tracer& Get() {
        static Tracer tracer;
        return tracer;
    }

    void PushNamespace(std::string_view name) {}
    void PushScope(std::string_view name) {}
    void PopNamespace() {}
    void PopScope() {}
};

#   define PUSH_TRACE_NAMESPACE(Name) Tracer::Get().PushNamespace(Name)
#   define PUSH_TRACE_SCOPE(Name) Tracer::Get().PushScope(Name)
#   define POP_TRACE_NAMESPACE() Tracer::Get().PopNamespace()
#   define POP_TRACE_SCOPE() Tracer::Get().PopScope()

#else

#   define TRACE_PUSH_NAMESPACE(Name)
#   define TRACE_PUSH_SCOPE(Name)
#   define TRACE_POP_NAMESPACE() 
#   define TRACE_POP_SCOPE() 

#endif