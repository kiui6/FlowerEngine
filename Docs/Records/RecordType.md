# RecordType

Each Record has its own unique 32-bit type ID ([ID32](../Types/ID32.md)).

Essential property of RecordType are their stability across builds. Future versions of the engine must never change already existing record types.

Record type can be retrieved statically(including at compile-time) using `Record::StaticType()` or at runtime using `Record::GetType()`.