// ICharClassPlayAudioPresetEvent's unit (tools/tu_map.py): setting the event's preset name (+0x10). A
// non-empty name also resolves "audiopreset:system/snd/presets/<name>" into the id and path pair at
// +0x18 (fn_710001f074, fn_71000267f8). Names and layouts are placeholders.
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkAudioPresetEvent_ovb0135 {
    u8 pad_0x0[0x10];
    CStrId name;                // 0x10
    UnkStrIdPathResource path;  // 0x18
};

void fn_7100beeadc(UnkAudioPresetEvent_ovb0135* self, const CStrId* name) {
    self->name = *name;
    if (self->name.data == &g_unk_7101d47108->empty)
        return;
    fn_71000267f8(&self->path, UnkPathIdsFromText(CRntString_ConcatCStringFirst("audiopreset:system/snd/presets/",
                                                                 &base::reflection::refl_RntStringOf(self->name)))
                                   .ids);
}
