// Adds or updates an entry (key, two ids, two 4-float values, a weight) in a fixed table of 50,
// reusing the last entry with the same key and ids, then pushes the first value to the target slot.
#include "types.h"
#include "base/global/CStrId.h"

using base::global::CStrId;

struct UnkVec4 {
    f32 x, y, z, w;
    UnkVec4& operator=(const UnkVec4& other);  // fn_710007b518
};

struct UnkVec4Pod {
    f32 x, y, z, w;
};

struct UnkKey {
    u64 field_0x0;
    CStrId field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

bool fn_710080f9e0(const UnkKey* a, const UnkKey* b);
void fn_710080fb00(UnkKey* key, void* owner);

struct UnkSlotTarget {
    u8 pad_0x0[0x30];
    UnkVec4Pod* field_0x30;
};

struct UnkSlots {
    u8 pad_0x0[0x14c];
    UnkVec4 field_0x14c[10];
    UnkSlotTarget* field_0x1f0[10];
    UnkVec4 field_0x240[10];
    UnkSlotTarget* field_0x2e0[10];
};

UnkSlots* fn_710080fa28(UnkKey* key);

struct UnkEntry {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    UnkVec4 field_0xc;
    UnkVec4 field_0x1c;
    u8 pad_0x2c[0x4];
    UnkKey field_0x30;
    u32 field_0x48;
    f32 field_0x4c;
};

struct UnkTable {
    u8 pad_0x0[0x48];
    u32 count;
    u8 pad_0x4c[0x4];
    UnkEntry* entries[50];
};

// NON_MATCHING(minor): register allocation; the original spills `value` around the search loop and
// keeps `a` in a register (two extra moves), this build does the opposite.
void fn_7100926874(UnkTable* self, const UnkKey& key, u32 id, const UnkVec4& a, const UnkVec4& b, u32 kind,
                   u32 value, f32 weight) {
    s32 idx = -1;
    for (u64 i = 0; i < self->count; ++i) {
        UnkEntry* e = self->entries[i];
        if (fn_710080f9e0(&e->field_0x30, &key) && e->field_0x4 == id && e->field_0x8 == kind)
            idx = i;
    }
    if (idx == -1) {
        if (self->count >= 50)
            return;
        idx = self->count++;
    } else if ((u32)idx >= 50) {
        return;
    }
    UnkEntry* e = self->entries[(u32)idx];
    e->field_0x0 = value;
    e->field_0x30 = key;
    fn_710080fb00(&e->field_0x30, self);
    e->field_0x4 = id;
    e->field_0x8 = kind;
    e->field_0xc = a;
    e->field_0x1c = b;
    e->field_0x4c = weight;
    e->field_0x48 = 0;
    switch (e->field_0x8) {
    case 0: {
        UnkSlots* s = fn_710080fa28(&e->field_0x30);
        u32 n = e->field_0x4;
        s->field_0x240[n] = e->field_0xc;
        if (UnkSlotTarget* t = s->field_0x2e0[n])
            __builtin_memcpy(t->field_0x30, &e->field_0xc, sizeof(UnkVec4Pod));
        break;
    }
    case 1: {
        UnkSlots* s = fn_710080fa28(&e->field_0x30);
        u32 n = e->field_0x4;
        s->field_0x14c[n] = e->field_0xc;
        if (UnkSlotTarget* t = s->field_0x1f0[n])
            __builtin_memcpy(t->field_0x30, &e->field_0xc, sizeof(UnkVec4Pod));
        break;
    }
    }
}
