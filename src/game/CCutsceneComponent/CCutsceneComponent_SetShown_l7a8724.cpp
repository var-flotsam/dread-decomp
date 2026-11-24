// CCutsceneComponent's unit: sets whether an entry's actor is shown at a place of the component's list: unless the
// entry's link refuses (vf_0x20), and only when the place is the list's current one, records the flag (growing the
// per-place flags) and, once the link resolves to an actor, shows or hides it. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCutsceneComponent.h"
#include "game/UnkActorLink_l7a58c0.h"
#include "game/UnkActorRef_l7a58c0.h"
#include "game/UnkCutsceneEntry_l7a58c0.h"
#include "game/UnkCutsceneList_l7a58c0.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkCutsceneEntry_A5_b65;

struct UnkCutsceneActor_l7a58c0 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x17b];
    u8 field_0x1b3;
};
static_assert(__builtin_offsetof(UnkCutsceneActor_l7a58c0, field_0x1b3) == 0x1b3, "layout");

static_assert(__builtin_offsetof(UnkCutsceneList_l7a58c0, current) == 0x178, "layout");

static_assert(sizeof(UnkActorLink_l7a58c0) == 0x40, "layout");

static_assert(__builtin_offsetof(UnkCutsceneEntry_l7a58c0, field_0x81) == 0x81, "layout");

static inline UnkCutsceneActor_l7a58c0* Actor(const base::UnkRefHandle& handle) {
    if (base::UnkRefCounted* p = handle.ptr)
        return static_cast<UnkActorRef_l7a58c0*>(p)->actor;
    return nullptr;
}

// Moves the flags to a new array of new_capacity, freeing the old one.
static inline u8* Reallocate(base::global::CRntVector<u8>& v, u32 new_capacity) {
    u8* p = static_cast<u8*>(v.allocator->vf_0x40(new_capacity, 8, v.flags));
    if (v.data) {
        __builtin_memcpy(p, v.data, v.count);
        v.allocator->vf_0x48(v.data);
    }
    v.data = p;
    v.capacity = new_capacity;
    return p;
}

struct UnkCutsceneComponent_l7a8724 {
    u8 pad_0x0[0xd0];
    base::global::CRntVector<UnkElem_0x88> entries;  // 0xd0
    u8 pad_0xf8[0x108];
    UnkCutsceneList_l7a58c0* list;  // 0x200
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a8724, list) == 0x200, "layout");

static inline UnkCutsceneEntry_l7a58c0* Entry(UnkCutsceneComponent_l7a8724* self, u32 index) {
    return reinterpret_cast<UnkCutsceneEntry_l7a58c0*>(&fn_71007a6b58(&self->entries, index));
}

// Resolves the link on first use (state 1 to 2) and answers its actor, or null.
static inline UnkCutsceneActor_l7a58c0* ResolvedActor(UnkActorLink_l7a58c0* link) {
    if (link->state == 1) {
        {
            UnkRefPtr fresh = fn_710023b580(&link->source);
            fn_7100234f78(&link->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        link->state = 2;
    }
    return Actor(link->handle);
}

void CCutsceneComponent_SetEntryFlagAtCurrentListIndex(void* object, u32 index, u32 place, bool shown) {
    UnkCutsceneComponent_l7a8724* self = static_cast<UnkCutsceneComponent_l7a8724*>(object);
    UnkActorLink_l7a58c0* link = &Entry(self, index)->link;
    if (link->vf_0x20())
        return;
    u32 i;
    for (i = 0; i < self->list->count; ++i) {
        if (fn_710106b284(self->list, i) == self->list->current)
            break;
    }
    if (i != place)
        return;

    // Grows the flags to cover the place, the new ones cleared.
    base::global::CRntVector<u8>& flags = Entry(self, index)->flags;
    if (flags.field_0x14 && flags.count <= place) {
        u32 cap = flags.capacity;
        if (cap <= place && flags.step != 0) {
            u32 grown = (place / flags.step + 1) * flags.step;
            if (cap < grown) {
                Reallocate(flags, grown);
                cap = grown;
            }
        }
        u32 n = place + 1;
        if (cap < n)
            Reallocate(flags, n);
        __builtin_memset(&flags.data[flags.count], 0, n - flags.count);
        flags.count = n;
    }
    flags.data[place] = shown;

    if (ResolvedActor(&Entry(self, index)->link))
        fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(ResolvedActor(&Entry(self, index)->link)), shown);
}
