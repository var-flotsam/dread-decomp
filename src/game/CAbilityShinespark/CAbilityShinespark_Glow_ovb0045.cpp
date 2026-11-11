// CAbilityShinespark's unit (tools/tu_map.py unit 433): the shinespark glow (fn_7100328e68), the
// nearest-target visitor (fn_710033265c) and the "ExecutedCoolShinesparks" blackboard counter
// (fn_7100331544). Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU39.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// ---- The glow: a pulse between two colours on the model updater ----

struct UnkV4_b47 {
    f32 x, y, z, w;
    UnkV4_b47() {}
    UnkV4_b47(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkV4_b47(const UnkV4_b47& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};
void fn_710093adc0(base::reflection::UnkReflObject* comp, const void* a, const void* b, f32 time);
extern const u64 g_unk_7101648ed8 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101648ee8 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101648ef8 __attribute__((visibility("hidden")));

static inline UnkV4_b47 operator*(const UnkV4_b47& a, f32 s) { return fn_7100030830(&a, s); }
static inline UnkV4_b47 operator+(const UnkV4_b47& a, const UnkV4_b47& b) { return fn_7100030770(&a, &b); }

struct UnkTunable;
extern UnkTunable g_unk_71726b0378;

struct UnkShinesparkTunable_b0045 {
    u8 pad_0x0[0x28];
    f32 pulse_rate;  // 0x28
};

struct UnkShinesparkActor_b0045 {
    u8 pad_0x0[0x228];
    UnkReflObject* model_updater;  // 0x228
};

struct UnkShinesparkLink_b0045 {
    u8 pad_0x0[0x30];
    UnkShinesparkActor_b0045* actor;
};

struct UnkShinesparkGlow_b0045 {
    u8 pad_0x0[0xc0];
    UnkShinesparkLink_b0045* link;
    u8 pad_0xc8[0x750 - 0xc8];
    f32 glow_time;  // 0x750
};

struct UnkSmAbility3_A2_b63;

void fn_7100328e68(UnkSmAbility3_A2_b63* ability, f32 dt) {
    UnkShinesparkGlow_b0045* self = reinterpret_cast<UnkShinesparkGlow_b0045*>(ability);
    self->glow_time += dt;
    UnkReflObject* comp = self->link->actor->model_updater;
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_7100939af4())) {
        const UnkShinesparkTunable_b0045* tunable =
            reinterpret_cast<const UnkShinesparkTunable_b0045*>(&g_unk_71726b0378);
        f32 t = sinf(self->glow_time * tunable->pulse_rate) * 0.5f + 0.5f;
        const UnkV4_b47& from = *reinterpret_cast<const UnkV4_b47*>(&g_unk_7101648ed8);
        const UnkV4_b47& to = *reinterpret_cast<const UnkV4_b47*>(&g_unk_7101648ee8);
        UnkV4_b47 c = from * t + to * (1.0f - t);
        fn_710093adc0(comp, &c, &g_unk_7101648ef8, -1.0f);
    }
}

// ---- The nearest target ----

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};
extern UnkVisitResult_b0040 g_unk_71726a4778;  // the "skip" result
extern u64 g_unk_71726a4780;                   // its guard

static inline UnkVisitResult_b0040 Skip_b0045() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

// A candidate target: two handles, then plain words; the one at 0x38 is its distance.
struct UnkSparkTarget_b0045 {
    UnkAssignedSlot slot;
    UnkAssignedSlot slot_0x8;
    u32 words[13];  // 0x10

    f32 Distance() const { return *reinterpret_cast<const f32*>(&words[10]); }

    UnkSparkTarget_b0045& operator=(const UnkSparkTarget_b0045& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkSparkNearest_b0045 {
    u8 pad_0x0[8];
    UnkSparkTarget_b0045 best;  // 0x8
    bool has_best;              // 0x50
};

static inline f32 Sign_b0045(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_710033265c(UnkSparkNearest_b0045* self, const UnkSparkTarget_b0045* target) {
    if (!self->has_best || (s32)Sign_b0045(self->best.Distance() - target->Distance()) > 0) {
        self->has_best = true;
        self->best = *target;
        return UnkVisitResult_b0040(1);
    }
    return Skip_b0045();
}

// ---- Cool shinesparks ----

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

// As src/game/UnkSubareaSetupState.cpp and src/game_s7/fn_7100fc4b34.cpp declare it.
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern UnkPropertyEx<0x160> g_unk_71727594f8;  // "ExecutedCoolShinesparks"

struct UnkShinespark_b56;
struct UnkDirOut_b56;

struct UnkSparkVec3_b0045 {
    f32 x, y, z;
    UnkSparkVec3_b0045(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkSparkVec3_b0045 operator+(const UnkSparkVec3_b0045& o) const {
        return UnkSparkVec3_b0045(x + o.x, y + o.y, z + o.z);
    }
};

struct UnkShinesparkCool_b0045 {
    u8 pad_0x0[0x3ac];
    UnkSparkVec3_b0045 dir;  // 0x3ac
    u8 pad_0x3b8[0x441 - 0x3b8];
    bool in_air;  // 0x441
    u8 pad_0x442[0x448 - 0x442];
    CStrId name;  // 0x448
};

static inline UnkSubId_7100fc4b34 SubId(UnkPropertyEx<0x160>& def, const CStrId& name) {
    UnkQualifiedName* qualified = reinterpret_cast<UnkQualifiedName*>(def.field_0x10);
    if (!qualified->str || !qualified->str[0])
        fn_7100ae9d7c(qualified, reinterpret_cast<UnkIdSource*>(&def), g_unk_7101d470e0);
    return fn_71003327a4(qualified, &name);
}

// Whether the direction held at +0x3ac, tilted up, is straight up (direction 1 of fn_710032f678).
static inline bool AimsUp(UnkShinesparkCool_b0045* self) {
    UnkSparkVec3_b0045 dir = self->dir + UnkSparkVec3_b0045(0.0f, 0.0f, 1.0f);
    return fn_710032f678(reinterpret_cast<UnkShinespark_b56*>(self), reinterpret_cast<const UnkDirOut_b56*>(&dir)) == 1;
}

// Counts a cool shinespark: an air shinespark aimed straight up stores true under the actor's name in
// "ExecutedCoolShinesparks" in the current scenario's blackboard section.
void fn_7100331544(UnkShinesparkCool_b0045* self) {
    if (!self->in_air || !AimsUp(self))
        return;
    if (self->name.data == g_unk_7101d470e0.data)
        return;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkTypedValue v;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    UnkSubId_7100fc4b34 key = SubId(g_unk_71727594f8, self->name);
    bool value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
        if (!manager->field_0x968.field_0xe8) {
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
        }
    }
}
