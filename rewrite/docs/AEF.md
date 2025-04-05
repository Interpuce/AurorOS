# AurorOS's Executable Format (AEF) 1.0 Specification

This version of AEF format has the following structure: 

```[header][standard_version][binary_size][data_address][text_address][entry_address][data]```

Where:

- `[header]` is the executable header, specific for AEF format ([read more](#specification-aef-header)),
- `[standard_version]` represents the version of AEF specification ([read more](#specification-aef-standard-version)),
- `[binary_size]` represents the executable size ([read more](#specification-binary-size)),
- `[data_address]`, `[text_address]` and `[entry_address]` are relative to file start section addresses ([read more](#specification-data-address)) 

## Specification: AEF Header

Five bytes specific for AurorOS's Executable Format to detect the file type. **Always** `AEF--`.

## Specification: AEF Standard Version

AEF loaders should accept any parsable 16-bit integer, that specifies the existing AEF version.  
For version 1.0 `[standard_version]` should be `1`.  
Any value that does not have standard assigned should throw error.

## Specification: Binary Size

`[binary_size]` is a **32-bit unsigned integer** representing the total file size in bytes.

AEF loaders **must** validate that the size of the file **exactly matches** this value.  
Any mismatch should result in a hard error (corrupted or malicious file).

## Specification: Data Address

`[data_address]` is a **32-bit unsigned integer** representing the **offset in bytes** in the file where the `.data` section begins.

The loader should seek to this offset and load the section into memory as initialized global/static data.

- If the value is `0`, it means there is **no** `.data` section.
- The loader must ensure the section stays within the binary size.

## Specification: Text Address

`[text_address]` is a **32-bit unsigned integer** representing the **offset in bytes** in the file where the `.text` section (machine code) begins.

The loader must:

- Seek to this offset in the file,
- Load the binary code into memory,
- Mark it as executable.

**This field must never be `0`. If it is, the loader must reject the file.**

## Specification: Entry Address

`[entry_address]` is a **32-bit unsigned integer** representing the **relative offset inside the `.text` section** where execution should begin.

For example, if `.text` starts at offset `0x30` in file and `entry_address` is `0x00`, execution begins at `0x30`.

- The value must be **within bounds** of the `.text` section.
- Invalid values cause the loader to reject the executable.

## Data Layout Summary

After the `[header]` and `[standard_version]`, the AEF format contains six 32-bit unsigned fields in little-endian order:

| Field           | Offset (from start) | Size (bytes) | Description                                              |
|-----------------|---------------------|--------------|----------------------------------------------------------|
| `binary_size`   | +0x05               | 4            | Total size of file (must match actual size)              |
| `data_address`  | +0x09               | 4            | Offset of `.data` section                                |
| `text_address`  | +0x11               | 4            | Offset of `.text` section                                |
| `entry_address` | +0x15               | 4            | Offset inside `.text` to start execution                 |

Then comes raw binary content at specified offsets.

## Notes for Implementers

- All numeric fields are **little-endian**.
- All addresses are **relative to file start**, except for `entry_address`.
- All addresses must not be negative.
- Sections must not overlap.
- Fields must be validated carefully to avoid corrupted/malicious files.
- Loader must fail gracefully and securely on any invalid AEF file.