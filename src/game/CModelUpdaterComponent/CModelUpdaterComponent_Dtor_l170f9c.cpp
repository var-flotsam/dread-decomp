// CModelUpdaterComponent unit (tools/tu_map.py): the model updater's destructor. It hands a loaded model
// back to the model cache when the owner is a character (or takes it off the render manager's layer),
// then destroys its set-up record, its shared object, its event list and its ids. Names and layouts
// are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflObject;

// The cache of loaded models, created on first use.
struct UnkResourceCache_ca4ec;
struct UnkHashedSetOwner;
struct UnkHashedHandle;
extern UnkResourceCache_ca4ec* g_unk_71727bed88;
void fn_7100eca4ec(UnkResourceCache_ca4ec* self);
bool fn_7100ecafa8(UnkHashedSetOwner* self, UnkHashedHandle* owner);
struct UnkModel_l170f9c;
void fn_7100eca914(UnkResourceCache_ca4ec* self, CFilePathStrId* path, UnkModel_l170f9c* model);

// The set-up record's destructor (its three ids at 0x68).
template <u32 At>
struct UnkStrIdTripleAt;
void fn_7101171280(UnkStrIdTripleAt<0x68>* self);

static inline UnkResourceCache_ca4ec* ModelCache() {
    if (!g_unk_71727bed88) {
        auto* cache = static_cast<UnkResourceCache_ca4ec*>(fn_71000839a0(0xd8, 0x3f));
        fn_7100eca4ec(cache);
        g_unk_71727bed88 = cache;
    }
    return g_unk_71727bed88;
}

// A string id's destructor on the pool `global` points to (decrement and test in two steps).
static inline void ReleaseId(UnkStringData*& data, UnkStringPool*& global) {
    if (UnkStringPool* pool = global) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                StringPool_Release(pool, data);
                pool = global;
            }
        }
        data = &pool->empty;
    }
}

struct UnkModel_l170f9c {
    void** vtable;
    u8 item[0x130];  // 0x8, the render item
};

struct UnkModelOwner_l170f9c {
    u8 pad_0x0[0x28];
    UnkReflObject* object;  // 0x28
};

struct UnkModelEventNode_l170f9c {
    u8 pad_0x0[0x18];
    UnkStringData* name;  // 0x18
    u32 field_0x20;
    UnkModelEventNode_l170f9c* next;  // 0x28
};

struct UnkModelEventList_l170f9c {
    void** vtable;
    u32 count;  // 0x8
    UnkModelEventNode_l170f9c* head;  // 0x10
    UnkModelEventNode_l170f9c* tail;  // 0x18
    base::IAllocator* allocator;  // 0x20
    u32 flags;  // 0x28

    ~UnkModelEventList_l170f9c() {
        vtable = g_unk_7101cb46a0 + 2;
        for (UnkModelEventNode_l170f9c* n = head; n;) {
            UnkModelEventNode_l170f9c* next = n->next;
            ReleaseId(n->name, g_unk_7101d47108);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkShared_l170f9c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object when the count drops to zero
    s32 ref_count;
};

struct UnkModelUpdater_l170f9c : UnkBaseComponent {
    u32 layer;  // 0x40
    CFilePathStrId loaded_path;  // 0x48
    UnkModel_l170f9c* model;  // 0x50
    CFilePathStrId path;  // 0x58
    u8 pad_0x60[0x28];
    UnkStringData* name;  // 0x88
    u8 pad_0x90[0x90];
    UnkModelEventList_l170f9c events;  // 0x120
    UnkShared_l170f9c* shared;  // 0x150
    bool field_0x158;
    u8 pad_0x159[0x7];
    u8 setup[0x88];  // 0x160
};
static_assert(__builtin_offsetof(UnkModelUpdater_l170f9c, events) == 0x120, "layout");
static_assert(__builtin_offsetof(UnkModelUpdater_l170f9c, setup) == 0x160, "layout");

// Hands the loaded model back to the model cache when the owner is a character; false when the cache
// does not take it.
static inline bool ReturnToCache(UnkModelUpdater_l170f9c* self) {
    if (self->loaded_path.data == &g_unk_7101d47200->empty)
        return false;
    UnkResourceCache_ca4ec* cache = ModelCache();
    UnkReflObject* object = static_cast<UnkModelOwner_l170f9c*>(self->field_0x30)->object;
    UnkReflObject* character = object && fn_710009d50c(object->vf_0x0(), fn_71011abbd4()) ? object : nullptr;
    if (!fn_7100ecafa8(reinterpret_cast<UnkHashedSetOwner*>(cache), reinterpret_cast<UnkHashedHandle*>(character)))
        return false;
    fn_7100eca914(ModelCache(), &self->loaded_path, self->model);
    return true;
}

void fn_7101170f9c(UnkModelUpdater_l170f9c* self) {
    self->vtable = g_unk_7101cb4508 + 2;
    if (self->model) {
        if (!ReturnToCache(self)) {
            UnkModel_l170f9c* model = self->model;
            fn_71002199d8(g_unk_7101d47300,
                          model ? reinterpret_cast<UnkItemBase_71011a91c4*>(model->item) : nullptr, self->layer);
        }
        self->model = nullptr;
    }
    fn_7101171280(reinterpret_cast<UnkStrIdTripleAt<0x68>*>(self->setup));
    if (UnkShared_l170f9c* shared = self->shared) {
        if (--shared->ref_count == 0)
            shared->vf_0x10();
    }
    self->events.~UnkModelEventList_l170f9c();
    ReleaseId(self->name, g_unk_7101d47108);
    ReleaseId(self->path.data, g_unk_7101d47200);
    ReleaseId(self->loaded_path.data, g_unk_7101d47200);
    fn_71010c4194(self);
}
