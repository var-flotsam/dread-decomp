// Constructor: runs the base constructor, sets the derived vtable and a default 1.0 scale, builds
// three empty string ids (at 0x30, 0x38, 0x40) and clears the following state before calling the
// object's own virtual slot 3 (a tail call).
#include "types.h"

extern void* g_unk_7101a26cb0[];
void fn_71000003d4(void* self, const char* s, bool flag);
void fn_7100393e68(void* self);

struct UnkSelf {
    void** vtable;
    u8 pad_0x8[0x24];
    f32 field_0x2c;
    u8 id_0x30[8];
    u8 id_0x38[8];
    u8 id_0x40[8];
    u8 field_0x48[11];

    UnkSelf* vf_0x18() { return reinterpret_cast<UnkSelf* (*)(UnkSelf*)>(vtable[3])(this); }
};

UnkSelf* fn_7100393558(UnkSelf* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a26cb0 + 2;
    self->field_0x2c = 1.0f;
    fn_71000003d4(self->id_0x30, "", true);
    fn_71000003d4(self->id_0x38, "", true);
    fn_71000003d4(self->id_0x40, "", true);
    __builtin_memset(self->field_0x48, 0, 11);
    return self->vf_0x18();
}
