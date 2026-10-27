#pragma once

// Reading a reflected object from a file: fn_71000f0568 sets up a read target for the object's
// value and fn_710000f850 (or fn_710000f750, from a root node) parses the file into it through the
// caller's callbacks. Used by src/base/UnkReflObjectRead.cpp, the skip helper of
// src/base/animation/CBlendSpaceResource.cpp and the file loaders under src/game/ (for example
// src/game/UnkObjectFileRead.cpp). Names and layouts are placeholders.

#include "base/reflection/UnkReflValue.h"
#include "types.h"

namespace base::reflection {
class CType;
}

// What a reflected value is read into: up to 100 type-erased entries of 0xd8 bytes after a count,
// each destroyed through the function at +0x40 of its table.
struct UnkReadEntryOps {
    void* fns[8];
    void (*destroy)(void* storage);
};
struct UnkReadNode {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkReadEntry {
    UnkReadNode* field_0x0;  // its +0x10 is what the parser starts from
    u8 storage[0xc8];
    UnkReadEntryOps* ops;
};
static_assert(sizeof(UnkReadEntry) == 0xd8, "UnkReadEntry placeholder layout");

struct UnkReadTarget {
    u32 field_0x0;
    u32 count;
    UnkReadEntry entries[100];
    u8 field_0x5468;  // cleared after the set-up by fn_7100010c60
    u8 pad_0x5469[7];
};
static_assert(sizeof(UnkReadTarget) == 0x5470, "UnkReadTarget placeholder layout");

// Sets up `self` to read the object `value` describes, in `mode` (5 for most loaders, 0xc for the
// GUI's). Not decompiled yet.
void fn_71000f0568(UnkReadTarget* self, const base::reflection::UnkReflValue* value, u32 mode);

// The file being read (each reader defines its fields: a CRntFile and two status words).
struct UnkFileReader;

// The parser's callbacks with the file and the target.
struct UnkParseCallbacks {
    u64 words[6];
};
struct UnkParseContext {
    UnkParseCallbacks callbacks;
    UnkFileReader* reader;
    UnkReadTarget* target;

    UnkParseContext(const UnkParseCallbacks& callbacks_, UnkFileReader* reader_, UnkReadTarget* target_) {
        for (u32 i = 0; i < 6; ++i)
            callbacks.words[i] = callbacks_.words[i];
        reader = reader_;
        target = target_;
    }
    // The other context's callbacks and file with another target.
    UnkParseContext(const UnkParseContext& other, UnkReadTarget* target_) {
        for (u32 i = 0; i < 6; ++i)
            callbacks.words[i] = other.callbacks.words[i];
        reader = other.reader;
        target = target_;
    }
};

// Parses a value of `type` with the context's callbacks into its target; nonzero on success (a
// 32-bit result: its caller in src/base/UnkReflObjectRead.cpp compares the whole word). Not
// decompiled yet.
s32 fn_710000f850(UnkParseContext* self, const base::reflection::CType* type, void* x2);
