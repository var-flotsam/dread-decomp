// CAIGotoPointNavigationState::STarget's unit (tools/tu_map.py): a copy assignment of a target record:
// three words (+0x8 to +0x10) and a weak reference (+0x18) to the target object, re-taken through
// the object's own weak handle (game_s2/UnkWeakHandle.h). Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeakTargetRecord_ovb0072 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkWeakHandle* target;  // 0x18
};

UnkWeakTargetRecord_ovb0072* fn_7100702c18(UnkWeakTargetRecord_ovb0072* self, const UnkWeakTargetRecord_ovb0072* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0xc = other->field_0xc;
    self->field_0x10 = other->field_0x10;
    if (self != other)
        CopyWeakRef(&self->target, other->target);
    return self;
}
