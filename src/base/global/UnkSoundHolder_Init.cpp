// From unit 45 of tools/tu_map.py (0x710006b6a4-0x710006d2a0): the start-up of the 0x48-byte
// singleton g_unk_7101d47340 (src/base/global/UnkSoundHolder.cpp has its constructor): its first
// word, a sound group (0x998 bytes, allocated with flags 0x51 and cleared), is built over the base
// fn_710006daf0 with the vtable g_unk_7101a1a180 and then initialised through its vtable slot 2.
// The layout is a placeholder.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a1a180[];

struct UnkSoundGroup_710006cdb8 {
    void** vtable;
    u8 unk_0x8[0x990];

    void vf_0x10() { reinterpret_cast<void (*)(UnkSoundGroup_710006cdb8*)>(vtable[2])(this); }
};

void fn_710006daf0(UnkSoundGroup_710006cdb8* self);

// The holder is left incomplete as in src/engine/UnkEngineStart.cpp; its first word is the group.
struct UnkSoundHolder;

void fn_710006cdb8(UnkSoundHolder* holder) {
    auto* group = static_cast<UnkSoundGroup_710006cdb8*>(fn_71000839a0(0x998, 0x51));
    memset(group, 0, 0x998);
    fn_710006daf0(group);
    group->vtable = g_unk_7101a1a180 + 2;
    *reinterpret_cast<UnkSoundGroup_710006cdb8**>(holder) = group;
    if (group)
        group->vf_0x10();
}
