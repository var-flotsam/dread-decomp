// CGroundShockerAttack's unit (tools/tu_map.py unit 856): the update. After the attack base's update, and
// only while the owner's AI component is a CGroundShockerAIComponent (otherwise fn_71005a0f00 ends it), the
// step at +0x34 is advanced: step 1 measures the distance to the ground and moves to 2 (or ends the attack),
// steps 3 and 5 count their timers down and then fire the shock (steps 4 and 7). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManagerIdTable.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;
using base::reflection::UnkTunable;

// As src/reflection/CCharClassGroundShockerAttack.cpp declares it.
extern UnkTunable g_unk_71726e7be0;

extern const char g_unk_710164a070[] __attribute__((visibility("hidden")));
extern char g_unk_71726e8048[] __attribute__((visibility("hidden")));
extern char g_unk_71726e8050[] __attribute__((visibility("hidden")));

struct UnkGroundShockerComponent;
void fn_71005a5110(UnkGroundShockerComponent* self, f32 value);
void fn_71005a50ec(UnkReflObject* ai);
void fn_71005a4bd8(UnkReflObject* ai, f32 value);
bool fn_71005a4c84(UnkReflObject* ai, const char* name, f32* distance, CVector3D* point);
void fn_7101110964(UnkReflObject* ai, const char* name, const u64* key, const u32* flags, f32 a, f32 b);

struct UnkGroundShockerTunable_l5a0c78 {
    u8 pad_0x0[0x34];
    f32 field_0x34;
};

struct UnkGroundShockerOwner_l5a0c78 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkGroundShockerAttack_l5a0c78 {
    u8 pad_0x0[0x8];
    UnkGroundShockerOwner_l5a0c78* owner;  // 0x8
    u8 pad_0x10[0x20];
    u32 previous;  // 0x30
    u32 step;      // 0x34
    f32 timer_3;   // 0x38
    f32 timer_5;   // 0x3c
};

void fn_71005a0f00(UnkGroundShockerAttack_l5a0c78* self, bool done);

// The key of the id table's id at +0x10c0.
static inline u64 ShockKey() {
    const CStrId& id = *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + 0x10c0);
    return id.data->Key();
}

void fn_71005a0c78(UnkGroundShockerAttack_l5a0c78* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkReflObject* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005a4004())) {
        fn_71005a0f00(self, false);
        return;
    }
    u32 step = self->step;
    self->previous = step;
    switch (step) {
    case 1: {
        f32 distance = -1.0f;
        CVector3D point;
        point.x = 0.0f;
        point.y = 0.0f;
        point.z = 0.0f;
        if (!fn_71005a4c84(ai, g_unk_710164a070, &distance, &point) || !(distance >= 0.0f)) {
            fn_71005a0f00(self, false);
            return;
        }
        fn_71005a5110(reinterpret_cast<UnkGroundShockerComponent*>(ai), distance);
        self->step = 2;
        break;
    }
    case 3: {
        if (!(self->timer_3 > 0.0f))
            return;
        f32 t = self->timer_3 - dt;
        self->timer_3 = t;
        if (!(t <= 0.0f))
            return;
        u64 key = ShockKey();
        u32 flags = 0;
        fn_7101110964(ai, g_unk_71726e8048, &key, &flags, -1.0f, 0.0f);
        self->step = 4;
        break;
    }
    case 5: {
        if (!(self->timer_5 > 0.0f))
            return;
        f32 t = self->timer_5 - dt;
        self->timer_5 = t;
        if (!(t <= 0.0f))
            return;
        u64 key = ShockKey();
        u32 flags = 0;
        fn_7101110964(ai, g_unk_71726e8050, &key, &flags, -1.0f, 0.0f);
        fn_71005a50ec(ai);
        fn_71005a4bd8(ai, reinterpret_cast<const UnkGroundShockerTunable_l5a0c78*>(&g_unk_71726e7be0)->field_0x34);
        self->step = 7;
        break;
    }
    }
}
