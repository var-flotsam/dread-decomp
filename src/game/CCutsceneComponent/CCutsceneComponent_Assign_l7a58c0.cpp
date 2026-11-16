// CCutsceneComponent's unit: sets a cutscene entry's actor link (0x88-byte entry) from another link; when it is set,
// grows the per-place flags to the entry's place in its owner's list, and when it is resolved, takes the actor's
// name and flag and shows or hides the actor by the place's flag. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CStrId;

struct UnkCutsceneEntry_A5_b65;
UnkCutsceneEntry_A5_b65* fn_710106b284(void* object, u32 id);

struct UnkCutsceneActor_l7a58c0 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x17b];
    u8 field_0x1b3;
};
static_assert(__builtin_offsetof(UnkCutsceneActor_l7a58c0, field_0x1b3) == 0x1b3, "layout");

// The object an entry's handle refers to: the actor at +0x10.
struct UnkActorRef_l7a58c0 : base::UnkRefCounted {
    UnkCutsceneActor_l7a58c0* actor;  // 0x10
};

struct UnkCutsceneList_l7a58c0 {
    u8 pad_0x0[0x130];
    u32 count;  // 0x130
    u8 pad_0x134[0x44];
    UnkCutsceneEntry_A5_b65* current;  // 0x178
};
static_assert(__builtin_offsetof(UnkCutsceneList_l7a58c0, current) == 0x178, "layout");

// A lazily resolved reference to an actor: the reflected value naming it, the state (1: not yet resolved,
// 2: resolved) and the handle.
struct UnkActorLink_l7a58c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    UnkErasedVector source;     // 0x8
    u32 state;                  // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38
};
static_assert(sizeof(UnkActorLink_l7a58c0) == 0x40, "layout");

// The 0x88-byte entry the cutscene component keeps per actor.
struct UnkCutsceneEntry_l7a58c0 {
    virtual void vf_0x0();
    base::global::CRntVector<u8> flags;  // 0x8: one per place in the list
    CStrId name;                         // 0x30
    UnkActorLink_l7a58c0 link;           // 0x38
    UnkCutsceneList_l7a58c0* list;       // 0x78
    u8 field_0x80;
    u8 field_0x81;
};
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

void fn_71007a58c0(UnkCutsceneEntry_l7a58c0* self, const UnkActorLink_l7a58c0* link) {
    fn_7100234f78(&self->link.handle, &link->handle);
    self->link.state = link->state;
    fn_710023b29c(&self->link.source, &link->source);
    if (self->link.state == 0)
        return;
    if (self->link.state == 2) {
        if (UnkCutsceneActor_l7a58c0* actor = Actor(self->link.handle)) {
            if (self->name == g_unk_7101d470e0)
                self->name = actor->name;
            self->field_0x80 = actor->field_0x1b3;
        }
    }
    if (self->list) {
        u32 i;
        for (i = 0; i < self->list->count; ++i) {
            if (self->list->current == fn_710106b284(self->list, i))
                break;
        }
        // Grows the flags to cover place i, the new ones cleared.
        base::global::CRntVector<u8>& flags = self->flags;
        if (flags.field_0x14 && flags.count <= i) {
            u32 cap = flags.capacity;
            if (cap <= i && flags.step != 0) {
                u32 grown = (i / flags.step + 1) * flags.step;
                if (cap < grown) {
                    Reallocate(flags, grown);
                    cap = grown;
                }
            }
            u32 n = i + 1;
            if (cap < n)
                Reallocate(flags, n);
            __builtin_memset(&flags.data[flags.count], 0, n - flags.count);
            flags.count = n;
        }
        if (self->link.state == 2) {
            bool on = flags.data[i] != 0;
            fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(Actor(self->link.handle)), on);
        }
    }
    self->field_0x81 = self->field_0x80;
}
