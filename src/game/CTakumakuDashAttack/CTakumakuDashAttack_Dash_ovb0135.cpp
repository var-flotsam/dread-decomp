// CTakumakuDashAttack's unit (tools/tu_map.py): the dash attack's update (fn_71006cec98), its turn towards
// the target (fn_71006d0244) and its animation events (fn_71006d0494). The attack (see
// CTakumakuDashAttack_Start_b58.cpp) keeps the Takumaku AI component at +0x30, its state at +0x38 and the
// requested one at +0x3c, a countdown at +0x60, whether the target is in front (+0x64) and the distance
// to it (+0x68). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CTakumakuAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern void* g_unk_7101a24b58[];

struct UnkTakumakuDir_b58 {
    f32 x, y, z;
};
extern const UnkTakumakuDir_b58 g_unk_710164a7a0 __attribute__((visibility("hidden")));
extern const UnkTakumakuDir_b58 g_unk_710164a7ac __attribute__((visibility("hidden")));

// The facing query handed to the owner's +0x298 (as CTakumakuDashAttack_Start_b58.cpp builds it).
struct UnkTakumakuQuery_ovb0135 {
    void** vtable;
    UnkTakumakuDir_b58 field_0x8;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkTakumakuQuery_ovb0135() {
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(&field_0x8, 0, 13);
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
    }
};

struct UnkTakumakuEntry_ovb0135 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkTakumakuCharClass_ovb0135 {
    u8 pad_0x0[0x640];
    f32 field_0x640;
    bool field_0x644;
    bool field_0x645;
    u8 pad_0x646[6];
    f32 field_0x64c;
    f32 field_0x650;
};

// An optional target point (unaligned inside the AI component).
struct __attribute__((packed)) UnkTakumakuTarget_ovb0135 {
    bool set;
    u8 pad_0x1[2];
    f32 x, y, z;  // 0x3
};

// The Takumaku AI component (CTakumakuAIComponent).
struct UnkTakumakuAI_ovb0135 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x13d4 - 0x18];
    f32 field_0x13d4;
    u8 pad_0x13d8[0x1610 - 0x13d8];
    UnkReflObject* field_0x1610;
    u8 pad_0x1618[0x14699 - 0x1618];
    UnkTakumakuTarget_ovb0135 target;  // 0x14699
    u8 pad_0x146a8[0x14759 - 0x146a8];
    bool field_0x14759;
};
static_assert(__builtin_offsetof(UnkTakumakuAI_ovb0135, field_0x14759) == 0x14759, "layout");

struct UnkTakumakuDash_ovb0135 {
    void** vtable;
    UnkActor* actor;  // 0x8
    u8 pad_0x10[0x20];
    UnkTakumakuAI_ovb0135* ai;  // 0x30
    s32 state;                  // 0x38
    s32 next;                   // 0x3c
    u8 pad_0x40[0x20];
    f32 countdown;  // 0x60
    bool in_front;  // 0x64
    u8 pad_0x65[3];
    f32 distance;     // 0x68
    f32 field_0x6c;   // 0x6c
    f32 field_0x70;   // 0x70
    bool field_0x74;  // 0x74
    bool started;     // 0x75
};

struct UnkTakumakuStartAttack_b58;
bool fn_71006cfcac(UnkTakumakuStartAttack_b58* self);
struct UnkStruct_7101176ea8;
void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);
bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls);

bool fn_71006cf018(UnkTakumakuDash_ovb0135* self, bool a, bool b);
bool fn_71006cf55c(UnkTakumakuDash_ovb0135* self, f32 dt);
bool fn_71006cee2c(UnkTakumakuDash_ovb0135* self, f32 dt);
bool fn_71006cfab0(UnkTakumakuDash_ovb0135* self);

const CStrId* fn_71006d3e70(UnkTakumakuAI_ovb0135* ai);
const CStrId* fn_71006d40cc(UnkTakumakuAI_ovb0135* ai);

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

