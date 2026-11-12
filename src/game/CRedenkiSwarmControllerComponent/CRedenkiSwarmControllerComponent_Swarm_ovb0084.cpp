// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): the swarm controller's states,
// path checks, member handlers and the arrays it keeps. The groups are the 0x148-byte entries at
// +0x50 of the object at +0x10 of +0xe8, counted at its +0x110; the tuning is at +0x560. Names and
// layouts are placeholders.
#include <new>
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/CSwarmControllerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU32.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::CClass;
using base::math::CVector2D;
using base::math::CVector3D;

// ---- fn_710064f184: the reset: the base reset (fn_710073807c), state 0, fn_710073f7bc with 0, +0x534 back to -1, the per-group lists
// (+0x488, +0x500) and flags (+0x4d8) resized to the group count, +0x338 from g_unk_71726fc5ac. ----

struct UnkRedenkiSwarmController;
extern u32 g_unk_71726fc5ac;

// The layout of a base::UnkVector, freed in place.
struct UnkRedenkiSubList_ovb0084 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// An array of those lists; growing it is out of line.
struct UnkRedenkiSubLists_ovb0084 {
    UnkRedenkiSubList_ovb0084* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710065b1b0(UnkRedenkiSubLists_ovb0084* self, u32 n);
void fn_710065b2f0(UnkRedenkiSubLists_ovb0084* self, u32 n);

static inline __attribute__((always_inline)) void ResizeLists(UnkRedenkiSubLists_ovb0084* lists, u32 n,
                                                              void (*grow)(UnkRedenkiSubLists_ovb0084*, u32)) {
    if (lists->count < n) {
        grow(lists, n);
    } else if (lists->count > n) {
        for (u64 i = n; i < lists->count; i++)
            lists->data[i].Free();
        lists->count = n;
    }
}

// The layout of a base::UnkVector of flags.
struct UnkRedenkiFlagArray_Reset_ovb0084 {
    bool* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 new_capacity) {
        bool* p = static_cast<bool*>(allocator->vf_0x40(new_capacity, 8, flags));
        if (data) {
            __builtin_memcpy(p, data, count);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }

    __attribute__((always_inline)) void Resize(u32 n) {
        if (count < n) {
            if (capacity < n)
                Reallocate(n);
            __builtin_memset(data + count, 0, n - count);
            count = n;
        } else if (count > n) {
            count = n;
        }
    }
};

struct UnkRedenkiResetCountList_ovb0084 {
    u8 pad_0x0[0x110];
    u32 count;  // 0x110
};

struct UnkRedenkiResetCountData_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiResetCountList_ovb0084* list;  // 0x10
};

struct UnkRedenkiFullReset_ovb0084 {
    u8 pad_0x0[0xe8];
    UnkRedenkiResetCountData_ovb0084* data;  // 0xe8
    u8 pad_0xf0[0x338 - 0xf0];
    u32 field_0x338;
    u8 pad_0x33c[0x488 - 0x33c];
    UnkRedenkiSubLists_ovb0084 lists;  // 0x488
    u8 pad_0x4b0[0x4d8 - 0x4b0];
    UnkRedenkiFlagArray_Reset_ovb0084 groupFlags;  // 0x4d8
    UnkRedenkiSubLists_ovb0084 otherLists;  // 0x500
    u8 pad_0x528[0x530 - 0x528];
    u32 state;  // 0x530
    f32 field_0x534;
    u32 field_0x538;
};

void fn_710073807c(UnkRedenkiFullReset_ovb0084* self);

void fn_710064f184(UnkRedenkiFullReset_ovb0084* self) {
    fn_710073807c(self);
    self->state = 0;
    fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 0);
    self->field_0x534 = -1.0f;
    self->field_0x538 = 0;
    if (self->data && self->data->list) {
        u32 count = self->data->list->count;
        ResizeLists(&self->lists, count, fn_710065b1b0);
        self->groupFlags.Resize(count);
        ResizeLists(&self->otherLists, count, fn_710065b2f0);
    }
    self->field_0x338 = g_unk_71726fc5ac;
}

// ---- fn_7100650158: entry `index` of the tuning's table (+0x708) in mode 2 when both its ids are set, else the base
// entry (fn_710074e2c0). ----

// As src/game/CSwarmControllerComponent/CSwarmControllerComponent_Entry_A5_b67.cpp defines it, with
// its destructor (fn_7100026f70).
struct UnkCounted_A5_b67 {
    u8 pad_0x0[0x54];
    s32 count;
};

struct UnkCountedRef_A5_b67 {
    UnkCounted_A5_b67* p;
    UnkCountedRef_A5_b67(const UnkCountedRef_A5_b67& other) : p(nullptr) {
        if (other.p) {
            ++other.p->count;
            p = other.p;
        }
    }
};

// The ids are union members so that only fn_7100026f70 releases them.
struct UnkIdPair;

struct UnkSwarmEntry_A5_b67 {
    union {
        CStrId name;
    };
    union {
        CFilePathStrId path;
    };
    UnkCountedRef_A5_b67 ref;
    UnkSwarmEntry_A5_b67(const UnkSwarmEntry_A5_b67& other) : name(other.name), path(other.path), ref(other.ref) {}
    ~UnkSwarmEntry_A5_b67() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
};

