// Unit 2702 (hint CAIComponent): resets the AI after the base step: clears its flags, timers, path state, node
// pool and string-id lists, takes the owner's position as home, sets the four tracked ids (when the definition
// tracks and the owner has a track holder), hooks the track callback up to the owner's tracked object, resets
// every part and then runs vf_0x718. Names and layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkScenarioIdList_ovb0097;
void CCharClassEventScenarioComponent_ResizeStrIdList(UnkScenarioIdList_ovb0097* self, u32 n);
void fn_71004db7f8();

inline const CStrId& TrackedId_l0fe740() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    // The slot at +0x24f0 (game/UnkGameManagerIdTable.h names no member there yet).
    return *reinterpret_cast<const CStrId*>(&s_table.pad_0x2440[0x24f0 - 0x2440]);
}

// A 4-byte element whose out-of-line destructor is empty.
struct UnkStateKey_l0fe740 {
    u32 value;
    ~UnkStateKey_l0fe740() { fn_71002874dc(this); }
};

// The target state at +0x68 (the object fn_7100119f98 enables, game/UnkTU108.h).
struct UnkTargetState_l0fe740 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u8 pad_0xc[0x1c];
    u32 field_0x28;
    bool enabled;  // 0x2c
    bool field_0x2d;
    u8 pad_0x2e[0x22];
    u32 field_0x50;
    u8 pad_0x54[0x1c];
    UnkStateKey_l0fe740* keys;  // 0x70
    u32 key_count;              // 0x78
    u8 pad_0x7c[0x1c];
    u32 field_0x98;

    void Reset() {
        field_0x8 = 0;
        field_0x28 = 0;
        enabled = false;
        field_0x2d = true;
        field_0x50 = 0;
        for (u64 i = 0; i < key_count; ++i)
            keys[i].~UnkStateKey_l0fe740();
        key_count = 0;
        field_0x98 = 0;
        fn_7100119f98(reinterpret_cast<UnkTileOutline*>(this), false);
    }
};
static_assert(sizeof(UnkTargetState_l0fe740) == 0xa0, "layout");

// Up to eight string ids with their count in front (4-byte aligned).
struct __attribute__((packed)) UnkIdList_l0fe740 {
    u32 count;
    CStrId ids[8];  // 0x4

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            ids[i].~CStrId();
        count = 0;
    }
    ~UnkIdList_l0fe740() { Clear(); }
};
static_assert(sizeof(UnkIdList_l0fe740) == 0x44, "layout");

// Up to three such lists, with their count in front.
struct __attribute__((packed)) UnkIdListSet_l0fe740 {
    u32 count;
    UnkIdList_l0fe740 lists[3];  // 0x4

    UnkIdList_l0fe740* begin() { return lists; }
    UnkIdList_l0fe740* end() { return &lists[count]; }
    void Clear() {
        for (u64 i = 0; i < count; ++i)
            lists[i].Clear();
        count = 0;
    }
};
static_assert(sizeof(UnkIdListSet_l0fe740) == 0xd0, "layout");

struct UnkResettablePart_l0fe740 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Reset();  // 0x20
};

struct __attribute__((packed)) UnkPartList_l0fe740 {
    u32 count;
    UnkResettablePart_l0fe740* items[25];  // 0x4

    UnkResettablePart_l0fe740** begin() { return items; }
    UnkResettablePart_l0fe740** end() { return &items[count]; }
};

struct UnkPoolNode_l0fe740 {
    u8 pad_0x0[0x18];
    UnkPoolNode_l0fe740* next;  // 0x18
};

struct UnkNodeAllocator_l0fe740;
struct UnkNodeAllocatorVtable_l0fe740 {
    void* unk_0x0[9];
    void (*free)(UnkNodeAllocator_l0fe740* self, UnkPoolNode_l0fe740* node);  // 0x48
};
struct UnkNodeAllocator_l0fe740 {
    UnkNodeAllocatorVtable_l0fe740* vtable;
};

// The track hook: the callback and its context, and the handle on the tracked object.
struct UnkTrackLink_l0fe740 {
    u8 pad_0x0[0x8];
    void* context;          // 0x8
    void (*callback)();     // 0x10
    UnkWeakHandle* handle;  // 0x18
};

struct UnkResetAIDef_l0fe740 {
    u8 pad_0x0[0x41a];
    bool tracks;  // 0x41a
    u8 pad_0x41b[0xc9];
    bool hooks_track;  // 0x4e4
};

struct UnkResetAI_l0fe740;
struct UnkResetAIVtable_l0fe740 {
    void* unk_0x0[0x33];
    bool (*vf_0x198)(UnkResetAI_l0fe740* self);
    void* unk_0x1a0[0xaf];
    void (*vf_0x718)(UnkResetAI_l0fe740* self);
};
static_assert(__builtin_offsetof(UnkResetAIVtable_l0fe740, vf_0x718) == 0x718, "layout");

