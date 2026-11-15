// game::logic::collision::CCollider unit: move assignment of a reflected iterator value (index and a
// base::global::CRntVector element, held in place or by reference) from another one; unlike
// base::UnkIterValue's it has no check against assigning to itself. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkVecIterValue {
    struct Value {
        u32 index;
        base::global::CRntVector<u8> element;
    };
    struct Ref {
        const u32* index;
        union {
            base::global::CRntVector<u8>* element;
        };
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;  // 0 empty, 1 owns `value`, 2 refers through `ref`

    u32& Index() { return state == 2 ? *const_cast<u32*>(ref.index) : value.index; }
    base::global::CRntVector<u8>& Element() { return state == 2 ? *ref.element : value.element; }
};

void fn_7100d431f0(UnkVecIterValue* self, UnkVecIterValue* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<base::global::CRntVector<u8>&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<base::global::CRntVector<u8>&&>(*other->ref.element);
    }
}
