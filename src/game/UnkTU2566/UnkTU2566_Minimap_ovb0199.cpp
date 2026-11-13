// Minimap manager unit (translation unit 2566 of data/tu_map.csv; reflected as CMinimapManager::
// CTunableMinimapManager, the same unit as src/game/CMinimapManager.cpp). The functions here work on
// the minimap owner (the object behind the game manager world pointer) at raw offsets: the hidden
// icon kinds (+0xd642c8) that callbacks show again, the kind ids (+0xd642f0), the keyed table
// (+0xd643f0), the camera follow (fn_7100e910fc) and the blackboard properties it publishes
// (fn_7100ea585c, fn_7100e9b6ac). Names and layouts are placeholders; the owner is declared by
// src/game/CMinimapManager.cpp as UnkMinimapOwner, so the layouts here carry their own names and
// are cast to it at the call sites.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkNoArg.h"
#include "base/UnkTimedCurve.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2564.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2567.h"
#include "game/UnkTU39.h"
#include "game/UnkTU83.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

#define HIDDEN __attribute__((visibility("hidden")))

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkMinimapOwner;

// A rectangle returned in memory, copied word by word (as in src/game/CMinimapManager.cpp).
struct UnkMinimapRect {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    UnkMinimapRect() {}
    UnkMinimapRect(f32 x0, f32 y0, f32 x1, f32 y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1) {}
    UnkMinimapRect(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
    UnkMinimapRect& operator=(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
        return *this;
    }
};

// The owner as the camera follow and the properties see it.
struct UnkMmOwner_b0199 {
    u8 unk_0x0[0x110];
    CStrId field_0x110;  // the area the map shows
    u8 unk_0x118[0xd63d30 - 0x118];
    UnkMinimapRect view;  // 0xd63d30
};

// The hidden icon kinds (+0xd642c8): found by id, shown again by a callback that erases the kind
// (fn_71000ce604) and refreshes the map (fn_7100e957a8). Find() returns the slot past the last kind
// when there is none, and callers compare it with the end through the vector.
void fn_7100e9565c(base::UnkVector<CStrId>* self, const CStrId* id);

struct UnkMmKinds_b0199 : base::UnkVector<CStrId> {
    CStrId* Find(const CStrId& id) {
        for (u32 i = 0; i < count; ++i)
            if (data[i].data == id.data)
                return data + i;
        return data + count;
    }
};

struct UnkMmKindsOwner_b0199 {
    u8 unk_0x0[0x110];
    CStrId field_0x110;
    u8 unk_0x118[0xd642c8 - 0x118];
    UnkMmKinds_b0199 hidden_kinds;
};
static_assert(__builtin_offsetof(UnkMmKindsOwner_b0199, hidden_kinds) == 0xd642c8, "layout");

// Callbacks of the owner: when the kind is hidden, show it again.
bool fn_7100eb31f0(void* self_) {
    UnkMmKindsOwner_b0199* self = (UnkMmKindsOwner_b0199*)self_;
    UnkMmKinds_b0199* kinds = &self->hidden_kinds;
    CStrId* it = kinds->Find(g_unk_71727c83a8);
    if (it != &kinds->data[kinds->count]) {
        fn_71000ce604(kinds, it);
        fn_7100e957a8((UnkMinimapOwner*)self);
    }
    return false;
}

bool fn_7100eb331c(void* self_) {
    UnkMmKindsOwner_b0199* self = (UnkMmKindsOwner_b0199*)self_;
    UnkMmKinds_b0199* kinds = &self->hidden_kinds;
    CStrId* it = kinds->Find(g_unk_71727c83c8);
    if (it != &kinds->data[kinds->count]) {
        fn_71000ce604(kinds, it);
        fn_7100e957a8((UnkMinimapOwner*)self);
    }
    return false;
}

// When the kind is hidden, adds the two kinds that go with it.
bool fn_7100eb3278(void* self_) {
    UnkMmKindsOwner_b0199* self = (UnkMmKindsOwner_b0199*)self_;
    UnkMmKinds_b0199* kinds = &self->hidden_kinds;
    CStrId* it = kinds->Find(g_unk_71727c8378);
    if (it != &kinds->data[kinds->count]) {
        fn_7100e9565c(kinds, &g_unk_71727c8378);
        fn_7100e9565c(kinds, &g_unk_71727c8380);
        fn_7100e957a8((UnkMinimapOwner*)self);
    }
    return false;
}

// Like fn_7100e9d1f8 (src/game/CMinimapManager.cpp) for one more kind each, while an area is loaded.
void fn_7100e9d428(UnkMinimapOwner* self_) {
    UnkMmKindsOwner_b0199* self = (UnkMmKindsOwner_b0199*)self_;
    if (self->field_0x110.data == &g_unk_7101d47108->empty)
        return;
    UnkMmKinds_b0199* kinds = &self->hidden_kinds;
    CStrId* it = kinds->Find(g_unk_71727c83c0);
    if (it != &kinds->data[kinds->count]) {
        fn_71000ce604(kinds, it);
        fn_7100e957a8(self_);
    }
}

void fn_7100e9d4d0(UnkMinimapOwner* self_) {
    UnkMmKindsOwner_b0199* self = (UnkMmKindsOwner_b0199*)self_;
    if (self->field_0x110.data == &g_unk_7101d47108->empty)
        return;
    UnkMmKinds_b0199* kinds = &self->hidden_kinds;
    CStrId* it = kinds->Find(g_unk_71727c8058);
    if (it != &kinds->data[kinds->count]) {
        fn_71000ce604(kinds, it);
        fn_7100e957a8(self_);
    }
}

// ---- fn_7100ea9eb8 ----
// Destroys a named record: releases the string ids of its items, frees its two arrays and releases its own id.
struct UnkMmEntry_b0199 {
    CStrId name;
    u8 unk_0x8[0x18];
};
struct UnkMmNamed_b0199 {
    CStrId name;
    UnkMmEntry_b0199* items;
    void* extra;
    u32 count;
    u8 unk_0x1c[0xc];
    base::UnkAllocator* allocator;
};

void fn_7100ea9eb8(UnkMmNamed_b0199* self) {
    if (self->items) {
        for (u32 i = 0; i < self->count; ++i)
            self->items[i].name.Reset();
    }
    self->count = 0;
    if (self->extra)
        self->allocator->vf_0x48(self->extra);
    if (self->items)
        self->allocator->vf_0x48(self->items);
    self->name.Reset();
}

// ---- fn_7100ea6234 ----
// Adds a kind id to the owner list (+0xd642f0) unless it is there.
struct UnkMmIds_b0199 {
    u8 unk_0x0[0xd642f0];
    base::UnkStrIdVec kinds;
};

void fn_7100ea6234(UnkMmIds_b0199* self, const CStrId* id) {
    base::UnkStrIdVec* kinds = &self->kinds;
    u64 n = kinds->count;
    CStrId* p = kinds->data;
    for (; n != 0; --n, ++p)
        if (p->data == id->data)
            return;
    kinds->PushBack(*id);
}

// ---- fn_7100e9b4b0 ----
// Clears the world kind ids (+0xd642f0) and the minimap state, then runs the object reference target.
void fn_7100b44f08(void* target);
void fn_7100e96824(UnkMinimapOwner* self, u32 a, void* b);
void fn_7100e9405c(UnkMinimapOwner* self, const void* v);
extern u8 g_unk_71729cdb08[] HIDDEN;
extern u8 g_unk_710164be60[] HIDDEN;

struct UnkMmRef_b0199 {
    u8 unk_0x0[0x10];
    void* target;
};
struct UnkMmWorld_b0199 {
    u8 unk_0x0[0xd63f40];
    UnkMmRef_b0199* ref;  // 0xd63f40
    u8 unk_0xd63f48[0x3a8];
    base::UnkStrIdVec ids;  // 0xd642f0
    u8 unk_0xd64318[0x438];
    u32 field_0xd64750;
};
static_assert(__builtin_offsetof(UnkMmWorld_b0199, ids) == 0xd642f0, "layout");
static_assert(__builtin_offsetof(UnkMmWorld_b0199, field_0xd64750) == 0xd64750, "layout");

void fn_7100e9b4b0(void* self_) {
    UnkMmWorld_b0199* self = (UnkMmWorld_b0199*)self_;
    self->field_0xd64750 = *(u32*)((u8*)&g_unk_71727ca410 + 0x2b0);
    for (u64 i = 0; i < self->ids.count; ++i)
        self->ids.data[i].Reset();
    self->ids.count = 0;
    fn_7100e96824((UnkMinimapOwner*)self, 0, g_unk_71729cdb08);
    fn_7100e9405c((UnkMinimapOwner*)self, g_unk_710164be60);
    if (self->ref && self->ref->target)
        fn_7100b44f08(self->ref->target);
}

// ---- fn_7100ea5960 ----
// Sets the value of the keyed record and refreshes (fn_7100ea585c).
void fn_7100ea09a0(UnkMinimapOwner* self, void* record, u32 flag);

struct UnkMmRecord_b0199 {
    u32 field_0x0;
    u8 unk_0x4[0x114];
};
struct UnkMmKeyed_b0199 {
    UnkMmRecord_b0199* values;
    u32* keys;
    u32 count;

