// CCharClassPlayEnvironmentStreamEvent's default constructor (the name from reflection): on the
// event base (fn_7100067ea4), a volume of 1 at +0xc, an empty file path at +0x10, then a cleared
// flag and word. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CFilePathStrId;

extern char g_unk_7101aa5e38[];

struct UnkPlayEnvironmentStreamEvent {
    void* vtable;
    u8 unk_0x8[0x4];
    f32 volume;  // 0xc
    CFilePathStrId path;  // 0x10
    bool field_0x18;
    u32 field_0x1c;
};

void fn_7100cca6d4(UnkPlayEnvironmentStreamEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5e38 + 0x10;
    self->volume = 1.0f;
    new (&self->path) CFilePathStrId("", true);
    self->field_0x18 = false;
    self->field_0x1c = 0;
}
