// CAudioComponent unit (tools/tu_map.py): the per-frame update. The component's sounds leave the live
// count while the slots and links are updated, the stopped links are erased, and the sounds left are
// counted back in. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAudioComponent.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

extern bool g_unk_717270f624;
extern s32 g_unk_717270f620;

void fn_710071da58(UnkEntitySoundComponent* self);
void fn_710071ddac(UnkEntitySoundComponent* self);
void fn_710071e27c(UnkEntitySoundComponent* self, f32 dt);

struct UnkAudioLinkCopy_ovb0015;
void CAudioComponent_CopyConstructLinkEntry(UnkAudioLinkCopy_ovb0015* self, const UnkAudioLinkCopy_ovb0015* other);

// CStrId::Reset on the file-path pool.
struct UnkAudioPathId_l71dfc4 {
    base::global::UnkStringData* data;

    void Reset() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A sound link (0xb0 bytes).
struct UnkAudioLink_l71dfc4 {
    u8 pad_0x0[0x38];
    CStrId field_0x38;
    UnkAudioPathId_l71dfc4 field_0x40;
    u8 pad_0x48[0x48];
    u8 field_0x90[0x18];  // the id pair (UnkIdPair)
    u8 pad_0xa8[0x1];
    bool stopped;  // 0xa9
    u8 pad_0xaa[0x6];
};
static_assert(sizeof(UnkAudioLink_l71dfc4) == 0xb0, "layout");

struct UnkAudioLinks_l71dfc4 {
    UnkAudioLink_l71dfc4* data;
    u32 count;
    u32 capacity;
};

struct UnkAudioComponent_l71dfc4 {
    u8 pad_0x0[0x50];
    u32 sound_count;  // 0x50
    u8 pad_0x54[0x1c];
    UnkAudioLinks_l71dfc4 links;  // 0x70
    u8 pad_0x80[0x68];
    s32 field_0xe8;
};
static_assert(__builtin_offsetof(UnkAudioComponent_l71dfc4, field_0xe8) == 0xe8, "layout");

void fn_710071dfc4(UnkAudioComponent_l71dfc4* self, f32 dt) {
    g_unk_717270f624 = false;
    u32 playing = self->sound_count;
    s32 live = g_unk_717270f61c - playing;
    g_unk_717270f61c = live < 0 ? 0 : live;

    UnkEntitySoundComponent* component = reinterpret_cast<UnkEntitySoundComponent*>(self);
    fn_710071da58(component);
    fn_710071ddac(component);
    fn_710071e27c(component, dt);

    // Erase the stopped links: each is destroyed and the later ones move down by one.
    UnkAudioLinks_l71dfc4& links = self->links;
    for (UnkAudioLink_l71dfc4* it = links.data; it != links.data + links.count;) {
        if (!it->stopped) {
            ++it;
            continue;
        }
        u32 index = it - links.data;
        if (index < links.count) {
            // The link's ids: the id pair, then the file-path id and the string id.
            UnkAudioLink_l71dfc4& erased = links.data[index];
            fn_7100026f70(reinterpret_cast<UnkIdPair*>(erased.field_0x90));
            erased.field_0x40.Reset();
            erased.field_0x38.Reset();
            for (u64 i = index; i < links.count - 1; ++i) {
                UnkAudioLink_l71dfc4* at = &links.data[i];
                CAudioComponent_CopyConstructLinkEntry(reinterpret_cast<UnkAudioLinkCopy_ovb0015*>(at),
                              reinterpret_cast<UnkAudioLinkCopy_ovb0015*>(at + 1));
                UnkAudioLink_l71dfc4& moved = (&links.data[i])[1];
                fn_7100026f70(reinterpret_cast<UnkIdPair*>(moved.field_0x90));
                moved.field_0x40.Reset();
                moved.field_0x38.Reset();
            }
            links.count = links.count - 1;
        }
        it = links.data + index;
    }

    g_unk_717270f61c += self->sound_count + self->links.count;
    g_unk_717270f620 += self->field_0xe8;
}
