# Per Frane Code Guideline
Per Frame code should focus on minimal overhead, but don't ignore readability and recognizable features like STL containers and data types.

## Return types
For per-frame code it's highly adviced to minimize constructor/destructor calls and maximize reuse of objects.

### std::optional
Instead of `std::optional` using of `bool` returning function with a `&result` argument is recommended. This saves construction and destruction and allows code reuse and/or more efficient memory management. For consistency, at success such function shall return `true`, and `false` at failure.

```c++
bool FetchRecordMemory(RecordID id, RecordMemory& result) {
    //...
    result = someData;
    return true;
}

// In caller
RecordMemory memory;
if(!FetchRecordMemory(1, memory)) {
    // Handle error
}
```