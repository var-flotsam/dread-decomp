// CScenario unit (unit 2735 of tools/tu_map.py): the scenario's constructor. It builds the
// members, files the "entities", "sounds" and "lights" categories, and fills the two area-name
// tables (sprite images and label texts by scenario) the first time one is built.
// Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkCloneRecords.h"
#include "base/core/CBaseObject.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU63.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStrIdBase;

extern char g_unk_7101cb5528[];

struct UnkScenarioScript_ovb0032;
struct UnkPoolList_deb914;
struct UnkNavMesh47f8;
template <class Tag>
struct UnkSlotArray;
struct UnkTag_71007ed25c;
template <class T>
struct UnkKeyedArray_b62a1;
template <base::global::UnkStringPool*& Pool>
struct UnkMovedId_b62a1;
using UnkMovedStrId_b62a1 = UnkMovedId_b62a1<g_unk_7101d47108>;

void fn_710025b068(char* self);
void fn_7100de5f98(UnkPoolList_deb914* self, u32 n, u32 a, u32 flags, base::UnkAllocator* allocator);
void fn_71002747f8(UnkNavMesh47f8* self);
void fn_71007ed25c(UnkSlotArray<UnkTag_71007ed25c>* self, u32 capacity);
void fn_7100e147cc(void* self);
void fn_7100e14898(void* self, void* scenario);
void fn_71011a4eb4(void* self);
void fn_71011a1174(void* self);
void fn_71000d97fc(UnkKeyedArray_b62a1<UnkMovedStrId_b62a1>* self, u32 capacity);

// The label texts and the sprite images by scenario (string ids by string id).
extern UnkGameManagerIdMap g_unk_7172a0bf10;
extern UnkGameManagerIdMap g_unk_7172a0bee8;

// A container with a vtable of its own (+0x888).
struct UnkScenarioTable_l19fe50 {
    void** vtable;
    u32 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    base::UnkAllocator* allocator;
    u32 flags;
};

struct UnkScenario_l19fe50 {
    void** vtable;
    u8 pad_0x8[0x8];
    CStrId sLevelID;
    CStrId sScenarioID;
    char field_0x20[0xc8];
    char field_0xe8[0xc8];
    char field_0x1b0[0xc8];
    CRntVector<CStrId> categories;  // +0x278
    char field_0x2a0[0x70];
    char field_0x310[0x70];
    char field_0x380[0x70];
    bool field_0x3f0;
    u8 pad_0x3f1[0x7];
    char navmesh[0x428];  // +0x3f8
    void* field_0x820;
    CRntVector<void*> colliders;  // +0x828
    void* field_0x850;
    char field_0x858[0x30];
    UnkScenarioTable_l19fe50 field_0x888;
    char field_0x8b8[0x60];
    u64 field_0x918;
    u64 field_0x920;
    bool field_0x928;
};
static_assert(__builtin_offsetof(UnkScenario_l19fe50, categories) == 0x278, "UnkScenario_l19fe50 layout");
static_assert(__builtin_offsetof(UnkScenario_l19fe50, field_0x820) == 0x820, "UnkScenario_l19fe50 layout");
static_assert(__builtin_offsetof(UnkScenario_l19fe50, field_0x928) == 0x928, "UnkScenario_l19fe50 layout");

// The two tables' storage: values and keys at the same positions, grown by `step`.
struct UnkAreaNameTable_l19fe50 {
    CStrId* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

// Grows the array to n ids, moving the ids over and freeing the old array.
static inline void ReserveMoving(CRntVector<CStrId>* ids, u32 n) {
    if (ids->capacity < n) {
        CStrId* items = static_cast<CStrId*>(ids->allocator->vf_0x40((u64)n * sizeof(CStrId), 8, ids->flags));
        if (CStrId* old = ids->data) {
            for (u32 i = 0; i < ids->count; ++i) {
                CStrId::MoveConstructReloading(&items[i], old[i]);
                old[i].~CStrId();
            }
            ids->allocator->vf_0x48(ids->data);
        }
        ids->data = items;
        ids->capacity = n;
    }
}

// Appends `id`, moved.
static inline void PushBackMoved(CRntVector<CStrId>* ids, CStrId& id) {
    u32 n = ids->count;
    u32 c = ids->capacity;
    if (c <= n && ids->step != 0)
        c = (n / ids->step + 1) * ids->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    fn_710000542c(reinterpret_cast<base::UnkStrIdVec*>(ids), c);
    CStrId::MoveConstructReloading(&ids->data[ids->count], id);
    ++ids->count;
}

// Maps `key` to `value` in an area-name table.
static inline void AddAreaName(UnkGameManagerIdMap* map, const char* key, const char* value) {
    UnkAreaNameTable_l19fe50* table = reinterpret_cast<UnkAreaNameTable_l19fe50*>(map);
    CStrId k(key, true);
    UnkStrIdBase v(value, true);
    if (table->count == table->capacity)
        fn_71000d97fc(reinterpret_cast<UnkKeyedArray_b62a1<UnkMovedStrId_b62a1>*>(table),
                      table->count + table->step);
    new (&table->keys[table->count]) CStrId(k);
    table->values[table->count].data = v.data;
    ++table->count;
}

void fn_710119fe50(UnkScenarioScript_ovb0032* scenario, const char* level, const char* name) {
    UnkScenario_l19fe50* self = reinterpret_cast<UnkScenario_l19fe50*>(scenario);
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb5528 + 0x10);
    new (&self->sLevelID) CStrId(level, true);
    new (&self->sScenarioID) CStrId(name, true);
    fn_710025b068(self->field_0x20);
    fn_710025b068(self->field_0xe8);
    fn_710025b068(self->field_0x1b0);
    new (&self->categories) CRntVector<CStrId>(5);
    ReserveMoving(&self->categories, 0x800);
    fn_7100de5f98(reinterpret_cast<UnkPoolList_deb914*>(self->field_0x2a0), 0x400, 0, 5, nullptr);
    fn_7100de5f98(reinterpret_cast<UnkPoolList_deb914*>(self->field_0x310), 0x400, 0, 5, nullptr);
    fn_7100de5f98(reinterpret_cast<UnkPoolList_deb914*>(self->field_0x380), 0x400, 0, 5, nullptr);
    self->field_0x3f0 = false;
    fn_71002747f8(reinterpret_cast<UnkNavMesh47f8*>(self->navmesh));
    fn_7100023854(reinterpret_cast<UnkNode*>(self->navmesh));
    self->field_0x820 = nullptr;
    new (&self->colliders) CRntVector<void*>(5);
    fn_71007ed25c(reinterpret_cast<UnkSlotArray<UnkTag_71007ed25c>*>(&self->colliders), 0x800);
    self->field_0x850 = nullptr;
    fn_7100e147cc(self->field_0x858);
    self->field_0x888.vtable = reinterpret_cast<void**>(g_unk_7101a1b030 + 2);
    self->field_0x888.field_0x8 = 0;
    self->field_0x888.field_0x10 = 0;
    self->field_0x888.field_0x18 = 0;
    self->field_0x888.allocator = fn_7100083990();
    self->field_0x888.flags = 0x13;
    fn_71011a4eb4(self->field_0x8b8);
    self->field_0x918 = 0;
    self->field_0x920 = 0;
    self->field_0x928 = true;
    fn_7100e14898(self->field_0x858, self);
    fn_71011a1174(self);

