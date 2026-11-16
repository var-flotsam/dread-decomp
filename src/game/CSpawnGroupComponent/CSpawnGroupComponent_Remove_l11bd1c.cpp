// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_710111bd1c drops an actor from the group:
// from its targets (+0x90, one fewer counted at +0x198) and then notifies (fn_710111bb74), or else from
// its objects (+0x40), or, held in neither, only notifies. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkSpawnActor_ovb0065;
struct UnkSpawnGroupNotify_ovb0065;
void fn_710111bb74(UnkSpawnGroupNotify_ovb0065* self, const char* event, UnkSpawnActor_ovb0065* spawned);

template <class Tag>
struct UnkSlotArray;
struct UnkTag_710031da18;
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* v, u32 capacity);

// A counted reference to an object whose +0x10 is the actor.
struct UnkSpawnRef_l11bd1c : base::UnkRefCounted {
    UnkSpawnActor_ovb0065* actor;
};

static inline UnkSpawnActor_ovb0065* ActorOf(const base::UnkRefHandle& ref) {
    return ref.ptr ? static_cast<UnkSpawnRef_l11bd1c*>(ref.ptr)->actor : nullptr;
}

// Keeps the first n references: drops the ones after them, or adds empty ones.
static inline void Resize(CRntVector<base::UnkRefHandle>& refs, u32 n) {
    if (n >= refs.count) {
        fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(&refs), n);
        for (u32 i = refs.count; i < n; ++i)
            refs.data[i].ptr = nullptr;
    } else {
        for (u64 i = n; i < refs.count; ++i) {
            if (base::UnkRefCounted* p = refs.data[i].ptr) {
                s32& count = p->ref_count;
                if (--count == 0)
                    delete p;
            }
        }
    }
    refs.count = n;
}

// Removes the reference to `actor`, moving the last one into its place.
static inline bool Remove(CRntVector<base::UnkRefHandle>& refs, UnkSpawnActor_ovb0065* actor) {
    base::UnkRefHandle* it = refs.data;
    base::UnkRefHandle* end = it + refs.count;
    for (; it != end; ++it) {
        if (UnkSpawnActor_ovb0065* held = ActorOf(*it)) {
            if (held == actor)
                break;
        }
    }
    if (it == end)
        return false;
    base::UnkRefHandle* last = end - 1;
    if (it != last)
        fn_710031d208(it, last);
    Resize(refs, refs.count - 1);
    return true;
}

struct UnkSpawnGroupRemove_l11bd1c {
    u8 pad_0x0[0x40];
    CRntVector<base::UnkRefHandle> objects;  // 0x40
    u8 pad_0x68[0x28];
    CRntVector<base::UnkRefHandle> targets;  // 0x90
    u8 pad_0xb8[0xe0];
    s32 target_count;  // 0x198
};
static_assert(__builtin_offsetof(UnkSpawnGroupRemove_l11bd1c, target_count) == 0x198, "layout");

// The declaration CSwifterSpawnGroupComponent's unit shares gives the parameters as untyped.
struct UnkSwifterSpawnGroup_b49a4;

void fn_710111bd1c(UnkSwifterSpawnGroup_b49a4* group, void* event_name, void* removed) {
    auto* self = reinterpret_cast<UnkSpawnGroupRemove_l11bd1c*>(group);
    auto* event = static_cast<const char*>(event_name);
    auto* actor = static_cast<UnkSpawnActor_ovb0065*>(removed);
    if (!actor)
        return;
    if (Remove(self->targets, actor))
        --self->target_count;
    else if (Remove(self->objects, actor))
        return;
    fn_710111bb74(reinterpret_cast<UnkSpawnGroupNotify_ovb0065*>(self), event, actor);
}
