// CEnemyMovement unit: tail-calls the virtual at slot 0x370 of its own object.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEnemyMovement_l0cdc48 {
    void** vtable;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010cdc48(UnkEnemyMovement_l0cdc48* self) {
    typedef void (*Fn)(UnkEnemyMovement_l0cdc48*);
    reinterpret_cast<Fn>(self->vtable[0x370 / 8])(self);
}