    // The categories the scenario files its objects under.
    {
        char category[0x100];
        __builtin_strcpy(category, "entities");
        // The id is built in place by the out-of-line constructor from a string (fn_7100000850).
        alignas(CStrId) u8 storage[sizeof(CStrId)];
        UnkBufferString<0x100, g_unk_7101a194b8> text(category);
        CStrId* id = reinterpret_cast<CStrId*>(storage);
        CStrId_ConstructFromRntString(id, *text.Str(), true);
        PushBackMoved(&self->categories, *id);
        id->~CStrId();
    }
    {
        char category[0x100];
        __builtin_strcpy(category, "sounds");
        // The id is built in place by the out-of-line constructor from a string (fn_7100000850).
        alignas(CStrId) u8 storage[sizeof(CStrId)];
        UnkBufferString<0x100, g_unk_7101a194b8> text(category);
        CStrId* id = reinterpret_cast<CStrId*>(storage);
        CStrId_ConstructFromRntString(id, *text.Str(), true);
        PushBackMoved(&self->categories, *id);
        id->~CStrId();
    }
    {
        char category[0x100];
        __builtin_strcpy(category, "lights");
        // The id is built in place by the out-of-line constructor from a string (fn_7100000850).
        alignas(CStrId) u8 storage[sizeof(CStrId)];
        UnkBufferString<0x100, g_unk_7101a194b8> text(category);
        CStrId* id = reinterpret_cast<CStrId*>(storage);
        CStrId_ConstructFromRntString(id, *text.Str(), true);
        PushBackMoved(&self->categories, *id);
        id->~CStrId();
    }

    UnkGameManagerIdMap* sprites = &g_unk_7172a0bee8;
    if (sprites->count == 0) {
        AddAreaName(sprites, "s010_cave", "AreaNames_Artaria/ARTARIA");
        AddAreaName(sprites, "s020_magma", "AreaNames_Cataris/CATARIS");
        AddAreaName(sprites, "s030_baselab", "AreaNames_Dairon/DAIRON");
        AddAreaName(sprites, "s040_aqua", "AreaNames_Burenia/BURENIA");
        AddAreaName(sprites, "s050_forest", "AreaNames_Ghavoran/GHAVORAN");
        AddAreaName(sprites, "s060_quarantine", "AreaNames_Elun/ELUN");
        AddAreaName(sprites, "s070_basesanc", "AreaNames_Ferenia/FERENIA");
        AddAreaName(sprites, "s080_shipyard", "AreaNames_Hanubia/HANUBIA");
        AddAreaName(sprites, "s090_skybase", "AreaNames_Itorash/ITORASH");
    }
    UnkGameManagerIdMap* labels = &g_unk_7172a0bf10;
    if (labels->count == 0) {
        AddAreaName(labels, "s010_cave", "TERM_AREANAME_CAVES_SUBHEADER");
        AddAreaName(labels, "s020_magma", "TERM_AREANAME_MAGMA_SUBHEADER");
        AddAreaName(labels, "s030_baselab", "TERM_AREANAME_LABORATORY_SUBHEADER");
        AddAreaName(labels, "s040_aqua", "TERM_AREANAME_AQUA_SUBHEADER");
        AddAreaName(labels, "s050_forest", "TERM_AREANAME_FOREST_SUBHEADER");
        AddAreaName(labels, "s060_quarantine", "TERM_AREANAME_QUARANTINE_SUBHEADER");
        AddAreaName(labels, "s070_basesanc", "TERM_AREANAME_SANCTUARY_SUBHEADER");
        AddAreaName(labels, "s080_shipyard", "TERM_AREANAME_SHIPYARD_SUBHEADER");
        AddAreaName(labels, "s090_skybase", "TERM_AREANAME_SKYBASE_SUBHEADER");
    }
}
