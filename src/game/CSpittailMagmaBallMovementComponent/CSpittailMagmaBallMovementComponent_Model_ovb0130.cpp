// CSpittailMagmaBallMovementComponent's unit (tools/tu_map.py, unit 1069): the pool's factory for the ball's
// model (fn_71006cae2c), the ball's handle appended to its spawner's list (fn_71006ca2ac) and the set-up
// that creates its model (fn_71006ca3fc). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU291.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// As src/base/UnkTaggedObjectSubCtors.cpp defines it.
struct UnkTaggedSubObject127;
struct UnkTaggedSubArg;
// As src/game/CSpittailMagmaBallMovementComponent/fn_71006caf80.cpp defines it.
struct UnkSpittailModelHolder;
bool fn_71006caf80(UnkSpittailModelHolder* self, const base::global::CFilePathStrId& path);

struct UnkModel;

struct UnkPoolNode_ovb0130 {
    void* key;
    void* value;
    UnkPoolNode_ovb0130* next;   // 0x10: the list of all nodes
    UnkPoolNode_ovb0130* prev;   // 0x18
    UnkPoolNode_ovb0130* chain;  // 0x20: the next node in the bucket
};

// The pool's chained hash map, as src/game/CAbilitySonar/CAbilitySonar_Pulse_ovb0085.cpp has it.
struct UnkPoolMap_ovb0130 {
    using Node = UnkPoolNode_ovb0130;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;       // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    __attribute__((always_inline)) void Insert(void* key, void* value) {
        Node** slot = nullptr;
        if (bucket_count) {
            u32 i = (u32)(u64)key % bucket_capacity;
            slot = &buckets[i];
        }
        Node* c = *slot;
        auto* n = static_cast<Node*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            n->key = key;
            n->value = value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = c;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            __builtin_memcpy(&n->prev, &tail, sizeof(Node*));  // the original reloads the tail
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkDeletable_ovb0130 {
    void** vtable;
    void Delete() { reinterpret_cast<void (*)(UnkDeletable_ovb0130*)>(vtable[1])(this); }
};

// fn_71006cae2c: the pool's factory for the magma ball's model (0x218 bytes), loaded from `path` unless it
// is empty and filed in the pool's map under its scene node (+0x8). Declared with a void result in
// src/game/CAreaFXComponent/CAreaFXComponent_Models_ovb0081.cpp.
// Kept out of line: the original calls it.
__attribute__((noinline)) void* fn_71006cae2c(UnkRenderPool* pool, const CFilePathStrId& path, UnkModel** out, CStrId* name) {
    UnkModel* object = static_cast<UnkModel*>(fn_71000839a0(0x218, 0x38));
    fn_710014333c(reinterpret_cast<UnkTaggedSubObject127*>(object), reinterpret_cast<UnkTaggedSubArg*>(name));
    void* node = nullptr;
    if (object) {
        fn_710020d32c(reinterpret_cast<char*>(object) + 0x20);
        if (path.data != &g_unk_7101d47200->empty &&
            !fn_71006caf80(reinterpret_cast<UnkSpittailModelHolder*>(object), path)) {
            reinterpret_cast<UnkDeletable_ovb0130*>(object)->Delete();
            return nullptr;
        }
        node = reinterpret_cast<u8*>(object) + 8;
        reinterpret_cast<UnkPoolMap_ovb0130*>(pool)->Insert(node, object);
    }
    if (out)
        *out = object;
    return node;
}

using base::reflection::UnkReflObject;

// As src/game/CAbilityMultiLockon/CAbilityMultiLockon_Targets_ovb0092.cpp declares them.
template <class Tag>
struct UnkSlotArray;
struct UnkTag_710031da18;
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* v, u32 capacity);

struct UnkSpittailMovement_ovb0130;
void fn_71006df774(UnkSpittailMovement_ovb0130* self);

struct UnkHandlePtr_ovb0130 {
    UnkWeakHandle* p = nullptr;
};

