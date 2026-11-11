// Unit 1123 of tools/tu_map.py (no name hint). fn_71007033c8: after fn_71007038c0, hands the owner's effect
// sink (+0x218) a parameter block (vtable g_unk_7101a24b80, as
// src/game/CCharClassSetDesiredViewDirToPlayerEvent/CCharClassSetDesiredViewDirToPlayerEvent_Run_b49.cpp builds
// one): a zero at +0x8 from two flags depending on whether the owner's AI is a CAutclastAIComponent, the
// pair at +0xb8 when +0xc4 is set, a zero at +0x10 when both +0xea and +0xeb are set; the request flag
// (+0xe9) is cleared afterwards. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "types.h"

struct UnkVec3_b257c {
    f32 x;
    f32 y;
    f32 z;
};

template <class T>
struct UnkOpt_b257c {
    T value;
    bool has;
};

struct UnkPair_b257c {
    u64 first;
    u32 second;
    bool has;
};

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkSinkParams_b257c {
    void** vtable;
    UnkOpt_b257c<u32> field_0x8;
    UnkOpt_b257c<f32> field_0x10;
    UnkPair_b257c field_0x18;
    UnkOpt_b257c<UnkVec3_b257c> field_0x28;
    u32 kind;  // 0x38
};

extern void* g_unk_7101a24b80[];

struct UnkEffectSink;

struct UnkOwner_b257c {
    u8 pad_0x0[0x218];
    UnkEffectSink* sink;  // 0x218
    u8 pad_0x220[0x20];
    base::reflection::UnkReflObject* ai;  // 0x240
};

struct UnkTU1123_b257c {
    u8 pad_0x0[0x30];
    UnkOwner_b257c* owner;  // 0x30
    u8 pad_0x38[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
    bool field_0xc4;
    u8 pad_0xc5[0x23];
    u8 field_0xe8;
    bool field_0xe9;
    bool field_0xea;
    bool field_0xeb;
    bool field_0xec;
    bool field_0xed;
};

void fn_71007038c0(UnkTU1123_b257c* self);

void fn_71007033c8(UnkTU1123_b257c* self) {
    fn_71007038c0(self);
    if (!self->owner->sink)
        return;
    UnkSinkParams_b257c params;
    params.vtable = g_unk_7101a24b80 + 2;
    params.field_0x8.has = false;
    params.field_0x10.has = false;
    params.kind = 0;
    __builtin_memset(&params.field_0x18, 0, 13);
    __builtin_memset(&params.field_0x28, 0, 13);
    base::reflection::UnkReflObject* ai = self->owner->ai;
    bool set = false;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71003b8554())) {
        if (self->field_0xed && !self->field_0xec)
            set = true;
    } else if (!self->field_0xed || !self->field_0xec) {
        set = true;
    }
    if (set) {
        params.field_0x8.value = 0;
        params.field_0x8.has = true;
    }
    if (self->field_0xc4) {
        u64 first = self->field_0xb8;
        u32 second = self->field_0xc0;
        params.field_0x18.first = first;
        params.field_0x18.second = second;
        params.field_0x18.has = true;
    }
    if (self->field_0xeb && self->field_0xea) {
        params.field_0x10.value = 0;
        params.field_0x10.has = true;
    }
    u32 flags = self->field_0xe9 ? self->field_0xe8 != 0 : 0;
    fn_71010c7e6c(self->owner->sink, &params, flags);
    self->field_0xe9 = false;
}
