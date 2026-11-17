// behaviortree::CShineonFollowPathAction's unit (tools/tu_map.py unit 1056): the start. After the base
// start, when the actor's AI is a CShineonAIComponent and the actor has a navigation (+0x298), the AI's
// part (+0x1350) gives the actor whose path the Shineon follows; with a CCharClassShineonAIComponent
// char class the navigation gets a kind-4 follow-path block holding that path and the char class's
// tunables. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU365.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern char g_unk_7101a264a8[];

struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

// As src/game/CMagnetMovablePlatformComponent/CMagnetMovablePlatformComponent_Point_b52.cpp declares it.
struct UnkDashPath;
struct UnkMagnetPathHolder_b52 {
    u8 unk_0x0[0x40];
    UnkDashPath* path;
};
UnkMagnetPathHolder_b52* fn_7100382150(UnkActor* actor);

// The follow-path block's base part (its vtable g_unk_7101a264d0); each value with its "set" flag after it.
struct UnkFollowPathParamsBase_l6bd8bc {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u32 field_0x18;
    bool has_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u32 kind;  // 0x34
    void* path;  // 0x38
    bool has_path;  // 0x40
    u32 field_0x44;
    bool has_0x44;
    u32 field_0x4c;
    bool has_0x4c;
    u32 field_0x54;
    bool has_0x54;
    u32 field_0x5c;
    bool has_0x5c;
    u32 field_0x64;
    bool has_0x64;
    u32 field_0x6c;
    bool has_0x6c;
    u32 field_0x74;
    bool has_0x74;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u8 field_0x88;
    u8 pad_0x89[7];
    u8 field_0x90;
    bool field_0x91;
    bool has_0x91;
    bool field_0x93;
    bool has_0x93;
    u8 pad_0x95[7];
    u8 field_0x9c;
    u8 pad_0x9d;
    u8 field_0x9e;
    u8 pad_0x9f;
    u8 field_0xa0;
    u64 field_0xa8;
    u64 field_0xb0;
    u8 field_0xb8;
    u64 field_0xc0;
    u64 field_0xc8;
    u8 field_0xd0;
    // A byte array (laid out as a CRntVector): the data, count, capacity, growth step, a flag, the
    // allocator's flags and the allocator.
    u8* data;  // 0xd8
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0xec;
    u32 flags;
    base::UnkAllocator* allocator;  // 0xf8
    u8 field_0x100;
    u8 pad_0x101;
    u8 field_0x102;
    u8 pad_0x103;
    u8 field_0x104;
    u8 pad_0x105;
    u8 field_0x106;
    u8 pad_0x107;

