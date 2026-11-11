// A deleting destructor in an unnamed unit (0x71006f3390-0x71006f379c; the shape of fn_7100a5054c
// without a base): restore the vtable, destroy the two members (fn_7100d62bd8) and free the object.
// Names and layouts are placeholders.
#include "game/UnkTU1110.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

struct UnkObject_71006f3418 {
    void** vtable;
    u8 field_0x8[0x120];
    u8 field_0x128[0x120];
};

void fn_71006f3418(UnkObject_71006f3418* self) {
    self->vtable = g_unk_7101a6b5e0 + 2;
    fn_7100d62bd8(self->field_0x128);
    fn_7100d62bd8(self->field_0x8);
    operator delete(self);
}
