// GUI::CScrollMenuEntryItemRenderer (unit 1716 of tools/tu_map.py): refreshes the item's display object (+0x230),
// either through its slot 0xd0 or, when the flag at 0x278 is clear, by a slot 0x1b8 call with a static
// argument. Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7172765ff0[] __attribute__((visibility("hidden")));

struct UnkScrollItemDisplay {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0(void* arg);
};

struct UnkScrollItemRenderer {
    u8 pad_0x0[0x230];
    UnkScrollItemDisplay* display;
    u8 pad_0x238[0x40];
    bool flag_0x278;
};

typedef void (*UnkSetFn)(UnkScrollItemDisplay*, const char*, bool, bool);

void fn_7100b75b04(UnkScrollItemRenderer* self, void* arg) {
    UnkScrollItemDisplay* display = self->display;
    if (!display)
        return;
    if (self->flag_0x278) {
        display->vf_0xd0(arg);
        return;
    }
    UnkSetFn set = (*(UnkSetFn**)display)[0x1b8 / 8];
    set(display, g_unk_7172765ff0, true, true);
}
