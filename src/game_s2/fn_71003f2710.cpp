// CBigkranXSpitAttack: the update of state 2. Counts the timer at +0xdc down; when it has run
// out, or state 1's update (fn_71003f28c8) says so, switches to state 3. Otherwise a second timer
// (+0xe4) is counted down; when that is out too, the next entry of the pattern table of the
// attack's data (+0x148, entries of two floats and a word) is played: the owner's cached vector is
// worked out when it has none, fn_71003f18f8 gets a flag telling whether the entry's floats are
// both close to zero, a virtual call (slot 0xe8) hands over the attack's values, and the timer
// is reloaded from the entry's word before stepping to the next entry (wrapping at the count).
#include "types.h"

// Not trivially destructible, so returned in memory like the original's.
struct UnkVec3_71003f2710 {
    f32 x;
    f32 y;
    f32 z;
    ~UnkVec3_71003f2710() {}
};

struct UnkSpitEntry {
    f32 field_0x0;
    f32 field_0x4;
    u32 field_0x8;
};

struct UnkSpitData {
    u8 pad_0x0[0x148];
    UnkSpitEntry* field_0x148;
    u32 field_0x150;
};

struct UnkSpitOwner_71003f2710 {
    u8 pad_0x0[0x38];
    u8 field_0x38[0x40];
    UnkVec3_71003f2710 field_0x78;
    u8 pad_0x84[0xc];
    u32 field_0x90;
    u8 pad_0x94[0x1ac];
    void* field_0x240;
    u8 pad_0x248[0x88];
    void* field_0x2d0;
};

struct UnkSpitUpdate {
    void** vtable;
    UnkSpitOwner_71003f2710* field_0x8;
    u8 pad_0x10[0x18];
    UnkSpitData* field_0x28;
    u8 pad_0x30[0x2c];
    u8 field_0x5c[0x18];
    u8 field_0x74[0x20];
    u8 pad_0x94[0x40];
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
    u8 pad_0xe0[4];
    f32 field_0xe4;
    u32 field_0xe8;
    u8 pad_0xec[0xc];
    u8 field_0xf8[8];
};

bool fn_71003f28c8(UnkSpitUpdate* self, f32 delta);
void fn_71003f24f4(UnkSpitUpdate* self, u32 state);
UnkVec3_71003f2710 fn_7100079b18(void* object, f32 value);
void fn_71003f18f8(UnkSpitUpdate* self, bool a, bool b, const UnkVec3_71003f2710* v);
void fn_71009f92d0(void* object);

static inline UnkVec3_71003f2710 UnkCachedVec(UnkSpitOwner_71003f2710* owner) {
    if (!(owner->field_0x90 & 2)) {
        UnkVec3_71003f2710 r = fn_7100079b18(owner->field_0x38, owner->field_0x78.z);
        owner->field_0x78 = r;
        owner->field_0x90 |= 2;
        return r;
    }
    return owner->field_0x78;
}

// NON_MATCHING(minor): same behaviour; the original copies the cached vector with 32-bit pairs
// (ldp/stp) where ours uses a 64-bit and a 32-bit access, and loads the table index before the
// table pointer.
void fn_71003f2710(UnkSpitUpdate* self, f32 delta) {
    void* target = self->field_0x8->field_0x240;
    UnkSpitData* data = self->field_0x28;
    if (!target || !data)
        return;
    self->field_0xdc -= delta;
    if (self->field_0xdc < 0.001f || fn_71003f28c8(self, delta)) {
        fn_71003f24f4(self, 3);
        return;
    }
    if (self->field_0xe4 > 0.001f) {
        self->field_0xe4 -= delta;
        return;
    }
    UnkSpitEntry* entry = &self->field_0x28->field_0x148[self->field_0xe8];
    u32 next = entry->field_0x8;
    bool still = (__builtin_fabsf(entry->field_0x0) < 0.1f) & (__builtin_fabsf(entry->field_0x4) < 0.1f);
    UnkSpitOwner_71003f2710* owner = self->field_0x8;
    UnkVec3_71003f2710 v = UnkCachedVec(owner);
    fn_71003f18f8(self, still, still, &v);
    reinterpret_cast<void (*)(UnkSpitUpdate*, void*, void*, void*, f32, f32)>(self->vtable[0xe8 / 8])(
        self, self->field_0x5c, self->field_0x74, self->field_0xf8, self->field_0xd4, self->field_0xd8);
    fn_71009f92d0(self->field_0x8->field_0x2d0);
    *reinterpret_cast<u32*>(&self->field_0xe4) = next;
    u32 index = self->field_0xe8;
    self->field_0xe8 = index >= self->field_0x28->field_0x150 - 1 ? 0 : index + 1;
}
