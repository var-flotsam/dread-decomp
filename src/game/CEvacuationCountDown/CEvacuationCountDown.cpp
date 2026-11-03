// CEvacuationCountDown: fn_71007da578 runs fn_71007dc10c, then, unless the flag at +0x79 is set
// and the one at +0x7a is clear, sends "OnHide" to the GUI object at +0x10 of +0x80 (its
// vf_0x1e8); otherwise it runs its own vf_0x168. Names and layouts are placeholders.
#include "types.h"

struct UnkCountDownGui {
    virtual void vf_0x0();
    void SendEvent(const char* name, bool now) {
        reinterpret_cast<void (*)(UnkCountDownGui*, const char*, bool)>((*reinterpret_cast<void***>(this))[0x1e8 / 8])(
            this, name, now);
    }
};

struct UnkCountDownGuiHolder {
    u8 pad_0x0[0x10];
    UnkCountDownGui* gui;
};

struct UnkEvacuationCountDown {
    virtual void vf_0x0();
    void Vf168() { reinterpret_cast<void (*)(UnkEvacuationCountDown*)>((*reinterpret_cast<void***>(this))[0x168 / 8])(this); }
    u8 pad_0x8[0x71];
    bool field_0x79;
    bool field_0x7a;
    u8 pad_0x7b[5];
    UnkCountDownGuiHolder* field_0x80;
};

void fn_71007dc10c(UnkEvacuationCountDown* self);

void fn_71007da578(UnkEvacuationCountDown* self) {
    fn_71007dc10c(self);
    if (self->field_0x79 && !self->field_0x7a)
        self->Vf168();
    else
        self->field_0x80->gui->SendEvent("OnHide", false);
}