template <const base::reflection::CClass* (*Getter)()>
static inline bool IsA(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), Getter());
}

// The target's position: the AI component's target point when it has one.
static inline UnkVec3f TargetPos(UnkTakumakuDash_ovb0135* self, const UnkVec3f& target) {
    UnkVec3f p = target;
    if (self->ai && self->ai->target.set)
        p = *reinterpret_cast<const UnkVec3f*>(&self->ai->target.x);
    return p;
}

// The update: the base's (fn_71003609a8), then the facing and distance to the target and the state's step.
void fn_71006cec98(UnkTakumakuDash_ovb0135* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    self->state = self->next;
    self->in_front = false;
    self->distance = -1.0f;
    if (UnkActor* target = fn_7100d7c990(self->actor)) {
        UnkVec3f target_pos(target->field_0x44, target->field_0x54, target->field_0x64);
        UnkActor* actor = self->actor;
        u8* actor_ai = static_cast<u8*>(actor->field_0x240);
        if (actor_ai && actor_ai[0x121e]) {
            f32 x = actor->field_0x44;
            UnkVec3f pos = TargetPos(self, target_pos);
            f32 d = x - pos.x;
            self->distance = d;
            f32 side = Sign(d);
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
            self->in_front = side == Sign(m.m[2]);
        }
    }
    switch (self->state) {
    case 2:
        if (fn_71006cee2c(self, dt)) {
            self->next = 3;
            return;
        }
        if (fn_71006cf018(self, false, false))
            self->next = 4;
        return;
    case 3:
        if (fn_71006cf018(self, true, false) || fn_71006cf55c(self, dt))
            self->next = 4;
        return;
    case 4:
        if (fn_71006cf018(self, true, false))
            return;
        if (fn_71006cfab0(self))
            return;
        fn_71006cfcac(reinterpret_cast<UnkTakumakuStartAttack_b58*>(self));
        return;
    }
}

// The AI component's animation ids (CStrId-like: compared by their first word).
struct UnkTakumakuId_ovb0135 {
    u64 value;
};
const UnkTakumakuId_ovb0135* fn_71006d3ea4(UnkTakumakuAI_ovb0135* ai);
const UnkTakumakuId_ovb0135* fn_71006d3f34(UnkTakumakuAI_ovb0135* ai);
const u64* fn_71006d3ee4(UnkTakumakuAI_ovb0135* ai);
const u64* fn_71006d3ec8(UnkTakumakuAI_ovb0135* ai);
UnkTakumakuEntry_ovb0135** fn_71006d3ebc(UnkTakumakuAI_ovb0135* ai);
UnkTakumakuEntry_ovb0135** fn_71006d3eb0(UnkTakumakuAI_ovb0135* ai);

// Turns towards the target with the animation for `id`: the turn variant (fn_71006d3ee4 in front,
// fn_71006d3ec8 otherwise) when `id` is one of the two turn ids, the plain one (fn_71006d3ebc,
// fn_71006d3eb0) else; once sent (mode 0) and when the entry is a CCharClassTurnTrack, the owner's
// +0x298 gets the facing query.
void fn_71006d0244(UnkTakumakuDash_ovb0135* self, const UnkTakumakuId_ovb0135* id) {
    void* animation = self->actor->field_0x220;
    if (!animation || !self->ai)
        return;
    bool turn = id->value == fn_71006d3ea4(self->ai)->value || id->value == fn_71006d3f34(self->ai)->value;
    u64 hash;
    if (self->in_front && fn_71006d44a0(self->ai))
        hash = turn ? *fn_71006d3ee4(self->ai) : (*fn_71006d3ebc(self->ai))->hash;
    else
        hash = turn ? *fn_71006d3ec8(self->ai) : (*fn_71006d3eb0(self->ai))->hash;
    u8* entry = static_cast<u8*>(
        fn_7101176d34(static_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(hash)));
    if (!entry)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (!fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask))
        return;
    const base::reflection::CClass* turn_track = fn_7100d2b3a8();
    if (!fn_710006ba58(entry + 0x170, turn_track))
        return;
    void* target = self->actor->field_0x298;
    if (!target)
        return;
    UnkTakumakuQuery_ovb0135 query;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
    query.field_0x8 = m.m[2] >= 0.0f ? g_unk_710164a7ac : g_unk_710164a7a0;
    query.field_0x14 = 1;
    fn_71006f4b0c(target, &query, 0);
}

