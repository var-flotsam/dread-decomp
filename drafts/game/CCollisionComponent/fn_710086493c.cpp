// CCollisionComponent unit (tools/tu_map.py unit 1319): appends to `out` the weak handle of every
// registered collider whose mask (vf_0x58) shares a bit with `mask`, and of every collider group
// one of whose colliders does. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CType;

struct UnkAreaFXEntries_ovb0081;

// A registered collider or collider group, as far as this needs it.
struct UnkCollisionTarget_l86493c {
    virtual const CType* vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkWeakHandle** vf_0x28();  // the weak handle slot
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
};

// A game::logic::collision::CColliderGroup: its colliders in an inline array.
struct UnkColliderGroup_l86493c : UnkCollisionTarget_l86493c {
    u8 unk_0x8[0x11c];
    u32 count;  // 0x124
    u8 colliders[8 * sizeof(UnkCollisionTarget_l86493c*)];

    UnkCollisionTarget_l86493c** begin() { return reinterpret_cast<UnkCollisionTarget_l86493c**>(colliders); }
    UnkCollisionTarget_l86493c** end() { return begin() + count; }
};

struct UnkCollisionTargets_l86493c {
    u8 unk_0x0[0x98];
    base::global::CRntVector<UnkCollisionTarget_l86493c*> objects;  // 0x98
};

// Grows the handle list to hold `capacity` entries.
void fn_7100864cec(base::global::CRntVector<UnkWeakHandle*>* self, u32 capacity);

// A counted reference to a weak handle, held while it is appended.
struct UnkHandleRef_l86493c {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l86493c(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_l86493c() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot of the list: a counted reference to a weak handle.
struct UnkHandleSlot_l86493c {
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

// The weak handle of `target`, created on first use.
static inline UnkWeakHandle* WeakHandleOf(UnkCollisionTarget_l86493c* target) {
    UnkWeakHandle** slot = target->vf_0x28();
    if (!slot)
        return nullptr;
    UnkWeakHandle* handle = *slot;
    if (!handle) {
        handle = static_cast<UnkWeakHandle*>(fn_71000839a0(0x18, 0x3e));
        handle->ref_count = 0;
        *reinterpret_cast<void***>(handle) = &g_unk_7101a209e0[2];
        handle->target = reinterpret_cast<UnkWeakTarget*>(target);
        AssignRef(slot, handle);
    }
    return handle;
}

static inline void AppendHandle(base::global::CRntVector<UnkWeakHandle*>& list, const UnkHandleRef_l86493c& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100864cec(&list, new_capacity);
    UnkHandleSlot_l86493c* slot = new (&list.data[list.count]) UnkHandleSlot_l86493c();
    slot->Assign(value.handle);
    ++list.count;
}

void fn_710086493c(void* container, u32 mask, UnkAreaFXEntries_ovb0081* out) {
    UnkCollisionTargets_l86493c* self = static_cast<UnkCollisionTargets_l86493c*>(container);
    base::global::CRntVector<UnkWeakHandle*>& list = *reinterpret_cast<base::global::CRntVector<UnkWeakHandle*>*>(out);
    fn_7100864cec(&list, self->objects.count);
    for (u64 i = 0; i < self->objects.count; ++i) {
        UnkCollisionTarget_l86493c* object = self->objects.data[i];
        if (!object)
            continue;
        if (fn_710009d50c(object->vf_0x0(), fn_7100d361c0()) && (object->vf_0x58() & mask)) {
            UnkHandleRef_l86493c handle(WeakHandleOf(object));
            AppendHandle(list, handle);
        } else if (fn_710009d50c(object->vf_0x0(), fn_7100d454fc())) {
            UnkColliderGroup_l86493c* group = static_cast<UnkColliderGroup_l86493c*>(object);
            for (UnkCollisionTarget_l86493c** it = group->begin(); it != group->end(); ++it) {
                if ((*it)->vf_0x58() & mask) {
                    UnkHandleRef_l86493c handle(WeakHandleOf(group));
                    AppendHandle(list, handle);
                    break;
                }
            }
        }
    }
}
