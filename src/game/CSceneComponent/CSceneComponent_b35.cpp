// A function in the CSceneComponent unit: runs fn_71010c4228 on the object, sets the flag at +0x44, then
// calls virtual slot 45 with the byte at +0x1b3 of the object at +0x18. Names and layouts are
// placeholders.
#include "game/CComponent.h"
#include "types.h"

struct UnkParent;

struct UnkSceneOwner {
    u8 unk_0x0[0x1b3];
    u8 field_0x1b3;
};
struct UnkSceneComponent {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkSceneOwner* owner;
    u8 pad_0x20[0x24];
    bool field_0x44;
};

// NON_MATCHING(minor): the original reads the owner's flag at +0x1b3 before the vtable slot, this reads it
// after; the instructions are the same. An empty barrier on `value` made it match; the readable form is
// kept and tools/equiv_test.py passes it (CLAUDE.md: readability outranks a match).
void fn_71009f194c(UnkSceneComponent* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->field_0x44 = true;
    u8 value = self->owner->field_0x1b3;
    reinterpret_cast<void (*)(UnkSceneComponent*, u8)>(self->vtable[45])(self, value);
}
