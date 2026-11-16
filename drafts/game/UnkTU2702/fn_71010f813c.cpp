// CAIComponent unit (tools/tu_map.py unit 2702): the AI component's constructor: the component base, the
// vtable, then every member in order (lists, bounds, the blackboard at +0x1260, callback sets, pools and
// the navigation path). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CComponent.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU341.h"
#include "game/UnkTU39.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;

struct CBlackboard;
void fn_7101046974(CBlackboard* blackboard);

extern void* g_unk_7101cb1dd0[];  // CAIComponent's vtable
void fn_71010f8804(void* self, u32 n, u32 a, u32 flags, base::UnkAllocator* allocator);
void fn_71010f8930(void* self, u32 n, u32 a, u32 flags, base::UnkAllocator* allocator);
void fn_71010f8a48(void* self, u32 n, base::UnkAllocator* allocator, u32 flags);
void fn_7101115fb8(void* self);
void fn_7101115f48(void* self);

// A list with the bounds of what it holds (empty: the minimum above the maximum).
struct UnkBoundsList_l0f813c {
    CRntVector<void*> items;
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    u32 field_0x40;

    UnkBoundsList_l0f813c()
        : items(7), field_0x28(0), field_0x2c(false), field_0x2d(true), min_x(__FLT_MAX__), min_y(__FLT_MAX__),
          max_x(-__FLT_MAX__), max_y(-__FLT_MAX__), field_0x40(0) {}
};
static_assert(sizeof(UnkBoundsList_l0f813c) == 0x48, "");

// A callback set: its base keeps one list, the class a second.
struct UnkCallbackSetBase_l0f813c {
    void** vtable;
    CRntVector<void*> items;  // 0x8
    u32 field_0x30;

    UnkCallbackSetBase_l0f813c() : vtable(g_unk_7101a20998 + 2), items(0xf), field_0x30(0) {}
};

struct UnkCallbackSet_l0f813c : UnkCallbackSetBase_l0f813c {
    u64 field_0x38;
    CRntVector<void*> pending;  // 0x40
    u32 field_0x68;

    UnkCallbackSet_l0f813c() : field_0x38(0), pending(0xf), field_0x68(0) { vtable = g_unk_7101a20950 + 2; }
};
static_assert(sizeof(UnkCallbackSet_l0f813c) == 0x70, "");

// A method bound to an object.
struct UnkBoundCall_l0f813c {
    void* object;
    void (*method)(void* self);
};

// Three lists and a flag.
struct UnkListGroup_l0f813c {
    CRntVector<void*> a;
    CRntVector<void*> b;
    CRntVector<void*> c;
    bool field_0x78;

    UnkListGroup_l0f813c() : a(0xf), b(0xf), c(0xf), field_0x78(false) {}
};
static_assert(sizeof(UnkListGroup_l0f813c) == 0x80, "");

// An entry of the named table (0x1c bytes).
struct UnkNamedEntry_l0f813c {
    u8 pad_0x0[0x1c];
};

// A named table of 20 entries with two callbacks bound to the component.
struct UnkNamedTable_l0f813c {
    void** vtable;
    CRntString name;              // 0x8
    u64 capacity;                 // 0x38
    UnkNamedEntry_l0f813c* items;  // 0x40
    u64 field_0x48;
    u64 field_0x50;
    UnkBoundCall_l0f813c on_a;  // 0x58
    u64 field_0x68;
    UnkBoundCall_l0f813c on_b;  // 0x70
};

