// behaviortree::CGroundShockerFollowPathAction unit (tools/tu_map.py unit 857): the action's start. After the
// base start, with the owner's CBaseGroundShockerAIComponent and its navigation's
// CCharClassAINavigationComponent, it keeps the path of the AI's target (+0x30, its index +0x38) and hands the
// navigation a follow-path request (vtable g_unk_7101a264d0, kind 2): the path, speed 1, distances 50 and 9999,
// the AI's node (+0x14678) when set and its direction (+0x1467c) when not zero. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;

// An optional value: the value, then whether it is set.
template <class T>
struct UnkOpt_l5a1c40 {
    T value;
    bool has;

    void Set(const T& v) {
        value = v;
        has = true;
    }
};

struct UnkOptVec3_l5a1c40 {
    UnkVec3f value;
    bool has;

    void Clear() {
        value.x = 0.0f;
        value.y = 0.0f;
        value.z = 0.0f;
        has = false;
    }
    void Set(const UnkVec3f& v) {
        value = v;
        has = true;
    }
};

// An optional path, 4-byte aligned.
struct __attribute__((packed, aligned(4))) UnkOptPath_l5a1c40 {
    u64 value;
    bool has;

    void Set(u64 v) {
        value = v;
        has = true;
    }
};

struct UnkParamSlot_l5a1c40 {
    bool flag;
    u64 field_0x8;
    u64 field_0x10;

    void Clear() {
        flag = false;
        field_0x8 = 0;
        field_0x10 = 0;
    }
};

// A list the request owns, freed through its allocator.
struct UnkParamList_l5a1c40 {
    bool flag;
    void* data;
    u32 size;
    u32 capacity;

    void Clear() {
        flag = false;
        data = nullptr;
        size = 0;
        capacity = 0;
    }
};

// A navigation request (CFlyingFollowPathAction's kind, vtable g_unk_7101a264d0).
struct UnkFollowPathRequest_l5a1c40 {
    void* vtable;
    UnkOptVec3_l5a1c40 field_0x8;
    UnkOpt_l5a1c40<f32> speed;  // 0x18
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u32 kind;                                    // 0x34
    UnkOptPath_l5a1c40 path;                     // 0x38
    UnkOpt_l5a1c40<u32> path_index;              // 0x44
    UnkOpt_l5a1c40<f32> field_0x4c;
    UnkOpt_l5a1c40<f32> min_distance;            // 0x54
    UnkOpt_l5a1c40<f32> max_distance;            // 0x5c
    UnkOpt_l5a1c40<f32> field_0x64;
    UnkOpt_l5a1c40<f32> field_0x6c;
    UnkOpt_l5a1c40<f32> field_0x74;
    UnkOptVec3_l5a1c40 direction;                // 0x7c
    f32 field_0x8c;
    bool field_0x90;
    UnkOpt_l5a1c40<bool> field_0x91;
    UnkOpt_l5a1c40<bool> field_0x93;
    s32 node;       // 0x98
    bool has_node;  // 0x9c
    UnkOpt_l5a1c40<bool> field_0x9d;
    UnkParamSlot_l5a1c40 field_0xa0[2];
    UnkParamList_l5a1c40 list;                   // 0xd0
    u32 field_0xe8;
    bool field_0xec;
    u32 field_0xf0;
    base::IAllocator* allocator;               // 0xf8
    bool field_0x100;
    UnkOpt_l5a1c40<bool> field_0x101;
    UnkOpt_l5a1c40<bool> field_0x103;
    UnkOpt_l5a1c40<bool> field_0x105;

    UnkFollowPathRequest_l5a1c40() {
        field_0x8.Clear();
        speed.has = false;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = false;
        kind = 0;
        vtable = g_unk_7101a264d0 + 0x10;
        path.has = false;
        path_index.has = false;
        field_0x4c.has = false;
        min_distance.has = false;
        max_distance.has = false;
        field_0x64.has = false;
        field_0x6c.has = false;
        field_0x74.has = false;
        field_0x90 = false;
        field_0x91.has = false;
        field_0x93.has = false;
        has_node = false;
        field_0x9d.has = false;
        field_0xa0[0].Clear();
        field_0xa0[1].Clear();
        list.Clear();
        direction.Clear();
        field_0xe8 = 10;
        field_0xec = true;
        field_0xf0 = 15;
        allocator = GetDefaultAllocator();
        field_0x100 = false;
        field_0x101.has = false;
        field_0x103.has = false;
        field_0x105.has = false;
    }