struct UnkHandleRef_ovb0130 {
    UnkWeakHandle* p;
    UnkHandleRef_ovb0130(UnkWeakHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_ovb0130() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

struct UnkVulkranAI_ovb0130 {
    u8 pad_0x0[0x14760];
    base::UnkVector<UnkHandlePtr_ovb0130> balls;  // 0x14760
};

struct UnkSpittailActor_ovb0130 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkSpittailLink_ovb0130 {
    u8 pad_0x0[0x10];
    UnkSpittailActor_ovb0130* actor;  // 0x10
};

struct UnkSpittailMovement_ovb0130 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x230 - 0x38];
    UnkSpittailLink_ovb0130* spawner;  // 0x230
    u8 pad_0x238[0x5d8 - 0x238];
    bool field_0x5d8;
};

// fn_71006ca2ac: after the base step (fn_71006df774) the flag at +0x5d8 is cleared, and the ball's handle is
// appended to the spawning CVulkranAIComponent's list (+0x14760) when the spawner's actor has one.
void fn_71006ca2ac(UnkSpittailMovement_ovb0130* self) {
    fn_71006df774(self);
    self->field_0x5d8 = false;
    UnkSpittailLink_ovb0130* link = self->spawner;
    if (!link || !link->actor)
        return;
    UnkReflObject* o = link->actor->ai;
    if (!o || !fn_710009d50c(o->vf_0x0(), fn_71006db2b8()))
        return;
    base::UnkVector<UnkHandlePtr_ovb0130>& v = reinterpret_cast<UnkVulkranAI_ovb0130*>(o)->balls;
    UnkHandleRef_ovb0130 ref(fn_7100d78378(self->owner));
    u32 n = v.count;
    u32 capacity = v.capacity;
    if (capacity <= n && v.step != 0)
        capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(&v), capacity);
    UnkWeakHandle** slot = &v.data[v.count].p;
    __builtin_memset(slot, 0, sizeof(*slot));
    AssignRef(slot, ref.p);
    ++v.count;
}

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// As src/game/CAreaFXComponent/CAreaFXComponent_Models_ovb0081.cpp declares them.
// As src/game/CVulkranMagmaBallMovementComponent/CVulkranMagmaBallMovementComponent.cpp defines it.
struct UnkVulkranMagmaBallMovement;
struct UnkKraidCreatureArg;
bool fn_71006df684(UnkVulkranMagmaBallMovement* self, UnkKraidCreatureArg* arg);

extern CFilePathStrId g_unk_7172708918 __attribute__((visibility("hidden")));  // the magma ball's model

// An id built from a string out of line, released like CStrId.
struct UnkSpittailBuiltId_ovb0130 {
    union {
        CStrId id;
    };

    explicit UnkSpittailBuiltId_ovb0130(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkSpittailBuiltId_ovb0130() { id.~CStrId(); }
};

struct UnkSpittailBall_ovb0130 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x2e8];
    CStrId field_0x300;
    u8 pad_0x308[0x2d8];
    UnkReflObject* field_0x5e0;
    UnkModel* model;  // 0x5e8
};
static_assert(__builtin_offsetof(UnkSpittailBall_ovb0130, model) == 0x5e8, "layout");

static inline UnkSceneNode* Node_ovb0130(UnkModel* model) {
    return model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

bool fn_71006ca3fc(UnkSpittailBall_ovb0130* self, UnkKraidCreatureArg* arg) {
    bool result = fn_71006df684(reinterpret_cast<UnkVulkranMagmaBallMovement*>(self), arg);
    UnkReflObject* obj = self->field_0x10;
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_71006c9848())) {
        self->field_0x5e0 = nullptr;
    } else {
        self->field_0x5e0 = obj;
        UnkSpittailBuiltId_ovb0130 built(*reinterpret_cast<const CRntString*>(reinterpret_cast<const char*>(obj) + 0x68));
        self->field_0x300.ReleaseAndTake(built.id);
    }
    if (!self->model) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkModel* model = nullptr;
        fn_71006cae2c(manager->field_0x218, g_unk_7172708918, &model, &g_unk_7101d470e0);
        fn_7100219ae0(manager, Node_ovb0130(model), 0);
        self->model = model;
        if (model) {
            fn_71001436a0(model, 0);
            fn_710014364c(self->model, 0);
        }
    }
    return result;
}