struct UnkSwarmController_A5_b67;
UnkSwarmEntry_A5_b67 fn_710074e2c0(UnkSwarmController_A5_b67* self, s32 index);

struct UnkRedenkiEntryTuning_ovb0084 {
    u8 pad_0x0[0x708];
    UnkSwarmEntry_A5_b67 entries[1];  // 0x708
};

struct UnkRedenkiEntryCtrl_ovb0084 {
    u8 pad_0x0[0x390];
    s32 mode;  // 0x390
    u8 pad_0x394[0x560 - 0x394];
    UnkRedenkiEntryTuning_ovb0084* tuning;  // 0x560
};

UnkSwarmEntry_A5_b67 fn_7100650158(UnkRedenkiEntryCtrl_ovb0084* self, s32 index) {
    if (self->mode == 2) {
        UnkSwarmEntry_A5_b67 entry = self->tuning->entries[index];
        if (entry.name.data != &g_unk_7101d47108->empty && entry.path.data != &g_unk_7101d47200->empty)
            return entry;
    }
    return fn_710074e2c0(reinterpret_cast<UnkSwarmController_A5_b67*>(self), index);
}

// ---- fn_71006503ac: after fn_710074c304, every member of every active group that fn_7100744e1c accepts is started
// (fn_7100655280); then fn_710073f888. ----

struct UnkSwarmList_A5_b65;
struct UnkSwarmMember_ovb6;
struct UnkRedenkiController_b55;
struct UnkTarget;
void fn_710074c304(UnkSwarmList_A5_b65* self);
void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target);

struct UnkRedenkiMember_ovb0084;

struct UnkRedenkiGroup_Members_ovb0084 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0xb8 - 0x25];
    UnkRedenkiMember_ovb0084** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];
};

struct UnkRedenkiGroupList_Members_ovb0084 {
    u8 pad_0x0[0x50];
    UnkRedenkiGroup_Members_ovb0084* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    bool IsActive(s64 i) {
        if (i >= count || !groups)
            return false;
        return groups[i].active;
    }
    UnkRedenkiGroup_Members_ovb0084* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkRedenkiData_Members_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiGroupList_Members_ovb0084* list;  // 0x10
};

struct UnkRedenkiCtrl_Members_ovb0084 {
    u8 pad_0x0[0xe8];
    UnkRedenkiData_Members_ovb0084* data;  // 0xe8
};

void fn_710073f888(UnkRedenkiCtrl_Members_ovb0084* self);

void fn_71006503ac(UnkRedenkiCtrl_Members_ovb0084* self) {
    fn_710074c304(reinterpret_cast<UnkSwarmList_A5_b65*>(self));
    for (s32 i = 0; i < self->data->list->count; i++) {
        UnkRedenkiGroupList_Members_ovb0084* list = self->data->list;
        if (!list->IsActive(i))
            continue;
        UnkRedenkiGroup_Members_ovb0084* group = &list->groups[i];
        for (u64 j = 0; j < group->memberCount; j++) {
            UnkRedenkiMember_ovb0084* member = group->members[j];
            if (fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
                fn_7100655280(reinterpret_cast<UnkRedenkiController_b55*>(self), reinterpret_cast<UnkTarget*>(member));
        }
    }
    fn_710073f888(self);
}

// ---- fn_7100652288: the start: ids at +0x318/+0x534/+0x538 from the tuning, then a path from the swarm's position
// (vf_0x2c0, with the player flag of PlayerFlag unless the tuning's +0x58 is set) starts the effect;
// without one, a point near the target goes to the record at +0x31c and fn_710073f7bc gets 1. ----

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkPlayer;
struct UnkSwarmHold_b66a4;
struct UnkSwarmUpdate_b65a4;
struct UnkParent_71007455dc;
struct UnkRedenkiSwarmController;
UnkVec3Result fn_7100745e24(UnkParent_71007455dc* parent);
extern char g_unk_710164a408[];

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Update_b65a4.cpp
// defines them.
struct UnkSwarmPoint_b65a4 {
    u64 value;
};

struct UnkSwarmRecord_b65a4 {
    bool active;
};

struct UnkRedenkiKeyHolder_ovb0084 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkRedenkiBillboard_ovb0084 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkRedenkiKeyHolder_ovb0084* field_0xd0;
};

struct UnkRedenkiChain2_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiBillboard_ovb0084* field_0x10;
};

struct UnkRedenkiChain1_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiChain2_ovb0084* field_0x10;
};

// The player's state as the swarm checks it: the object at +0x208, its +0x2d08, whose +0x10 is the
// state.
struct UnkRedenkiPlayerMode_ovb0084 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkRedenkiPlayerPart_ovb0084 {
    u8 pad_0x0[0x2d08];
    UnkRedenkiPlayerMode_ovb0084* mode;  // 0x2d08
};

struct UnkRedenkiPlayer_ovb0084 {
    u8 pad_0x0[0x208];
    UnkRedenkiPlayerPart_ovb0084* part;  // 0x208
};

// The game manager's two flags at +0xc4 and +0xc5.
struct UnkRedenkiManagerFlags_ovb0084 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    bool field_0xc5;
};

