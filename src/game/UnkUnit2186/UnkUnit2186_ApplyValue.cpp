// Unit 2186: a small callable. With its state at +0x38 set it ends in fn_7100c7b920 (never returns);
// otherwise, when the id it points to (+0x10) is valid (not -1) and the owner (+0x30) resolves to an
// object (fn_71008d5d2c), hands the object the id, the value it points to (+0x0) and 0
// (fn_71008e7f98). Names and layouts are placeholders.
#include "game/UnkTU1376.h"
#include "types.h"

struct UnkUnit2186Object;
[[noreturn]] void fn_7100c7b920();
void fn_71008e7f98(UnkUnit2186Object* object, const s64* id, f32 value, f32 time);

struct UnkUnit2186Callable {
    const f32* value;
    u8 pad_0x8[8];
    const s64* id;
    u8 pad_0x18[0x18];
    void* owner;
    u32 state;
};

void fn_7100c7b8bc(UnkUnit2186Callable* self) {
    if (self->state != 0)
        fn_7100c7b920();
    if (*self->id == -1)
        return;
    UnkUnit2186Object* object = fn_71008d5d2c(self->owner);
    if (object == nullptr)
        return;
    fn_71008e7f98(object, self->id, *self->value, 0.0f);
}