struct UnkResetAI_l0fe740 {
    UnkResetAIVtable_l0fe740* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x7];
    bool field_0x3f;
    u64 field_0x40;
    bool field_0x48;
    u8 pad_0x49[0x3];
    s32 field_0x4c;
    u8 pad_0x50[0x8];
    u64 field_0x58;
    u8 pad_0x60[0x8];
    UnkTargetState_l0fe740 target_state;  // 0x68
    u8 pad_0x108[0x1000];
    u64 field_0x1108;
    u32 field_0x1110;
    u32 field_0x1114;
    bool field_0x1118;
    u8 pad_0x1119[0x3];
    u32 field_0x111c;
    u32 field_0x1120;
    f32 field_0x1124[7];
    u32 field_0x1140;
    u8 pad_0x1144[0x4];
    u64 field_0x1148;
    bool field_0x1150;
    u8 pad_0x1151[0x3];
    u32 field_0x1154;
    bool field_0x1158;
    u8 pad_0x1159[0x17];
    u32 node_count;  // 0x1170
    u8 pad_0x1174[0x4];
    UnkPoolNode_l0fe740* nodes;  // 0x1178
    UnkPoolNode_l0fe740* last_node;  // 0x1180
    u8 pad_0x1188[0x8];
    UnkNodeAllocator_l0fe740* node_allocator;  // 0x1190
    u8 pad_0x1198[0x48];
    bool field_0x11e0;
    u8 pad_0x11e1[0x40];
    bool field_0x1221;
    u8 pad_0x1222[0x2];
    u8 scenario_ids[0x24];  // 0x1224
    bool field_0x1248;
    u8 pad_0x1249[0x3];
    u32 field_0x124c;
    u32 field_0x1250;
    UnkVec3f home;  // 0x1254
    u8 blackboard[0x8];  // 0x1260
    u8 pad_0x1268[0x16c];
    UnkVec3f field_0x13d4;
    UnkIdListSet_l0fe740 id_lists;  // 0x13e0
    UnkIdList_l0fe740 ids;  // 0x14b0
    u32 field_0x14f4;
    u32 field_0x14f8;
    u8 pad_0x14fc[0x1c];
    u32 field_0x1518;
    u8 pad_0x151c[0x94];
    UnkRefObject* field_0x15b0;
    u8 pad_0x15b8[0x18];
    u16 field_0x15d0;
    u8 pad_0x15d2[0x2];
    u32 field_0x15d4;
    bool field_0x15d8;
    u8 pad_0x15d9[0x3];
    u32 field_0x15dc[3];
    u8 pad_0x15e8[0x28];
    UnkResetAIDef_l0fe740* def;  // 0x1610
    UnkPartList_l0fe740 parts;  // 0x1618
    u16 field_0x16e4;
    bool field_0x16e6;
    u8 pad_0x16e7[0x9];
    u32 field_0x16f0;
    u8 pad_0x16f4[0x1c];
    u32 field_0x1710;
    bool field_0x1714;
    bool field_0x1715;
    u8 pad_0x1716[0x6a];
    CStrId field_0x1780;
    u32 field_0x1788;
    u8 pad_0x178c[0x1];
    bool field_0x178d;
    u8 pad_0x178e[0x2];
    u32 field_0x1790;
    bool field_0x1794;
    u8 pad_0x1795[0x3];
    f32 field_0x1798;
    u16 field_0x179c;
    u8 pad_0x179e[0x2];
    bool field_0x17a0;
    u8 pad_0x17a1[0x12cf7];
    UnkTrackLink_l0fe740 track;  // 0x14498
    u8 pad_0x144b8[0x48];
    f32 field_0x14500[2];
    u8 pad_0x14508[0x14];
    f32 field_0x1451c;
    u8 pad_0x14520[0x114];
    u32 field_0x14634[9];
    u64 field_0x14658[2];
    u32 field_0x14668;
};
static_assert(__builtin_offsetof(UnkResetAI_l0fe740, field_0x1108) == 0x1108, "layout");
static_assert(__builtin_offsetof(UnkResetAI_l0fe740, id_lists) == 0x13e0, "layout");
static_assert(__builtin_offsetof(UnkResetAI_l0fe740, def) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkResetAI_l0fe740, field_0x1780) == 0x1780, "layout");
static_assert(__builtin_offsetof(UnkResetAI_l0fe740, track) == 0x14498, "layout");
static_assert(__builtin_offsetof(UnkResetAI_l0fe740, field_0x14668) == 0x14668, "layout");

void fn_71010fee24(UnkResetAI_l0fe740* self, const CStrId* id, u32 slot);

// Resets every part, then runs the AI's own vf_0x718.
inline void ResetParts(UnkResetAI_l0fe740* self) {
    for (UnkResettablePart_l0fe740* part : self->parts)
        part->Reset();
    self->vtable->vf_0x718(self);
}

