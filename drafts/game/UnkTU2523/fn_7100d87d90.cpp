// UnkTU2523 (CEntity): copy constructor of the cached (index, callback set) value of an array iterator:
// empty, owned (1) or referring to the iterator's index and element (2); either way an owned copy.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/global/CRntVector.h"
#include "game/UnkCallbackBase.h"
#include "types.h"

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
};

struct UnkIterValue_ld87d90 {
    struct Value {
        u32 index;
        UnkCallbackSet set;
    };
    struct Ref {
        const u32* index;
        const UnkCallbackSet* set;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;
};

void fn_7100d87d90(UnkIterValue_ld87d90* self, const UnkIterValue_ld87d90* other) {
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        const auto* from = reinterpret_cast<const UnkIterValue_ld87d90::Value*>(other->storage);
        auto* value = reinterpret_cast<UnkIterValue_ld87d90::Value*>(self->storage);
        u32 index = from->index;
        new (&value->set) UnkCallbackSet(from->set);
        value->index = index;
    } else if (other->state == 2) {
        self->state = 1;
        const auto* ref = reinterpret_cast<const UnkIterValue_ld87d90::Ref*>(other->storage);
        auto* value = reinterpret_cast<UnkIterValue_ld87d90::Value*>(self->storage);
        u32 index = *ref->index;
        new (&value->set) UnkCallbackSet(*ref->set);
        value->index = index;
    }
}
