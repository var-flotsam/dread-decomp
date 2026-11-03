// CSubAreaManager's unit (tools/tu_map.py unit 2577): pops the last entry of a ring of 64 16-byte entries
// (storage at +0x20, end pointer at +0x10, count at +0x18) and returns it: the entry's string id is moved
// out (the slot keeps the empty id), its three bytes at +0x8 are copied, then the slot's id is released.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkSubAreaExtra_b49a6 {
    u16 field_0x0;
    u8 field_0x2;
};

struct UnkSubAreaEntry_b49a6 {
    UnkStringData* id;
    UnkSubAreaExtra_b49a6 extra;  // 0x8

    ~UnkSubAreaEntry_b49a6();  // not reached here: the result is built in place

    // The id's two-step release (as base::global::CStrId::Reset).
    void ResetId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }

    UnkSubAreaEntry_b49a6(UnkSubAreaEntry_b49a6&& other) {
        id = other.id;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&other.id, &empty, sizeof(empty));
        __builtin_memcpy(&extra, &other.extra, 3);
    }
};

struct UnkSubAreaRing_b49a6 {
    u8 pad_0x0[0x10];
    UnkSubAreaEntry_b49a6* end;
    u64 count;
    UnkSubAreaEntry_b49a6 storage[64];

    UnkSubAreaEntry_b49a6& Last() { return end == storage ? storage[63] : end[-1]; }
    void PopBack() {
        UnkSubAreaEntry_b49a6* last = &Last();
        last->ResetId();
        end = last;
        --count;
    }
};

UnkSubAreaEntry_b49a6 fn_7100ef14b8(UnkSubAreaRing_b49a6* self) {
    UnkSubAreaEntry_b49a6 r(static_cast<UnkSubAreaEntry_b49a6&&>(self->Last()));
    self->PopBack();
    return r;
}
