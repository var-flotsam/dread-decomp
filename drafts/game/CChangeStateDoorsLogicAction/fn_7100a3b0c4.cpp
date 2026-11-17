// CChangeStateDoorsLogicAction's unit (tools/tu_map.py unit 1547): builds a door-state value
// (fn_7100a3b0c4) from a variant holding either such a value (index 1) or references to its parts
// (index 2): a word, a callback set (vtable, callback vector, state, counted owner) and a second word.
// The result always holds a value. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkDoorCallbackSet_la3b0c4 {
    void** vtable;
    UnkErasedVector callbacks;  // 0x8
    u32 state;                  // 0x30
    base::UnkRefHandle owner;   // 0x38

    // Constructs this as a copy of `other`.
    void ConstructFrom(const UnkDoorCallbackSet_la3b0c4& other) {
        vtable = &g_unk_7101a20998[2];
        callbacks.data = nullptr;
        callbacks.count = 0;
        callbacks.capacity = 0;
        callbacks.step = 10;
        callbacks.field_0x14 = true;
        callbacks.flags = 0xf;
        callbacks.allocator = fn_7100083990();
        state = 0;
        vtable = &g_unk_7101a33fa0[2];
        owner.ptr = nullptr;
        if (base::UnkRefCounted* p = other.owner.ptr) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = owner.ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            owner.ptr = p;
        }
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};
static_assert(sizeof(UnkDoorCallbackSet_la3b0c4) == 0x40, "layout");

struct UnkDoorState_la3b0c4 {
    u32 field_0x0;
    UnkDoorCallbackSet_la3b0c4 set;  // 0x8
    u32 field_0x48;
};

// The parts a reference to a door state points at.
struct UnkDoorStateTail_la3b0c4 {
    UnkDoorCallbackSet_la3b0c4 set;
    u32 field_0x40;
};

struct UnkDoorStateRef_la3b0c4 {
    const u32* field_0x0;
    const UnkDoorStateTail_la3b0c4* tail;
};

struct UnkDoorStateVariant_la3b0c4 {
    alignas(8) u8 storage[0x50];
    u32 index;  // 0x50

    UnkDoorState_la3b0c4& value() { return *reinterpret_cast<UnkDoorState_la3b0c4*>(storage); }
    const UnkDoorState_la3b0c4& value() const { return *reinterpret_cast<const UnkDoorState_la3b0c4*>(storage); }
    const UnkDoorStateRef_la3b0c4& ref() const { return *reinterpret_cast<const UnkDoorStateRef_la3b0c4*>(storage); }

};

void fn_7100a3b0c4(UnkDoorStateVariant_la3b0c4* self, const UnkDoorStateVariant_la3b0c4* other) {
    self->index = 0;
    switch (other->index) {
    case 1: {
        self->index = 1;
        const UnkDoorState_la3b0c4& src = other->value();
        UnkDoorState_la3b0c4& v = self->value();
        v.field_0x0 = src.field_0x0;
        v.set.ConstructFrom(src.set);
        v.field_0x48 = src.field_0x48;
        break;
    }
    case 2: {
        self->index = 1;
        const UnkDoorStateRef_la3b0c4& r = other->ref();
        const UnkDoorStateTail_la3b0c4* tail = r.tail;
        UnkDoorState_la3b0c4& v = self->value();
        v.field_0x0 = *r.field_0x0;
        v.set.ConstructFrom(tail->set);
        v.field_0x48 = tail->field_0x40;
        break;
    }
    }
}