    ~UnkFollowPathRequest_l5a1c40() {
        vtable = g_unk_7101a264d0 + 0x10;
        if (list.data) {
            list.size = 0;
            allocator->vf_0x48(list.data);
            list.capacity = 0;
        }
    }
};
static_assert(__builtin_offsetof(UnkFollowPathRequest_l5a1c40, node) == 0x98, "layout");
static_assert(__builtin_offsetof(UnkFollowPathRequest_l5a1c40, list) == 0xd0, "layout");
static_assert(sizeof(UnkFollowPathRequest_l5a1c40) == 0x108, "layout");

struct UnkShockerAI_l5a1c40 : UnkReflObject {
    u8 pad_0x8[0x1350 - 0x8];
    u8 target_part[0x13b8 - 0x1350];  // 0x1350
    u32 path_index;                    // 0x13b8
    u8 pad_0x13bc[0x14678 - 0x13bc];
    s32 node;                          // 0x14678
    UnkVec3f direction;  // 0x1467c
};
static_assert(__builtin_offsetof(UnkShockerAI_l5a1c40, direction) == 0x1467c, "layout");

struct UnkShockerNavClass_l5a1c40 : UnkReflObject {
    u8 pad_0x8[0x74 - 0x8];
    bool field_0x74;
};

struct UnkShockerNavigation_l5a1c40 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
};

struct UnkShockerOwner_l5a1c40 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    UnkShockerNavigation_l5a1c40* navigation;  // 0x298
};

struct UnkShockerTargetLink_l5a1c40 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkShockerPathHolder_l5a1c40 {
    u8 pad_0x0[0x30];
    u8 path[8];  // 0x30
};

struct UnkShockerFollowPath_l5a1c40 {
    u8 pad_0x0[0x18];
    UnkShockerOwner_l5a1c40* owner;  // 0x18
    u8 pad_0x20[0x30 - 0x20];
    void* path;       // 0x30
    u32 path_index;   // 0x38
};

// A component more than 0.001 away from zero.
static inline bool NotZero_l5a1c40(f32 v) {
    return !(v + 0.001f >= 0.0f) || !(v + -0.001f <= 0.0f);
}

void fn_71005a1c40(UnkShockerFollowPath_l5a1c40* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    auto* ai = static_cast<UnkShockerAI_l5a1c40*>(self->owner->ai);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71005a2224()))
        ai = nullptr;
    UnkShockerNavigation_l5a1c40* navigation = self->owner->navigation;
    if (!navigation)
        return;
    UnkReflObject* char_class = navigation->char_class;
    if (!char_class)
        return;
    bool is_nav = fn_710009d50c(char_class->vf_0x0(), fn_71006f5fbc());
    auto* nav_class = is_nav ? static_cast<UnkShockerNavClass_l5a1c40*>(char_class) : nullptr;
    if (!ai || !is_nav)
        return;

    auto* link = reinterpret_cast<UnkShockerTargetLink_l5a1c40*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(ai->target_part)));
    if (link && link->actor) {
        auto* holder = reinterpret_cast<UnkShockerPathHolder_l5a1c40*>(fn_7100382150(link->actor));
        self->path = holder->path;
        self->path_index = ai->path_index;
    }

    UnkFollowPathRequest_l5a1c40 request;
    request.kind = 2;
    request.path.Set(reinterpret_cast<u64>(self->path));
    request.path_index.Set(self->path_index);
    request.speed.Set(1.0f);
    request.min_distance.Set(50.0f);
    request.max_distance.Set(9999.0f);
    request.field_0x101.Set(false);
    request.field_0x105.Set(nav_class->field_0x74);
    request.field_0x9d.Set(true);
    s32 node = ai->node;
    if (node >= 0) {
        request.node = node;
        request.has_node = true;
    }
    UnkVec3f direction = ai->direction;
    if (NotZero_l5a1c40(direction.x) || NotZero_l5a1c40(direction.y) || NotZero_l5a1c40(direction.z))
        request.direction.Set(direction);
    fn_71006f4b0c(navigation, &request, 1);
}
