// Unit 2702 (CAIComponent): the copy assignment of an iterator's cached (index, callback set element)
// value, shaped as the water pool's. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkAiElem_l1135c4 {
    void* vtable;
    UnkErasedVector callbacks;  // 0x8
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    u32 field_0x40;
};
static_assert(__builtin_offsetof(UnkAiElem_l1135c4, field_0x40) == 0x40, "UnkAiElem_l1135c4 layout");

// The cached (index, element) value: empty (0), owned (1) or referring to an iterator's (2).
struct UnkAiIterValue_l1135c4 {
    union {
        struct {
            u32 index;
            UnkAiElem_l1135c4 element;
        } value;
        struct {
            u32* index;
            UnkAiElem_l1135c4* element;
        } ref;
    };
    u32 state;  // 0x50

    u32& Index() { return state == 2 ? *ref.index : value.index; }
    UnkAiElem_l1135c4& Element() { return state == 2 ? *ref.element : value.element; }
};

UnkAiIterValue_l1135c4* fn_71011135c4(UnkAiIterValue_l1135c4* self, UnkAiIterValue_l1135c4* other) {
    if (self == other)
        return self;
    self->Index() = other->Index();
    UnkAiElem_l1135c4& src = other->Element();
    UnkAiElem_l1135c4& dst = self->Element();
    const base::UnkRefHandle* src_owner = &src.owner;
    fn_7100234f78(&dst.owner, src_owner);
    dst.field_0x30 = src.field_0x30;
    fn_710023b29c(&dst.callbacks, &src.callbacks);
    dst.field_0x40 = src.field_0x40;
    return self;
}