    UnkMmRecord_b0199* Find(u32 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return values + i;
        return values + count;
    }
};
struct UnkMmOwner2_b0199 {
    u8 unk_0x0[0xd643f0];
    UnkMmKeyed_b0199 table;
};

void fn_7100ea5960(UnkMmOwner2_b0199* self, u32 key, u32 value) {
    UnkMmRecord_b0199* it = self->table.Find(key);
    if (it != self->table.values + self->table.count) {
        it->field_0x0 = value;
        fn_7100ea09a0((UnkMinimapOwner*)self, it, 0);
        fn_7100ea585c((UnkMmOwner_b0199*)self);
    }
}

// ---- fn_7100ea585c ----
// Publishes the owner keyed table (+0xd643f0) under the current scenario section in the blackboard.
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern UnkPropertyEx<> g_unk_71727ca190;
extern base::reflection::CType g_unk_71727cb000;

void fn_7100ea585c(UnkMmOwner_b0199* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_7100dfbda4(manager, self->field_0x110);
    UnkQualifiedId* qualified = reinterpret_cast<UnkQualifiedId*>(g_unk_71727ca190.field_0x10);
    if (qualified->id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(qualified, reinterpret_cast<UnkIdSource*>(&g_unk_71727ca190), g_unk_7101d470e0);
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
        if (!manager->field_0x968.field_0xe8) {
            UnkTypedValue v;
            v.value = (u8*)self + 0xd643f0;
            v.type = &g_unk_71727cb000;
            fn_7100332c80(s, (UnkPropertyName*)qualified, &v);
        }
    }
}

// ---- fn_7100e9b6ac ----
// Stores true under the sub-id of the given name in the minimap section of the blackboard.
struct UnkNamedDef_b0199 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkNamedDef_b0199 g_unk_71727c9c78;

static inline UnkSubId_7100fc4b34 SubId(UnkNamedDef_b0199& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

void fn_7100e9b6ac(void* world, void* item) {
    UnkTypedValue v;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkSubId_7100fc4b34 key = SubId(g_unk_71727c9c78, *(const CStrId*)item);
    bool value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71727cc990)) {
        if (!manager->field_0x968.field_0xe8) {
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
        }
    }
}

