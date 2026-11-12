// CModelUpdaterComponent unit (tools/tu_map.py, the name from reflection): the model updater's set-up
// record (fn_7101170da0), the wobble it restarts with random offsets (fn_7101172cec, fn_7101172de0),
// the random part shown of the model's slot table (fn_7101173a04) and a lookup in it (fn_71011743bc), its transform pushed to the model
// (fn_7101173824), the events it queues (fn_7101173c60), a reflected call that targets another
// actor's model updater (fn_7101173ddc), and the release of its loaded model (fn_7101174314). Names
// and layouts are placeholders inferred from the code.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkTU218.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU56.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringPool;

extern "C" int rand();

// --- The set-up record ---

// A string id (as CStrId) built empty: the default constructor.
struct UnkEmptyStrId_ovb0032 {
    base::global::UnkStringData* data;
    UnkEmptyStrId_ovb0032() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkScaleKey_ovb0032 {
    f32 field_0x0;
    f32 x, y, z;
    UnkScaleKey_ovb0032() : field_0x0(0.0f), x(1.0f), y(1.0f), z(1.0f) {}
};

struct UnkModelSetup_ovb0032 {
    bool field_0x0;
    UnkScaleKey_ovb0032 keys[6];  // 0x4
    UnkEmptyStrId_ovb0032 ids[3];  // 0x68
    u64 field_0x80;

