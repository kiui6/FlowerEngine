# Performance Critical Code
When writing Performance Critical Code, we have to ignore absolute majority of the style guideline and C++ features to allow maximum performance, often using manual memory management.

In such code, comments become dominantly important and usual guidelines of writing self-describing code are loosed if ambiguous code is crucial for achieving maximum performance.

Inlined, static, constexpr, consteval functions, fields and operations are prefered.