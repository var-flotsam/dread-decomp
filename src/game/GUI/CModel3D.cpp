// GUI::CModel3D's unit (unit 2753 of tools/tu_map.py, the name from reflection): two copies of its
// constructor over the display object's (fn_71011cce1c, game/UnkGuiDisplayObject.h); the second
// ends by calling the object's vf_0x18. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"

using base::global::CRntString;
using base::global::CStrId;

extern char g_unk_7101cb6608[];  // CModel3D's vtable

struct UnkGuiModel3D {
    char* vtable;
    u8 base_0x8[0x181];
    bool field_0x189;
    u8 pad_0x18a[0xe];
    u32 field_0x198;  // 3
    u8 pad_0x19c[0x4];
    CStrId field_0x1a0;
    bool field_0x1a8;
    u8 pad_0x1a9[0x7];
    CRntString field_0x1b0;
    CRntString field_0x1e0;
    bool field_0x210;
    u8 pad_0x211[0x7];
    u64 field_0x218;
    u8 pad_0x220[0x20];
    u64 field_0x240;
    u64 field_0x248;
    u64 field_0x250;
    u8 pad_0x258[0x8];
    u64 field_0x260;
    CStrId field_0x268;
    f32 field_0x270[2];  // -1, -1
};
static_assert(__builtin_offsetof(UnkGuiModel3D, field_0x270) == 0x270, "UnkGuiModel3D layout");

static inline void Construct(UnkGuiModel3D* self) {
    fn_71011cce1c(reinterpret_cast<UnkGuiDisplayObject*>(self));
    self->field_0x189 = true;
    self->vtable = g_unk_7101cb6608 + 0x10;
    self->field_0x198 = 3;
    new (&self->field_0x1a0) CStrId("", true);
    self->field_0x1a8 = true;
    new (&self->field_0x1b0) CRntString("");
    new (&self->field_0x1e0) CRntString("");
    self->field_0x210 = false;
    self->field_0x218 = 0;
    self->field_0x260 = 0;
    self->field_0x240 = 0;
    self->field_0x248 = 0;
    self->field_0x250 = 0;
    new (&self->field_0x268) CStrId("", true);
    self->field_0x270[0] = -1.0f;
    self->field_0x270[1] = -1.0f;
}

void fn_71011f3c4c(UnkGuiModel3D* self) {
    Construct(self);
}

void fn_71011f4d54(UnkGuiModel3D* self) {
    Construct(self);
    reinterpret_cast<UnkGuiDisplayObject*>(self)->vf_0x18();
}
