// behaviortree::CAutoolFollowPathAction's unit (tools/tu_map.py unit 568): runs the node. For an
// Autool AI whose movement target is in mode 2 and has a path, it sends a follow-path parameter
// block (fn_71006f4b0c) built from the AI's path state; below the tunable's speed limit the block
// asks for a stop. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::CClass;

extern base::reflection::UnkTunable g_unk_71726beb18;

// CTunableCharClassAutoolAIComponent's speed limit (+0x3c).
struct UnkAutoolPathTunable_l3bfe50 {
    u8 pad_0x0[0x3c];
    f32 speed_limit;  // 0x3c
};

struct UnkVec3_l3bfe50 {
    f32 x;
    f32 y;
    f32 z;
};

// An optional value: set fields carry a flag after the value.
struct UnkOptF32_l3bfe50 {
    f32 value;
    bool has;
    UnkOptF32_l3bfe50() : has(false) {}
    void Set(f32 v) {
        value = v;
        has = true;
    }
};

struct UnkOptVec3_l3bfe50 {
    UnkVec3_l3bfe50 value;
    bool has;
    UnkOptVec3_l3bfe50() : has(false) {}
    void Set(const UnkVec3_l3bfe50& v) {
        value.x = v.x;
        value.y = v.y;
        value.z = v.z;
        has = true;
    }
};

// The same with its value cleared too.
struct UnkOptVec3Zeroed_l3bfe50 : UnkOptVec3_l3bfe50 {
    UnkOptVec3Zeroed_l3bfe50() {
        value.x = 0.0f;
        value.y = 0.0f;
        value.z = 0.0f;
        has = false;
    }
};

struct UnkOptPair_l3bfe50 {
    bool has;
    u64 field_0x8;
    u64 field_0x10;
    UnkOptPair_l3bfe50() : has(false), field_0x8(0), field_0x10(0) {}
};

struct UnkPathPoint_l3bfe50 {
    u8 data[0x10];
};

// The follow-path parameter block (vtable g_unk_7101a264d0).
struct UnkFollowPathParams_l3bfe50 {
    void* vtable;
    UnkOptVec3_l3bfe50 position;  // 0x8
    UnkOptF32_l3bfe50 speed;      // 0x18
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u32 kind;  // 0x34
    u8 pad_0x38[4];
    UnkOptF32_l3bfe50 values[8];  // 0x3c
    UnkOptVec3Zeroed_l3bfe50 field_0x7c;
    u8 pad_0x8c[4];
    bool field_0x90;
    u8 pad_0x91;
    bool field_0x92;
    u8 pad_0x93;
    bool field_0x94;
    u8 pad_0x95[7];
    bool field_0x9c;
    u8 pad_0x9d;
    bool field_0x9e;
    UnkOptPair_l3bfe50 pairs[2];  // 0xa0
    bool field_0xd0;
    base::global::CRntVector<UnkPathPoint_l3bfe50> points;  // 0xd8
    bool field_0x100;
    u8 pad_0x101;
    bool field_0x102;
    u8 pad_0x103;
    bool field_0x104;
    u8 pad_0x105;
    bool field_0x106;

    UnkFollowPathParams_l3bfe50()
        : vtable(g_unk_7101a264d0 + 0x10), field_0x20(0), field_0x28(0), field_0x30(false), kind(0),
          field_0x90(false), field_0x92(false), field_0x94(false), field_0x9c(false),
          field_0x9e(false), field_0xd0(false), points(15), field_0x100(false), field_0x102(false),
          field_0x104(false), field_0x106(false) {}
    ~UnkFollowPathParams_l3bfe50() {
        vtable = g_unk_7101a264d0 + 0x10;
        if (points.data) {
            points.count = 0;
            points.allocator->vf_0x48(points.data);
            points.capacity = 0;
        }
    }
};
static_assert(__builtin_offsetof(UnkFollowPathParams_l3bfe50, points) == 0xd8, "layout");
static_assert(sizeof(UnkFollowPathParams_l3bfe50) == 0x108, "layout");

struct UnkAutoolPathState_l3bfe50 {
    f32 speed;  // 0x0
    u8 pad_0x4[4];
    f32 field_0x8;
    f32 field_0xc;
    UnkVec3_l3bfe50 position;  // 0x10
};

struct UnkAutoolCharClass_l3bfe50 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkAutoolCharClass_l3bfe50*)>(vtable[0])(this); }
};

// An AI component of any kind.
struct UnkAIComponent_l3bfe50 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkAIComponent_l3bfe50*)>(vtable[0])(this); }
};

struct UnkAutoolAI_l3bfe50 : UnkAIComponent_l3bfe50 {
    u8 pad_0x8[0x8];
    UnkAutoolCharClass_l3bfe50* char_class;  // 0x10
    u8 pad_0x18[0x14778 - 0x18];
    UnkAutoolPathState_l3bfe50 path;  // 0x14778
};

struct UnkPathMode_l3bfe50 {
    u8 pad_0x0[0x10];
    u32 mode;  // 0x10
};

struct UnkPathSource_l3bfe50 {
    u8 pad_0x0[0x10];
    UnkPathMode_l3bfe50* mode;  // 0x10
};

struct UnkPathTarget_l3bfe50 {
    UnkPathSource_l3bfe50* source;  // 0x0
    u8 pad_0x8[0x8];
    void* path;  // 0x10
};

struct UnkMovement_l3bfe50 {
    u8 pad_0x0[0xb0];
    UnkPathTarget_l3bfe50* target;  // 0xb0
};

struct UnkAutoolOwner_l3bfe50 {
    u8 pad_0x0[0x240];
    UnkAIComponent_l3bfe50* ai;  // 0x240
    u8 pad_0x248[0x50];
    UnkMovement_l3bfe50* movement;  // 0x298
};

struct UnkAutoolFollowPath_l3bfe50 {
    u8 pad_0x0[0x18];
    UnkAutoolOwner_l3bfe50* owner;  // 0x18
};

static inline UnkAutoolAI_l3bfe50* AsAutoolAI(UnkAIComponent_l3bfe50* ai) {
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), fn_71003c2a94()) ? static_cast<UnkAutoolAI_l3bfe50*>(ai) : nullptr;
}

s32 fn_71003bfe50(UnkAutoolFollowPath_l3bfe50* self) {
    UnkAutoolAI_l3bfe50* ai = AsAutoolAI(self->owner->ai);
    UnkMovement_l3bfe50* movement = self->owner->movement;
    if (!ai || !movement)
        return 6;
    // The character class is checked but not used in the original.
    if (UnkAutoolCharClass_l3bfe50* charClass = ai->char_class)
        fn_710009d50c(charClass->vf_0x0(), fn_71003c855c());
    UnkPathTarget_l3bfe50* target = movement->target;
    if (target->source && target->source->mode && target->source->mode->mode == 2 && target->path) {
        UnkAutoolPathState_l3bfe50* state = &ai->path;
        UnkFollowPathParams_l3bfe50 params;
        params.kind = 2;
        params.values[4].Set(state->field_0xc);
        params.position.Set(state->position);
        params.speed.Set(state->field_0x8);
        f32 limit = reinterpret_cast<UnkAutoolPathTunable_l3bfe50*>(&g_unk_71726beb18)->speed_limit;
        if (state->speed <= limit) {
            params.speed.Set(0.0f);
            params.values[4].Set(100.0f);
        }
        fn_71006f4b0c(movement, &params, 0);
    }
    return 2;
}
