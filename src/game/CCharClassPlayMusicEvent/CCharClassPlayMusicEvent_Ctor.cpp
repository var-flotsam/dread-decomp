// CCharClassPlayMusicEvent's default constructor (the name from reflection): on the event base
// (fn_7100067ea4), an empty file path at +0x10, then a word of 5, a volume of 1, a cleared flag and
// -1 at +0x24. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CFilePathStrId;

extern char g_unk_7101aa5e68[];

struct UnkPlayMusicEvent {
    void* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path;  // 0x10
    u32 field_0x18;
    f32 volume;  // 0x1c
    bool field_0x20;
    f32 field_0x24;
};

void fn_7100ccb6f0(UnkPlayMusicEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5e68 + 0x10;
    new (&self->path) CFilePathStrId("", true);
    self->field_0x18 = 5;
    self->volume = 1.0f;
    self->field_0x20 = false;
    self->field_0x24 = -1.0f;
}
