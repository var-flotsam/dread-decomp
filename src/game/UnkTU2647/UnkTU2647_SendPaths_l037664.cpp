// Unit 2647 (sound::CAudioPreset): walks the bank's sixteen presets in table order and hands every
// non-empty file-path id of their first lists to the sound system. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "types.h"

using base::global::CFilePathStrId;

extern u32 g_unk_710164c2d4[16];

struct UnkPathIdList_b0192 {
    CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkAudioPresetPair_b0192_l037664 {
    UnkPathIdList_b0192 first;
    UnkPathIdList_b0192 second;
};

struct UnkAudioPresetBank_b0192 {
    u8 pad_0x0[0x68];
    UnkAudioPresetPair_b0192_l037664 items[16];
};

void fn_7101037664(UnkAudioPresetBank_b0192* self) {
    for (u32 index : g_unk_710164c2d4) {
        UnkPathIdList_b0192& list = self->items[index].first;
        for (u32 i = 0; i < list.count; ++i) {
            UnkPathNode* path = reinterpret_cast<UnkPathNode*>(&list.data[i]);
            if (*reinterpret_cast<void**>(path) != &g_unk_7101d47200->empty)
                fn_710010808c(g_unk_7101d4d820, path);
        }
    }
}
