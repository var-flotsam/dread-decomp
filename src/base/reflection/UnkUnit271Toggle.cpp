// From unit 271 of tools/tu_map.py (0x71001ec8bc-0x71001f2a08, hint base::reflection): sets the flag
// at +0x15a, tells the two parts at +0x10 and +0x18 (fn_71001ebe30, fn_71001ea560) whether it is
// clear, and sends the owner at +0x8 (its vf_0x78) one of two stack messages, which differ only in
// their vtables (reached through the GOT). Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101a1eef0[];  // the message's vtable when the flag is set
extern char g_unk_7101a1ef20[];  // and when it is clear

void fn_71001ebe30(void* part, bool clear);
void fn_71001ea560(void* part, bool clear);

struct UnkToggleMessage {
    void* vtable;
};

class UnkToggleReceiver {
public:
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
    virtual void vf_0x78(UnkToggleMessage* message);
};

struct UnkUnit271Object {
    void* vtable;
    UnkToggleReceiver* owner;  // 0x8
    void* part_0x10;
    void* part_0x18;
    u8 pad_0x20[0x15a - 0x20];
    bool flag;  // 0x15a
};

void fn_71001ef9a4(UnkUnit271Object* self, bool flag) {
    if (self->flag != flag)
        self->flag = flag;
    if (self->part_0x10)
        fn_71001ebe30(self->part_0x10, !self->flag);
    if (self->part_0x18)
        fn_71001ea560(self->part_0x18, !self->flag);
    if (self->flag) {
        UnkToggleMessage message;
        message.vtable = g_unk_7101a1eef0 + 0x10;
        self->owner->vf_0x78(&message);
    } else {
        UnkToggleMessage message;
        message.vtable = g_unk_7101a1ef20 + 0x10;
        self->owner->vf_0x78(&message);
    }
}
