// CAudioComponent unit (tools/tu_map.py, the name from reflection): the sound handle of the entry
// (0xd0 bytes each, array at +0x48, count at +0x50) whose key at +0x28 equals *key, or the empty
// handle. Names and layouts are placeholders.
#include "types.h"

struct UnkSoundRef;
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkAudioEntry_b64a4 {
    u8 pad_0x0[0x28];
    u64 key;
    u8 pad_0x30[0x48];
    UnkSoundHandle handle;  // 0x78
    u8 pad_0x88[0x48];
};

struct UnkAudioFind_b64a4 {
    u8 pad_0x0[0x48];
    UnkAudioEntry_b64a4* entries;
    u32 count;
};

// Signature as src/game/CEntity_2.cpp declares it.
struct UnkEntitySoundComponent;

UnkSoundHandle fn_710071a338(UnkEntitySoundComponent* component, void* a) {
    UnkAudioFind_b64a4* self = reinterpret_cast<UnkAudioFind_b64a4*>(component);
    const u64* key = static_cast<const u64*>(a);
    UnkAudioEntry_b64a4* e = self->entries;
    for (UnkAudioEntry_b64a4* end = e + self->count; e != end; ++e) {
        if (e->key == *key)
            return e->handle;
    }
    return g_unk_7108b6c460;
}