struct UnkRedenkiStartTuning_ovb0084 {
    u8 pad_0x0[0x58];
    bool field_0x58;
    u8 pad_0x59[0x480 - 0x59];
    u32 field_0x480;
    u8 pad_0x484[0x4b4 - 0x484];
    u32 field_0x4b4;
};

struct UnkRedenkiStart_ovb0084 {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkRedenkiChain1_ovb0084* field_0xe8;
    u8 pad_0xf0[0x2c8 - 0xf0];
    u8 path[0x50];  // 0x2c8
    s32 field_0x318;
    UnkSwarmRecord_b65a4 record;  // 0x31c
    u8 pad_0x31d[0x534 - 0x31d];
    u32 field_0x534;
    u32 field_0x538;
    u8 pad_0x53c[0x560 - 0x53c];
    UnkRedenkiStartTuning_ovb0084* tuning;  // 0x560

    bool FindPath(UnkVec3Result* from, void* path, bool flag) {
        return reinterpret_cast<bool (*)(UnkRedenkiStart_ovb0084*, UnkVec3Result*, void*, bool)>(
            vtable[0x2c0 / 8])(this, from, path, flag);
    }
};

static inline bool PlayerFlag() {
    UnkRedenkiPlayer_ovb0084* player =
        reinterpret_cast<UnkRedenkiPlayer_ovb0084*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->part || !player->part->mode || player->part->mode->state != 6)
        return false;
    UnkRedenkiManagerFlags_ovb0084* flags = reinterpret_cast<UnkRedenkiManagerFlags_ovb0084*>(g_unk_7101d47310);
    if (flags->field_0xc5)
        return false;
    return flags->field_0xc4;
}

void fn_7100652288(UnkRedenkiStart_ovb0084* self) {
    UnkSwarmPoint_b65a4 point;
    UnkRedenkiChain1_ovb0084* c1 = self->field_0xe8;
    UnkRedenkiBillboard_ovb0084* billboard = nullptr;
    if (c1 && c1->field_0x10) {
        UnkRedenkiBillboard_ovb0084* obj = c1->field_0x10->field_0x10;
        billboard =
            obj && fn_710009d50c(
                       reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_ovb0084*)>(obj->vtable[0])(obj),
                       fn_710072be34())
                ? obj
                : nullptr;
    }
    self->field_0x318 = -1;
    self->field_0x538 = self->tuning->field_0x4b4;
    self->field_0x534 = self->tuning->field_0x480;
    UnkVec3Result pos = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
    bool flag = !self->tuning->field_0x58 && PlayerFlag();
    if (self->FindPath(&pos, self->path, flag)) {
        void* manager = g_unk_71726da048;
        if (!manager) {
            manager = fn_71000839a0(0xe0, 0x3f);
            fn_7100e1bca4(manager);
            g_unk_71726da048 = manager;
        }
        u64 key = billboard->field_0xd0->field_0x30;
        fn_710074e190(self, fn_7100e1cb50(manager, &key, g_unk_710164a408), 1.0f);
        return;
    }
    UnkSwarmUpdate_b65a4* update = reinterpret_cast<UnkSwarmUpdate_b65a4*>(self);
    UnkVec3Result target = fn_7100745e24(fn_7100739c64(update));
    point.value = 0;
    if (fn_710073fea8(update, &target, &point, 50.0f)) {
        fn_7100737d4c(&self->record, &point);
        self->record.active = true;
    }
    fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 1);
}

// ---- fn_7100655390: whether a route to `point` can be found (fn_710073cbe4) from the target's position, unless the
// player is in state 6 or the area (vf_0x1b8 / vf_0x1b0) does not contain the point. ----

struct UnkSwarmUpdate_b65a4;
struct UnkParent_71007455dc;
UnkVec3Result fn_7100745e24(UnkParent_71007455dc* parent);
struct UnkObject_71002ad0dc;

struct UnkRedenkiScratch_ovb0084 {
    u8 pad_0x0[0x10];
    u8 path[0x8];  // 0x10, the route found (an UnkRedenkiPath_ovb0084)
    u8 pad_0x18[0x12a88 - 0x18];
    UnkRedenkiScratch_ovb0084() { fn_71002acfe4(this); }
    ~UnkRedenkiScratch_ovb0084() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};

struct UnkRedenkiStateSub_ovb0084 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

// What fn_7100df57f4 answers: its state at +0xb4, or (in state 7) the one of its +0x20.
struct UnkRedenkiPlayerState_ovb0084 {
    u8 pad_0x0[0x20];
    UnkRedenkiStateSub_ovb0084* sub;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;  // 0xb4
};

struct UnkRedenkiCheckArea_ovb0084 {
    void** vtable;
    bool Contains(base::math::CVector2D* point) {
        return reinterpret_cast<bool (*)(UnkRedenkiCheckArea_ovb0084*, base::math::CVector2D*)>(vtable[0x50 / 8])(
            this, point);
    }
};

struct UnkRedenkiCheck_ovb0084 {
    void** vtable;
    u8 pad_0x8[0x390 - 0x8];
    u32 mode;  // 0x390

    UnkRedenkiCheckArea_ovb0084* Area(u32 slot) {
        return reinterpret_cast<UnkRedenkiCheckArea_ovb0084* (*)(UnkRedenkiCheck_ovb0084*)>(vtable[slot / 8])(this);
    }
};

