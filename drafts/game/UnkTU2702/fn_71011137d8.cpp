// Unit 2702: the move assignment of the callback-set array iterator's cached value. Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkAssignElem_l1137d8 {
    void* vtable;
    UnkErasedVector callbacks;  // 0x8
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    u32 field_0x40;
};
static_assert(__builtin_offsetof(UnkAssignElem_l1137d8, field_0x40) == 0x40, "UnkAssignElem_l1137d8 layout");

// The cached (index, element) value: empty (0), owned (1) or referring to an iterator's (2).
struct UnkAssignValue_l1137d8 {
    union {
        struct {
            u32 index;
            UnkAssignElem_l1137d8 element;
        } value;
        struct {
            u32* index;
            UnkAssignElem_l1137d8* element;
        } ref;
    };
    u32 state;  // 0x50

    u32& Index() { return state == 2 ? *ref.index : value.index; }
    UnkAssignElem_l1137d8& Element() { return state == 2 ? *ref.element : value.element; }
};

UnkAssignValue_l1137d8* fn_71011137d8(UnkAssignValue_l1137d8* self, UnkAssignValue_l1137d8* other) {
    UnkAssignElem_l1137d8* src;
    if (other->state == 1) {
        self->Index() = other->value.index;
        src = &other->value.element;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        src = other->ref.element;
    } else {
        return self;
    }
    UnkAssignElem_l1137d8& dst = self->Element();
    fn_7100234f78(&dst.owner, &src->owner);
    dst.field_0x30 = src->field_0x30;
    fn_710023b29c(&dst.callbacks, &src->callbacks);
    src->field_0x30 = 0;
    fn_7100ad3588(&src->callbacks);
    src->owner.Set(nullptr);
    dst.field_0x40 = src->field_0x40;
    return self;
}
