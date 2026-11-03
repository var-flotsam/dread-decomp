// A constructor in CReserveTankInfo's unit (near copy of fn_71007bd974): the base constructor
// (fn_7101043904), the vtable 0x7101caa4d0, two members at +0x10 and +0x20 (fn_71002ef5bc), then
// the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "types.h"

void fn_7101043904(void* self);
extern char g_unk_7101caa4d0[];

struct UnkReserveTankObj {
    void** vtable;
    u8 pad_0x8[0x8];
    u8 field_0x10[0x10];
    u8 field_0x20[0x10];
};

void fn_7100f11520(UnkReserveTankObj* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101caa4d0) + 2;
    fn_71002ef5bc(self->field_0x10);
    fn_71002ef5bc(self->field_0x20);
    reinterpret_cast<void (*)(UnkReserveTankObj*)>(self->vtable[3])(self);
}