struct UnkRedenkiSwarm_ovb0084;
bool fn_710073cbe4(UnkRedenkiSwarm_ovb0084* self, CVector2D* from, CVector2D* to, UnkRedenkiScratch_ovb0084* scratch,
                   bool flag);

bool fn_7100655390(UnkRedenkiCheck_ovb0084* self, CVector2D* point, bool flag) {
    UnkRedenkiPlayerState_ovb0084* player =
        static_cast<UnkRedenkiPlayerState_ovb0084*>(fn_7100df57f4(g_unk_7101d47310));
    s32 state = player->state;
    if (state == 7) {
        if (!player->sub)
            goto check;
        state = player->sub->state;
    }
    if (state == 6)
        return false;
check:
    UnkRedenkiCheckArea_ovb0084* area = self->mode ? self->Area(0x1b8) : self->Area(0x1b0);
    if (area && !area->Contains(point))
        return false;
    UnkRedenkiScratch_ovb0084 scratch;
    UnkParent_71007455dc* target = fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self));
    if (!target)
        return false;
    CVector2D p2;
    UnkVec3Result p = fn_7100745e24(target);
    fn_710007b430(&p2, reinterpret_cast<const base::math::CVector3D*>(&p));
    return fn_710073cbe4(reinterpret_cast<UnkRedenkiSwarm_ovb0084*>(self), &p2, point, &scratch, flag);
}

// ---- fn_7100656238: the far check: when the target is ahead on the path by more than 1.5 times the tuned distance
// (+0x46c), the state becomes 4 or 3 (by +0x554); fn_710065634c's go-ahead makes it 4. ----

struct UnkDaivoSwarm_b0012;
struct UnkSwarmUpdate_b65a4;
struct UnkParent_71007455dc;
struct UnkPathPoints;
f32 fn_71010598e0(UnkPathPoints* self, u32 a, u32 b);

// Read as one 64-bit word: the area in the low half, the index in the high half.
struct UnkRedenkiPathPos_ovb0084 {
    u64 bits;
    s32 Area() const { return (s32)bits; }
    u32 Index() const { return (u32)(bits >> 32); }
};

// The swarm's target as fn_7100739c64 gives it: its place on the path and its distance from that
// point.
struct UnkRedenkiTarget_ovb0084 {
    u8 pad_0x0[0x50];
    UnkRedenkiPathPos_ovb0084 pathPos;  // 0x50
    u8 pad_0x58[0xc];
    f32 pathOffset;  // 0x64
};

struct UnkRedenkiArea_ovb0084 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual s32 vf_0x28();
};

struct UnkRedenkiCloseTuning_ovb0084 {
    u8 pad_0x0[0x46c];
    f32 farDistance;  // 0x46c
    u8 pad_0x470[0x8];
    f32 closeDistance;  // 0x478
};

struct UnkRedenkiSwarmPath_ovb0084 {
    u8 pad_0x0[0x10];
    u32 count;  // 0x10
};

struct UnkRedenkiClose_ovb0084 {
    void** vtable;
    u8 pad_0x8[0x80 - 0x8];
    UnkRedenkiArea_ovb0084* area;  // 0x80
    u8 pad_0x88[0x2c8 - 0x88];
    UnkRedenkiSwarmPath_ovb0084 path;  // 0x2c8
    u8 pad_0x2dc[0x53c - 0x2dc];
    u32 pathIndex;  // 0x53c
    u8 pad_0x540[0x554 - 0x540];
    bool field_0x554;
    u8 pad_0x555[0x560 - 0x555];
    UnkRedenkiCloseTuning_ovb0084* tuning;  // 0x560

    void SetState(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkRedenkiClose_ovb0084*, u32, u32)>(vtable[0x2e8 / 8])(this, a, b);
    }
};

void fn_7100656238(UnkRedenkiClose_ovb0084* self) {
    bool leave = fn_710065634c(reinterpret_cast<UnkDaivoSwarm_b0012*>(self));
    UnkRedenkiTarget_ovb0084* target = reinterpret_cast<UnkRedenkiTarget_ovb0084*>(
        fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self)));
    UnkRedenkiPathPos_ovb0084 pos = target->pathPos;
    u32 targetIndex = pos.Index();
    u32 index = self->pathIndex;
    if (index < targetIndex) {
        f32 distance = 3.40282347e+38f;
        if (self->area->vf_0x28() == pos.Area()) {
            if (self->path.count > targetIndex && self->path.count > index)
                distance = fn_71010598e0(reinterpret_cast<UnkPathPoints*>(&self->path), targetIndex, index) -
                           target->pathOffset;
        }
        if (distance > self->tuning->farDistance * 1.5f)
            self->SetState(self->field_0x554 ? 4 : 3, 0);
    }
    if (leave)
        self->SetState(4, 0);
}

// ---- fn_7100656da8: the attack state's entry: every group's flag (+0x4d8, grown on demand) is cleared, bit 0 of +0x338
// too, +0x558 takes the tuning's +0x4a0 and +0x528 is cleared. ----

