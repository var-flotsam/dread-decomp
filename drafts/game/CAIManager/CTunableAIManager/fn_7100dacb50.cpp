// CTunableAIManager unit: removes `actor` from a fixed array of up to seven counted references (count
// +0x8, slots from +0xc), moving the last one into its place, then renumbers the remaining actors by slot
// and clears the flag at +0x44 when none is left. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "types.h"

using base::UnkRefCounted;
using base::UnkRefHandle;

base::UnkRefHandle* fn_7100daddec(base::UnkRefHandle* self, const base::UnkRefHandle* other);

struct UnkSlotActor_ldacb50 {
    u8 pad_0x0[0x14c54];
    u32 slot;  // 0x14c54
};

// A counted object whose +0x10 is the actor.
struct UnkSlotRef_ldacb50 : UnkRefCounted {
    UnkSlotActor_ldacb50* actor;
};

static inline UnkSlotActor_ldacb50* ActorOf(const UnkRefHandle& ref) {
    return ref.ptr ? static_cast<UnkSlotRef_ldacb50*>(ref.ptr)->actor : nullptr;
}

// Holds a reference for as long as it lives.
struct UnkRefHold_ldacb50 {
    UnkRefHandle ref;

    explicit UnkRefHold_ldacb50(const UnkRefHandle& other) : ref(other) {
        if (ref.ptr)
            ++ref.ptr->ref_count;
    }
    ~UnkRefHold_ldacb50() {
        if (ref.ptr && --ref.ptr->ref_count == 0)
            delete ref.ptr;
    }
};

struct UnkSlotList_ldacb50 {
    void** vtable;
    u32 count;                            // 0x8
    u8 storage[7 * sizeof(UnkRefHandle)];  // 0xc
    bool field_0x44;

    UnkRefHandle* items() { return reinterpret_cast<UnkRefHandle*>(storage); }

    // The slot holding `actor`, or the end.
    UnkRefHandle* begin() { return items(); }
    UnkRefHandle* end() { return items() + count; }
    UnkRefHandle* Find(const UnkSlotActor_ldacb50* actor) {
        for (UnkRefHandle* it = begin(); it != end(); ++it) {
            if (it->ptr && static_cast<UnkSlotRef_ldacb50*>(it->ptr)->actor == actor)
                return it;
        }
        return end();
    }

    // Keeps the first n references: drops the ones after them, or adds empty ones.
    void Resize(u32 n) {
        if (n >= count) {
            for (u32 i = count; i < n; ++i)
                items()[i].ptr = nullptr;
        } else {
            for (u64 i = n; i < count; ++i) {
                if (UnkRefCounted* p = items()[i].ptr) {
                    if (--p->ref_count == 0)
                        delete p;
                }
            }
        }
        count = n;
    }
};

void fn_7100dacb50(UnkSlotList_ldacb50* self, UnkSlotActor_ldacb50* actor) {
    UnkRefHandle* it = self->Find(actor);
    UnkRefHandle* end = self->end();
    if (it != end) {
        UnkRefHandle* last = end - 1;
        if (last != it)
            fn_7100daddec(it, last);
        self->Resize(self->count - 1);
    }
    if (self->count == 0)
        self->field_0x44 = false;
    u32 slot = 0;
    for (UnkRefHandle& ref : *self) {
        UnkRefHold_ldacb50 hold(ref);
        ActorOf(ref)->slot = slot++;
    }
}