void fn_71010fe740(UnkResetAI_l0fe740* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->field_0x178d = false;
    self->field_0x1794 = false;
    self->field_0x1798 = -1.0f;
    self->field_0x4c = -1;
    self->field_0x1790 = 0;
    self->field_0x3f = false;
    self->field_0x58 = 0;
    self->field_0x1110 = 0;
    self->field_0x1250 = 0;
    self->field_0x40 = 0;
    self->field_0x48 = false;
    for (UnkPoolNode_l0fe740* node = self->nodes; node;) {
        UnkPoolNode_l0fe740* next = node->next;
        self->node_allocator->vtable->free(self->node_allocator, node);
        node = next;
    }
    self->node_count = 0;
    self->nodes = nullptr;
    self->last_node = nullptr;
    fn_7101047ea8(self->blackboard);
    self->field_0x1140 = 0;
    self->field_0x111c = 0;
    self->field_0x1120 = 0;
    self->field_0x1118 = true;
    self->field_0x1148 = 0;
    self->field_0x1150 = false;
    self->field_0x1154 = 0;
    self->field_0x1158 = false;
    for (UnkIdList_l0fe740* list = self->id_lists.begin(); list != self->id_lists.end(); ++list)
        list->Clear();
    self->id_lists.Clear();
    self->ids.Clear();
    if (self->def)
        self->field_0x11e0 = self->vtable->vf_0x198(self);

    UnkActor* owner = self->owner;
    self->home = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    self->field_0x13d4.x = -1.0f;
    self->field_0x13d4.y = -1.0f;
    self->field_0x13d4.z = -1.0f;
    self->target_state.Reset();
    self->field_0x1108 = 0;
    self->field_0x1124[0] = -1.0f;
    self->field_0x1124[1] = -1.0f;
    self->field_0x1124[2] = -1.0f;
    self->field_0x1124[3] = -1.0f;
    self->field_0x1124[4] = -1.0f;
    self->field_0x1124[5] = -1.0f;
    self->field_0x1124[6] = -1.0f;
    self->field_0x16e4 = 0;
    self->field_0x16e6 = false;
    self->field_0x16f0 = 0;
    self->field_0x1710 = 0;
    self->field_0x1714 = false;
    self->field_0x1715 = true;
    self->field_0x1780 = g_unk_7101d470e0;
    self->field_0x1788 = 0;
    self->field_0x1518 = 0;
    self->field_0x179c = 0;
    self->field_0x17a0 = false;
    self->field_0x1221 = false;
    CCharClassEventScenarioComponent_ResizeStrIdList(reinterpret_cast<UnkScenarioIdList_ovb0097*>(self->scenario_ids), 0);
    self->field_0x1248 = false;
    self->field_0x124c = 0;
    self->field_0x14f4 = 0;
    self->field_0x14f8 = 0;
    self->field_0x1114 = 0;
    self->field_0x14500[0] = 0.0f;
    self->field_0x14500[1] = -1.0f;
    self->field_0x1451c = -1.0f;
    if (UnkRefObject* p = self->field_0x15b0) {
        if (--p->ref_count == 0)
            p->vf_0x10();
        self->field_0x15b0 = nullptr;
    }
    self->field_0x15d4 = 5;
    self->field_0x15d0 = 0;
    self->field_0x15d8 = false;
    self->field_0x15dc[0] = 0;
    self->field_0x15dc[1] = 0;
    self->field_0x15dc[2] = 0;
    self->field_0x14634[0] = 0;
    self->field_0x14634[1] = 0;
    self->field_0x14634[2] = 0;
    self->field_0x14634[3] = 0;
    self->field_0x14634[4] = 0;
    self->field_0x14634[5] = 0;
    self->field_0x14634[6] = 0;
    self->field_0x14634[7] = 0;
    self->field_0x14634[8] = 0;
    self->field_0x14658[0] = 0;
    self->field_0x14658[1] = 0;
    self->field_0x14668 = 0;

    UnkResetAIDef_l0fe740* def = self->def;
    const bool tracks = def->tracks && self->owner->field_0x220;
    if (tracks) {
        const CStrId id = TrackedId_l0fe740();
        fn_71010fee24(self, &id, 0);
        fn_71010fee24(self, &id, 6);
        fn_71010fee24(self, &id, 11);
        fn_71010fee24(self, &id, 12);
    }
    if (tracks || def->hooks_track) {
        UnkTrackLink_l0fe740* link = &self->track;
        UnkWeakHandle* handle = link->handle;
        link->context = self;
        link->callback = fn_71004db7f8;
        auto* target = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
        if (handle) {
            if (UnkWeakTarget* t = handle->target)
                fn_710117b3f0(t, link);
            if (UnkWeakHandle* p = link->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                link->handle = nullptr;
            }
        }
        if (target) {
            fn_7100316018(target, &link->handle);
            fn_710117b3e8(link->handle ? link->handle->target : nullptr, link);
        }
    }

    ResetParts(self);
}
