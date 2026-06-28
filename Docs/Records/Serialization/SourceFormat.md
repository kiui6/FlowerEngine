# Record Source Format
Records are contained in `.master`, `.plugin` and `.save` files, united by the format.

### Glossary
| Term     | Meaning                                                                              |
|----------|--------------------------------------------------------------------------------------|
| Source   | A file containing records                                                            |
| POID     | 16 most significant bits of a [RecordID](../RecordID.md), signifying Plugin Order ID |

## Format
### Header
```c++
struct SerialHeader
{
    u32 magic = 0xEDEDEDED;
    u32 version = CURRENT_PLUGIN_FILE_VERSION;
    u64 sourceID = 0;

    u64 recordsLutCount = 0;
    u64 recordsLutOffset = 0;

    u64 recordsBlobSize = 0;
    u64 recordsBlobOffset = 0;

    u64 dependenciesOffset = 0;
    u16 dependencyCount = 0;

    u16 infoSectionOffset = 0;

    u32 checksum = 0;
};
```
<sub>*Diag 1. Structure of a source file header*</sub>

### Information Section (InfS)
Information section is located in the beginning of the file and serves exclusively metadata purpose.
InfS contains multiple text blocks, such as authors, copyright, description, name, logo.
```c++
struct InformationSectionBlock {
    u32 keySize;
    u32 contentSize;
    u32 next;

    char key[keySize];
    char content[contentSize];
};
```
<sub>*Diag 2. Pseudo code of an information block*</sub>

### Plugin Dependency List (PDL)
```c++
struct SerialDependency
{
    u64 pluginOrderID;
    u64 sourceID;
};
```
<sub>*Diag 3. Structure of a PDL entry*</sub>

### Record Look-Up Table (RLUT)
```c++
struct SerialLUTEntry
{
    u64 id;
    u64 offset;
    u32 type;
    u16 flags;
    u16 fieldsCount;
};
```
<sub>*Diag 4. Structure of an RLUT entry*</sub>

### Record Field Blob (ReFB)

## Parsing rules
### Base Detection Rules
Record is considered base if `POID` is equal to 0x0000

### Delta Detection Rules
Record is considered delta if `POID` is *not* equal to 0x0000 