#pragma once

// A byte stream that digests the values written to it (state checksums; the unit before
// CRntString's). The constructor, writer, buffer getter and destructor are fn_710007be8c,
// fn_710007c3f0, fn_710007c1a4 and fn_710007c0e4; the digest is fn_710007b750(data, buffer, -1).
// The same 0x38-byte object, built with the allocation flags 0x3d, sits in the property bag
// (src/game_s7/UnkPropBagS7.h). It is base::global::CRntFile (base/global/CRntFile.h; UnkHashStream_0x38
// is another name for it, and the digest's destination is its `data`). Names are placeholders.

#include "base/global/CRntFile.h"
#include "types.h"

// The stream is the in-memory file (its writer fn_710007c3f0 and the allocation fn_710007bf9c are
// declared in base/global/CRntFile.h).
using UnkHashStream_0x38 = base::global::CRntFile;

void fn_710007be8c(UnkHashStream_0x38* s, u32 kind, void* arg);  // constructor
u64 fn_710007c1a4(UnkHashStream_0x38* s);  // the buffer written so far
void fn_710007c0e4(UnkHashStream_0x38* s);  // destructor
u32 fn_710007b750(void* dest, u64 buffer, s32 count);  // the digest of the buffer

struct UnkScopedHashStream : UnkHashStream_0x38 {
    UnkScopedHashStream(s32 kind, void* arg) { fn_710007be8c(this, kind, arg); }
    ~UnkScopedHashStream() { fn_710007c0e4(this); }

    void Begin(s32 size) { fn_710007bf9c(this, size); }  // the block's initial size
    void PutBytes(const void* data, u32 size) { fn_710007c3f0(this, data, size); }

    // Writes a copy of the value, so each call owns its own temporary.
    template <class T>
    void Put(T v) {
        fn_710007c3f0(this, &v, sizeof(T));
    }

    s32 Finish() { return fn_710007b750(data, fn_710007c1a4(this), -1); }
};