struct UnkAIComponent_l0f813c {
    void** vtable;
    u8 pad_0x8[0x35];
    bool field_0x3d;
    u8 pad_0x3e;
    u8 field_0x3f[10];
    u8 pad_0x49[3];
    s32 field_0x4c;
    u8 field_0x50[0x13];
    u8 pad_0x63[5];
    UnkBoundsList_l0f813c bounds_0x68;
    CRntVector<void*> field_0xb0;
    CRntVector<void*> field_0xd8;
    u32 field_0x100;
    u8 pad_0x104[0x1108 - 0x104];
    u64 field_0x1108;
    u64 field_0x1110;
    bool field_0x1118;
    u8 pad_0x1119[3];
    f32 field_0x111c[2];
    f32 field_0x1124[2];
    f32 field_0x112c[2];
    f32 field_0x1134[2];
    f32 field_0x113c[2];
    u8 pad_0x1144[4];
    u64 field_0x1148;
    bool field_0x1150;
    u8 pad_0x1151[3];
    u32 field_0x1154;
    bool field_0x1158;
    u8 pad_0x1159[3];
    f32 field_0x115c[2];
    u8 pad_0x1164[4];
    u8 pool_0x1168[0x78];
    bool field_0x11e0;
    u8 pad_0x11e1[0x1204 - 0x11e1];
    f32 field_0x1204[2];
    f32 field_0x120c[2];
    f32 field_0x1214[2];
    bool field_0x121c;
    bool field_0x121d;
    u8 field_0x121e[4];
    u8 pad_0x1222[2];
    u32 field_0x1224;
    u8 pad_0x1228[0x20];
    bool field_0x1248;
    u8 pad_0x1249[3];
    u32 field_0x124c[5];
    alignas(8) u8 blackboard[0xf0];  // 0x1260 (CBlackboard)
    UnkCallbackSet_l0f813c callbacks_0x1350;
    u64 field_0x13c0;
    f32 field_0x13c8[2];
    u16 field_0x13d0;
    bool field_0x13d2;
    u8 pad_0x13d3;
    f32 field_0x13d4[2];
    f32 field_0x13dc[2];
    u8 pad_0x13e4[0x14b0 - 0x13e4];
    u32 field_0x14b0;
    u8 pad_0x14b4[0x14f4 - 0x14b4];
    u8 field_0x14f4[0x28];
    u8 pad_0x151c[4];
    u8 pool_0x1520[0x70];
    void* field_0x1590[8];
    u16 field_0x15d0;
    u8 pad_0x15d2[2];
    u32 field_0x15d4;
    bool field_0x15d8;
    u8 pad_0x15d9[3];
    u32 field_0x15dc[5];
    UnkBoundCall_l0f813c on_update;  // 0x15f0
    UnkNavMeshHit hit_0x1600;
    u8 pad_0x160c[4];
    u64 field_0x1610;
    u32 field_0x1618;
    u8 pad_0x161c[0x16e4 - 0x161c];
    u16 field_0x16e4;
    UnkBoundsList_l0f813c bounds_0x16e8;
    UnkBoundsList_l0f813c bounds_0x1730;
    u32 field_0x1778;
    u8 pad_0x177c[4];
    CStrId field_0x1780;
    u32 field_0x1788;
    u16 field_0x178c;
    u8 pad_0x178e[2];
    u32 field_0x1790;
    bool field_0x1794;
    u8 pad_0x1795[3];
    f32 field_0x1798;
    u16 field_0x179c;
    u8 pad_0x179e[2];
    u16 field_0x17a0;
    u8 pad_0x17a2[6];
    UnkNamedTable_l0f813c table_0x17a8;
    UnkListGroup_l0f813c lists_0x1828;
    UnkListGroup_l0f813c lists_0x18a8;
    UnkListGroup_l0f813c lists_0x1928;
    u64 field_0x19a8;
    u64 field_0x19b0;
    u64 field_0x19b8;
    bool field_0x19c0;
    u8 pad_0x19c1[7];
    u8 path_0x19c8[0x14450 - 0x19c8];
    UnkNavMeshHit hit_0x14450;
    f32 field_0x1445c[2];
    UnkNavMeshHit hit_0x14464;
    f32 field_0x14470[2];
    UnkNavMeshHit hit_0x14478;
    u32 field_0x14484[2];
    u32 field_0x1448c[2];
    bool field_0x14494;
    u8 pad_0x14495[3];
    void** listener_vtable;  // 0x14498
    u64 field_0x144a0;
    u64 field_0x144a8;
    u64 field_0x144b0;
    u8 field_0x144b8[0x50];
    u64 field_0x14508;
    bool field_0x14510;
    u8 pad_0x14511[3];
    u32 field_0x14514;
    u8 pad_0x14518[4];
    f32 field_0x1451c;
    CRntVector<void*> field_0x14520;
    CRntVector<void*> field_0x14548;
    u64 field_0x14570;
    u8 field_0x14578[0xb0];
    u8 field_0x14628[0x44];
    u8 pad_0x1466c[0x10];
    u32 field_0x1467c[2];
    u32 field_0x14684;
    u8 pad_0x14688;
    bool field_0x14689;
    u8 pad_0x1468a[0x1469c - 0x1468a];
    u8 field_0x1469c[0x10];
};
static_assert(__builtin_offsetof(UnkAIComponent_l0f813c, callbacks_0x1350) == 0x1350, "");
static_assert(__builtin_offsetof(UnkAIComponent_l0f813c, table_0x17a8) == 0x17a8, "");
static_assert(__builtin_offsetof(UnkAIComponent_l0f813c, hit_0x14450) == 0x14450, "");
static_assert(__builtin_offsetof(UnkAIComponent_l0f813c, field_0x14578) == 0x14578, "");
static_assert(__builtin_offsetof(UnkAIComponent_l0f813c, field_0x1469c) == 0x1469c, "");

