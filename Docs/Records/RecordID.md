# RecordID

RecordID is an unique 64-bit identifier of a [Record](./Record.h).

Essential property of RecorIDs are their stability across file containers. At runtime, no overlap between IDs should ever happen.

## Anatomy
RecordID's most significant 16 bits represent index of a plugin in load order.
The rest of an ID is the unique ID.

```
0x0005|0000000000F3A1
     ^
  Plugin ID
```

`0x0000` is reserved for Master File. 

`0xFFFF` is reserved for Runtime records. 