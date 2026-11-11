// CFrozenComponent's unit (tools/tu_map.py unit 1260): the frozen animation's track ids (from the
// virtual getters at 0x1f8 and 0x200), the thaw blend and its layer weight, and the destructor. Names
// and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkComponent.h"
#include "game/animtree/CLayerNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct CFrozenComponent2;
template <u32 Offset>
struct UnkSmOwner_A5_b62;  // src/game/UnkStateMachineUpdateA5_b62.cpp
// As src/game/CAutsniperShootAttack/fn_71003d5a14.cpp declares it.
bool fn_7101179c3c(UnkAnimationComponent* self, u64 key);

struct UnkLayerNode;
void fn_7101177a70(void* handle, u64 key);

extern char g_unk_7172721120[] __attribute__((visibility("hidden")));

// The animation's tree nodes by name (parallel key and value arrays).
struct UnkFrozenNodeMap_ov110 {
    u8 pad_0x0[0x78];
    base::reflection::UnkReflObject** values;  // 0x78
    UnkStringData** keys;                      // 0x80
    u32 count;                                 // 0x88
};

static inline base::reflection::UnkReflObject** Find_ov110(UnkFrozenNodeMap_ov110* map, UnkStringData* const& key) {
    for (u32 i = 0; i < map->count; ++i)
        if (map->keys[i] == key)
            return &map->values[i];
    return &map->values[map->count];
}

struct UnkFrozenOwner_ov110 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

// The string data's hash (+0x30).
static inline u64 Hash_ov110(const CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
}

struct UnkFrozenComponent_ov110 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkFrozenOwner_ov110* owner;  // 0x30
    u8 pad_0x38[0x1f0];
    u8 machine[0xb0];  // 0x228, the state machine
    void* field_0x2d8;
    u8 pad_0x2e0[0xbc];
    f32 value;     // 0x39c
    f32 start;     // 0x3a0
    f32 target;    // 0x3a4
    f32 duration;  // 0x3a8
    f32 time;      // 0x3ac

    bool GetTrackIds(CStrId* track, CStrId* blend) {
        typedef bool (*Fn)(UnkFrozenComponent_ov110*, CStrId*, CStrId*);
        return (*reinterpret_cast<Fn**>(this))[0x1f8 / 8](this, track, blend);
    }
    bool GetThawIds(CStrId* track, CStrId* blend) {
        typedef bool (*Fn)(UnkFrozenComponent_ov110*, CStrId*, CStrId*);
        return (*reinterpret_cast<Fn**>(this))[0x200 / 8](this, track, blend);
    }
};
static_assert(__builtin_offsetof(UnkFrozenComponent_ov110, field_0x2d8) == 0x2d8, "layout");
static_assert(__builtin_offsetof(UnkFrozenComponent_ov110, value) == 0x39c, "layout");

// Clears both ids.
bool fn_71007fb18c(UnkFrozenComponent_ov110* self, CStrId* track, CStrId* blend) {
    *track = g_unk_7101d470e0;
    *blend = g_unk_7101d470e0;
    return true;
}

void fn_71007facf4(UnkFrozenComponent_ov110* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation || !self->field_0x2d8)
        return;
    CStrId track = g_unk_7101d470e0;
    CStrId blend = g_unk_7101d470e0;
    if (self->GetTrackIds(&track, &blend)) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (track.data != &pool->empty && blend.data != &pool->empty) {
            self->start = self->value;
            self->target = 0.0f;
            self->duration = 0.5f;
            self->time = 0.0f;
            fn_710117abfc(animation, 0, g_unk_7172721120, Hash_ov110(track));
        }
    }
    fn_710117abfc(animation, 0, g_unk_7172721120, 0xf4061ff25a2163d6ull);
}

void fn_71007f9d7c(UnkSmOwner_A5_b62<0x228>* owner, f32 dt) {
    UnkFrozenComponent_ov110* self = reinterpret_cast<UnkFrozenComponent_ov110*>(owner);
    CStrId track = g_unk_7101d470e0;
    CStrId blend = g_unk_7101d470e0;
    if (self->GetThawIds(&track, &blend)) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (track.data != &pool->empty && blend.data != &pool->empty &&
            !fn_7101179c3c(self->owner->animation, Hash_ov110(track))) {
            f32 value = self->value;
            f32 target = self->target;
            if (!(value + -0.00001f <= target) || !(value + 0.00001f >= target)) {
                f32 time = self->time + dt;
                self->time = time;
                if (time < self->duration) {
                    f32 t = time / self->duration;
                    t = !(t >= 1.0f) ? t : 1.0f;
                    f32 start = self->start;
                    target = start + t * (target - start);
                }
                self->value = target;
                fn_71007f95b8(reinterpret_cast<CFrozenComponent2*>(self), target);
            }
        }
    }
}