    __attribute__((always_inline)) UnkModelSetup_ovb0032() : field_0x0(false), field_0x80(0) {}
};
static_assert(__builtin_offsetof(UnkModelSetup_ovb0032, field_0x80) == 0x80, "UnkModelSetup_ovb0032 layout");

void fn_7101170da0(UnkModelSetup_ovb0032* self) {
    new (self) UnkModelSetup_ovb0032();
}

// --- The wobble ---

struct UnkVec3fCopy_ovb0032 {
    f32 x, y, z;
    UnkVec3fCopy_ovb0032& operator=(const UnkVec3fCopy_ovb0032& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkWobbleRange_ovb0032 {
    f32 base;
    f32 spread;
};

struct UnkWobble_ovb0032 {
    UnkWobbleRange_ovb0032 ranges[3];  // 0x0
    CStrId name;  // 0x18
    UnkVec3fCopy_ovb0032 field_0x20;
    UnkVec3fCopy_ovb0032 field_0x2c;
    f32 speed;  // 0x38
    f32 time;  // 0x3c
    f32 base[3];  // 0x40
    f32 spread[3];  // 0x4c
    f32 offset[3];  // 0x58
    f32 direction[3];  // 0x64
    f32 phase[3];  // 0x70
};
static_assert(__builtin_offsetof(UnkWobble_ovb0032, phase) == 0x70, "UnkWobble_ovb0032 layout");

struct UnkWobbleOwner_ovb0032 {
    u8 pad_0x0[0x70];
    UnkWobble_ovb0032 wobble;  // 0x70
};

static inline f32 RandRange_ovb0032(f32 lo, f32 hi) {
    f32 t = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return lo + (hi - lo) * t;
}

static inline f32 RandSign_ovb0032() { return (rand() & 1) ? 1.0f : -1.0f; }

// Restarts the wobble: copies the ranges' bases and spreads, then picks each axis's offset within
// half its base around it and a random direction.
void fn_7101172de0(UnkWobble_ovb0032* self, f32 speed) {
    self->speed = speed;
    self->time = 0.0f;
    for (int i = 0; i < 3; ++i) {
        self->base[i] = self->ranges[i].base;
        self->spread[i] = self->ranges[i].spread;
    }
    for (int i = 0; i < 3; ++i) {
        f32 v = self->base[i];
        self->offset[i] = RandRange_ovb0032(v - v * 0.5f, v + v * 0.5f);
        self->phase[i] = 0.0f;
        self->direction[i] = RandSign_ovb0032();
    }
}

void fn_7101172cec(UnkWobbleOwner_ovb0032* self, f32 speed, f32 base_x, f32 spread_x, f32 base_y, f32 spread_y,
                   f32 base_z, f32 spread_z, const CStrId& name, const UnkVec3fCopy_ovb0032& a,
                   const UnkVec3fCopy_ovb0032& b) {
    UnkWobble_ovb0032& w = self->wobble;
    w.ranges[0].base = base_x;
    w.ranges[0].spread = spread_x;
    w.ranges[1].base = base_y;
    w.ranges[1].spread = spread_y;
    w.ranges[2].base = base_z;
    w.ranges[2].spread = spread_z;
    w.name = name;
    w.field_0x20 = a;
    w.field_0x2c = b;
    fn_7101172de0(&w, speed);
}

// --- The model ---

struct UnkModelPartKey_ovb0032 {
    u8 pad_0x0[0x30];
    s64 id;  // 0x30
};

struct UnkModelPart_ovb0032 {
    u8 pad_0x0[0x10];
    UnkModelPartKey_ovb0032** key;  // 0x10
    u8 pad_0x18[0x8];
};

struct UnkModelParts_ovb0032 {
    u8 pad_0x0[0x34];
    u32 count;  // 0x34
    UnkModelPart_ovb0032* parts;  // 0x38
};

struct UnkModelSlots_ovb0032 {
    u8 pad_0x0[0x78];
    UnkModelParts_ovb0032* parts;  // 0x78
};

// The model the updater loaded: its render item at +8 and its slot table at +0x138.
struct UnkModel_ovb0032 {
    u8 pad_0x0[0x8];
    u8 item[0x130];  // 0x8
    UnkModelSlots_ovb0032* slots;  // 0x138
};

struct UnkModelUpdater_ovb0032 {
    u8 pad_0x0[0x40];
    u32 layer;  // 0x40
    u8 pad_0x44[0x4];
    CFilePathStrId loaded_path;  // 0x48
    UnkModel_ovb0032* model;  // 0x50
};

struct UnkShowNodeComponentB;
struct UnkShowNodeEvent;

// Shows one part of the model's slot table picked at random and hides the others.
void fn_7101173a04(UnkShowNodeComponentB* component, UnkShowNodeEvent*) {
    auto* self = reinterpret_cast<UnkModelUpdater_ovb0032*>(component);
    s32 count = self->model->slots->parts->count;
    if (count == 0)
        return;
    s32 pick = (rand() & 0x7fff) % count;
    for (s32 i = 0; i < count; ++i) {
        UnkModelParts_ovb0032* parts = self->model->slots->parts;
        u64 hash = (*parts->parts[i].key)->id;
        fn_7100199420(parts, &hash, i == pick);
    }
}

// Whether the model's slot table has a part with the key `*id`.
bool fn_71011743bc(UnkModelUpdater_ovb0032* self, const s64* id) {
    if (self->model) {
        UnkModelParts_ovb0032* parts = self->model->slots->parts;
        for (u64 i = 0; i < parts->count; ++i) {
            s64 key = *id;
            if ((*parts->parts[i].key)->id == key)
                return true;
        }
    }
    return false;
}

void fn_71001435b4(UnkModel_ovb0032* self, const UnkVec3f* angle);

// Passes the transform's translation (bit 0 of `what`) and angles (bit 1) to the model.
void fn_7101173824(UnkModelUpdater_ovb0032* self, UnkXform* xform, const u32* what) {
    if (!self->model)
        return;
    if (*what & 1) {
        UnkVec3f t = xform->GetTranslation();
        fn_7100143578(self->model, &t);
    }
    if (*what & 2) {
        UnkModel_ovb0032* model = self->model;
        UnkVec3f a = xform->GetAngle();
        fn_71001435b4(model, &a);
    }
}

// --- Releasing the model ---

// Releases the id in two steps (the original reloads it after the decrement) and leaves the empty one.
static inline void ResetPath_ovb0032(CFilePathStrId& path) {
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (path.data && path.data != &pool->empty) {
            --path.data->ref_count;
            if (path.data->ref_count == 0) {
                fn_71000803a4(pool, path.data);
                pool = g_unk_7101d47200;
            }
        }
        path.data = &pool->empty;
    }
}

// Drops the loaded model (takes it off the render manager's layer and forgets its path), then loads
// the next one (fn_7101171a08).
void fn_7101174314(void* object, const void* path, const CRntString* extra) {
    auto* self = static_cast<UnkModelUpdater_ovb0032*>(object);
    if (self->model) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->model->item),
                      self->layer);
        ResetPath_ovb0032(self->loaded_path);
        self->model = nullptr;
    }
    fn_7101171a08(self, path, extra);
}

