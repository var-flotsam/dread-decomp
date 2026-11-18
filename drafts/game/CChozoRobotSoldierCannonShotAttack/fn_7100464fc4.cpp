// CChozoRobotSoldierCannonShotAttack's unit (tools/tu_map.py): starts the attack. After the base reset it
// picks one of the definition's shots at random, clears the shot state, aims at the target's position
// (fn_710046514c) and, when the owner's AI component has a navigation object, hands it a kind-5 request
// facing back along the owner's z axis (or the view direction). Then resets both timers. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAIComponent.h"
#include "game/UnkAttack.h"
#include "game/UnkTU2772.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkSelf;
struct UnkVec3;
bool fn_710046514c(UnkSelf* self, UnkVec3* out);

template <class T>
struct UnkOpt_l464fc4 {
    T value;
    bool has;
};

// The block handed to the navigation object (kind 5, as game_s2/UnkParams.h), with its direction typed.
struct UnkCannonShotParams_l464fc4 {
    void** vtable;
    UnkOpt_l464fc4<UnkVec3f> direction;  // 0x8
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkCannonShotViewDir_l464fc4 {
    u8 pad_0x0[0x70];
    UnkVec3f dir;  // 0x70
};

struct UnkCannonShotOwner_l464fc4 {
    u8 pad_0x0[0x38];
    u8 xform[0xc];  // 0x38
    u8 pad_0x44[0x1f0 - 0x44];
    UnkCannonShotViewDir_l464fc4* view_dir;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkStateSet_1441* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkAIComponent* ai;  // 0x240
};

struct UnkCannonShotEntry_l464fc4 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
};

struct UnkCannonShotDef_l464fc4 {
    u8 pad_0x0[0xf0];
    UnkCannonShotEntry_l464fc4** shots;  // 0xf0
    u32 shot_count;  // 0xf8
};

struct UnkCannonShotPos_l464fc4 {
    u32 x, y, z;
};

struct UnkCannonShotState_l464fc4 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkCannonShot_l464fc4 {
    void** vtable;
    UnkCannonShotOwner_l464fc4* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkCannonShotDef_l464fc4* def;  // 0x28
    f32 field_0x30;
    f32 field_0x34;
    u32 field_0x38;
    UnkCannonShotState_l464fc4 state;  // 0x3c
    bool has_target;  // 0x4c
    u8 pad_0x4d[3];
    UnkCannonShotPos_l464fc4 target;  // 0x50
    u32 field_0x5c;
    u8 pad_0x60[8];
    bool field_0x68;
    u8 pad_0x69[3];
    u32 shot;  // 0x6c
};

f32 fn_710046534c(UnkCannonShot_l464fc4* self);

// The way the owner's z axis points back.
static inline UnkVec3f Back(UnkCannonShot_l464fc4* self) {
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->xform));
    return UnkVec3f(-m.m[2], -m.m[6], -m.m[10]);
}

void fn_7100464fc4(UnkCannonShot_l464fc4* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    u32 count = self->def->shot_count;
    u32 shot = count ? (fn_71012395a0() & 0x7fff) % count : 0;
    self->shot = shot;
    u32 value = self->def->shots[shot]->field_0x8;
    self->field_0x68 = false;
    self->state = UnkCannonShotState_l464fc4();
    self->field_0x5c = 0;
    self->field_0x38 = value;
    UnkCannonShotPos_l464fc4 pos;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    if (fn_710046514c(reinterpret_cast<UnkSelf*>(self), reinterpret_cast<UnkVec3*>(&pos))) {
        self->target.x = pos.x;
        self->target.y = pos.y;
        self->target.z = pos.z;
        self->has_target = true;
        UnkAIComponent* ai = self->owner->ai;
        if (ai) {
            if (UnkNavigation* nav = fn_71010fccf8(ai)) {
                UnkCannonShotParams_l464fc4 params;
                __builtin_memset(&params.direction, 0, 13);
                params.field_0x30 = 0;
                __builtin_memset(params.field_0x20, 0, 16);
                params.vtable = g_unk_7101a25b10 + 2;
                params.kind = 5;
                params.field_0x39 = 0;
                params.field_0x3b = 0;
                params.field_0x3d = 0;
                params.field_0x18 = 0;
                params.field_0x1c = 1;
                UnkStateSet_1441* animation = self->owner->animation;
                if (animation && fn_7101179870(animation)) {
                    params.direction.value = Back(self);
                } else {
                    params.direction.value = self->owner->view_dir->dir;
                }
                params.direction.has = true;
                fn_71006f4b0c(nav, &params, 1);
            }
        }
    }
    f32 time = fn_710046534c(self);
    self->field_0x34 = time;
    self->field_0x30 = time;
}
