// behaviortree::CNailongFollowPathAction's unit (tools/tu_map.py unit 935): the start. After the base start it
// sends the actor's target (+0x298) a follow-path request for the Nailong AI's path, with the speeds and
// distances of its char class for the alerted or the calm state. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU365.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern char g_unk_7101a264a8[];

struct UnkNailongCharClass_l60e308 : UnkReflObject {
    u8 pad_0x8[0x734];
    u32 field_0x73c;
    u32 field_0x740;
    u32 field_0x744;
    u32 field_0x748;
    u32 field_0x74c;
    u32 field_0x750;
    u32 field_0x754;
    u32 field_0x758;
    u32 field_0x75c;
    u32 field_0x760;
    u32 field_0x764;
    u32 field_0x768;
};
static_assert(__builtin_offsetof(UnkNailongCharClass_l60e308, field_0x768) == 0x768, "layout");

struct UnkNailongAI_l60e308 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* charclass;  // 0x10
    u8 pad_0x18[0x1338];
    u8 part[0x68];  // 0x1350, what fn_71002a76a4 takes
    u32 field_0x13b8;
    u8 pad_0x13bc[0x13474];
    UnkWeakHandle* path;  // 0x14830
};
static_assert(__builtin_offsetof(UnkNailongAI_l60e308, field_0x13b8) == 0x13b8, "layout");
static_assert(__builtin_offsetof(UnkNailongAI_l60e308, path) == 0x14830, "layout");

bool fn_7100622194(UnkNailongAI_l60e308* ai);

// The object the AI's path handle points at: the path is its member at +0x30.
struct UnkNailongPathOwner_l60e308 {
    u8 pad_0x0[0x30];
    u8 path[0x8];  // 0x30
};

struct UnkNailongFollowPathAction_l60e308 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

// A counted hold on a weak handle while its target is read.
struct UnkHeldHandle_l60e308 {
    UnkWeakHandle* handle;

    explicit UnkHeldHandle_l60e308(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHeldHandle_l60e308() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
    UnkWeakTarget* Target() const { return handle ? handle->target : nullptr; }
};

struct UnkFollowSlot_l60e308 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u32 field_0x8 = 0;
    u8 field_0xc = 0;
};

struct UnkFollowFlagged_l60e308 {
    bool flag = false;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
};

// A value of the request and whether it is given.
struct UnkFollowValue_l60e308 {
    u32 value;
    bool set;

    void Set(u32 v) {
        value = v;
        set = true;
    }
};

// The follow-path request's base part (CFlyingFollowPathAction's, vtable g_unk_7101a264d0); each value has a
// flag that says it is given.
struct UnkFollowParamsBase_l60e308 {
    void* vtable = g_unk_7101a264d0 + 0x10;  // 0x0
    UnkFollowSlot_l60e308 field_0x8;  // 0x8
    f32 field_0x18;
    bool field_0x1c = false;
    u64 field_0x20 = 0;
    u64 field_0x28 = 0;
    bool field_0x30 = false;
    u32 kind = 0;  // 0x34
    void* path;  // 0x38, set with has_path
    bool has_path = false;  // 0x40
    UnkFollowValue_l60e308 values[7];  // 0x44
    UnkFollowSlot_l60e308 field_0x7c;  // 0x7c
    u8 pad_0x8c[0x4];
    bool field_0x90 = false;
    bool field_0x91;
    bool field_0x92 = false;
    bool field_0x93;
    bool field_0x94 = false;
    u8 pad_0x95[0x7];
    bool field_0x9c = false;
    u8 pad_0x9d;
    bool field_0x9e = false;
    UnkFollowFlagged_l60e308 field_0xa0;  // 0xa0
    UnkFollowFlagged_l60e308 field_0xb8;  // 0xb8
    bool field_0xd0 = false;
    base::global::CRntVector<u32> list{15};  // 0xd8
    bool field_0x100 = false;
    u8 pad_0x101;
    bool field_0x102 = false;
    u8 pad_0x103;
    bool field_0x104 = false;
    u8 pad_0x105;
    bool field_0x106 = false;
    u8 pad_0x107;

    UnkFollowParamsBase_l60e308() {
        for (u32 i = 0; i < 7; ++i)
            values[i].set = false;
    }
    ~UnkFollowParamsBase_l60e308() {
        vtable = g_unk_7101a264d0 + 0x10;
        list.Free();
    }
};
static_assert(__builtin_offsetof(UnkFollowParamsBase_l60e308, list) == 0xd8, "layout");
static_assert(sizeof(UnkFollowParamsBase_l60e308) == 0x108, "layout");