// ---- fn_7100e910fc ----
// Moves the map to a position: the camera offset from the view centre, then the new view rectangle.
void fn_7100e90e6c(UnkMinimapOwner* self);

void fn_7100e910fc(UnkMmOwner_b0199* self, const f32* pos) {
    u8* base = (u8*)self + 0xd63d30;
    UnkXformNode* camera = *(UnkXformNode**)(base + 0x218);
    base::math::CVector2D v;
    {
        UnkVec3f eye = fn_7100206154((UnkXformNode*)((u8*)camera + 0x20));
        fn_710007b430(&v, (const base::math::CVector3D*)&eye);
    }
    const f32* off = (const f32*)(base + 0x20);
    f32 dx = v.x - off[0];
    f32 dy = v.y - off[1];
    {
        base::math::CVector2D p = *(const base::math::CVector2D*)pos;
        base::math::CVector2D out = {p.x - dx, p.y - dy};
        fn_7100e9405c((UnkMinimapOwner*)self, &out);
    }
    fn_7100e90e6c((UnkMinimapOwner*)self);
    UnkMinimapRect rect = fn_7100e948b4((UnkMinimapOwner*)self, fn_710002fb70((u8*)self + 0xd63e40));
    self->view = rect;
}

// ---- fn_7100e9867c ----
// Switches the shown area: starts the fade curve on the owner for a new area.
void fn_7100e96c60(UnkMinimapOwner* self);
void fn_7100e98768(UnkMinimapOwner* self);
extern s32 g_unk_710164b8b4;
extern const f32 g_unk_71727cc010[];
extern const char g_unk_7101603b01[] HIDDEN;

