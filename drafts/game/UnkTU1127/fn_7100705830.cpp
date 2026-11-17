// Unit 1127 (tools/tu_map.py): adds an actor to a list of weak handles when it is not there yet and
// its component at +0x1b8 is a CDoorLifeComponent; returns whether it was added. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU432.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;

// A counted reference to a weak handle, held while it is appended.
struct UnkHandleRef_l705830 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l705830(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_l705830() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot of the list: a counted reference to a weak handle.
struct UnkHandleSlot_l705830 {
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

static inline void Append(CRntVector<UnkWeakHandle*>& list, const UnkHandleRef_l705830& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = list.GrownCapacity(n);
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(&list), new_capacity);
    UnkHandleSlot_l705830* slot = new (&list.data[list.count]) UnkHandleSlot_l705830();
    slot->Assign(value.handle);
    ++list.count;
}

// An actor as seen here: its component at +0x1b8.
struct UnkDoorActor_l705830 : UnkWeakTarget {
    u8 pad_0x8[0x1b0];
    base::reflection::UnkReflObject* component;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkDoorActor_l705830, component) == 0x1b8, "layout");

struct UnkDoorList_l705830 {
    void* owner;
    CRntVector<UnkWeakHandle*> handles;  // 0x8
};

static inline bool Contains(const CRntVector<UnkWeakHandle*>& list, UnkWeakTarget* target) {
    bool found = false;
    for (UnkWeakHandle* const* it = list.begin(); it != list.end() && !found; ++it) {
        UnkWeakHandle* handle = *it;
        if ((handle ? handle->target : nullptr) == target)
            found = true;
    }
    return found;
}

bool fn_7100705830(UnkDoorList_l705830* self, UnkDoorActor_l705830* actor) {
    if (!actor || Contains(self->handles, actor))
        return false;
    base::reflection::UnkReflObject* component = actor->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_71008cb078()))
        return false;
    UnkHandleRef_l705830 handle(GetWeakHandle(actor));
    Append(self->handles, handle);
    return true;
}