// The layout of a base::UnkVector of flags, grown on demand by SetAt.
struct UnkRedenkiFlagArray_Attack_ovb0084 {
    bool* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 new_capacity) {
        bool* p = static_cast<bool*>(allocator->vf_0x40(new_capacity, 8, flags));
        if (data) {
            __builtin_memcpy(p, data, count);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }

    __attribute__((always_inline)) void Resize(u32 n) {
        if (capacity < n)
            Reallocate(n);
        __builtin_memset(data + count, 0, n - count);
        count = n;
    }

    __attribute__((always_inline)) void SetAt(u32 index, bool value) {
        if (growable && count <= index) {
            if (capacity <= index && step != 0) {
                u32 grown = (index / step + 1) * step;
                if (capacity < grown)
                    Reallocate(grown);
            }
            Resize(index + 1);
        }
        data[index] = value;
    }
};

struct UnkRedenkiCountList_ovb0084 {
    u8 pad_0x0[0x110];
    s32 count;  // 0x110
};

struct UnkRedenkiCountData_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiCountList_ovb0084* list;  // 0x10
};

struct UnkRedenkiAttackTuning_ovb0084 {
    u8 pad_0x0[0x4a0];
    u32 field_0x4a0;
};

struct UnkRedenkiAttack_ovb0084 {
    u8 pad_0x0[0xe8];
    UnkRedenkiCountData_ovb0084* data;  // 0xe8
    u8 pad_0xf0[0x338 - 0xf0];
    u32 field_0x338;
    u8 pad_0x33c[0x4d8 - 0x33c];
    UnkRedenkiFlagArray_Attack_ovb0084 groupFlags;  // 0x4d8
    u8 pad_0x500[0x528 - 0x500];
    bool field_0x528;
    u8 pad_0x529[0x558 - 0x529];
    u32 field_0x558;
    u8 pad_0x55c[0x4];
    UnkRedenkiAttackTuning_ovb0084* tuning;  // 0x560
};

void fn_7100656da8(UnkRedenkiAttack_ovb0084* self) {
    s32 count = self->data->list->count;
    for (s32 i = 0; i < count; i++)
        self->groupFlags.SetAt(i, false);
    self->field_0x338 &= ~1u;
    self->field_0x558 = self->tuning->field_0x4a0;
    self->field_0x528 = false;
}

// ---- fn_71006572f0: the state change: leaving state 4 (unless `keep`) clears every group's +0x68/+0x70/+0x78 with
// fn_710074c304 after each; then the new state's entry runs. ----

struct UnkSwarmList_A5_b65;
void fn_710074c304(UnkSwarmList_A5_b65* self);

struct UnkRedenkiResetGroup_ovb0084 {
    u8 pad_0x0[0x68];
    bool field_0x68;
    u8 pad_0x69[0x7];
    u64 field_0x70;
    u32 field_0x78;
    u8 pad_0x7c[0x148 - 0x7c];
};

struct UnkRedenkiResetList_ovb0084 {
    u8 pad_0x0[0x50];
    UnkRedenkiResetGroup_ovb0084* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    UnkRedenkiResetGroup_ovb0084* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkRedenkiResetData_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiResetList_ovb0084* list;  // 0x10
};

struct UnkRedenkiStates_ovb0084 {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkRedenkiResetData_ovb0084* data;  // 0xe8
    u8 pad_0xf0[0x1a4 - 0xf0];
    f32 field_0x1a4;
    u8 pad_0x1a8[0x2bc - 0x1a8];
    u32 field_0x2bc;
    u8 pad_0x2c0[0x530 - 0x2c0];
    u32 state;  // 0x530

    void Enter(u32 slot) {
        reinterpret_cast<void (*)(UnkRedenkiStates_ovb0084*)>(vtable[slot / 8])(this);
    }
};

void fn_71006572f0(UnkRedenkiStates_ovb0084* self, u32 state, bool keep) {
    if (self->state == state)
        return;
    if (self->state == 4 && !keep) {
        self->field_0x2bc = 0;
        self->field_0x1a4 = 0.0f;
        s32 count = self->data->list->count;
        for (s32 i = 0; i < count; i++) {
            UnkRedenkiResetGroup_ovb0084* group = self->data->list->At(i);
            group->field_0x68 = false;
            group->field_0x70 = 0;
            group->field_0x78 = 0;
            fn_710074c304(reinterpret_cast<UnkSwarmList_A5_b65*>(self));
        }
    }
    self->state = state;
    switch (state) {
    case 0:
        self->Enter(0x330);
        return;
    case 1:
        self->Enter(0x308);
        return;
    case 2:
        self->Enter(0x310);
        return;
    case 3:
        self->Enter(0x320);
        return;
    case 4:
        fn_7100656da8(reinterpret_cast<UnkRedenkiAttack_ovb0084*>(self));
        return;
    }
}

// ---- fn_7100658310: a route from the path's last point to `target` (fn_710073cbe4); its points after the first are added
// to the path (fn_71010592b0, fn_7100739bdc). ----

struct UnkObject_71002ad0dc;
struct UnkMinimapPath;
void fn_71010592b0(UnkMinimapPath* path, const CVector3D* point);

struct UnkRedenkiPathPoint_ovb0084 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* Position();  // 0x18
};