struct UnkMmName_b0199 {
    u8 unk_0x0[0x30];

    __attribute__((always_inline)) UnkMmName_b0199() {
        fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(this), g_unk_7101603b01);
    }
    __attribute__((always_inline)) ~UnkMmName_b0199() {
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
};

void fn_7100e9867c(void* world, const CStrId& area) {
    UnkMmOwner_b0199* self = (UnkMmOwner_b0199*)world;
    if (area.data == &g_unk_7101d47108->empty)
        return;
    if (area.data == self->field_0x110.data)
        return;
    fn_7100e96c60((UnkMinimapOwner*)self);
    const CStrId* current = fn_7100de9bf8(g_unk_7101d47310);
    if (area.data != current->data && !(g_unk_710164b8b4 > 3))
        fn_7100e98768((UnkMinimapOwner*)self);
    UnkTimedCurve* curve = (UnkTimedCurve*)((u8*)self + 0xd64620);
    const f32* v = g_unk_71727cc010;
    f32 x = v[0], y = v[1], z = v[2];
    UnkMmName_b0199 name;
    fn_7100075f78(x, y, z, curve, (const base::global::CRntString*)&name);
}

// ---- fn_7100e9d578 ----
// A component callback: for a grapple point component adds its two kinds and refreshes the map.
void fn_7100e9565c(base::UnkVector<CStrId>* self, const CStrId* id);

struct UnkMmOwner3_b0199 {
    u8 unk_0x0[0xd642c8];
    base::UnkVector<CStrId> hidden_kinds;
};

bool fn_7100e9d578(UnkMmOwner3_b0199* self, base::reflection::UnkReflObject* obj) {
    if (!obj)
        return false;
    if (fn_710009d50c(obj->vf_0x0(), fn_71008259c4())) {
        base::UnkVector<CStrId>* kinds = &self->hidden_kinds;
        fn_7100e9565c(kinds, &g_unk_71727c8358);
        fn_7100e9565c(kinds, &g_unk_71727c8360);
    } else if (fn_710009d50c(obj->vf_0x0(), fn_710082d5b8())) {
        base::UnkVector<CStrId>* kinds = &self->hidden_kinds;
        fn_7100e9565c(kinds, &g_unk_71727c8368);
        fn_7100e9565c(kinds, &g_unk_71727c8370);
    } else {
        return false;
    }
    fn_7100e957a8((UnkMinimapOwner*)self);
    return false;
}

// ---- fn_7100e94608 ----
// Initialises the two parts of a record and looks up its table entry (+0x5b4).
u64 fn_7100ea1b94(void* self, const void* a);

struct UnkMmRecord2_b0199 {
    u8 unk_0x0[0xc8];
};
struct UnkMmTable_b0199 {
    u8 unk_0x0[0xa8];
    UnkMmRecord2_b0199* records;  // 0xa8
    u64* keys;                    // 0xb0
    u32 count;                    // 0xb8

    UnkMmRecord2_b0199* Find(const CStrId& id) {
        u64* k = keys;
        for (u32 i = 0; i < count; ++i)
            if (k[i] == (u64)id.data)
                return records + i;
        return records + count;
    }
};
struct UnkMmLookup_b0199 {
    u8 unk_0x0[0x60];
    UnkMmTable_b0199* table;
};
struct UnkMmSrc_b0199 {
    u8 unk_0x0[0x28];
    u8 field_0x28[8];
    CStrId id;  // 0x30
};