    UnkFollowPathParamsBase_l6bd8bc() {
        field_0x8 = 0;
        field_0xc = 0;
        field_0x10 = 0;
        field_0x14 = 0;
        has_0x18 = false;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        kind = 0;
        vtable = g_unk_7101a264d0 + 0x10;
        has_path = false;
        has_0x44 = false;
        has_0x4c = false;
        has_0x54 = false;
        has_0x5c = false;
        has_0x64 = false;
        has_0x6c = false;
        has_0x74 = false;
        field_0x90 = 0;
        has_0x91 = false;
        has_0x93 = false;
        field_0x9c = 0;
        field_0x9e = 0;
        field_0xa0 = 0;
        field_0xa8 = 0;
        field_0xb0 = 0;
        field_0xb8 = 0;
        field_0xc0 = 0;
        field_0xc8 = 0;
        field_0xd0 = 0;
        data = nullptr;
        count = 0;
        capacity = 0;
        field_0x7c = 0;
        field_0x80 = 0;
        field_0x84 = 0;
        field_0x88 = 0;
        step = 10;
        field_0xec = true;
        flags = 15;
        allocator = fn_7100083990();
        field_0x100 = 0;
        field_0x102 = 0;
        field_0x104 = 0;
        field_0x106 = 0;
    }
    ~UnkFollowPathParamsBase_l6bd8bc() {
        vtable = g_unk_7101a264d0 + 0x10;
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(__builtin_offsetof(UnkFollowPathParamsBase_l6bd8bc, field_0x7c) == 0x7c, "layout");
static_assert(__builtin_offsetof(UnkFollowPathParamsBase_l6bd8bc, data) == 0xd8, "layout");
static_assert(sizeof(UnkFollowPathParamsBase_l6bd8bc) == 0x108, "layout");

// The kind-4 follow-path block (vtable g_unk_7101a264a8).
struct UnkFollowPathParams_l6bd8bc : UnkFollowPathParamsBase_l6bd8bc {
    u8 pad_0x108[4];
    u8 field_0x10c;
    u8 pad_0x10d[7];
    u8 field_0x114;
    u8 pad_0x115;
    u8 field_0x116;
    u8 pad_0x117;
    u8 field_0x118;
    u8 pad_0x119;
    u8 field_0x11a;
    u8 pad_0x11b[5];
    u8 field_0x120;
    u8 pad_0x121[7];
    u8 field_0x128;
    u8 pad_0x129;
    u8 field_0x12a;
    u8 pad_0x12b[5];
    u8 field_0x130;
    u8 pad_0x131[7];
    u8 field_0x138;
    bool field_0x139;
    bool has_0x139;
    bool field_0x13b;
    bool has_0x13b;
    u32 field_0x140;
    bool has_0x140;
    u32 field_0x148;
    bool has_0x148;
    u8 pad_0x14d;
    u8 field_0x14e;
    u8 pad_0x14f;
    u8 field_0x150;
    u32 field_0x154;
    u8 field_0x158;
    u32 field_0x15c;
    u8 field_0x160;
    u32 field_0x164;
    bool has_0x164;
    u32 field_0x16c;
    u8 field_0x170;
    u8 pad_0x171[7];

    UnkFollowPathParams_l6bd8bc() {
        vtable = g_unk_7101a264a8 + 0x10;
        kind = 4;
        field_0x10c = 0;
        field_0x114 = 0;
        field_0x116 = 0;
        field_0x118 = 0;
        field_0x11a = 0;
        field_0x120 = 0;
        field_0x128 = 0;
        field_0x12a = 0;
        field_0x130 = 0;
        field_0x138 = 0;
        has_0x139 = false;
        has_0x13b = false;
        has_0x140 = false;
        has_0x148 = false;
        field_0x14e = 0;
        field_0x150 = 0;
        field_0x158 = 0;
        field_0x160 = 0;
        has_0x164 = false;
        field_0x170 = 0;
    }
};
static_assert(__builtin_offsetof(UnkFollowPathParams_l6bd8bc, field_0x10c) == 0x10c, "layout");
static_assert(__builtin_offsetof(UnkFollowPathParams_l6bd8bc, field_0x140) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkFollowPathParams_l6bd8bc, has_0x164) == 0x168, "layout");
static_assert(sizeof(UnkFollowPathParams_l6bd8bc) == 0x178, "layout");

struct UnkShineonCharClass_l6bd8bc : UnkReflObject {
    u8 pad_0x8[0x504];
    u32 field_0x50c;
    u32 field_0x510;
    u32 field_0x514;
    u32 field_0x518;
    u32 field_0x51c;
};
static_assert(__builtin_offsetof(UnkShineonCharClass_l6bd8bc, field_0x50c) == 0x50c, "layout");

struct UnkShineonAI_l6bd8bc : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* charclass;  // 0x10
    u8 pad_0x18[0x1338];
    u8 part[0x68];  // 0x1350
    u32 field_0x13b8;
};
static_assert(__builtin_offsetof(UnkShineonAI_l6bd8bc, part) == 0x1350, "layout");
static_assert(__builtin_offsetof(UnkShineonAI_l6bd8bc, field_0x13b8) == 0x13b8, "layout");

struct UnkShineonFollowPathAction_l6bd8bc {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

void fn_71006bd8bc(UnkShineonFollowPathAction_l6bd8bc* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* object = static_cast<UnkReflObject*>(self->actor->field_0x240);
    if (!object)
        return;
    bool is_ai = fn_710009d50c(object->vf_0x0(), fn_71006bec7c());
    UnkShineonAI_l6bd8bc* ai = is_ai ? static_cast<UnkShineonAI_l6bd8bc*>(object) : nullptr;
    if (!is_ai)
        return;
    void* navigation = self->actor->field_0x298;
    if (!navigation)
        return;
    UnkFollowPathParams_l6bd8bc params;
    UnkBreakableTarget* target = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(ai->part));
    if (!target || !target->field_0x10)
        return;
    UnkMagnetPathHolder_b52* holder = fn_7100382150(static_cast<UnkActor*>(target->field_0x10));
    UnkReflObject* cc = ai->charclass;
    if (!cc || !fn_710009d50c(cc->vf_0x0(), fn_71006bddd4()))
        return;
    UnkShineonCharClass_l6bd8bc* charclass = static_cast<UnkShineonCharClass_l6bd8bc*>(cc);
    params.path = &holder->unk_0x0[0x30];
    params.has_path = true;
    params.field_0x44 = static_cast<UnkShineonAI_l6bd8bc*>(self->actor->field_0x240)->field_0x13b8;
    params.has_0x44 = true;
    params.field_0x18 = charclass->field_0x510;
    params.has_0x18 = true;
    params.field_0x91 = false;
    params.has_0x91 = true;
    params.field_0x93 = false;
    params.has_0x93 = true;
    params.field_0x5c = charclass->field_0x50c;
    params.has_0x5c = true;
    params.field_0x139 = true;
    params.has_0x139 = true;
    params.field_0x13b = true;
    params.has_0x13b = true;
    params.field_0x148 = charclass->field_0x514;
    params.has_0x148 = true;
    params.field_0x140 = charclass->field_0x518;
    params.has_0x140 = true;
    params.field_0x164 = charclass->field_0x51c;
    params.has_0x164 = true;
    fn_71006f4b0c(navigation, &params, 1);
}
