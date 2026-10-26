#pragma once

// base::global::CRntFile: a file held in memory (0x38 bytes). The name comes from reflection: the
// unit tools/tu_map.py puts fn_710007bb30 in registers it (initialiser fn_710007e600; its class
// function "DumpBinary" is fn_710007bdd8). The unit's constructor, reserve, write and buffer
// functions are the ones the hash stream (include/base/UnkHashStream.h, an alias of this type) uses, the same
// object under another name. Only the members the hand-written functions touch are spelled out;
// names and layout are placeholders inferred from the code.
//
// The file is open once `data` is set: a block from the allocator at +0x0, of which the first
// `size` bytes are the contents. `position` is the read/write offset into it.

#include "base/UnkVector.h"
#include "types.h"

namespace base::global {

struct CRntFile {
    base::UnkAllocator* allocator;  // +0x0: null until the first allocation (then the default one)
    u64 position;                   // +0x8
    u32 alloc_flags;                // +0x10: the allocation flags (0x3d for the property bag's)
    u8* data;  // +0x18: null while no block is allocated
    u64 size;  // +0x20
    u8 pad_0x28[0x8];
    u8 alloc_align;  // +0x30: the alignment byte passed to the allocator
    bool swap;       // +0x31: reads swap the bytes of each element (2, 4 and 8 byte ones)
    u8 pad_0x32[6];
};
static_assert(sizeof(CRntFile) == 0x38, "CRntFile layout");

}  // namespace base::global

// Moves the position: `whence` 0 sets it to `offset`, 1 adds `offset` to the current position,
// 2 puts it on the last byte (`offset` is ignored). Returns false when the target is not inside
// the contents (the position goes to their end) and when no block is allocated or `whence` is
// another value (the position stays). Defined in src/base/global/CRntFile.cpp.
bool fn_710007c00c(base::global::CRntFile* file, u64 offset, s32 whence);

// Allocates the block (`size` bytes, the position 0), which Clang puts in the first call of the
// write below as well; false when no allocator can be had. A block already there is
// replaced without being freed (it leaks).
bool fn_710007bf9c(base::global::CRntFile* file, s64 size);
// Makes room for `count` elements of `size` bytes at the position, allocating the block when there
// is none (and growing it, copying the contents over, when they don't fit).
void fn_710007c514(base::global::CRntFile* file, s64 size, s64 count);
// Writes `size` bytes at the position (growing the file), moves the position on; returns `size`.
u64 fn_710007c3f0(base::global::CRntFile* file, const void* data, u64 size);
// Reads `count` elements of `size` bytes from the position into `dest`, or what is left of the
// contents when that is less, swapping each element's bytes when `swap` is set (sizes 2, 4, 8);
// moves the position on and returns the bytes read (0 at the end or when there is no block).
u64 fn_710007c714(base::global::CRntFile* file, void* dest, u64 size, u64 count);