void fn_71010f813c(UnkBtAI_A2_b76* component) {
    UnkAIComponent_l0f813c* self = reinterpret_cast<UnkAIComponent_l0f813c*>(component);
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101cb1dd0 + 2;
    self->field_0x3d = false;
    __builtin_memset(self->field_0x3f, 0, sizeof(self->field_0x3f));
    self->field_0x4c = -1;
    __builtin_memset(self->field_0x50, 0, sizeof(self->field_0x50));
    new (&self->bounds_0x68) UnkBoundsList_l0f813c;
    new (&self->field_0xb0) CRntVector<void*>(0xf);
    new (&self->field_0xd8) CRntVector<void*>(0xf);
    self->field_0x100 = 0;

    self->field_0x1108 = 0;
    self->field_0x1110 = 0;
    self->field_0x1118 = true;
    self->field_0x111c[0] = 0.0f;
    self->field_0x111c[1] = 0.0f;
    self->field_0x1124[0] = -1.0f;
    self->field_0x1124[1] = -1.0f;
    self->field_0x112c[0] = -1.0f;
    self->field_0x112c[1] = -1.0f;
    self->field_0x1134[0] = -1.0f;
    self->field_0x1134[1] = -1.0f;
    self->field_0x113c[0] = -1.0f;
    self->field_0x113c[1] = 0.0f;
    self->field_0x1148 = 0;
    self->field_0x1150 = false;
    self->field_0x1154 = 0;
    self->field_0x1158 = false;
    self->field_0x115c[0] = 0.0f;
    self->field_0x115c[1] = 1.0f;
    fn_71010f8804(self->pool_0x1168, 10, 0, 4, nullptr);
    self->field_0x11e0 = false;
    self->field_0x1204[0] = 0.0f;
    self->field_0x1204[1] = 0.0f;
    self->field_0x120c[0] = 0.0f;
    self->field_0x120c[1] = 0.0f;
    self->field_0x1214[0] = 0.0f;
    self->field_0x1214[1] = 0.0f;
    self->field_0x121c = false;
    self->field_0x121d = true;
    __builtin_memset(self->field_0x121e, 0, sizeof(self->field_0x121e));
    self->field_0x1224 = 0;
    self->field_0x1248 = false;
    __builtin_memset(self->field_0x124c, 0, sizeof(self->field_0x124c));
    fn_7101046974(reinterpret_cast<CBlackboard*>(self->blackboard));
    new (&self->callbacks_0x1350) UnkCallbackSet_l0f813c;
    self->field_0x13c0 = 0;
    self->field_0x13c8[0] = -1.0f;
    self->field_0x13c8[1] = -1.0f;
    self->field_0x13d0 = 0;
    self->field_0x13d2 = false;
    self->field_0x13d4[0] = -1.0f;
    self->field_0x13d4[1] = -1.0f;
    self->field_0x13dc[0] = -1.0f;
    self->field_0x13dc[1] = 0.0f;
    self->field_0x14b0 = 0;
    __builtin_memset(self->field_0x14f4, 0, sizeof(self->field_0x14f4));
    fn_71010f8930(self->pool_0x1520, 0x96, 0, 4, nullptr);
    for (u32 i = 0; i < 8; ++i)
        self->field_0x1590[i] = nullptr;
    self->field_0x15d0 = 0;
    self->field_0x15d4 = 5;
    self->field_0x15d8 = false;
    __builtin_memset(self->field_0x15dc, 0, sizeof(self->field_0x15dc));
    self->on_update.object = self;
    self->on_update.method = fn_7101115fb8;
    new (&self->hit_0x1600) UnkNavMeshHit;
    self->field_0x1610 = 0;
    self->field_0x1618 = 0;
    self->field_0x16e4 = 0;
    new (&self->bounds_0x16e8) UnkBoundsList_l0f813c;
    new (&self->bounds_0x1730) UnkBoundsList_l0f813c;
    self->field_0x1778 = 0;
    fn_71000003d4(&self->field_0x1780, g_unk_710162e2ed, true);
    self->field_0x1788 = 0;
    self->field_0x178c = 0;
    self->field_0x1790 = 0;
    self->field_0x1794 = false;
    self->field_0x1798 = -1.0f;
    self->field_0x179c = 0;
    self->field_0x17a0 = 0;

    UnkNamedTable_l0f813c* table = &self->table_0x17a8;
    table->vtable = g_unk_7101a22878 + 2;
    new (&table->name) CRntString("IA");
    table->capacity = 20;
    // The entries, with their count in front of them.
    u64* block = static_cast<u64*>(fn_7100083ba0(sizeof(u64) + 20 * sizeof(UnkNamedEntry_l0f813c), 5));
    *block = 20;
    table->items = reinterpret_cast<UnkNamedEntry_l0f813c*>(block + 1);
    table->field_0x48 = 0;
    table->field_0x50 = 0;
    table->vtable = g_unk_7101a22858 + 2;
    table->on_a.object = self;
    table->on_a.method = fn_7101115f48;
    table->field_0x68 = 0;
    table->on_b.object = self;
    table->on_b.method = fn_7101115f48;

    new (&self->lists_0x1828) UnkListGroup_l0f813c;
    new (&self->lists_0x18a8) UnkListGroup_l0f813c;
    new (&self->lists_0x1928) UnkListGroup_l0f813c;
    self->field_0x19a8 = 0;
    self->field_0x19b0 = 0;
    self->field_0x19b8 = 0;
    self->field_0x19c0 = false;
    fn_71002acfe4(self->path_0x19c8);

    new (&self->hit_0x14450) UnkNavMeshHit;
    self->field_0x1445c[0] = 0.0f;
    self->field_0x1445c[1] = 0.0f;
    new (&self->hit_0x14464) UnkNavMeshHit;
    self->field_0x14470[0] = 0.0f;
    self->field_0x14470[1] = 0.0f;
    new (&self->hit_0x14478) UnkNavMeshHit;
    self->field_0x14484[0] = 0;
    self->field_0x14484[1] = 0;
    self->field_0x1448c[0] = 0;
    self->field_0x1448c[1] = 0;
    self->field_0x14494 = false;
    self->listener_vtable = g_unk_7101a22d48 + 2;
    self->field_0x144a0 = 0;
    self->field_0x144a8 = 0;
    self->field_0x144b0 = 0;
    fn_71010f8a48(self->field_0x144b8, 4, nullptr, 0xe);
    self->field_0x14508 = 0;
    self->field_0x14510 = false;
    self->field_0x14514 = 0;
    self->field_0x1451c = -1.0f;
    new (&self->field_0x14520) CRntVector<void*>(0xf);
    new (&self->field_0x14548) CRntVector<void*>(0xf);
    self->field_0x14570 = 0;
    fn_710002f4e4(self->field_0x14578);
    __builtin_memset(self->field_0x14628, 0, sizeof(self->field_0x14628));
    self->field_0x1467c[0] = 0;
    self->field_0x1467c[1] = 0;
    self->field_0x14684 = 0;
    self->field_0x14689 = false;
    __builtin_memset(self->field_0x1469c, 0, sizeof(self->field_0x1469c));
}