// --- Queued events ---

// A string id like CStrId whose destructor releases in two steps (the original reloads the id after
// the decrement).
class UnkEventName_ovb0032 : public base::global::UnkStrIdBase {
public:
    using UnkStrIdBase::UnkStrIdBase;
    UnkEventName_ovb0032(const UnkEventName_ovb0032& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkEventName_ovb0032() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkVec3fPod_ovb0032 {
    f32 x, y, z;
};

struct UnkModelEvent_ovb0032 {
    UnkVec3f position;  // 0x0
    UnkVec3fPod_ovb0032 field_0xc;
    UnkEventName_ovb0032 name;  // 0x18
    u32 field_0x20;

    UnkModelEvent_ovb0032()
        : position(0.0f, 0.0f, 0.0f), field_0xc{0.0f, 0.0f, 0.0f}, name(g_unk_710162e2ed, true), field_0x20(0) {}
};

struct UnkModelEventNode_ovb0032 {
    UnkModelEvent_ovb0032 event;
    UnkModelEventNode_ovb0032* next;  // 0x28
};

// A singly linked list of events (as base/UnkSList.h, with larger values).
struct UnkModelEventList_ovb0032 {
    u8 pad_0x0[0x8];
    union {
        u32 count;  // 0x8
    };
    UnkModelEventNode_ovb0032* head;  // 0x10
    UnkModelEventNode_ovb0032* tail;  // 0x18
    UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28

    void PushBack(const UnkModelEvent_ovb0032& event) {
        auto* n = static_cast<UnkModelEventNode_ovb0032*>(
            allocator->vf_0x40(sizeof(UnkModelEventNode_ovb0032), 8, flags));
        if (!n)
            return;
        new (&n->event) UnkModelEvent_ovb0032(event);
        n->next = nullptr;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }
};

struct UnkModelUpdaterEvents_ovb0032 {
    u8 pad_0x0[0x120];
    UnkModelEventList_ovb0032 events;  // 0x120
};

void fn_710116ed2c(UnkModelEvent_ovb0032* event, void* source);

// Reads an event (fn_710116ed2c) and queues it.
void fn_7101173c60(UnkModelUpdaterEvents_ovb0032* self, void* source) {
    UnkModelEvent_ovb0032 event;
    fn_710116ed2c(&event, source);
    self->events.PushBack(event);
}

// --- A reflected call on another actor's model updater ---

struct UnkStruct_7100233684;
namespace base::reflection {
class UnkReflObject;
}

struct UnkTriple_ovb0032 {
    f32 x, y, z;
    UnkTriple_ovb0032() : x(0.0f), y(0.0f), z(0.0f) {}
};

void fn_7101173ef4(UnkModelUpdater_ovb0032* self, UnkModelUpdater_ovb0032* target, const u64* part, bool flag,
                   const UnkTriple_ovb0032* a, const UnkTriple_ovb0032* b);

// Reads an entity name, a part name, a flag and two vectors from the stream and, when both this
// updater and the named entity's have a model, passes them on (fn_7101173ef4). Returns whether it did.
s32 fn_7101173ddc(void* object, const void* stream) {
    auto* self = static_cast<UnkModelUpdater_ovb0032*>(object);
    if (!self->model)
        return 0;
    u64 id;
    alignas(8) u32 kind;
    char entity_name[0x80];
    char part_name[0x80];
    bool flag;
    UnkTriple_ovb0032 a;
    UnkTriple_ovb0032 b;
    fn_710007cf30(const_cast<void*>(stream), "ssbffffff", entity_name, part_name, &flag, &a.x, &a.y, &a.z, &b.x,
                  &b.y, &b.z);
    UnkGameManager* manager = g_unk_7101d47310;
    u32 k = g_unk_71727bebb8;
    id = fn_7100001570(entity_name, __builtin_strlen(entity_name));
    kind = k;
    void* entity = fn_7100deba8c(manager, &id, &kind);
    if (!entity)
        return 0;
    auto* target = reinterpret_cast<UnkModelUpdater_ovb0032*>(
        fn_7100233684(static_cast<UnkStruct_7100233684*>(entity)));
    if (!target || !target->model)
        return 0;
    fn_710006663c(&id, part_name);
    fn_7101173ef4(self, target, &id, flag, &a, &b);
    return 1;
}
