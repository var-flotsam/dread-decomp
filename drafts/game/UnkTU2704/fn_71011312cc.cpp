// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): the move constructor of the cached (index, element)
// value of a callback set iterator; the element's move constructor is inlined. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "game/UnkCallbackBase.h"
#include <new>

#include "types.h"

struct UnkSpawnCallbackSet : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkSpawnCallbackSet(UnkSpawnCallbackSet&& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p) {
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

struct UnkSpawnIterValue {
    struct Value {
        u32 index;
        UnkSpawnCallbackSet element;
    };
    struct Ref {
        u32* index;
        UnkSpawnCallbackSet* element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    UnkSpawnIterValue(UnkSpawnIterValue&& other) : state(0) {
        Value* value = reinterpret_cast<Value*>(storage);
        if (other.state == 1) {
            state = 1;
            Value* from = reinterpret_cast<Value*>(other.storage);
            value->index = from->index;
            new (&value->element) UnkSpawnCallbackSet(static_cast<UnkSpawnCallbackSet&&>(from->element));
        } else if (other.state == 2) {
            state = 1;
            const Ref* ref = reinterpret_cast<const Ref*>(other.storage);
            UnkSpawnCallbackSet* element = ref->element;
            value->index = *ref->index;
            new (&value->element) UnkSpawnCallbackSet(static_cast<UnkSpawnCallbackSet&&>(*element));
        }
    }
};

UnkSpawnIterValue* fn_71011312cc(UnkSpawnIterValue* self, UnkSpawnIterValue* other) {
    return new (self) UnkSpawnIterValue(static_cast<UnkSpawnIterValue&&>(*other));
}
