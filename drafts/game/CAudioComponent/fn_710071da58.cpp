// CAudioComponent's unit: dropping the sound slots that no longer play, counting the ones that do.
// A dropped slot's preset link (an audio preset with the slot's name) is marked, and when no other link
// uses that preset its fade entry is restarted towards 1. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::reflection::UnkReflObject;

// As src/game/CAudioComponent/CAudioComponent_Sounds_ovb0015.cpp has them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

struct UnkAudioSound_ovb0015;
void fn_710071f878(UnkAudioSound_ovb0015* self);
struct UnkStruct_710071faf4;
void fn_710071faf4(UnkStruct_710071faf4* self, const UnkStruct_710071faf4* other);

// The sound object a slot's handle names while it is current.
static inline UnkSoundObject_b50* Resolve(const UnkSoundHandle& handle) {
    if (!handle.ref || handle.ref->serial != handle.serial || handle.serial == -1)
        return nullptr;
    return handle.ref->object;
}

// A string id seen only for its data (a CStrId on g_unk_7101d47108, a file-path id on g_unk_7101d47200).
struct UnkAudioId_l71da58 {
    UnkStringData* data;
};

// A sound slot of the component (0xd0 bytes): copied with fn_710071faf4, released with fn_710071f878.
struct UnkAudioSound_l71da58 {
    u8 pad_0x0[0x20];
    UnkAudioId_l71da58 name;  // 0x20
    u8 pad_0x28[0x78 - 0x28];
    UnkSoundHandle handle;  // 0x78
    u8 pad_0x88[0xb0 - 0x88];
    f32 field_0xb0;
    u8 pad_0xb4[0xc4 - 0xb4];
    bool keep : 1;  // 0xc4 bit 0: kept while field_0xb0 is not positive
    u8 pad_0xc5[0xd0 - 0xc5];

    UnkAudioSound_l71da58(UnkAudioSound_l71da58&& other) {
        fn_710071faf4(reinterpret_cast<UnkStruct_710071faf4*>(this),
                      reinterpret_cast<const UnkStruct_710071faf4*>(&other));
    }
    ~UnkAudioSound_l71da58() { fn_710071f878(reinterpret_cast<UnkAudioSound_ovb0015*>(this)); }
};
static_assert(sizeof(UnkAudioSound_l71da58) == 0xd0, "UnkAudioSound_l71da58 layout");

// A sound link of the component (0xb0 bytes).
struct UnkAudioLink_l71da58 {
    u8 pad_0x0[0x90];
    UnkAudioId_l71da58 name;  // 0x90
    UnkAudioId_l71da58 path;  // 0x98
    UnkReflObject* resource;  // 0xa0
    u8 pad_0xa8;
    bool released;  // 0xa9
    u8 pad_0xaa[0xb0 - 0xaa];
};
static_assert(sizeof(UnkAudioLink_l71da58) == 0xb0, "UnkAudioLink_l71da58 layout");

// A sound::CAudioPreset.
struct UnkAudioPreset_l71da58 : UnkReflObject {
    u8 pad_0x8[0x58 - 0x8];
    UnkAudioId_l71da58 name;  // 0x58
    u8 pad_0x60[0x5b4 - 0x60];
    f32 field_0x5b4;
    u8 pad_0x5b8[0x5c0 - 0x5b8];
    f32 field_0x5c0;
};

// A fade of a preset (0x18 bytes).
struct UnkAudioFade_l71da58 {
    u32 field_0x0;
    f32 value;     // 0x4
    f32 from;      // 0x8
    f32 to;        // 0xc
    f32 duration;  // 0x10
    f32 time;      // 0x14
};

// The fades by preset: values and keys side by side.
struct UnkAudioFadeMap_l71da58 {
    UnkAudioFade_l71da58* values;  // 0x0
    const void** keys;             // 0x8
    u32 count;                     // 0x10

    UnkAudioFade_l71da58* End() { return &values[count]; }
    UnkAudioFade_l71da58* Find(const void* key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

struct UnkAudioComponent_l71da58 {
    u8 pad_0x0[0x48];
    base::global::CRntVector<UnkAudioSound_l71da58> sounds;  // 0x48
    UnkAudioLink_l71da58* links;                             // 0x70
    u32 link_count;                                          // 0x78
    u8 pad_0x7c[0x98 - 0x7c];
    UnkAudioFadeMap_l71da58 fades;  // 0x98
    u8 pad_0xb0[0xe8 - 0xb0];
    u32 playing;  // 0xe8
};
static_assert(__builtin_offsetof(UnkAudioComponent_l71da58, playing) == 0xe8, "UnkAudioComponent_l71da58 layout");

void fn_710071da58(UnkAudioComponent_l71da58* self) {
    self->playing = 0;
    UnkAudioSound_l71da58* it = self->sounds.data;
    while (self->sounds.data + self->sounds.count != it) {
        if (Resolve(it->handle) || (it->keep && it->field_0xb0 <= 0.0f)) {
            if (Resolve(it->handle))
                ++self->playing;
            ++it;
            continue;
        }
        if (it->name.data != &g_unk_7101d47108->empty) {
            for (UnkAudioLink_l71da58 *link = self->links, *end = link + self->link_count; link != end; ++link) {
                if (link->name.data == &g_unk_7101d47108->empty || link->path.data == &g_unk_7101d47200->empty)
                    continue;
                UnkReflObject* resource = link->resource;
                if (!resource || !fn_710009d50c(resource->vf_0x0(), fn_710102df18()))
                    continue;
                UnkAudioPreset_l71da58* preset = static_cast<UnkAudioPreset_l71da58*>(resource);
                if (preset->name.data != it->name.data)
                    continue;
                // The last link to the preset restarts its fade towards 1.
                u32 users = 0;
                for (UnkAudioLink_l71da58 *other = self->links, *last = other + self->link_count; other != last; ++other)
                    if (other->resource == link->resource)
                        ++users;
                bool shared_elsewhere = users > 1;
                if (!shared_elsewhere) {
                    UnkAudioFade_l71da58* fade = self->fades.Find(link->resource);
                    if (fade != self->fades.End() && fade->to < 1.0f) {
                        f32 value = fade->value;
                        fade->from = value;
                        fade->to = 1.0f;
                        fade->time = 0.0f;
                        fade->duration = preset->field_0x5c0 * ((1.0f - value) / (1.0f - preset->field_0x5b4));
                    }
                }
                link->released = true;
                break;
            }
        }
        u32 index = it - self->sounds.data;
        self->sounds.EraseAt(index);
        it = self->sounds.data + index;
    }
}
