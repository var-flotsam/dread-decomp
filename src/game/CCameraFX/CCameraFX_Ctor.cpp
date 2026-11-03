// CCameraFX unit (unit 2696 of tools/tu_map.py, the name from reflection): the base constructor
// (shaped like fn_7100767fb8, CCameraFX_OscillationShake): the parent constructor fn_7101043904, the
// class vtable, an empty string id at +0x10 and cleared fields after it. fn_71010f0fa8 also makes the
// post-construction virtual call (slot 3). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

void fn_7101043904(void*);
extern char g_unk_7101cb1b78[];

struct UnkCameraFXBase {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkStrId field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
};

void fn_71010f0e04(char* self_) {
    UnkCameraFXBase* self = reinterpret_cast<UnkCameraFXBase*>(self_);
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb1b78 + 0x10);
    new (&self->field_0x10) UnkStrId("", true);
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = false;
}

void fn_71010f0fa8(UnkCameraFXBase* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb1b78 + 0x10);
    new (&self->field_0x10) UnkStrId("", true);
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = false;
    reinterpret_cast<void (*)(UnkCameraFXBase*)>(self->vtable[3])(self);
}
