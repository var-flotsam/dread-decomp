// A small arena with its storage inline (N bytes at +8, then the allocation cursor and a nesting
// count), one instantiation per size and unit (unit 25 of tools/tu_map.py, base::reflection::CType's,
// CRumbleManager's and CEmmyGrabSamusAttack's): whether a pointer lies in the storage, and the
// release that, when the count drops to zero, rewinds the cursor to the storage start if the
// pointer lies below it. Names and layouts are placeholders.
#include "types.h"

template <u32 N>
struct UnkInlineArena_b42a1 {
    u64 field_0x0;
    u8 storage[N];
    u8* cursor;
    u32 depth;

    __attribute__((always_inline)) bool Owns(const u8* p) const { return storage <= p && p < storage + N; }

    __attribute__((always_inline)) void Release(u8* p) {
        u32 d = depth;
        if (d == 1 && storage <= p && p < cursor)
            cursor = storage;
        depth = d - 1;
    }
};

bool fn_710001f3b0(const UnkInlineArena_b42a1<0x100>* self, const u8* p) { return self->Owns(p); }
bool fn_71000758b0(const UnkInlineArena_b42a1<0x200>* self, const u8* p) { return self->Owns(p); }
bool fn_710009f5f4(const UnkInlineArena_b42a1<0x20>* self, const u8* p) { return self->Owns(p); }
bool fn_710009fa90(const UnkInlineArena_b42a1<0x40>* self, const u8* p) { return self->Owns(p); }
bool fn_710009ff2c(const UnkInlineArena_b42a1<0x80>* self, const u8* p) { return self->Owns(p); }
bool fn_7100510e10(const UnkInlineArena_b42a1<0x8>* self, const u8* p) { return self->Owns(p); }

void fn_710009f5b8(UnkInlineArena_b42a1<0x20>* self, u8* p) { self->Release(p); }
void fn_710009fa54(UnkInlineArena_b42a1<0x40>* self, u8* p) { self->Release(p); }
void fn_710009fef0(UnkInlineArena_b42a1<0x80>* self, u8* p) { self->Release(p); }
