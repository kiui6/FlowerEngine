import std.io;
import std.mem;

struct PluginHeader
{
    u32 magic;
    u32 version;
    u64 pluginID;

    u64 recordsLutCount;
    u64 recordsLutOffset;

    u64 recordsBlobSize;
    u64 recordsBlobOffset;
    
    u64 dependenciesOffset;
    u16 dependencyCount;

    u16 _padding;

    u32 checksum;
};

enum FieldType : u32 {
    Integer = 0,
    Unsigned = 1,
    Float = 2,
    Boolean = 3,
    String = 4,
    List = 5,
    Map = 6
};

fn fieldDataSize(FieldType type) {
    if (type == FieldType::Integer) return 4;
    if (type == FieldType::Unsigned) return 4;
    if (type == FieldType::Float) return 8;
    if (type == FieldType::Boolean) return 1;
    if (type == FieldType::String) {
        u32 stringSize @ $-4;
        return stringSize;
    }
    std::warning("Unknown field type, assuming size 0");
    return 0;
};

struct RecordField
{
    u32 id;
    FieldType type;
    u8 stringSize[type == FieldType::String ? 4 : 0];
    u8 data[fieldDataSize(type)];
};

struct RecordLUTEntry
{
    u64 id;
    u64 offset;
    u32 type;
    u16 flags;
    u16 fieldsCount;
    
    RecordField fields[fieldsCount] @ header.recordsBlobOffset + offset;
};

PluginHeader header @ 0;
RecordLUTEntry lutEntries[header.recordsLutCount] @ header.recordsLutOffset;

fn main() {
    if (header.magic != 0x47554C50) {
        std::warning("Invalid magic number");
    } 
};