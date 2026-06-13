#pragma once
#include <type_traits>

template <class... T> struct AlwaysFalse : std::false_type {};