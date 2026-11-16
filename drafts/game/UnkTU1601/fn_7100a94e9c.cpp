// Unit 1601 (CThermalDeviceComponent): the copy constructor of the slot that holds an (index, callback
// set) value inline or two pointers to one; either way the copy owns its value. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkSetBase_la94e9c {
    char* vtable;
    UnkErasedVector callbacks;
    u32 field_0x30;

    // An empty array with growth step 10 and allocation flags 0xf.
    UnkSetBase_la94e9c() {
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        callbacks.data = nullptr;
        callbacks.count = 0;
        callbacks.capacity = 0;
        callbacks.step = 10;
        callbacks.field_0x14 = true;
        callbacks.flags = 0xf;
        callbacks.allocator = fn_7100083990();
        field_0x30 = 0;
    }
};

// A callback set with the refcounted owner it belongs to.
struct UnkSet_la94e9c : UnkSetBase_la94e9c {
    base::UnkRefHandle owner;

    UnkSet_la94e9c(const UnkSet_la94e9c& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
        owner.Set(other.owner.ptr);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};

struct UnkSetElem_la94e9c {
    UnkSet_la94e9c set;
    u32 field_0x40;
};
static_assert(sizeof(UnkSetElem_la94e9c) == 0x48, "element size");

// Empty (0), an owned value (1) or pointers to someone else's (2).
struct UnkSlot_la94e9c {
    struct Value {
        u32 index;
        UnkSetElem_la94e9c element;
    };
    struct Ref {
        u32* index;
        UnkSetElem_la94e9c* element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;
};
static_assert(__builtin_offsetof(UnkSlot_la94e9c, state) == 0x50, "state offset");

void fn_7100a94e9c(UnkSlot_la94e9c* self, const UnkSlot_la94e9c* other) {
    using Value = UnkSlot_la94e9c::Value;
    using Ref = UnkSlot_la94e9c::Ref;
    self->state = 0;
    switch (other->state) {
    case 1: {
        self->state = 1;
        const Value* from = reinterpret_cast<const Value*>(other->storage);
        new (self->storage) Value(*from);
        break;
    }
    case 2: {
        self->state = 1;
        const Ref* ref = reinterpret_cast<const Ref*>(other->storage);
        const UnkSetElem_la94e9c* element = ref->element;
        new (self->storage) Value{*ref->index, *element};
        break;
    }
    }
}
