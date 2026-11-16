// CRumbleComponent's unit (tools/tu_map.py): builds a rumble's parameters from a sound definition.
// The definition is read into a sound description with the rumble defaults (fn_7101037bc0), whose
// file, kind and tunables then fill `out`. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2647.h"
#include "game/UnkTU49.h"
#include "types.h"

using base::global::CStrId;
using base::global::CFilePathStrId;
using base::global::UnkStringData;

// What a rumble is played from (UnkRumbleParams of src/game/CRumbleComponent.cpp).
struct UnkRumbleDesc_l9dae9c {
    CFilePathStrId path;
    u64 field_0x8;
    u32 kind;        // 0x10
    f32 offset[3];   // 0x14
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;
};

struct UnkSoundDef;
struct UnkSoundDesc;

// A string id copied from another (retained), released in two steps.
struct UnkSoundName_l9dae9c {
    UnkStringData* data;

    explicit UnkSoundName_l9dae9c(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundName_l9dae9c() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

using UnkPathMember_l9dae9c = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// A file path copied from another (retained), released in two steps.
struct UnkSoundPath_l9dae9c {
    UnkStringData* data;

    explicit UnkSoundPath_l9dae9c(const CFilePathStrId& id) : data(id.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundPath_l9dae9c() { reinterpret_cast<UnkPathMember_l9dae9c*>(this)->~UnkPathMember_l9dae9c(); }
    const CFilePathStrId& Id() const { return reinterpret_cast<const CFilePathStrId&>(*this); }
};

// The sound description fn_7101037bc0 fills, with the defaults a rumble starts from.
struct UnkRumbleSoundDesc_l9dae9c {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_l9dae9c name;       // 0x18
    UnkSoundPath_l9dae9c path;       // 0x20
    f32 offset[3];                   // 0x28
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

    UnkRumbleSoundDesc_l9dae9c()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          offset{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkRumbleSoundDesc_l9dae9c, field_0x6c) == 0x6c, "layout");

void fn_71009dae9c(void* self, UnkSoundDef* def, f32* scales, u32 index, UnkRumbleDesc_l9dae9c* out) {
    if (!def)
        return;
    UnkRumbleSoundDesc_l9dae9c sound;
    fn_7101037bc0(def, reinterpret_cast<UnkSoundDesc*>(&sound), scales, index);
    out->path = sound.path.Id();
    out->field_0x8 = 0;
    out->kind = fn_7100071680(sound.kind);
    out->offset[0] = sound.offset[0];
    out->offset[1] = sound.offset[1];
    out->offset[2] = sound.offset[2];
    out->field_0x20[0] = sound.field_0x40;
    out->field_0x20[1] = sound.field_0x3c;
    out->field_0x20[2] = sound.field_0x5c > 0.0f ? sound.field_0x5c : sound.field_0x38;
    out->field_0x20[3] = sound.field_0x44;
    out->field_0x30 = sound.field_0x48 * 0.5f + 0.5f;
    out->field_0x34 = sound.field_0x4c;
    out->field_0x38 = sound.field_0x50;
    out->field_0x3c = 1.0f;
    out->field_0x40 = sound.field_0x6d;
    out->field_0x41 = sound.field_0x6c;
}