// The message the attack is told (kind at +0x0, the source at +0x8: its +0x10's id at +0x38).
struct UnkTakumakuMessageSource_ovb0135 {
    u8 pad_0x0[0x10];
    u8* field_0x10;
};
struct UnkTakumakuMessage_ovb0135 {
    s32 kind;
    u8 pad_0x4[4];
    UnkTakumakuMessageSource_ovb0135* source;  // 0x8
};
const CStrId* fn_71006d3e48(UnkTakumakuAI_ovb0135* ai);
bool fn_71006d58b0(UnkTakumakuAI_ovb0135* ai, const CStrId* id);
bool fn_71006d58f4(UnkTakumakuAI_ovb0135* ai, const CStrId* id);
void fn_71006d56d0(UnkTakumakuAI_ovb0135* ai);

static inline bool Same(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

// The animation events: on an animation's end (kind 3) the next state follows from the animation (4 after a
// turn, 2 with the class's delay after the wind-up fn_71006d3e48, 3 with the class's two values after the
// run fn_71006d3e70); on its start (kind 1) a turn starts the turn towards the target (fn_71006d0244).
bool fn_71006d0494(UnkTakumakuDash_ovb0135* self, const UnkTakumakuMessage_ovb0135* message) {
    const CStrId* id = &g_unk_7101d470e0;
    if (message->source) {
        u8* p = message->source->field_0x10;
        id = p ? reinterpret_cast<const CStrId*>(p + 0x38) : &g_unk_7101d470e0;
    }
    if (!self->ai || id->data == &g_unk_7101d47108->empty)
        return true;
    switch (message->kind) {
    case 3: {
        UnkTakumakuCharClass_ovb0135* char_class = nullptr;
        auto* ai_obj = static_cast<UnkReflObject*>(self->actor->field_0x240);
        if (ai_obj && IsA<fn_71006d3ba8>(ai_obj)) {
            UnkReflObject* obj = reinterpret_cast<UnkTakumakuAI_ovb0135*>(ai_obj)->field_0x1610;
            if (obj && IsA<fn_71006d2174>(obj))
                char_class = reinterpret_cast<UnkTakumakuCharClass_ovb0135*>(obj);
        }
        if (fn_71006d58b0(self->ai, id) || fn_71006d58f4(self->ai, id)) {
            self->next = 4;
            fn_71006d56d0(self->ai);
            return true;
        }
        if (Same(*id, *fn_71006d3e48(self->ai))) {
            if (char_class) {
                self->countdown = char_class->field_0x640;
                self->next = 2;
            }
            return true;
        }
        if (!Same(*id, *fn_71006d3e70(self->ai)))
            return true;
        if (char_class) {
            self->field_0x6c = char_class->field_0x64c;
            self->field_0x70 = char_class->field_0x650;
        }
        self->next = 3;
        return true;
    }
    case 1:
        if (Same(*id, *fn_71006d40cc(self->ai))) {
            self->next = 3;
            return true;
        }
        if (!Same(*id, *fn_71006d3e70(self->ai))) {
            if (!fn_71006d58b0(self->ai, id) && !fn_71006d58f4(self->ai, id))
                return true;
            UnkTakumakuId_ovb0135 hash;
            hash.value = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id->data) + 0x30);
            fn_71006d0244(self, &hash);
        }
        self->next = 4;
        return true;
    }
    return true;
}
