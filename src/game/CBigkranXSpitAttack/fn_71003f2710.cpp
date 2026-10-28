// CBigkranXSpitAttack: the update of state 2. Counts the timer at +0xdc down; when it has run
// out, or state 1's update (fn_71003f28c8) says so, switches to state 3. Otherwise a second timer
// (+0xe4) is counted down; when that is out too, the next entry of the pattern table of the
// attack's data (+0x148, entries of two floats and a word) is played: the owner's cached vector is
// worked out when it has none, fn_71003f18f8 gets a flag telling whether the entry's floats are
// both close to zero, a virtual call (slot 0xe8) hands over the attack's values, and the timer
// is reloaded from the entry's word before stepping to the next entry (wrapping at the count).
#include "game/UnkXformNode.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

// The owner actor (UnkActor) with the members this update reads past what UnkActor spells out.
struct UnkSpitOwner_71003f2710 {
    u8 pad_0x0[0x38];
    UnkXform field_0x38;
    u8 pad_0x94[0x1ac];
    void* field_0x240;
    u8 pad_0x248[0x88];
    void* field_0x2d0;
};

void fn_71003f18f8(UnkSpitAttack* self, bool a, bool b, const UnkVec3f* v);
void fn_71009f92d0(void* object);

static inline UnkSpitOwner_71003f2710* Owner(UnkSpitAttack* self) {
    return reinterpret_cast<UnkSpitOwner_71003f2710*>(self->field_0x8);
}

static inline UnkVec3f UnkCachedVec(UnkSpitOwner_71003f2710* owner) {
    return owner->field_0x38.GetAngle();
}
// NON_MATCHING(minor): same behaviour; the original loads the table index before the table
// pointer, and the 0.1f constant is built after the entry's loads.
void fn_71003f2710(UnkSpitAttack* self, f32 delta) {
    void* target = Owner(self)->field_0x240;
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
    UnkSpitOwner_71003f2710* owner = Owner(self);
    UnkVec3f v = UnkCachedVec(owner);
    fn_71003f18f8(self, still, still, &v);
    reinterpret_cast<void (*)(UnkSpitAttack*, void*, void*, void*, f32, f32)>(self->vtable[0xe8 / 8])(
        self, self->field_0x5c, self->field_0x74, self->field_0xf8, self->field_0xd4, self->field_0xd8);
    fn_71009f92d0(Owner(self)->field_0x2d0);
    *reinterpret_cast<u32*>(&self->field_0xe4) = next;
    u32 index = self->field_0xe8;
    self->field_0xe8 = index >= self->field_0x28->field_0x150 - 1 ? 0 : index + 1;
}