void fn_7100e94608(UnkMmLookup_b0199* self, UnkNoArg, UnkMmSrc_b0199* src, u8* dst) {
    u32 state = *(u32*)dst;
    if (state == 0) {
        fn_7100eb7680(dst + 4);
        state = 1;
        *(u32*)dst = state;
    }
    u64 v = fn_7100ea1b94(self, src->field_0x28);
    __builtin_memcpy(dst + 0x3c, &v, 8);
    if (state <= 1) {
        fn_7100eb7680(dst + 0x57c);
        *(u32*)dst = 2;
    }
    u8* result;
    if (src->id.data == &g_unk_7101d47108->empty || src->id.data == g_unk_71727c81a0.data) {
        result = nullptr;
    } else {
        UnkMmTable_b0199* t = self->table;
        UnkMmRecord2_b0199* it = t->Find(g_unk_71727c8110);
        result = it == t->records + t->count ? nullptr : (u8*)it;
    }
    __builtin_memcpy(dst + 0x5b4, &result, 8);
}

// ---- fn_7100e98228 ----
// Appends a key and a default bounds record (the empty id, an inverted box) to the list; returns it.
void fn_7100eb3c08(void* self, u32 capacity);

struct UnkMmBounds_b0199 {
    base::global::UnkStringData* id;
    f32 min[2];
    f32 max[2];
    u64 field_0x18;

    __attribute__((always_inline)) UnkMmBounds_b0199() {
        __builtin_memset(this, 0, sizeof(*this));
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id = &g_unk_7101d47108->empty;
        u64 hi = 0x7f7fffff7f7fffffull;
        u64 lo = 0xff7fffffff7fffffull;
        __builtin_memcpy(min, &hi, 8);
        __builtin_memcpy(max, &lo, 8);
    }
};
static_assert(sizeof(UnkMmBounds_b0199) == 0x20, "x");

struct UnkMmBoundsList_b0199 {
    UnkMmBounds_b0199* data;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

__attribute__((flatten)) UnkMmBounds_b0199* fn_7100e98228(UnkMmBoundsList_b0199* self, const u32* key) {
    if (self->count == self->capacity)
        fn_7100eb3c08(self, self->step + self->count);
    u32* slot = &self->keys[self->count];
    *slot = *key;
    UnkMmBounds_b0199* e = new (&self->data[self->count]) UnkMmBounds_b0199();
    (void)e;
    u32 n = self->count + 1;
    self->count = n;
    return n == 0 ? self->data : &self->data[n] - 1;
}

// ---- fn_7100e97f8c ----
// Appends an id key and a default cell to the list; returns the cell.
void fn_7100e97c60(void* self, u32 capacity);
void fn_7100ea9dcc(void* sub);

struct UnkMmCell_b0199 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
    f32 min[2];
    f32 max[2];
    u8 sub[0x50];
    u8 field_0x88;
    u8 unk_0x89[7];

    __attribute__((always_inline)) UnkMmCell_b0199() {
        __builtin_memset(this, 0, sizeof(*this));
        step = 10;
        flags = 14;
        allocator = fn_7100083990();
        u64 hi = 0x7f7fffff7f7fffffull;
        u64 lo = 0xff7fffffff7fffffull;
        __builtin_memcpy(min, &hi, 8);
        __builtin_memcpy(max, &lo, 8);
        fn_7100ea9dcc(sub);
        field_0x88 = 0;
    }
};
static_assert(sizeof(UnkMmCell_b0199) == 0x90, "x");

struct UnkMmCellList_b0199 {
    UnkMmCell_b0199* data;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

UnkMmCell_b0199* fn_7100e97f8c(UnkMmCellList_b0199* self, const CStrId* key) {
    if (self->count == self->capacity)
        fn_7100e97c60(self, self->step + self->count);
    CStrId* slot = &self->keys[self->count];
    base::global::UnkStringData* d = key->data;
    __builtin_memcpy(slot, &d, 8);
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
    UnkMmCell_b0199* e = new (&self->data[self->count]) UnkMmCell_b0199();
    (void)e;
    u32 n = self->count + 1;
    self->count = n;
    return n == 0 ? self->data : &self->data[n] - 1;
}
