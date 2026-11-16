// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): applies the pending reset and beam change, runs
// the parent's step (fn_7100839d70), then sends the owner's animation component a 0x82 under one id.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;  // reached through the GOT

// Timeline ids reached through the GOT.
extern u64 g_unk_710164ad48;
extern u64 g_unk_710164ad50;
extern u64 g_unk_710164ad58;
extern u64 g_unk_710164ad60;
extern u64 g_unk_710164ad68;

struct UnkGunBeamInfo_ovb0070 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkGunBeam_ovb0070 {
    UnkGunBeamInfo_ovb0070* info;
};

struct UnkSamusGun_ovb0070;
struct UnkGun_ovb0091;
UnkGunBeam_ovb0070* fn_7100840828(UnkSamusGun_ovb0070* self);
void fn_7100839d70(UnkGun_ovb0091* self);

// An entry of the array at +0x348: a counted reference and two words.
struct UnkGunShot_l85b80c {
    UnkRefObject* ref;
    u64 field_0x8;
    u32 field_0x10;

    ~UnkGunShot_l85b80c() {
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
static_assert(sizeof(UnkGunShot_l85b80c) == 0x18, "UnkGunShot_l85b80c layout");

struct UnkSamusGun_l85b80c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x2f8];
    bool field_0x330;
    u8 pad_0x331[0xf];
    bool field_0x340;
    u32 field_0x344;
    UnkGunShot_l85b80c* shots;
    u32 shot_count;
};
static_assert(__builtin_offsetof(UnkSamusGun_l85b80c, shot_count) == 0x350, "layout");

// A pending reset (+0x340) drops the entries and removes two timelines.
static inline void ApplyReset(UnkSamusGun_l85b80c* self) {
    if (self->field_0x340) {
        self->field_0x340 = false;
        self->field_0x344 = 0;
        for (u64 i = 0; i < self->shot_count; ++i)
            self->shots[i].~UnkGunShot_l85b80c();
        self->shot_count = 0;
        if (self->owner) {
            fn_7100d7c94c(self->owner, &g_unk_710164ad68);
            fn_7100d7c94c(self->owner, &g_unk_710164ad60);
        }
    }
}

// A pending beam change (+0x330) removes the owner's timeline that goes with the current beam.
static inline void ApplyBeamChange(UnkSamusGun_l85b80c* self) {
    if (self->field_0x330) {
        self->field_0x330 = false;
        if (self->owner) {
            switch (fn_7100840828(reinterpret_cast<UnkSamusGun_ovb0070*>(self))->info->id) {
            case 0x6803e9b3d5c26ba0:
            case 0xb23e2ea9edd95fb7:
                fn_7100d7c94c(self->owner, &g_unk_710164ad48);
                break;
            case 0xa87817d815624dd3:
                fn_7100d7c94c(self->owner, &g_unk_710164ad50);
                break;
            case 0xa7b8a8ac9c26afaa:
                fn_7100d7c94c(self->owner, &g_unk_710164ad58);
                break;
            }
        }
    }
}

void fn_710085b80c(UnkSamusGun_l85b80c* self) {
    ApplyReset(self);
    ApplyBeamChange(self);
    fn_7100839d70(reinterpret_cast<UnkGun_ovb0091*>(self));
    auto* animation = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x220);
    if (animation && fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))
        base::reflection::refl_EmitU32(animation, 0xa218d351a57ec697, 0x5d523009b8f43e5f, 0x82, &g_unk_710164b958,
                                       &g_unk_717276974c);
}