// A path as the queries see it: its point count (vf_0x28) and points (vf_0x30).
struct UnkRedenkiPath_ovb0084 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();  // 0x28
    virtual UnkRedenkiPathPoint_ovb0084* Point(u32 i);  // 0x30
};

struct UnkRedenkiSwarm_ovb0084;
bool fn_710073cbe4(UnkRedenkiSwarm_ovb0084* self, CVector2D* from, CVector2D* to, UnkRedenkiScratch_ovb0084* scratch,
                   bool flag);
void fn_7100739bdc(UnkRedenkiSwarm_ovb0084* self, u32 index, UnkRedenkiPath_ovb0084* path);

bool fn_7100658310(UnkRedenkiSwarm_ovb0084* self, UnkRedenkiPath_ovb0084* path, const CVector3D* target, bool flag) {
    UnkRedenkiScratch_ovb0084 scratch;
    CVector2D from;
    fn_710007b430(&from, path->Point(path->Count() - 1)->Position());
    CVector2D to;
    fn_710007b430(&to, target);
    bool found = fn_710073cbe4(self, &from, &to, &scratch, flag);
    if (found) {
        UnkRedenkiPath_ovb0084* route = reinterpret_cast<UnkRedenkiPath_ovb0084*>(scratch.path);
        for (u32 i = 1; i < route->Count(); i++) {
            fn_71010592b0(reinterpret_cast<UnkMinimapPath*>(path), route->Point(i)->Position());
            fn_7100739bdc(self, path->Count() - 1, path);
        }
    }
    return found;
}

// ---- fn_710065a37c: a member's dart: its effect (keyed by the billboard's +0xd0) sets its duration, it returns home
// (+0x10, +0x88) and gets a random time around g_unk_7101a5aed0. ----

extern "C" int rand();

struct UnkTarget;
extern char g_unk_710164a428[];
extern f32 g_unk_7101a5aed0;

static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange(f32 lo, f32 hi) {
    f32 t = Random01();
    return lo + (hi - lo) * t;
}

struct UnkRedenkiFxCtrl_ovb0084 {
    u8 pad_0x0[0xe8];
    UnkRedenkiChain1_ovb0084* field_0xe8;
};

// The effect fn_7100e1cb50 starts: its frame count and speed.
struct UnkRedenkiEffect_ovb0084 {
    u8 pad_0x0[0x8];
    u32 frames;  // 0x8
    u8 pad_0xc[0x14];
    f32 speed;  // 0x20
};

struct UnkRedenkiVec3_ovb0084 {
    f32 x, y, z;
};

struct UnkRedenkiHome_ovb0084 {
    u8 pad_0x0[0x88];
    UnkRedenkiVec3_ovb0084 position;  // 0x88
};

struct UnkRedenkiDart_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiHome_ovb0084* home;  // 0x10
    u8 pad_0x18[0x38 - 0x18];
    UnkRedenkiVec3_ovb0084 position;  // 0x38
    u8 pad_0x44[0x64 - 0x44];
    f32 duration;  // 0x64
    u8 pad_0x68[0x158 - 0x68];
    f32 field_0x158;
    f32 field_0x15c;
    f32 field_0x160;
    f32 field_0x164;
    u8 pad_0x168[0x4];
    f32 field_0x16c;
    u8 pad_0x170[0x180 - 0x170];
    bool field_0x180;
    bool field_0x181;
};

void fn_710065a37c(UnkRedenkiFxCtrl_ovb0084* self, UnkRedenkiDart_ovb0084* member) {
    member->field_0x180 = true;
    UnkRedenkiBillboard_ovb0084* obj = self->field_0xe8->field_0x10->field_0x10;
    UnkRedenkiBillboard_ovb0084* billboard =
        obj && fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_ovb0084*)>(obj->vtable[0])(obj),
                             fn_710072be34())
            ? obj
            : nullptr;
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = billboard->field_0xd0->field_0x30;
    UnkRedenkiEffect_ovb0084* effect =
        static_cast<UnkRedenkiEffect_ovb0084*>(fn_7100e1cb50(manager, &key, g_unk_710164a428));
    f32 duration = (f32)effect->frames / (effect->speed * g_unk_7101a5aed0 * 30.0f);
    fn_7100745440(reinterpret_cast<UnkTarget*>(member), effect, 0);
    member->duration = duration;
    member->field_0x181 = false;
    // Copied as raw words: the x and y together, then the z.
    const UnkRedenkiVec3_ovb0084* from = &member->home->position;
    u64 xy;
    u32 z;
    __builtin_memcpy(&xy, &from->x, sizeof(xy));
    __builtin_memcpy(&z, &from->z, sizeof(z));
    __builtin_memcpy(&member->position.x, &xy, sizeof(xy));
    __builtin_memcpy(&member->position.z, &z, sizeof(z));
    f32 time = RandomRange(g_unk_7101a5aed0 * 0.75f, g_unk_7101a5aed0 * 1.25f);
    member->field_0x158 = time;
    member->field_0x15c = time;
    member->field_0x164 = 0.0f;
    member->field_0x16c = member->field_0x160 * -8.0f / (time * time);
}

// ---- fn_710065a7dc: element `index` of an array of 0x28-byte cells, grown to it first when the array may grow. ----

