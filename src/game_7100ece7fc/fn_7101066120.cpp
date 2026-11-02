// The destructor of the inline functor of a callback slot, out of line (the slot's own copies
// inline it: src/game_7100ece7fc/fn_710106d7f8.cpp): restores the base vtable, detaches itself from
// its object if attached, then releases the object (fn_7100232124).
#include "types.h"

extern void* g_unk_7101cac988[];

struct UnkBoundObj {
    u8 unk_0x0[0xc8];
    void* field_0xc8;  // the functor attached to it
};

void fn_7100232124(UnkBoundObj* obj);

struct UnkOutOfLineFunctor {
    void** vtable;
    void* field_0x8;
    void* field_0x10;
    // A union member (no type-based alias info): it is reloaded after the object's field is
    // cleared.
    union {
        UnkBoundObj* obj;
    };

    __attribute__((always_inline)) ~UnkOutOfLineFunctor() {
        vtable = &g_unk_7101cac988[2];
        if (obj->field_0xc8 == this)
            obj->field_0xc8 = nullptr;
        fn_7100232124(obj);
    }
};

void fn_7101066120(UnkOutOfLineFunctor* self) { self->~UnkOutOfLineFunctor(); }
