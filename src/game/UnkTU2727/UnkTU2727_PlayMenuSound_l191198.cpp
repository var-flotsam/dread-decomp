// UnkTU2727 (main menu script unit): starts a sound (group 7, volume 1.4) unless the one already
// started through this object is still playing; the handle is kept at +0x440. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringEntry;

using UnkPathMember_l191198 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkStreamPath_l191198 {
    base::global::UnkStringData* data;

    explicit UnkStreamPath_l191198(const CStrId& id) {
        UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
        ++entry->tail.field_0x44;
        fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(this), entry->buffer, true);
    }
    ~UnkStreamPath_l191198() { reinterpret_cast<UnkPathMember_l191198*>(this)->~UnkPathMember_l191198(); }
    void operator=(const UnkPathMember_l191198& other) { *reinterpret_cast<UnkPathMember_l191198*>(this) = other; }
};

struct UnkStreamName_l191198 {
    base::global::UnkStringData* data;

    explicit UnkStreamName_l191198(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkStreamName_l191198() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

struct UnkMenuSoundDesc_l191198 {
    s32 group;
    UnkStreamPath_l191198 path;  // 0x8
    UnkStreamName_l191198 name;  // 0x10
    s32 direction;               // 0x18
    u32 field_0x1c;
    u32 field_0x20;
    u32 variant;                 // 0x24
    u32 field_0x28;
    f32 volume;                  // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;

    UnkMenuSoundDesc_l191198()
        : group(0), path(g_unk_7101d470e0), name(g_unk_7101d470e0),
          direction(1), field_0x1c(0), field_0x20(0xffffffff), variant(0), volume(1.0f), field_0x30(0.0f),
          field_0x34(0.0f), field_0x38(-1.0f) {}
};

struct UnkSoundSystemPlayView_ovb0074;
struct UnkSoundDesc_ovb0074;
struct UnkPlayingRef_l191198 {
    void* sound;
    u64 serial;
};
struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_l191198* ref;
    u64 serial;
};
UnkPlayingHandle_ovb0074 fn_7100108620(UnkSoundSystemPlayView_ovb0074* self, UnkSoundDesc_ovb0074* desc);
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
__attribute__((visibility("hidden"))) extern UnkPathMember_l191198 g_unk_7172a0b9c0;

struct UnkMenuSoundOwner_l191198 {
    u8 pad_0x0[0x440];
    UnkPlayingHandle_ovb0074 handle;  // 0x440
};

void fn_7101191198(UnkMenuSoundOwner_l191198* self, f32 value) {
    UnkPlayingRef_l191198* ref = self->handle.ref;
    if (ref && ref->serial == self->handle.serial && self->handle.serial != ~0ull && ref->sound)
        return;
    UnkMenuSoundDesc_l191198 desc;
    desc.group = 7;
    desc.path = g_unk_7172a0b9c0;
    desc.direction = -1;
    desc.volume = 1.4f;
    desc.field_0x30 = 1.0f;
    desc.field_0x38 = value;
    self->handle = fn_7100108620(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820),
                                 reinterpret_cast<UnkSoundDesc_ovb0074*>(&desc));
}
