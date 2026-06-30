# Record Source Format (Base Field Types)

## String
```c
struct FieldString {
    u64 stringSize;
    u64 stringOffset;
};
```

## Map

```c

struct FieldMap {
    u32 fieldID;
    u32 fieldType;
    u32 size;
    u32 keyType;
    u32 valueType;

    struct {
        u8 keyData[sizeof(keyType)];
        u8 valueData[sizeof(valueType)];
    } pairs[size];
};
```