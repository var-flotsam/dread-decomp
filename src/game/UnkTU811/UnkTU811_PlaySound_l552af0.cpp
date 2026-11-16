// UnkTU811 (CEmmyAIComponent's unit): plays the EMMI's sound g_unk_71726e2058 at the entity's position,
// fading out the one started before (the handle at +0x29528). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

using UnkPathMember_l552af0 = base::UnkStrIdMemberOf<g_unk_7101d47200>;
__attribute__((visibility("hidden"))) extern UnkPathMember_l552af0 g_unk_71726e2058;

// A string id copied from another (retained), released in two steps.
struct UnkSoundName_l552af0 {
    UnkStringData* data;

    explicit UnkSoundName_l552af0(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundName_l552af0() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

// A file path copied from another (retained), released in two steps.
struct UnkSoundPath_l552af0 {
    UnkStringData* data;

    explicit UnkSoundPath_l552af0(const CFilePathStrId& id) : data(id.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundPath_l552af0() { reinterpret_cast<UnkPathMember_l552af0*>(this)->~UnkPathMember_l552af0(); }
    void operator=(const UnkPathMember_l552af0& other) { *reinterpret_cast<UnkPathMember_l552af0*>(this) = other; }
};

// The sound description the entity plays, with its defaults.
struct UnkSoundDesc_l552af0 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_l552af0 name;  // 0x18
    UnkSoundPath_l552af0 path;  // 0x20
    f32 position[3];            // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundDesc_l552af0()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkSoundDesc_l552af0, field_0x6c) == 0x6c, "layout");

struct UnkEntity_l552af0 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};

// The handle's view of the reference block.
struct UnkSoundRefView_l552af0 {
    UnkSound* sound;
    s64 serial;
};

static inline UnkSound* Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l552af0* ref = reinterpret_cast<const UnkSoundRefView_l552af0*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}

struct UnkEmmyAI_l552af0 {
    u8 pad_0x0[0x30];
    UnkEntity_l552af0* entity;  // 0x30
    u8 pad_0x38[0x29528 - 0x38];
    UnkSoundHandle sound;  // 0x29528
};

void fn_7100552af0(UnkEmmyAI_l552af0* self) {
    UnkSoundDesc_l552af0 desc;
    desc.kind = 0;
    desc.path = g_unk_71726e2058;
    desc.field_0x6c = false;
    desc.field_0x6d = true;
    UnkEntity_l552af0* entity = self->entity;
    f32 x = entity->transform[0][3];
    f32 y = entity->transform[1][3];
    f32 z = entity->transform[2][3];
    desc.position[0] = x;
    desc.position[1] = y;
    desc.position[2] = z;
    desc.field_0x38 = 0.5f;
    desc.field_0x3c = 1000.0f;
    desc.field_0x40 = 10000.0f;
    desc.field_0x44 = 1.0f;
    desc.field_0x6e = true;
    desc.field_0x5c = 2.0f;
    if (UnkSound* playing = Playing(self->sound)) {
        fn_7100105a80(playing, 0.3f, 0.3f);
        self->sound = g_unk_7108b6c460;
    }
    self->sound = fn_7100d7c2f0(reinterpret_cast<UnkEntityWithSound*>(self->entity),
                                reinterpret_cast<const u32*>(&desc), false, true, true);
}
