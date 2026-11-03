// CCharClassSetRelativeCameraAnimationBaseEvent's unit (tools/tu_map.py): its constructor, on the
// event base (fn_7100067ea4): the vtable, a file-path id built from the empty id's string
// (refl_GlobalRntString), a cleared flag, a cleared word and a float of 1, three string ids built
// from "" with a cleared flag between, flags and floats (5, -1), a fourth string id and a last set
// flag. Names and the layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char g_unk_7101aa1be8[];
void fn_7100067ea4(void* self);  // the event base constructor

struct UnkSetRelativeCameraAnimationBaseEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path_0x10;
    bool field_0x18;
    u32 field_0x1c;
    f32 field_0x20;
    CStrId id_0x28;
    bool field_0x30;
    CStrId id_0x38;
    CStrId id_0x40;
    bool field_0x48;
    bool field_0x49;
    u16 field_0x4a;
    u32 field_0x4c;
    f32 field_0x50;
    bool field_0x54;
    bool field_0x55;
    f32 field_0x58;
    bool field_0x5c;
    CStrId id_0x60;
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
    bool field_0x6b;
};

void fn_7100bdb3b4(UnkSetRelativeCameraAnimationBaseEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1be8 + 0x10;
    fn_710001a270(&self->path_0x10, refl_GlobalRntString(), true);
    self->field_0x18 = false;
    self->field_0x1c = 0;
    self->field_0x20 = 1.0f;
    new (&self->id_0x28) CStrId("", true);
    self->field_0x30 = false;
    new (&self->id_0x38) CStrId("", true);
    new (&self->id_0x40) CStrId("", true);
    self->field_0x48 = true;
    self->field_0x49 = true;
    self->field_0x4a = 0;
    self->field_0x4c = 0;
    self->field_0x50 = 5.0f;
    self->field_0x54 = false;
    self->field_0x55 = false;
    self->field_0x58 = -1.0f;
    self->field_0x5c = true;
    new (&self->id_0x60) CStrId("", true);
    self->field_0x68 = false;
    self->field_0x69 = false;
    self->field_0x6a = false;
    self->field_0x6b = true;
}
