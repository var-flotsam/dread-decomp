// CEmmyGrabSamusAttack's unit: plays the grab sound (the unit's path g_unk_71726de578) on the attack's
// owner, from a description kept between calls, slower for a CEmmyProtoAIComponent. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The grab sound's file (in the unit's data).
extern CFilePathStrId g_unk_71726de578 __attribute__((visibility("hidden")));

// What a sound is played from, with its defaults.
struct UnkGrabSoundParams_l50f04c {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;            // 0x18
    CFilePathStrId path;  // 0x20
    u64 field_0x28;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
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

    UnkGrabSoundParams_l50f04c()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0), path(g_unk_7101d470e8),
          field_0x28(0), field_0x30(0.0f), field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f),
          field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0), field_0x4c(0), field_0x50(0.0f),
          field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f),
          field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkGrabSoundParams_l50f04c() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkGrabSoundParams_l50f04c, field_0x6e) == 0x6e, "layout");

struct UnkEmmyGrabAttack_l50f04c {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_710050f04c(UnkEmmyGrabAttack_l50f04c* self) {
    static UnkGrabSoundParams_l50f04c s_params;
    s_params.field_0x0 = 0;
    s_params.path = g_unk_71726de578;
    s_params.field_0x6d = true;
    auto* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    s_params.field_0x38 = ai && fn_710009d50c(ai->vf_0x0(), fn_71004fa488()) ? 2.5f : 3.5f;
    s_params.field_0x4c = 0;
    fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_params, &g_unk_7101d470e0, false, true, true);
}
