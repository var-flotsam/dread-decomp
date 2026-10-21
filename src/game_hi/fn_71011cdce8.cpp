// Sets a boolean state flag; marks the object dirty and, if its derived visibility changed, sends a change event.
#include "types.h"

struct UnkStruct_71011cdce8;

struct UnkStruct_71011cdce8_vt {
    void* slots[20];
    void (*vf_0xa0)(UnkStruct_71011cdce8*);
};

struct UnkStruct_71011cdce8 {
    UnkStruct_71011cdce8_vt* vtable;
    char pad8[0x74 - 0x8];
    bool field_0x74;
    char pad75;
    u8 field_0x76;
    char pad77[0xb4 - 0x77];
    bool field_0xb4;
    char padb5[0x144 - 0xb5];
    u32 field_0x144;
    char pad148[0x183 - 0x148];
    bool field_0x183;
    bool field_0x184;
};

struct UnkEvent_0x28 {
    char data[0x28];
};

void fn_71011cee40(UnkStruct_71011cdce8* self);
void fn_710120235c(UnkEvent_0x28* ev, UnkStruct_71011cdce8* sender, const s32* type, void* extra);
void fn_7101202488(UnkEvent_0x28* ev);
void fn_71011b6e0c(void* mgr, UnkEvent_0x28* ev);
extern void* g_unk_717271e8f8;

static inline u8 IsShown_71011cdce8(UnkStruct_71011cdce8* self) {
    if (!self->field_0x74)
        return 0;
    if (self->field_0x183)
        fn_71011cee40(self);
    return self->field_0x76;
}

static inline void Notify_71011cdce8(UnkStruct_71011cdce8* self, s32 kind) {
    UnkEvent_0x28 ev;
    fn_710120235c(&ev, self, &kind, nullptr);
    fn_71011b6e0c(g_unk_717271e8f8, &ev);
    fn_7101202488(&ev);
}

void fn_71011cdce8(UnkStruct_71011cdce8* self, bool value) {
    if (value == self->field_0xb4)
        return;
    if (self->field_0x183)
        fn_71011cee40(self);
    u8 before = IsShown_71011cdce8(self);
    self->field_0xb4 = value;
    if (!self->field_0x184) {
        self->field_0x184 = true;
        self->vtable->vf_0xa0(self);
    }
    self->field_0x183 = true;
    self->field_0x144 |= 0x40;
    fn_71011cee40(self);
    bool after = IsShown_71011cdce8(self) != 0;
    if (before == after)
        return;
    if (after)
        Notify_71011cdce8(self, 0);
    else
        Notify_71011cdce8(self, 1);
}
