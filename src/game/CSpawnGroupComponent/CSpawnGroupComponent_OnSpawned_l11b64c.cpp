// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_710111b64c, the step after an actor was
// spawned: appends the actor's weak handle to the targets (+0x90) and counts it (+0x198, +0x194); a
// group that spawns on its own restarts its timer and calls the owner's script functions for a full
// group and for the last spawn; then the script function for each spawn is called with the event's
// name and the actor's id. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CPickableComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

template <class Tag>
struct UnkSlotArray;
struct UnkTag_710031da18;
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* v, u32 capacity);

// An actor as the script calls see it: its id (+0x30) and its script (+0x1e0).
struct UnkSpawnActor_l11b64c {
    u8 pad_0x0[0x30];
    CStrId id;  // 0x30
    u8 pad_0x38[0x1a8];
    void* script;  // 0x1e0
};

// The text of an actor's id (the use count of its string is incremented first).
static inline const char* IdText(UnkSpawnActor_l11b64c* actor) {
    const char* text = base::reflection::refl_RntStringOf(actor->id).data;
    return text ? text : g_unk_710162e2ed;
}

// A counted reference to a weak handle, held while it is appended.
struct UnkHandleRef_l11b64c {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l11b64c(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_l11b64c() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot of the targets: a counted reference to a weak handle.
struct UnkHandleSlot_l11b64c {
    UnkWeakHandle* handle;

    void Assign(UnkWeakHandle* p) {
        if (p) {
            ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
    }
};

static inline void Append(CRntVector<UnkWeakHandle*>& list, const UnkHandleRef_l11b64c& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(&list), new_capacity);
    UnkHandleSlot_l11b64c* slot = new (&list.data[list.count]) UnkHandleSlot_l11b64c();
    slot->Assign(value.handle);
    ++list.count;
}

struct UnkSpawnGroupSpawned_l11b64c {
    u8 pad_0x0[0x30];
    UnkSpawnActor_l11b64c* owner;  // 0x30
    u8 pad_0x38[0x58];
    CRntVector<UnkWeakHandle*> targets;  // 0x90
    u8 pad_0xb8[0x82];
    bool self_spawning;  // 0x13a
    u8 pad_0x13b[0x41];
    f32 timer;  // 0x17c
    u8 pad_0x180[0xc];
    s32 last_spawn;   // 0x18c
    s32 full_count;   // 0x190
    s32 spawn_count;  // 0x194
    s32 target_count; // 0x198
    u8 pad_0x19c[0x1d];
    bool call_script;  // 0x1b9
    u8 pad_0x1ba[0x6e];
    const char* on_spawned;  // 0x228
    u8 pad_0x230[0x58];
    const char* on_full;  // 0x288
    u8 pad_0x290[0x28];
    const char* on_last_spawn;  // 0x2b8
};
static_assert(__builtin_offsetof(UnkSpawnGroupSpawned_l11b64c, timer) == 0x17c, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupSpawned_l11b64c, call_script) == 0x1b9, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupSpawned_l11b64c, on_last_spawn) == 0x2b8, "layout");

// Calls the owner's script function `function` with the owner's id.
static inline void CallOwnerScript(UnkSpawnGroupSpawned_l11b64c* self, const char* const& function) {
    UnkSpawnActor_l11b64c* owner = self->owner;
    if (void* script = owner->script) {
        const char* name = function;
        if (name && *name && self->call_script)
            fn_71009cde54(script, name, "s", IdText(owner));
    }
}

// The declaration CSwifterSpawnGroupComponent's unit shares gives the group's type.
struct UnkSwifterSpawnGroup_ovb0114;

void fn_710111b64c(UnkSwifterSpawnGroup_ovb0114* group, const char* name, UnkWeakTarget* spawned) {
    auto* self = reinterpret_cast<UnkSpawnGroupSpawned_l11b64c*>(group);
    {
        UnkHandleRef_l11b64c handle(GetWeakHandle(spawned));
        Append(self->targets, handle);
    }
    ++self->target_count;
    ++self->spawn_count;
    if (self->self_spawning) {
        self->timer = 0.0f;
        if (self->target_count == self->full_count)
            CallOwnerScript(self, self->on_full);
        if (self->spawn_count == self->last_spawn)
            CallOwnerScript(self, self->on_last_spawn);
    }
    const char* function = self->on_spawned;
    if (function && *function && self->call_script) {
        if (void* script = self->owner->script)
            fn_71009cde54(script, function, "ss", name,
                          IdText(reinterpret_cast<UnkSpawnActor_l11b64c*>(spawned)));
    }
}
