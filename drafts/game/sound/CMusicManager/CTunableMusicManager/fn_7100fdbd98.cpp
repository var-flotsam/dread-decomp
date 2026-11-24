// sound::CMusicManager::CTunableMusicManager's unit: fills two requests from the music definitions of the
// current scenario and area (the area falling back to a default id) for one kind of music: each item of
// kind 0 or 1 sets the request of that kind and adds its file paths to it. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

using base::global::CStrId;

// The names of the music kinds.
extern const char* const g_unk_7101cac300[] __attribute__((visibility("hidden")));

// A hash map by name whose nodes are also kept in a list.
template <class Node>
struct UnkMusicDefMap_dbd98 {
    base::IAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    Node** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    Node* Find(const CStrId& key) {
        Node** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = key.Hash() % divisor;
            bucket = buckets + i;
        }
        if (!bucket)
            return nullptr;
        for (Node* n = *bucket; n; n = n->chain)
            if (n->key.data == key.data)
                return n;
        return nullptr;
    }
};

// One item of a kind's list: which request it sets, the request's values and its file paths.
struct UnkMusicDefItem_dbd98 {
    u32 kind;                 // 0x0: 0 or 1
    UnkPathVec_b0019 paths;   // 0x8
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u8 field_0x4c;
    u8 pad_0x4d[0x2b];
};
static_assert(sizeof(UnkMusicDefItem_dbd98) == 0x78, "UnkMusicDefItem_dbd98 layout");

struct UnkMusicKindNode_dbd98 {
    CStrId key;
    u64 field_0x8;
    base::global::CRntVector<UnkMusicDefItem_dbd98> items;  // 0x10
    u8 pad_0x38[0x10];
    UnkMusicKindNode_dbd98* chain;  // 0x48
};

// An area's definitions: its kinds by name.
struct UnkMusicArea_dbd98 {
    u64 field_0x0;
    UnkMusicDefMap_dbd98<UnkMusicKindNode_dbd98> kinds;  // 0x8
    u8 pad_0x50[0x10];
};

struct UnkMusicAreaNode_dbd98 {
    CStrId key;
    UnkMusicArea_dbd98 value;        // 0x8
    UnkMusicAreaNode_dbd98* chain;   // 0x68
};

// A scenario's definitions: its areas by name.
struct UnkMusicScenario_dbd98 {
    u64 field_0x0;
    UnkMusicDefMap_dbd98<UnkMusicAreaNode_dbd98> areas;  // 0x8
    u8 pad_0x50[0x10];
};

struct UnkMusicScenarioNode_dbd98 {
    CStrId key;
    UnkMusicScenario_dbd98 value;        // 0x8
    UnkMusicScenarioNode_dbd98* chain;   // 0x68
};

struct UnkMusicScenarioPtrNode_dbd98 {
    CStrId key;
    UnkMusicScenario_dbd98* value;           // 0x8
    u8 pad_0x10[0x10];
    UnkMusicScenarioPtrNode_dbd98* chain;    // 0x20
};

// The music manager's two scenario maps.
struct UnkMusicManagerDefs_dbd98 {
    u8 pad_0x0[0x178];
    UnkMusicDefMap_dbd98<UnkMusicScenarioPtrNode_dbd98> scenario_refs;  // 0x178
    u8 pad_0x1c0[0x48];
    UnkMusicDefMap_dbd98<UnkMusicScenarioNode_dbd98> scenarios;         // 0x208
};

// A request the definitions fill (0x60 bytes, as the slot lists keep them).
struct UnkMusicDefRequest_dbd98 {
    u32 kind;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u8 field_0x24;
    u8 field_0x25;
    u8 field_0x26;
    UnkPathVec_b0019 paths;  // 0x28
    u8 pad_0x50[0x10];
};

struct UnkMusicDefCounted_dbd98 {
    u32 value;
};

using UnkPathId_dbd98 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

static inline void ClearPaths(UnkPathVec_b0019& paths) {
    for (u64 i = 0; i < paths.count; ++i)
        paths.data[i].~UnkPathRecord_b0019();
    paths.count = 0;
}

// Adds a default record at the end and returns it.
static inline UnkPathRecord_b0019& AddPath(UnkPathVec_b0019& paths) {
    u32 n = paths.count + 1;
    paths.Reserve(n);
    for (u32 i = paths.count; i < n; ++i)
        CChozoCommanderAIComponent_ConstructFilePathRecord(reinterpret_cast<UnkPathRecord*>(&paths.data[i]));
    paths.count = n;
    UnkPathRecord_b0019* end = &paths.data[n];
    return n ? end[-1] : *end;
}

static inline const char* KindName(s32 kind) {
    if ((u32)kind <= 7)
        return g_unk_7101cac300[kind];
    return g_unk_710162e2ed;
}

void fn_7100fdbd98(UnkMusicManagerDefs_dbd98* self, const void* unused, s32 kind, UnkMusicDefRequest_dbd98* first,
                   UnkMusicDefRequest_dbd98* second, base::global::CRntVector<UnkMusicDefCounted_dbd98>* list_a,
                   base::global::CRntVector<UnkMusicDefCounted_dbd98>* list_b) {
    list_a->Clear();
    list_b->Clear();
    ClearPaths(first->paths);
    ClearPaths(second->paths);
    UnkGameManager* manager = g_unk_7101d47310;
    UnkMusicScenario_dbd98* scenario = nullptr;
    if (UnkMusicScenarioPtrNode_dbd98* ref = self->scenario_refs.Find(manager->field_0x2238))
        scenario = ref->value;
    else if (self->scenarios.Find(manager->field_0x2238))
        scenario = &self->scenarios.Find(manager->field_0x2238)->value;
    if (!scenario)
        return;
    CStrId area_name = manager->field_0x2230;
    UnkMusicAreaNode_dbd98* area = scenario->areas.Find(area_name);
    if (!area)
        area = scenario->areas.Find(base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x1b1]);
    if (!area)
        return;
    UnkMusicKindNode_dbd98* defs = area->value.kinds.Find(CStrId(KindName(kind), true));
    if (!defs)
        return;
    for (u32 i = 0; i < defs->items.count; ++i) {
        UnkMusicDefItem_dbd98& item = defs->items.data[i];
        if (item.kind > 1)
            return;
        UnkMusicDefRequest_dbd98* request = item.kind == 0 ? first : second;
        request->field_0x4 = item.field_0x48;
        request->field_0x10 = item.field_0x30;
        request->field_0xc = item.field_0x34;
        request->kind = 6;
        request->field_0x8 = item.field_0x38;
        request->field_0x14 = item.field_0x3c;
        request->field_0x18 = item.field_0x40;
        request->field_0x1c = item.field_0x44;
        request->field_0x25 = item.field_0x4c;
        for (u64 j = 0; j < item.paths.count; ++j) {
            UnkPathRecord_b0019& path = AddPath(request->paths);
            const UnkPathRecord_b0019& source = item.paths.data[j];
            reinterpret_cast<UnkPathId_dbd98&>(path.id) = reinterpret_cast<const UnkPathId_dbd98&>(source.id);
            path.field_0x8 = source.field_0x8;
        }
    }
}