// The Nailong's request (vtable g_unk_7101a264a8, kind 4).
struct UnkFollowParams_l60e308 : UnkFollowParamsBase_l60e308 {
    u8 pad_0x108[0x4];
    bool field_0x10c;
    u8 pad_0x10d[0x7];
    bool field_0x114;
    u8 pad_0x115;
    bool field_0x116;
    u8 pad_0x117;
    bool field_0x118;
    u8 pad_0x119;
    bool field_0x11a;
    u8 pad_0x11b[0x5];
    bool field_0x120;
    u8 pad_0x121[0x7];
    bool field_0x128;
    u8 pad_0x129;
    bool field_0x12a;
    u8 pad_0x12b[0x5];
    bool field_0x130;
    u8 pad_0x131[0x7];
    bool field_0x138;
    bool field_0x139;
    bool field_0x13a;
    bool field_0x13b;
    bool field_0x13c;
    u8 pad_0x13d[0x3];
    u32 field_0x140;
    bool field_0x144;
    u8 pad_0x145[0x3];
    u32 field_0x148;
    bool field_0x14c;
    u8 pad_0x14d;
    bool field_0x14e;
    u8 pad_0x14f;
    bool field_0x150;
    u8 pad_0x151[0x7];
    bool field_0x158;
    u8 pad_0x159[0x7];
    bool field_0x160;
    u8 pad_0x161[0x3];
    u32 field_0x164;
    bool field_0x168;
    u8 pad_0x169[0x3];
    u32 field_0x16c;
    bool field_0x170;
    u8 pad_0x171[0x7];

    UnkFollowParams_l60e308() {
        vtable = g_unk_7101a264a8 + 0x10;
        field_0x10c = false;
        field_0x114 = false;
        field_0x116 = false;
        field_0x118 = false;
        field_0x11a = false;
        field_0x120 = false;
        field_0x128 = false;
        field_0x12a = false;
        field_0x130 = false;
        field_0x138 = false;
        field_0x13a = false;
        field_0x13c = false;
        field_0x144 = false;
        field_0x14c = false;
        field_0x14e = false;
        field_0x150 = false;
        field_0x158 = false;
        field_0x160 = false;
        field_0x168 = false;
        field_0x170 = false;
        kind = 4;
    }
};
static_assert(__builtin_offsetof(UnkFollowParams_l60e308, field_0x16c) == 0x16c, "layout");
static_assert(sizeof(UnkFollowParams_l60e308) == 0x178, "layout");

static inline UnkNailongAI_l60e308* OwnerAI(UnkNailongFollowPathAction_l60e308* self) {
    return static_cast<UnkNailongAI_l60e308*>(self->actor->field_0x240);
}

// The AI's char class, checked each time it is read.
static inline UnkNailongCharClass_l60e308* CharClass(UnkNailongAI_l60e308* ai) {
    UnkReflObject* object = ai->charclass;
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), fn_7100614184()) ? static_cast<UnkNailongCharClass_l60e308*>(object)
                                                             : nullptr;
}

void fn_710060e308(UnkNailongFollowPathAction_l60e308* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* object = static_cast<UnkReflObject*>(self->actor->field_0x240);
    if (!object)
        return;
    bool is_nailong = fn_710009d50c(object->vf_0x0(), fn_710061f464());
    UnkNailongAI_l60e308* ai = is_nailong ? static_cast<UnkNailongAI_l60e308*>(object) : nullptr;
    if (!is_nailong)
        return;
    void* target = self->actor->field_0x298;
    if (!target)
        return;

    UnkFollowParams_l60e308 params;
    UnkBreakableTarget* follow = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(ai->part));
    if (!follow || !reinterpret_cast<UnkWeakHandle*>(follow)->target)
        return;
    UnkWeakTarget* owner;
    {
        UnkHeldHandle_l60e308 held(ai->path);
        owner = held.Target();
    }
    params.path = reinterpret_cast<UnkNailongPathOwner_l60e308*>(owner)->path;
    params.has_path = true;
    params.field_0x18 = 1.0f;
    params.field_0x1c = true;
    params.values[0].Set(OwnerAI(self)->field_0x13b8);
    params.field_0x91 = false;
    params.field_0x92 = true;
    params.field_0x93 = false;
    params.field_0x94 = true;
    params.field_0x139 = true;
    params.field_0x13a = true;
    params.field_0x13b = true;
    params.field_0x13c = true;

    if (fn_7100622194(ai)) {
        params.values[3].Set(CharClass(ai)->field_0x764);
        params.field_0x16c = CharClass(ai)->field_0x760;
        params.field_0x170 = true;
        params.field_0x148 = CharClass(ai)->field_0x768;
        params.field_0x14c = true;
        params.field_0x140 = CharClass(ai)->field_0x758;
        params.field_0x144 = true;
        params.values[1].Set(CharClass(ai)->field_0x754);
        params.field_0x164 = CharClass(ai)->field_0x75c;
        params.field_0x168 = true;
    } else {
        params.values[3].Set(CharClass(ai)->field_0x74c);
        params.field_0x16c = CharClass(ai)->field_0x748;
        params.field_0x170 = true;
        params.field_0x148 = CharClass(ai)->field_0x750;
        params.field_0x14c = true;
        params.field_0x140 = CharClass(ai)->field_0x740;
        params.field_0x144 = true;
        params.values[1].Set(CharClass(ai)->field_0x73c);
        params.field_0x164 = CharClass(ai)->field_0x744;
        params.field_0x168 = true;
    }
    fn_71006f4b0c(target, &params, 1);
}
