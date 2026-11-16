// CCutsceneComponent's unit: finds the source's current entry (99999 when there is none) and passes
// each actor, and the main actor, its flag for that entry (fn_71002334fc).
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CCutsceneComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;

struct UnkCutsceneEntry_A5_b65;
UnkCutsceneEntry_A5_b65* fn_710106b284(void* object, u32 id);

struct UnkCutsceneSource_l7a7c40 {
    u8 pad_0x0[0x130];
    u32 count;  // 0x130
    u8 pad_0x134[0x44];
    UnkCutsceneEntry_A5_b65* current;  // 0x178
};

// A weak handle's view: the object it points at (null once it is gone).
struct UnkCutsceneActorHandle_l7a7c40 {
    u8 pad_0x0[0x10];
    UnkStruct_71002334fc* target;  // 0x10
};

// A reflected property naming an actor, resolved once into a weak handle to it.
struct UnkCutsceneActorRef_l7a7c40 {
    void** vtable;
    u8 values[0x28];  // 0x8
    s32 state;  // 0x30: 0 none, 1 unresolved, 2 resolved
    UnkCutsceneActorHandle_l7a7c40* actor;  // 0x38
};

struct UnkCutsceneActor_l7a7c40 {
    void** vtable;
    CRntVector<bool> flags;  // 0x8
    u8 pad_0x30[0x8];
    UnkCutsceneActorRef_l7a7c40 ref;  // 0x38
};

struct UnkCutsceneComponent_l7a7c40 {
    u8 pad_0x0[0xd0];
    CRntVector<UnkElem_0x88> actors;  // 0xd0
    u8 pad_0xf8[0x68];
    CRntVector<bool> flags;  // 0x160
    u8 pad_0x188[0x58];
    UnkCutsceneActorHandle_l7a7c40* main_actor;  // 0x1e0
    u8 pad_0x1e8[0x18];
    UnkCutsceneSource_l7a7c40* source;  // 0x200
    u8 pad_0x208[0x8];
    bool has_main_actor;  // 0x210
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a7c40, has_main_actor) == 0x210, "layout");

static inline UnkCutsceneActor_l7a7c40* ActorAt(UnkCutsceneComponent_l7a7c40* self, u32 index) {
    return reinterpret_cast<UnkCutsceneActor_l7a7c40*>(&fn_71007a6b58(&self->actors, index));
}

static inline UnkStruct_71002334fc* Target(UnkCutsceneActorHandle_l7a7c40* h) {
    return h ? h->target : nullptr;
}

static inline void Update(UnkCutsceneActorRef_l7a7c40* e) {
    if (e->state == 1) {
        {
            UnkRefPtr r = fn_710023b580(e->values);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&e->actor),
                          reinterpret_cast<const base::UnkRefHandle*>(&r));
        }
        e->state = 2;
    }
}

static inline UnkStruct_71002334fc* Get(UnkCutsceneActorRef_l7a7c40* e) {
    Update(e);
    return Target(e->actor);
}

void fn_71007a7c40(UnkCutsceneComponent_l7a7c40* self) {
    u32 index = 99999;
    for (u32 i = 0; i < self->source->count; ++i) {
        if (fn_710106b284(self->source, i) == self->source->current) {
            index = i;
            break;
        }
    }
    for (u32 i = 0; i < self->actors.count; ++i) {
        if (Get(&ActorAt(self, i)->ref)) {
            UnkStruct_71002334fc* actor = Get(&ActorAt(self, i)->ref);
            fn_71002334fc(actor, ActorAt(self, i)->flags.GrowAt(index));
        }
    }
    if (self->has_main_actor) {
        UnkStruct_71002334fc* actor = Target(self->main_actor);
        fn_71002334fc(actor, self->flags.GrowAt(index));
    }
}
