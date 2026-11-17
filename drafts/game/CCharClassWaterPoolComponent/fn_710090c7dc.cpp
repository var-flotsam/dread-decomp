// CCharClassWaterPoolComponent's unit: copy-constructs an iterator's cached (index, element) value
// of a callback-set array; an owned value or a reference both become an owned copy.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "game/UnkCallbackBase.h"
#include "types.h"

// As src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp defines them.
struct UnkCallbackSet : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet(const UnkCallbackSet& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }

    UnkCallbackSet(UnkCallbackSet&& other);  // fn_71007b1928

    __attribute__((always_inline)) ~UnkCallbackSet() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkCallbackSetElem {
    UnkCallbackSet set;
    u32 field_0x40;
};

// The cached (index, element) value: empty (0), owned (1) or referring to the iterator's (2).
struct UnkIterValue {
    struct Value {
        u32 index;
        UnkCallbackSetElem element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    struct Ref {
        u32* index;
        UnkCallbackSetElem* element;
    };

    UnkIterValue() : state(0) {}
    // An owned value either way: a reference's element is moved into a value of its own.
    __attribute__((always_inline)) void MoveFrom(UnkIterValue& other) {
        state = 0;
        Value* value = reinterpret_cast<Value*>(storage);
        if (other.state == 1) {
            state = 1;
            Value* from = reinterpret_cast<Value*>(other.storage);
            value->index = from->index;
            new (&value->element) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(from->element));
        } else if (other.state == 2) {
            state = 1;
            const Ref* ref = reinterpret_cast<const Ref*>(other.storage);
            UnkCallbackSetElem* element = ref->element;
            value->index = *ref->index;
            new (&value->element) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*element));
        }
    }
    ~UnkIterValue() {
        if (state == 2) {
        } else if (state == 1) {
            reinterpret_cast<Value*>(storage)->~Value();
        } else {
            return;
        }
        state = 0;
    }
};

void fn_710090c7dc(UnkIterValue* self, const UnkIterValue* other) {
    self->state = 0;
    UnkIterValue::Value* value = reinterpret_cast<UnkIterValue::Value*>(self->storage);
    if (other->state == 1) {
        self->state = 1;
        new (value) UnkIterValue::Value(*reinterpret_cast<const UnkIterValue::Value*>(other->storage));
    } else if (other->state == 2) {
        self->state = 1;
        const UnkIterValue::Ref* ref = reinterpret_cast<const UnkIterValue::Ref*>(other->storage);
        const UnkCallbackSetElem* element = ref->element;
        value->index = *ref->index;
        new (&value->element) UnkCallbackSetElem(*element);
    }
}
