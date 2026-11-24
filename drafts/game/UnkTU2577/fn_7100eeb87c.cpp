// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fills `out` with the ids the
// manager keeps under the object's id (dropping the empty ones), then refreshes the object's entries in
// the scenario's first layer. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU83.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkScenarioLayer_ovb0032;
struct UnkScenarioKey_ovb0032;
void* CScenario_FindLayerValueByKey(UnkScenarioLayer_ovb0032* layers, u32 index, UnkScenarioKey_ovb0032* const* key);

// A string id: moves take the id and leave the pool's empty string.
struct UnkId_eb87c {
    union {
        UnkStringData* data;
    };
    UnkId_eb87c(UnkId_eb87c&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkId_eb87c() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

using UnkIdVector_eb87c = base::global::CRntVector<UnkId_eb87c>;

// An empty id list, destroyed at exit by the vector's destructor of the CCollisionComponent unit.
struct UnkEmptyIdList_eb87c {
    UnkIdVector_eb87c ids;

    UnkEmptyIdList_eb87c() : ids(0xf) {}
    ~UnkEmptyIdList_eb87c() asm("fn_71008600a8");
};

// The ids kept under one key.
struct UnkIdEntry_eb87c {
    UnkStringData* key;
    UnkIdVector_eb87c ids;  // 0x8
};

struct UnkSubAreaMgr_eb87c {
    u8 pad_0x0[0x888];
    base::global::CRntVector<UnkIdEntry_eb87c*> entries;  // 0x888
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_eb87c, entries) == 0x888, "UnkSubAreaMgr_eb87c layout");

struct UnkSceneObject_eb87c {
    u8 pad_0x0[0x28];
    UnkStringData* id;  // 0x28
    u8 pad_0x30[0x20];
    UnkScenarioKey_ovb0032* key;  // 0x50
};

struct UnkWeakTarget_eb87c {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

// An entry of the layer's list (0x40 bytes).
struct UnkLayerEntry_eb87c {
    u8 pad_0x0[0x8];
    u8 member[0x28];  // 0x8, the property read for the actor
    u32 state;        // 0x30
    base::UnkRefHandle handle;  // 0x38
};
static_assert(sizeof(UnkLayerEntry_eb87c) == 0x40, "UnkLayerEntry_eb87c layout");

struct UnkLayerEntries_eb87c {
    UnkLayerEntry_eb87c* data;
    u32 count;
};

static inline const UnkIdVector_eb87c& FindIds(UnkSubAreaMgr_eb87c* self, UnkStringData* id) {
    UnkIdEntry_eb87c** entries = self->entries.data;
    for (UnkIdEntry_eb87c **p = entries, **end = entries + self->entries.count; p != end; ++p) {
        if ((*p)->key == id)
            return (*p)->ids;
    }
    static UnkEmptyIdList_eb87c s_empty;
    return s_empty.ids;
}

// Destroys the id at `index` and moves the later ones down.
static inline void EraseAt(UnkIdVector_eb87c* v, u32 index) {
    if (index < v->count) {
        v->data[index].~UnkId_eb87c();
        for (u32 i = index; i < v->count - 1; ++i) {
            new (&v->data[i]) UnkId_eb87c(static_cast<UnkId_eb87c&&>(v->data[i + 1]));
            v->data[i + 1].~UnkId_eb87c();
        }
        v->count = v->count - 1;
    }
}

void fn_7100eeb87c(UnkSubAreaMgr_eb87c* self, UnkSceneObject_eb87c* object, UnkIdVector_eb87c* out) {
    out->Clear();
    if (!object)
        return;

    if (object->id != &g_unk_7101d47108->empty) {
        fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(out),
                      reinterpret_cast<const UnkStruct_0x28*>(&FindIds(self, object->id)));
        for (u32 i = out->count; i != 0; --i) {
            if (out->data[i - 1].data == &g_unk_7101d47108->empty)
                EraseAt(out, i - 1);
        }
    }

    UnkLayerEntries_eb87c* entries = static_cast<UnkLayerEntries_eb87c*>(CScenario_FindLayerValueByKey(
        reinterpret_cast<UnkScenarioLayer_ovb0032*>(g_unk_7101d47310->field_0x608), 0, &object->key));
    if (!entries)
        return;
    for (UnkLayerEntry_eb87c* e = entries->data, *end = e + entries->count; e != end; ++e) {
        if (e->state == 1) {
            UnkRefPtr actor = fn_710023b580(e->member);
            fn_7100234f78(&e->handle, reinterpret_cast<const base::UnkRefHandle*>(&actor));
            e->state = 2;
        }
        if (UnkWeakTarget_eb87c* target = reinterpret_cast<UnkWeakTarget_eb87c*>(e->handle.ptr)) {
            if (target->object)
                fn_7100360044(target->object);
        }
    }
}
