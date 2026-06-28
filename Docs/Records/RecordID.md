# RecordID

RecordID is an unique 64-bit identifier of a [Record](./Record.h).

An essential property of RecorIDs is their stability across file containers. At runtime, no overlap between IDs should ever happen.

## Anatomy
RecordID's most significant 16 bits represent index of a plugin in load order.
The rest of an ID is the unique 48-bit identifier, generated sequentially and atomically by the [RecordLibrary](./RecordLibrary.md) at runtime.

```
0x0005|0000000000F3A1
     ^
Plugin Order ID
```

`0x0000` is reserved for Master File. 

`0xFFFF` is reserved for Runtime records. It is a temporary placeholder and is never saved to disk. During serialization, it is replaced with the actual plugin order ID.

## Serialization
In `Editor` or at `Runtime`, creation of new records sets their PluginOrderID to `0xFFFF`(Runtime records).
Later, at serialization, `0xFFFF` records become `0x0000` records in the file.

### Relative Plugin ID
Plugin files contain a map of relative plugin order ID, that corresponds to a unique ID of a plugin: 
|Relative Plugin Order ID|  Unique Plugin ID  |
|------------------------|--------------------|
|         0x0001         | 0x9FE17DEE273CAA01 |

When reading, resolving a record ID relative to a plugin file consists of 3 steps:

`Owning Plugin` - a plugin that owns the original base state of a record we're searching for.
`Modifying Plugin` - a plugin that has `Owning Plugin` as a master and modifies the original base state of a record we're searching for with delta.
`Plugin Relative Record ID` - a Record ID with `Plugin Order ID` relative to the querried plugin

1. Get `Unique Plugin ID` of an `Owning Plugin`.
2. Fetch `Relative Plugin Order ID` of an `Owning Plugin` in a `Modifying Plugin`.
3. Get Record data using resolved `Plugin Relative Record ID`