// Sets the thaw blend layer's weight and, at (about) zero, stops the thaw track.
void fn_71007f95b8(CFrozenComponent2* component, f32 weight) {
    UnkFrozenComponent_ov110* self = reinterpret_cast<UnkFrozenComponent_ov110*>(component);
    CStrId track = g_unk_7101d470e0;
    CStrId blend = g_unk_7101d470e0;
    if (self->GetThawIds(&track, &blend)) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (track.data != &pool->empty && blend.data != &pool->empty) {
            UnkAnimationComponent* animation = self->owner->animation;
            if (animation) {
                UnkFrozenNodeMap_ov110* map = reinterpret_cast<UnkFrozenNodeMap_ov110*>(animation->field_0x40);
                base::reflection::UnkReflObject** it = Find_ov110(map, blend.data);
                if (it != &map->values[map->count]) {
                    base::reflection::UnkReflObject* node = *it;
                    if (node && fn_710009d50c(node->vf_0x0(), fn_71002df4ec()) &&
                        fn_710009d50c(node->vf_0x0(), fn_71002e43a4()))
                        fn_71002e465c(reinterpret_cast<UnkLayerNode*>(node), weight);
                }
            }
            if (weight + 0.001f >= 0.0f && weight + -0.001f <= 0.0f)
                fn_7101177a70(animation, Hash_ov110(track));
        }
    }
}

extern void* g_unk_7101a740d0[];
extern char g_unk_7101a74370[];
extern void* g_unk_7101a743c8[];

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

struct UnkFrozenHolderTag_ov110;
using UnkFrozenHolder_ov110 = base::UnkValueHolder<UnkFrozenHolderTag_ov110>;

static inline __attribute__((always_inline)) void DestroyHolder_ov110(UnkFrozenHolder_ov110* self) {
    for (int i = 3; i >= 0; --i)
        self->pairs[i].~UnkClearedPair();
    self->vtable = g_unk_7101a74370 + 0x10;
    fn_71000a1c60(&self->value);
}

struct UnkFrozenLink_ov110 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

struct UnkFrozenListener_ov110 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkFrozenLink_ov110* link;  // 0x18
};

static inline __attribute__((always_inline)) void DestroyListener_ov110(UnkFrozenListener_ov110* self) {
    self->vtable = &g_unk_7101a22d48[2];
    UnkFrozenLink_ov110* link = self->link;
    if (!link)
        return;
    if (void* owner = link->owner) {
        fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), self);
        link = self->link;
        if (!link)
            return;
    }
    if (--link->ref_count == 0)
        link->Release();
}

struct UnkFrozenMachine_ov110 {
    void** vtable;
    u64 field_0x8;
    void* current;   // 0x10
    void* previous;  // 0x18
    u32 count;       // 0x20
};

template <class T>
static inline __attribute__((always_inline)) void Free_ov110(base::UnkVector<T>* v) {
    if (v->data) {
        v->count = 0;
        v->allocator->vf_0x48(v->data);
        v->capacity = 0;
    }
}

struct UnkFrozenComponentDtor_ov110 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkFrozenListener_ov110 listener;  // 0x48
    UnkFrozenHolder_ov110 holder_0x68;
    UnkFrozenHolder_ov110 holder_0xd8;
    UnkFrozenHolder_ov110 holder_0x148;
    UnkFrozenHolder_ov110 holder_0x1b8;
    UnkFrozenMachine_ov110 machine;  // 0x228
    u8 pad_0x250[0x88];
    void* field_0x2d8;
    base::UnkVector<void**> slots;  // 0x2e0
    base::UnkVector<void*> field_0x308;
    base::UnkVector<void*> field_0x330;
    u8 pad_0x358[0x8];
    UnkLinkList* link;     // 0x360
    void* callback_owner;  // 0x368
    void* callback;        // 0x370
};
static_assert(__builtin_offsetof(UnkFrozenComponentDtor_ov110, machine) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkFrozenComponentDtor_ov110, link) == 0x360, "layout");

// The destructor: unhooks the link (+0x360), clears the callback and every slot the array at +0x2e0
// points at, frees the three arrays, then destroys the state machine, the four value holders, the
// listener (+0x48) and the base component.
struct UnkAreaFXBase_ovb0081;  // src/game/CAreaFXComponent/CAreaFXComponent_Lifecycle_ovb0081.cpp

void fn_71007f8d80(UnkAreaFXBase_ovb0081* base) {
    UnkFrozenComponentDtor_ov110* self = reinterpret_cast<UnkFrozenComponentDtor_ov110*>(base);
    self->vtable = &g_unk_7101a740d0[2];
    UnkLinkList** slot = &self->link;
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
    self->callback = nullptr;
    self->callback_owner = nullptr;
    for (void*** it = self->slots.data, ***end = it + self->slots.count; it != end; ++it)
        **it = nullptr;
    self->slots.count = 0;
    self->field_0x308.count = 0;
    self->field_0x330.count = 0;
    Free_ov110(&self->field_0x330);
    Free_ov110(&self->field_0x308);
    Free_ov110(&self->slots);
    self->machine.current = nullptr;
    self->machine.vtable = &g_unk_7101a743c8[2];
    self->machine.previous = nullptr;
    self->machine.count = 0;
    DestroyHolder_ov110(&self->holder_0x1b8);
    DestroyHolder_ov110(&self->holder_0x148);
    DestroyHolder_ov110(&self->holder_0xd8);
    DestroyHolder_ov110(&self->holder_0x68);
    DestroyListener_ov110(&self->listener);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
