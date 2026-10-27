// An unidentified polymorphic object (vtable unk_7101a1d2c0, destroyed by fn_710014c790) that
// is registered under a string id and holds two inline fixed-capacity arrays of records:
// its constructor, and the resize of the first array (16 records of 0x58 bytes, each starting
// with a string id).
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

extern void* unk_7101a1d2c0[];  // vtable (reached through the GOT)

void fn_710014d9e8(void* block, u32 arg);

namespace {

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id defaulting to the pool's empty string; released in two steps, like
// base::UnkStrIdMember.
struct UnkEntryId {
    UnkStringData* data;

    __attribute__((always_inline)) UnkEntryId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkEntryId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A record of the first array: a string id and zero-initialised fields; +0x4c is left as it is.
struct UnkEntry_0x58 {
    UnkEntryId id;
    u32 field_0x8[17];
    u32 field_0x4c;
    u64 field_0x50;

    __attribute__((always_inline)) UnkEntry_0x58() : field_0x50(0) {
        for (u32& v : field_0x8)
            v = 0;
    }
};

static_assert(sizeof(UnkEntry_0x58) == 0x58, "UnkEntry_0x58");

// The first array: a count followed by the records (the object keeps it at +0x144, so the
// records are 8-byte aligned).
struct UnkEntryBlock {
    u32 count;
    u8 storage[16 * sizeof(UnkEntry_0x58)];

    UnkEntryBlock() : count(0) {}

    UnkEntry_0x58* Data() { return reinterpret_cast<UnkEntry_0x58*>(storage); }

    void Resize(u32 n) {
        if (count < n) {
            for (u32 i = count; i < n; ++i)
                new (&Data()[i]) UnkEntry_0x58;
        } else if (count != n) {
            for (u64 i = n; i < count; ++i)
                Data()[i].~UnkEntry_0x58();
        }
        count = n;
    }
};

}  // namespace

// The object. The counts at +0x40 and +0x6c8 are followed by 0x100 bytes the constructor leaves
// as they are, and the second array (count at +0x7cc, resized by fn_710014d838) by 0x400.
struct UnkRegisteredEntryObject {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    UnkEntryId id;
    void* handle;
    u32 field_0x40;
    u8 pad_0x44[0x100];
    UnkEntryBlock field_0x144;
    u32 field_0x6c8;
    u8 pad_0x6cc[0x100];
    u32 field_0x7cc;
    u8 pad_0x7d0[0x400];
    u64 field_0xbd0;
    u64 field_0xbd8;
    f32 field_0xbe0;
    f32 field_0xbe4;
    u64 field_0xbe8;
    u64 field_0xbf0;
    UnkGlobal_710b372a98* field_0xbf8;
    u32 field_0xc00;
    f32 field_0xc04;
    u32 field_0xc08;
    bool field_0xc0c;
    bool field_0xc0d;

    __attribute__((always_inline)) UnkRegisteredEntryObject();
};

static_assert(sizeof(UnkRegisteredEntryObject) == 0xc10, "UnkRegisteredEntryObject");

inline UnkRegisteredEntryObject::UnkRegisteredEntryObject()
    : vtable(reinterpret_cast<void**>(&unk_7101a1d2c0[2])), field_0x28(1), handle(nullptr), field_0x40(0),
      field_0x6c8(0), field_0x7cc(0), field_0xbd0(0), field_0xbd8(0), field_0xbe0(0.0f), field_0xbe4(1.0f),
      field_0xbe8(0), field_0xbf0(0), field_0xc00(1), field_0xc04(1.0f), field_0xc08(0), field_0xc0c(true),
      field_0xc0d(true) {
    field_0x40 = 0;
    fn_710014d9e8(&field_0x144, 0);
    field_0x6c8 = 0;
    field_0x8 = 0;
    field_0x10 = 0;
    field_0x18 = 0;
    field_0x20 = 0;
    field_0xbf8 = g_unk_710b372a98;
}

void fn_710014c660(UnkRegisteredEntryObject* self) {
    new (self) UnkRegisteredEntryObject;
}

// NON_MATCHING(minor): register allocation only: the grow loop's record pointer and remaining
// count swap x26 and x27.
void fn_710014d9e8(void* block, u32 arg) {
    static_cast<UnkEntryBlock*>(block)->Resize(arg);
}
