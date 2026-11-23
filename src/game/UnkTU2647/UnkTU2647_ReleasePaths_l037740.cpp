// Unit 2647 (sound::CAudioPreset): unless the preset's field 0x54 is set, hands every file-path id of
// the first list of each of its sixteen list pairs back to the sound system. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;

struct UnkPathListPair_l037740 {
    CRntVector<CFilePathStrId> first;
    CRntVector<CFilePathStrId> second;
};

struct UnkAudioPreset_l037740 {
    u8 pad_0x0[0x54];
    u32 field_0x54;
    u8 pad_0x58[0x10];
    UnkPathListPair_l037740 items[16];  // 0x68
};
static_assert(__builtin_offsetof(UnkAudioPreset_l037740, items) == 0x68, "layout");

void fn_7101037740(UnkAudioPreset_l037740* self) {
    if (self->field_0x54 != 0)
        return;
    for (UnkPathListPair_l037740& pair : self->items) {
        for (u32 i = 0; i < pair.first.count; ++i)
            fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode_CEntity_2*>(&pair.first.data[i]));
    }
}
