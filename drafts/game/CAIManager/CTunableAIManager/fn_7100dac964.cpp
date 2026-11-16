// CTunableAIManager unit: appends `actor`'s weak handle to a fixed array of up to seven counted references
// (count +0x8, slots from +0xc), gives the actor the parameter of its slot from a table, renumbers the actors
// by slot and clears the flag at +0x44 when none is listed. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// One row per slot.
struct UnkSlotParams_ldac964 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};
__attribute__((visibility("hidden"))) extern const UnkSlotParams_ldac964 g_unk_710164bc3c[];

struct UnkSlotActor_ldac964 : UnkWeakTarget {
    u8 pad_0x8[0x14c40 - 0x8];
    u32 field_0x14c40;
    u8 pad_0x14c44[0x10];
    u32 slot;  // 0x14c54
};

// Holds a reference for as long as it lives.
struct UnkRefHold_ldac964 {
    UnkWeakHandle* ptr;

    explicit UnkRefHold_ldac964(UnkWeakHandle* p) : ptr(p) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkRefHold_ldac964() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

static inline UnkSlotActor_ldac964* ActorOf(UnkWeakHandle* handle) {
    return handle ? static_cast<UnkSlotActor_ldac964*>(handle->target) : nullptr;
}

struct UnkSlotList_ldac964 {
    void** vtable;
    u32 count;                              // 0x8
    u8 storage[7 * sizeof(UnkWeakHandle*)];  // 0xc
    bool field_0x44;

    UnkWeakHandle** items() { return reinterpret_cast<UnkWeakHandle**>(storage); }
    UnkWeakHandle** begin() { return items(); }
    UnkWeakHandle** end() { return items() + count; }

    void PushBack(UnkWeakHandle* handle) {
        UnkWeakHandle** slot = &items()[count];
        *slot = nullptr;
        AssignRef(slot, handle);
        ++count;
    }

    // Tells every listed actor its slot.
    void Renumber() {
        if (count == 0)
            field_0x44 = false;
        u32 slot = 0;
        for (UnkWeakHandle* handle : *this) {
            UnkRefHold_ldac964 hold(handle);
            ActorOf(hold.ptr)->slot = slot++;
        }
    }
};

void fn_7100dac964(UnkSlotList_ldac964* self, UnkSlotActor_ldac964* actor) {
    actor->field_0x14c40 = g_unk_710164bc3c[self->count].field_0x10;
    {
        UnkRefHold_ldac964 handle(GetWeakHandle(actor));
        self->PushBack(handle.ptr);
    }
    self->Renumber();
}
