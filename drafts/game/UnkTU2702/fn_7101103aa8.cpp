// Unit 2702 (hint CAIComponent): the AI component's update: advances its timers by `dt`, tracks whether
// the AI is ignored, refreshes the path flags, runs the per-frame steps and clears three blackboard flags.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2702.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

struct UnkAIC_b0144;
struct UnkTargetSelf_w;
void fn_71011048c0(UnkAIC_b0144* self);
void fn_71010ffc38(UnkTargetSelf_w* self, f32 dt);
bool fn_71006ff63c(void* motion, f32* scale);
f32 fn_71010d0c64(void* movement);
void fn_7101104274(void* self);
void fn_7101104998(void* self);

struct UnkMotion_l103aa8 {
    u8 pad_0x0[0x10];
    u32 kind;
};

struct UnkNavEntry_l103aa8 {
    u8 pad_0x0[0x10];
    UnkMotion_l103aa8* motion;
};

struct UnkNavRecords_l103aa8 {
    UnkNavEntry_l103aa8* first;
};

struct UnkNav_l103aa8 {
    u8 pad_0x0[0xb0];
    UnkNavRecords_l103aa8* records;
};

struct UnkMovement_l103aa8 {
    u8 pad_0x0[0x118];
    f32 speed;
};

struct UnkLife_l103aa8 {
    u8 pad_0x0[0x70];
    u32 state;
};

struct UnkOwner_l103aa8 {
    u8 pad_0x0[0x1b8];
    UnkLife_l103aa8* life;  // 0x1b8
    u8 pad_0x1c0[0x30];
    UnkMovement_l103aa8* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* character;  // 0x220
    void* model;  // 0x228
    u8 pad_0x230[0x68];
    UnkNav_l103aa8* nav;  // 0x298
    u8 pad_0x2a0[0x68];
    u32 field_0x308;
    bool field_0x30c;
    u8 pad_0x30d[0x3];
    bool field_0x310;
};
static_assert(__builtin_offsetof(UnkOwner_l103aa8, field_0x310) == 0x310, "layout");

struct UnkTunable_l103aa8 {
    u8 pad_0x0[0xc8];
    u32 mode;
};

struct UnkAI_l103aa8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l103aa8* owner;  // 0x30
    u8 pad_0x38[0x7];
    u8 field_0x3f;
    f32 field_0x40;
    f32 field_0x44;
    u8 field_0x48;
    u8 pad_0x49[0x3];
    s32 field_0x4c;
    u8 field_0x50[0x8];
    f32 field_0x58;
    f32 field_0x5c;
    bool field_0x60;
    bool field_0x61;
    u8 pad_0x62[0x10ae];
    f32 field_0x1110;
    f32 field_0x1114;
    u8 pad_0x1118[0x4];
    f32 field_0x111c;
    f32 field_0x1120;
    f32 field_0x1124;
    f32 field_0x1128;
    f32 field_0x112c;
    f32 field_0x1130;
    f32 field_0x1134;
    f32 field_0x1138;
    f32 field_0x113c;
    s32 field_0x1140;
    u8 pad_0x1144[0x18];
    f32 field_0x115c;
    u8 pad_0x1160[0x80];
    bool field_0x11e0;
    u8 pad_0x11e1[0x40];
    bool ignored;  // 0x1221
    u8 pad_0x1222[0x2];
    u32 field_0x1224;
    u8 pad_0x1228[0x20];
    bool field_0x1248;
    u8 pad_0x1249[0x3];
    f32 field_0x124c;
    u8 pad_0x1250[0x10];
    CBlackboard blackboard;  // 0x1260
    u8 pad_blackboard[0x13d4 - 0x1260 - sizeof(CBlackboard)];
    f32 field_0x13d4;
    f32 field_0x13d8;
    f32 field_0x13dc;
    u8 pad_0x13e0[0x114];
    f32 field_0x14f4;
    f32 field_0x14f8;
    u8 pad_0x14fc[0xd4];
    bool field_0x15d0;
    u8 pad_0x15d1[0x3f];
    UnkTunable_l103aa8* tunable;  // 0x1610
    u8 pad_0x1618[0xcc];
    bool field_0x16e4;
    u8 pad_0x16e5[0xa7];
    bool field_0x178c;
    u8 pad_0x178d[0x3];
    f32 field_0x1790;
    bool field_0x1794;
    u8 pad_0x1795[0x3];
    f32 field_0x1798;
    u8 pad_0x179c[0x5];
    bool field_0x17a1;

    void vf_0x200(s32 a, bool b) {
        reinterpret_cast<void (*)(UnkAI_l103aa8*, s32, bool)>(vtable[0x200 / 8])(this, a, b);
    }
    void vf_0x308() { reinterpret_cast<void (*)(UnkAI_l103aa8*)>(vtable[0x308 / 8])(this); }
    void vf_0x3d8() { reinterpret_cast<void (*)(UnkAI_l103aa8*)>(vtable[0x3d8 / 8])(this); }
    void vf_0x708(f32 dt) { reinterpret_cast<void (*)(UnkAI_l103aa8*, f32)>(vtable[0x708 / 8])(this, dt); }
    void vf_0x710(f32 dt) { reinterpret_cast<void (*)(UnkAI_l103aa8*, f32)>(vtable[0x710 / 8])(this, dt); }
    void vf_0x730() { reinterpret_cast<void (*)(UnkAI_l103aa8*)>(vtable[0x730 / 8])(this); }
    void vf_0x748(u8* a, u8* b, s32* c, u8* d) {
        reinterpret_cast<void (*)(UnkAI_l103aa8*, u8*, u8*, s32*, u8*)>(vtable[0x748 / 8])(this, a, b, c, d);
    }
    void vf_0x790() { reinterpret_cast<void (*)(UnkAI_l103aa8*)>(vtable[0x790 / 8])(this); }
    void vf_0x798() { reinterpret_cast<void (*)(UnkAI_l103aa8*)>(vtable[0x798 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x1140) == 0x1140, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x115c) == 0x115c, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x11e0) == 0x11e0, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x124c) == 0x124c, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x13d4) == 0x13d4, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x14f4) == 0x14f4, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, tunable) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x16e4) == 0x16e4, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x17a1) == 0x17a1, "layout");