// An entry of 0x28 bytes: six words and a 16-byte tail copied as raw bytes.
struct UnkRedenkiCell_ovb0084 {
    u32 values[6];
    u8 tail[0x10];  // 0x18

    UnkRedenkiCell_ovb0084() = default;
    UnkRedenkiCell_ovb0084(const UnkRedenkiCell_ovb0084& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
        values[3] = other.values[3];
        values[4] = other.values[4];
        values[5] = other.values[5];
        __builtin_memcpy(tail, other.tail, sizeof(tail));
    }
};
static_assert(sizeof(UnkRedenkiCell_ovb0084) == 0x28, "UnkRedenkiCell_ovb0084 layout");

// The layout of a base::UnkVector of cells, grown on demand by At.
struct UnkRedenkiCellArray_ovb0084 {
    UnkRedenkiCell_ovb0084* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 new_capacity) {
        UnkRedenkiCell_ovb0084* p = static_cast<UnkRedenkiCell_ovb0084*>(
            allocator->vf_0x40((u64)new_capacity * sizeof(UnkRedenkiCell_ovb0084), 8, flags));
        if (data) {
            UnkRedenkiCell_ovb0084* src = data;
            UnkRedenkiCell_ovb0084* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                new (dst) UnkRedenkiCell_ovb0084(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }

    __attribute__((always_inline)) void Resize(u32 n) {
        if (capacity < n)
            Reallocate(n);
        for (u32 i = count; i < n; i++)
            new (&data[i]) UnkRedenkiCell_ovb0084();
        count = n;
    }
};

UnkRedenkiCell_ovb0084* fn_710065a7dc(UnkRedenkiCellArray_ovb0084* self, u32 index) {
    if (self->growable && self->count <= index) {
        if (self->capacity <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (self->capacity < grown)
                self->Reallocate(grown);
        }
        self->Resize(index + 1);
    }
    return &self->data[index];
}

// ---- fn_710065aa30: the swarm's speed: the base (+0x1a4) outside mode 2; else by state from the tuning (+0x45c..+0x464),
// 200, or in state 4 the first while any active flagged group exists. ----

struct UnkRedenkiGroup_Speed_ovb0084 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x148 - 0x25];
};

struct UnkRedenkiGroupList_Speed_ovb0084 {
    u8 pad_0x0[0x50];
    UnkRedenkiGroup_Speed_ovb0084* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    bool IsActive(s64 i) {
        if (i >= count || !groups)
            return false;
        return groups[i].active;
    }
    UnkRedenkiGroup_Speed_ovb0084* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkRedenkiData_Speed_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiGroupList_Speed_ovb0084* list;  // 0x10
};

struct UnkRedenkiTuning_ovb0084 {
    u8 pad_0x0[0x45c];
    f32 field_0x45c;
    f32 field_0x460;
    f32 field_0x464;
};

struct UnkRedenkiCtrl_Speed_ovb0084 {
    u8 pad_0x0[0xe8];
    UnkRedenkiData_Speed_ovb0084* data;  // 0xe8
    u8 pad_0xf0[0x1a4 - 0xf0];
    f32 field_0x1a4;
    u8 pad_0x1a8[0x390 - 0x1a8];
    s32 mode;  // 0x390
    u8 pad_0x394[0x4d8 - 0x394];
    bool* groupFlags;  // 0x4d8
    u8 pad_0x4e0[0x530 - 0x4e0];
    u32 state;  // 0x530
    u8 pad_0x534[0x560 - 0x534];
    UnkRedenkiTuning_ovb0084* tuning;  // 0x560
};

f32 fn_710065aa30(UnkRedenkiCtrl_Speed_ovb0084* self) {
    f32 result = self->field_0x1a4;
    if (self->mode != 2)
        return result;
    switch (self->state) {
    case 0:
        result = self->tuning->field_0x45c;
        break;
    case 1:
        result = 200.0f;
        break;
    case 2:
        result = self->tuning->field_0x460;
        break;
    case 4: {
            UnkRedenkiGroupList_Speed_ovb0084* list = self->data->list;
            for (s32 i = 0; i < list->count; i++) {
                if (list->IsActive(i) && self->groupFlags[i])
                    return self->tuning->field_0x45c;
            }
            return self->tuning->field_0x460;
    }
    case 3:
        result = self->tuning->field_0x464;
        break;
    }
    return result;
}

// ---- fn_710065ab50: a member's message: after the base handler (fn_710074ddd8), the expected type restarts the member;
// in state 1 the member's effect starts and its +0x64 and +0x160 are reset. ----

struct UnkSelf;
struct UnkTarget;
struct UnkMessage;
struct UnkSwarmMember_ovb6;
struct UnkRedenkiController_b55;
void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target);
extern char g_unk_710164a438[];

struct UnkRedenkiHandler_ovb0084 {
    u8 pad_0x0[0xe8];
    UnkRedenkiChain1_ovb0084* field_0xe8;
    u8 pad_0xf0[0x530 - 0xf0];
    u32 state;  // 0x530
};

struct UnkRedenkiTypeRef_ovb0084 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkRedenkiMessage_ovb0084 {
    UnkRedenkiTypeRef_ovb0084* type;
};

struct UnkRedenkiMemberFx_ovb0084 {
    u8 pad_0x0[0x64];
    f32 field_0x64;
    u8 pad_0x68[0x160 - 0x68];
    f32 field_0x160;
};

