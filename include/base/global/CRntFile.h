#pragma once

// base::global::CRntFile: a file held in memory (0x38 bytes). The name comes from reflection: the
// unit tools/tu_map.py puts fn_710007bb30 in registers it (initialiser fn_710007e600; its class
// function "DumpBinary" is fn_710007bdd8). The unit's constructor, reserve, write and buffer
// functions are the ones include/base/UnkHashStream.h spells out for the hash stream, the same
// object under another name. Only the members the hand-written functions touch are spelled out;
// names and layout are placeholders inferred from the code.
//
// The file is open once `data` is set: a block from the allocator at +0x0, of which the first
// `size` bytes are the contents. `position` is the read/write offset into it.

#include "types.h"

namespace base::global {

struct CRntFile {
    u8 pad_0x0[0x8];
    u64 position;
    u8 pad_0x10[0x8];
    u8* data;  // +0x18: null while no block is allocated
    u64 size;  // +0x20
    u8 pad_0x28[0x10];
};
static_assert(sizeof(CRntFile) == 0x38, "CRntFile layout");

}  // namespace base::global

// Moves the position: `whence` 0 sets it to `offset`, 1 adds `offset` to the current position,
// 2 puts it on the last byte (`offset` is ignored). Returns false when the target is not inside
// the contents (the position goes to their end) and when no block is allocated or `whence` is
// another value (the position stays). Defined in src/base/global/CRntFile.cpp.
bool fn_710007c00c(base::global::CRntFile* file, u64 offset, s32 whence);
