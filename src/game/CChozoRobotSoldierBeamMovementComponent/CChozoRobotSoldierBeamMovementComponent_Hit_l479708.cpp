// CChozoRobotSoldierBeamMovementComponent's unit (tools/tu_map.py unit 703): the beam's hit test. Skips its
// own target, a nearly horizontal beam meeting a level-3 actor at the owner's height, and repeat hits; then
// records the hit, tells a Chozo robot soldier AI and sets the timeline mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkObj;
bool fn_7100ac4ec0(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5);
struct UnkSplash_b59a4;
// Declared returning void elsewhere; it returns 1 when the hit was taken.
s32 fn_7100ac82c0(UnkSplash_b59a4* self, void* other);

struct UnkBeamLevel_l479708 {
    u8 pad_0x0[0x10];
    s32 level;  // 0x10
};

struct UnkBeamOtherPart_l479708 {
    u8 pad_0x0[0x2d08];
    UnkBeamLevel_l479708* level;  // 0x2d08
};

struct UnkBeamLink_l479708 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkBeamHitInfo_l479708 {
    u8 pad_0x0[0x14];
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 extra;      // 0x38
    u8 pad_0x3c;
    u8 flags;  // 0x3d
};

struct UnkBeamAI_l479708 {
    virtual const base::reflection::CType* vf_0x0();
};

struct UnkBeamAIListener_l479708 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90(UnkActor* owner, UnkActor* other);
};

struct UnkBeamHit_l479708 {
    s32 id;
    u32 value_0;
    u32 value_1;
    u32 value_2;
    u32 value_3;
    u32 value_4;
    u32 value_5;
    u32 value_6;
    u32 value_7;
    u32 value_8;
    u32 extra;
    u32 kind;
};

struct UnkSoldierBeam_l479708 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x40 - 0x38];
    f32 dir[3];  // 0x40
    u8 pad_0x4c[0x230 - 0x4c];
    UnkBeamLink_l479708* link;  // 0x230
    u8 pad_0x238[0x3b0 - 0x238];
    UnkBeamHit_l479708 hit;  // 0x3b0
    u8 pad_0x3e0[0x4b8 - 0x3e0];
    u64 field_0x4b8;
    s32 field_0x4c0;
    u8 pad_0x4c4[0x5d8 - 0x4c4];
    bool field_0x5d8;
    u8 pad_0x5d9;
    bool field_0x5da;
};

bool fn_7100479708(UnkSoldierBeam_l479708* self, UnkActor* other, void* a2, void* a3, UnkBeamHitInfo_l479708* info,
                   void* a5) {
    UnkActor* target = self->link ? self->link->actor : nullptr;
    if (target == other || self->field_0x5da)
        return false;
    if (other) {
        auto* part = static_cast<UnkBeamOtherPart_l479708*>(other->field_0x208);
        if (part && part->level && part->level->level == 3) {
            f32 dot = self->dir[0] + self->dir[1] * 0.0f + self->dir[2] * 0.0f;
            if (__builtin_fabsf(dot) > 0.97f) {
                f32 otherY = other->field_0x54;
                f32 ownerY = self->owner->field_0x54;
                if (__builtin_fabsf(ownerY - otherY) < 20.0f)
                    return false;
            }
        }
    }
    if (!fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, info, a5))
        return false;
    self->field_0x5da = true;
    self->field_0x4b8 = 0;
    self->field_0x4c0 = 0;
    if (other) {
        u32 v0 = info->field_0x14;
        u32 v1 = info->field_0x18;
        u32 v2 = info->field_0x1c;
        u32 v3 = info->field_0x20;
        u32 v4 = info->field_0x24;
        u32 v5 = info->field_0x28;
        u32 v6 = info->field_0x2c;
        u32 v7 = info->field_0x30;
        u32 v8 = info->field_0x34;
        u32 extra = (info->flags & 2) ? info->extra : 0;
        self->hit.id = -1;
        self->hit.value_0 = v0;
        self->hit.value_1 = v1;
        self->hit.value_2 = v2;
        self->hit.value_3 = v3;
        self->hit.value_4 = v4;
        self->hit.value_5 = v5;
        self->hit.value_6 = v6;
        self->hit.value_7 = v7;
        self->hit.value_8 = v8;
        self->hit.extra = extra;
        self->hit.kind = 0x238;
        if (fn_7100ac82c0(reinterpret_cast<UnkSplash_b59a4*>(self), other) == 1) {
            if (UnkBeamLink_l479708* link = self->link) {
                if (UnkActor* actor = link->actor) {
                    if (auto* ai = static_cast<UnkBeamAI_l479708*>(actor->field_0x240)) {
                        if (fn_710009d50c(ai->vf_0x0(), fn_710046ea2c())) {
                            auto* listener = reinterpret_cast<UnkBeamAIListener_l479708*>(
                                reinterpret_cast<u8*>(ai) + 0x149a8);
                            listener->vf_0x90(self->owner, other);
                        }
                    }
                }
            }
        }
    }
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        u64 off = 0x2b5ea1fd8a588e4aull;
        u64 on = 0xd3ff1fc021c49baaull;
        const u64* key = self->field_0x5d8 ? &on : &off;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
    }
    return true;
}