bool fn_710065ab50(UnkRedenkiHandler_ovb0084* self, UnkRedenkiMemberFx_ovb0084* member,
                   UnkRedenkiMessage_ovb0084* message) {
    UnkTarget* target = reinterpret_cast<UnkTarget*>(member);
    if (fn_710074ddd8(reinterpret_cast<UnkSelf*>(self), target, reinterpret_cast<UnkMessage*>(message)))
        return true;
    if (message->type->id == 0xfab221522cf4e099ull && fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
        fn_7100655280(reinterpret_cast<UnkRedenkiController_b55*>(self), target);
    if (self->state != 1)
        return false;
    UnkRedenkiChain1_ovb0084* c1 = self->field_0xe8;
    UnkRedenkiBillboard_ovb0084* billboard = nullptr;
    if (c1 && c1->field_0x10) {
        UnkRedenkiBillboard_ovb0084* obj = c1->field_0x10->field_0x10;
        billboard =
            obj && fn_710009d50c(
                       reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_ovb0084*)>(obj->vtable[0])(obj),
                       fn_710072be34())
                ? obj
                : nullptr;
    }
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = billboard->field_0xd0->field_0x30;
    fn_7100745440(target, fn_7100e1cb50(manager, &key, g_unk_710164a438), 0);
    member->field_0x64 = 1.0f;
    member->field_0x160 = 0.0f;
    return true;
}

// ---- fn_710065b430: Reserve of an array of named lists (0x38 bytes: a list, a word, a string id), moved over one by one. ----

// The layout of a base::UnkVector, freed when destroyed and left empty when moved.
template <class T>
struct UnkOwnedVector_ovb0084 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkOwnedVector_ovb0084(UnkOwnedVector_ovb0084&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkOwnedVector_ovb0084() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A word copied bytewise.
struct UnkRawWord_ovb0084 {
    u32 value;
    UnkRawWord_ovb0084(const UnkRawWord_ovb0084& other) { __builtin_memcpy(&value, &other.value, sizeof(value)); }
};

// An entry of 0x38 bytes: a list of ids, a value and a name.
struct UnkRedenkiNamedList_ovb0084 {
    UnkOwnedVector_ovb0084<u32> items;
    UnkRawWord_ovb0084 field_0x28;
    base::UnkStrIdMember name;  // 0x30

    UnkRedenkiNamedList_ovb0084(UnkRedenkiNamedList_ovb0084&& other)
        : items(static_cast<UnkOwnedVector_ovb0084<u32>&&>(other.items)), field_0x28(other.field_0x28),
          name(static_cast<base::UnkStrIdMember&&>(other.name)) {}
};
static_assert(sizeof(UnkRedenkiNamedList_ovb0084) == 0x38, "UnkRedenkiNamedList_ovb0084 layout");

void fn_710065b430(base::UnkVector<UnkRedenkiNamedList_ovb0084>* self, u32 n) {
    if (self->capacity >= n)
        return;
    UnkRedenkiNamedList_ovb0084* p = static_cast<UnkRedenkiNamedList_ovb0084*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkRedenkiNamedList_ovb0084), 8, self->flags));
    if (self->data) {
        UnkRedenkiNamedList_ovb0084* src = self->data;
        UnkRedenkiNamedList_ovb0084* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkRedenkiNamedList_ovb0084(static_cast<UnkRedenkiNamedList_ovb0084&&>(*src));
            src->~UnkRedenkiNamedList_ovb0084();
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = n;
}

// ---- fn_710065b578: Resize of an array of 0x20-byte slots, the new ones cleared. ----

// An entry of 0x20 bytes: a flag and seven words, copied field by field.
struct UnkRedenkiSlot_ovb0084 {
    u8 active;
    u32 values[7];  // 0x4

    UnkRedenkiSlot_ovb0084() = default;
    UnkRedenkiSlot_ovb0084(const UnkRedenkiSlot_ovb0084& other) {
        __builtin_memcpy(&active, &other.active, sizeof(active));
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
        values[3] = other.values[3];
        values[4] = other.values[4];
        values[5] = other.values[5];
        values[6] = other.values[6];
    }
};
static_assert(sizeof(UnkRedenkiSlot_ovb0084) == 0x20, "UnkRedenkiSlot_ovb0084 layout");

// The layout of a base::UnkVector of slots.
struct UnkRedenkiSlotArray_ovb0084 {
    UnkRedenkiSlot_ovb0084* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows the array to `n` slots, the new ones cleared.
void fn_710065b578(UnkRedenkiSlotArray_ovb0084* self, u32 n) {
    if (self->capacity < n) {
        UnkRedenkiSlot_ovb0084* p = static_cast<UnkRedenkiSlot_ovb0084*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkRedenkiSlot_ovb0084), 8, self->flags));
        if (self->data) {
            UnkRedenkiSlot_ovb0084* src = self->data;
            UnkRedenkiSlot_ovb0084* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src)
                new (dst) UnkRedenkiSlot_ovb0084(*src);
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (u32 i = self->count; i < n; i++)
        new (&self->data[i]) UnkRedenkiSlot_ovb0084();
    self->count = n;
}
