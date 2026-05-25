# Performance Critical Code
When writing Performance Critical Code, we have to ignore absolute majority of the style guideline and C++ features to allow maximum performance, often using manual memory management.

In such code, comments become dominantly important and usual guidelines of writing self-describing code are relaxed if ambiguous code is crucial for achieving maximum performance.

Inlined, static, constexpr, consteval functions, fields and operations are preferred.

When writing performance critical code, you should prefer custom allocators or fixed‑size containers over STL containers. Avoid virtual classes, abstractions or frequent manual memory allocations unless necessary.

## Memory
### Memory allocation
In critical sections, `new`, `delete`, `std::make_unique` and other such allocation strategies should be avoided. Instead, reusing objects or allocating from a custom allocator(e.g. Arena) is recommended.

Following allocators are presented in Utility:
 * ArenaAllocator (fixed size) 
 * DynamicArenaAllocator (dynamic size) 
 * TypedArenaAllocator (fixed size, takes type and elements that can be stored in a single block) 
 * TypedDynamicArenaAllocator (dynamic size, takes type and elements that can be stored in a single block) 

One example of a case where using custom allocator is preferred is when you push trivial data types not knowing size of the resulting array, hence being unable to call `std::vector::reserve()`.
```c++
// Element is a trivially constructible and destructible structure

Dont:
std::vector<Element> m_dataContainer;

// Clearing a vector results in destruction of the whole range
m_dataContainer.clear();
int value;
while(GetValueForElement(&value)) {
    // push_back can resize array, resulting in memory allocation, deallocation and O(n) calls of copy/move constructors and destructors.  
    m_dataContainer.push_back(Element(value));
}

Do:
TypedDynamicArenaAllocator<Element, 16> m_arena;

// This doesn't call any destructors, only sets cursor offset to 0
m_arena.Reset();
int value;
while(GetValueForElement(&value)) {
    // In dynamic allocators, Allocate can add new memory block if it's out of space, which will not deallocate old blocks. Allocate() doesn't call any constructors or destructors by itself.
    Element* element = m_arena.Allocate();
    element->value = value;
}

// Later access using raw pointer access or iterators
Element* element = m_arena.Get(m_arena.Size() - 1);
```
Be aware: These allocators are designed for performance critical low level memory management and therefore don't provide any safety checks or iterators unlike STL containers do. Addresses are never invalidated, but they can point to garbage memory. Any interaction with such allocator after `Reset()` using old indices or pointers is an undefined behavior.