// CCharClassPlayEnvironmentStreamEvent's unit (tools/tu_map.py): playing the event. A sound
// description (group 5) takes the event's stream path (+0x10), volume (+0xc), direction (+0x18) and
// the word at +0x1c, and goes to the sound system (g_unk_7101d4d820, fn_7100108620). Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/snd/CSoundSystem.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringEntry;

using UnkPathMember_ovb0138 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The file path of a string id's text (its use count at +0x44 goes up first).
struct UnkStreamPath_ovb0138 {
    base::global::UnkStringData* data;

    explicit UnkStreamPath_ovb0138(const CStrId& id) {
        UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
        ++entry->tail.field_0x44;
        fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(this), entry->buffer, true);
    }
    ~UnkStreamPath_ovb0138() { reinterpret_cast<UnkPathMember_ovb0138*>(this)->~UnkPathMember_ovb0138(); }
    void operator=(const UnkPathMember_ovb0138& other) { *reinterpret_cast<UnkPathMember_ovb0138*>(this) = other; }
};

// A string id copied from another (retained) and released in two steps.
struct UnkStreamName_ovb0138 {
    base::global::UnkStringData* data;

    explicit UnkStreamName_ovb0138(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkStreamName_ovb0138() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

// What the sound system plays (the shape of UnkSoundDesc_ovb0074 in
// src/base/snd/CSoundSystem/CSoundSystem_Groups_ovb0074.cpp).
struct UnkStreamDesc_ovb0138 {
    s32 group;
    UnkStreamPath_ovb0138 path;  // 0x8
    UnkStreamName_ovb0138 name;  // 0x10
    s32 direction;               // 0x18
    u32 field_0x1c;
    u32 field_0x20;
    u32 variant;                 // 0x24
    u32 field_0x28;
    f32 volume;                  // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;

    UnkStreamDesc_ovb0138()
        : group(0), path(g_unk_7101d470e0), name(g_unk_7101d470e0),
          direction(1), field_0x1c(0), field_0x20(0xffffffff), variant(0), volume(1.0f), field_0x30(0.0f),
          field_0x34(0.0f), field_0x38(-1.0f) {}
};

struct UnkSoundSystemPlayView_ovb0074;
struct UnkSoundDesc_ovb0074;
struct UnkPlayingRef_ovb0074;
struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;
};
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkPlayEnvironmentStreamEvent_ovb0138 {
    u8 pad_0x0[0xc];
    f32 volume;                   // 0xc
    UnkPathMember_ovb0138 path;  // 0x10
    bool reverse;                 // 0x18
    f32 field_0x1c;
};

void fn_7100cca4b8(UnkPlayEnvironmentStreamEvent_ovb0138* self) {
    UnkStreamDesc_ovb0138 desc;
    desc.group = 5;
    desc.path = self->path;
    desc.volume = self->volume;
    desc.direction = self->reverse ? -1 : 1;
    desc.field_0x30 = self->field_0x1c;
    fn_7100108620(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820), reinterpret_cast<UnkSoundDesc_ovb0074*>(&desc));
}
