// behaviortree::CSetAnimationForImpact's unit (tools/tu_map.py): the start step runs the base
// (fn_7100394bd0) and clears the flag at +0x4a; when the owner (+0x18) has the objects at +0x1d0
// and +0x1f0, picks the string id at +0x30 or +0x38 by the sign of the negated dot product of the
// first's vector (+0x100) and what the second's vf_0x210 gives, keeps it at +0x40, then sends its
// hash to the owner's sub-object at +0x220 with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h)
// and keeps the result at +0x4a. Names are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkSelf;
void fn_7100394bd0(UnkSelf*);

// Returned in memory (not trivially copyable).
struct UnkVec3_b55 {
    f32 x, y, z;
    UnkVec3_b55();
    UnkVec3_b55(const UnkVec3_b55& other);
};

struct UnkImpactSource_b55 {
    u8 pad_0x0[0x100];
    f32 x, y, z;
};

struct UnkImpactDir_b55 {
    virtual void vf_0x0();
    UnkVec3_b55 vf_0x210() { return reinterpret_cast<UnkVec3_b55 (*)(UnkImpactDir_b55*)>((*reinterpret_cast<void***>(this))[0x210 / 8])(this); }
};

struct UnkImpactOwner_b55 {
    u8 pad_0x0[0x1d0];
    UnkImpactSource_b55* field_0x1d0;
    u8 pad_0x1d8[0x18];
    UnkImpactDir_b55* field_0x1f0;
    u8 pad_0x1f8[0x28];
    void* field_0x220;
};

struct UnkSetAnimationForImpact_b55 {
    u8 pad_0x0[0x18];
    UnkImpactOwner_b55* owner;
    u8 pad_0x20[0x10];
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    u8 pad_0x48[0x2];
    bool field_0x4a;
};

void fn_710036de14(UnkSetAnimationForImpact_b55* self) {
    fn_7100394bd0(reinterpret_cast<UnkSelf*>(self));
    self->field_0x4a = false;
    UnkImpactSource_b55* source = self->owner->field_0x1d0;
    if (!source)
        return;
    UnkImpactDir_b55* dir = self->owner->field_0x1f0;
    if (!dir)
        return;
    f32 z = source->z, y = source->y, x = source->x;
    UnkVec3_b55 v = dir->vf_0x210();
    if (-x * v.x + -y * v.y + -z * v.z >= 0.0f)
        self->field_0x40 = self->field_0x30;
    else
        self->field_0x40 = self->field_0x38;
    UnkImpactOwner_b55* owner = self->owner;
    void* target = owner ? owner->field_0x220 : nullptr;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(self->field_0x40.data) + 0x30);
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    self->field_0x4a = fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