static_assert(__builtin_offsetof(UnkAI_l103aa8, field_0x15d0) == 0x15d0, "layout");

// Whether the owner's character class has the track `get_class` names.
static inline bool HasTrack(UnkOwner_l103aa8* owner, const base::reflection::CClass* (*get_class)()) {
    if (UnkAnimationComponent* character = owner->character)
        return fn_710117b348(character, get_class());
    return false;
}

// A countdown: runs down to zero.
static inline void CountDown(f32& t, f32 dt) {
    if (t > 0.0f) {
        t -= dt;
        if (t <= 0.0f)
            t = 0.0f;
    }
}

// A running time: counts up while not negative.
static inline void CountUp(f32& t, f32 dt) {
    if (t >= 0.0f)
        t += dt;
}

// Stores `value` under `name` in the blackboard section `section` names, unless the blackboard is locked.
static inline void SetFlag(CBlackboard* blackboard, const CStrId& section, const CStrId& name, bool value) {
    UnkTypedValue v;
    bool stored = value;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &stored;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&name)), &v);
        }
    }
}

static inline const CStrId& Id(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

void fn_7101103aa8(UnkEmmyCaveAIStart_b49* object, f32 dt) {
    UnkAI_l103aa8* self = reinterpret_cast<UnkAI_l103aa8*>(object);
    if (!fn_710024fbc8(self) || !self->tunable)
        return;

    bool suspended = HasTrack(self->owner, fn_7100c363c8);
    if (!suspended && self->field_0x11e0 && self->owner->field_0x308 >= 2) {
        void* model = self->owner->model;
        if (model ? fn_7101173964(model) : !fn_7100d7bea0(reinterpret_cast<UnkActor*>(self->owner), 1))
            fn_7100d7add4(reinterpret_cast<UnkActor_7101105e44*>(self->owner), true);
        self->field_0x11e0 = false;
    }
    CountDown(self->field_0x1790, dt);
    CountUp(self->field_0x1798, dt);


    if (self->field_0x1794) {
        bool fast = false;
        UnkOwner_l103aa8* owner = self->owner;
        if (UnkNav_l103aa8* nav = owner->nav) {
            if (UnkNavEntry_l103aa8* entry = nav->records->first) {
                UnkMotion_l103aa8* motion = entry->motion;
                UnkMovement_l103aa8* movement = owner->movement;
                if (motion && movement && motion->kind == 2) {
                    f32 scale = 1.0f;
                    if (fn_71006ff63c(motion, &scale)) {
                        f32 speed = movement->speed * 0.97f;
                        fast = speed > fn_71010d0c64(movement) * scale;
                    }
                }
            }
        }
        if (!fast)
            self->field_0x1794 = false;
        if (!self->field_0x1794)
            self->field_0x1798 = 0.0f;
    }

    self->field_0x1110 += dt;
    if (self->field_0x115c > 0.0f)
        self->field_0x115c -= dt;
    if (fn_7100d7bea0(reinterpret_cast<UnkActor*>(self->owner), self->tunable->mode)) {
        self->field_0x14f4 += dt;
        self->field_0x14f8 = 0.0f;
    } else {
        self->field_0x14f4 = 0.0f;
        self->field_0x14f8 += dt;
    }
    if (!self->owner->field_0x310)
        self->field_0x1114 += dt;

    CountUp(self->field_0x13d4, dt);
    CountUp(self->field_0x13d8, dt);
    CountUp(self->field_0x13dc, dt);
    CountUp(self->field_0x1124, dt);
    CountUp(self->field_0x1128, dt);
    CountUp(self->field_0x112c, dt);
    CountUp(self->field_0x1134, dt);
    CountUp(self->field_0x1138, dt);
    CountUp(self->field_0x113c, dt);
    CountDown(self->field_0x111c, dt);
    CountDown(self->field_0x1120, dt);
    if (self->field_0x60) {
        self->field_0x60 = false;
        self->field_0x1130 = self->field_0x112c;
        self->field_0x112c = 0.0f;
    }
    if (self->field_0x61) {
        self->field_0x61 = false;
        self->field_0x1134 = 0.0f;
    }

    if (self->field_0x16e4)
        self->vf_0x3d8();
    bool was_ignored = self->ignored;
    self->ignored = false;
    self->ignored = HasTrack(self->owner, fn_7100d05168);
    if (self->field_0x1248) {
        self->field_0x124c -= dt;
        self->ignored = true;
        if (self->field_0x124c <= 0.0f) {
            self->ignored = false;
            self->field_0x1248 = false;
        }
    }
    if (self->field_0x1224)
        self->ignored = true;
    if (was_ignored != self->ignored)
        self->vf_0x730();

    self->vf_0x200(self->field_0x1140 + 1, true);
    fn_71010ffc38(reinterpret_cast<UnkTargetSelf_w*>(self), dt);

    u8 old_3f = self->field_0x3f;
    u8 old_48 = self->field_0x48;
    self->field_0x4c = -1;
    self->vf_0x748(&self->field_0x3f, &self->field_0x48, &self->field_0x4c, self->field_0x50);
    if (old_3f != self->field_0x3f)
        self->vf_0x790();
    if (old_48 != self->field_0x48)
        self->vf_0x798();
    if (self->field_0x3f) {
        self->field_0x40 += dt;
        self->field_0x44 = 0.0f;
    } else {
        self->field_0x40 = 0.0f;
        self->field_0x44 += dt;
    }
    if (self->field_0x48) {
        self->field_0x58 += dt;
        self->field_0x5c = 0.0f;
    } else {
        self->field_0x58 = 0.0f;
        self->field_0x5c += dt;
    }
    fn_7101104274(self);

    if (!self->field_0x17a1) {
        UnkLife_l103aa8* life = self->owner->life;
        if (life && life->state == 2)
            self->vf_0x710(dt);
        else if (!self->ignored)
            self->vf_0x708(dt);
    }

    SetFlag(&self->blackboard, g_unk_71726b7cb8, Id(0x11c), false);
    SetFlag(&self->blackboard, g_unk_71726b7cb8, Id(0x117), false);
    SetFlag(&self->blackboard, g_unk_71726b7cb8, Id(0x11d), false);
    self->field_0x178c = false;
    fn_71011048c0(reinterpret_cast<UnkAIC_b0144*>(self));
    self->vf_0x308();
    if (self->field_0x15d0)
        fn_7101104998(self);
}
