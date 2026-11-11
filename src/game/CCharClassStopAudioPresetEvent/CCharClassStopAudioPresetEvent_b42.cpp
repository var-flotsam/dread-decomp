// CCharClassStopAudioPresetEvent's unit (tools/tu_map.py): its constructor, on the event base
// (fn_7100067ea4): the vtable, two copies of the empty string id (g_unk_7101d470e0), a copy of the
// empty file path (g_unk_7101d470e8), a cleared word, a float of -1 and a cleared word. Names and
// the layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char g_unk_7101aa1ea8[];

struct UnkStopAudioPresetEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
    CFilePathStrId path_0x20;
    u64 field_0x28;
    f32 field_0x30;
    u32 field_0x34;
};

void fn_7100be4928(UnkStopAudioPresetEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1ea8 + 0x10;
    new (&self->id_0x10) CStrId(g_unk_7101d470e0);
    new (&self->id_0x18) CStrId(g_unk_7101d470e0);
    new (&self->path_0x20) CFilePathStrId(g_unk_7101d470e8);
    self->field_0x28 = 0;
    self->field_0x30 = -1.0f;
    self->field_0x34 = 0;
}
