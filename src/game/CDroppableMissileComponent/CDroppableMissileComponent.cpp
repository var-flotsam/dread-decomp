// Constructors of two droppable-item components (CDroppableMissileComponent's unit, vtable
// 0x7101a71068, and CDroppablePowerBombComponent's, vtable 0x7101a711f8) over the same base
// (fn_71007ba0d8): the vtable, the two 0x10-byte members at +0xa8 and +0xb8 (fn_71002ef5bc), then
// the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkDroppableBase;
void fn_71007ba0d8(UnkDroppableBase* self);
extern void* g_unk_7101a71068[];
extern void* g_unk_7101a711f8[];

struct UnkDroppable {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 field_0xa8[0x10];
    u8 field_0xb8[0x10];
};

template <void** Vtable>
__attribute__((always_inline)) inline void ConstructDroppable(UnkDroppable* self) {
    fn_71007ba0d8(reinterpret_cast<UnkDroppableBase*>(self));
    self->vtable = Vtable + 2;
    fn_71002ef5bc(self->field_0xa8);
    fn_71002ef5bc(self->field_0xb8);
    reinterpret_cast<void (*)(UnkDroppable*)>(self->vtable[3])(self);
}

void fn_71007bc8a4(UnkDroppable* self) { ConstructDroppable<g_unk_7101a71068>(self); }
void fn_71007bd974(UnkDroppable* self) { ConstructDroppable<g_unk_7101a711f8>(self); }
