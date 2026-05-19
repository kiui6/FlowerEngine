# Record
Records are containers of persistant data for engine's data driven design.
Each Record has unique [RecordID](./RecordID.md) and might have flags attached to it


## Lifecycle

### Deletion
Records can be deleted either by the plugin designer through the editor, save file or at runtime.

Deleted records are not deleted right away to avoid breaking existing [RecordPtr](./RecordPtr.md) references, and instead marked with a [RecordFlags::Destroyed](./RecordFlags.md) flag.

Record Library also contains a set of RecordIDs of deleted records. Once all RecordPtr references are clear and record is eligible for garbage collection, sets keep data of deleted records.

Note: Records with [pluginID](./RecordID.md) `0xFFFF`(Runtime) are deleted completely at GC pass and are not stored in the deleted objects set, since such records can't be referenced by any higher load order plugin or save file, so we don't need to keep their state in the database.

### Garbage Collection
All Records leave RecordLibrary in RecordPtr object, that allows reference counting for Records. Once all RecordPtrs referencing specific Record are destroyed, Record becomes dangling and therefore eligible for garbage collection.

Once marked, Record will be tested by the GarbageCollection at the GC Pass to confirm it wasn't ressurected or marked dirty, and if it's still eligible it will be unloaded from the RAM completely.

Note: Records that are marked [RecordFlags::Dirty](./RecordFlags.md) will not be unloaded from RAM in the Editor. At runtime dirty records might be unloaded to the temporary save container to avoid RAM wasting.