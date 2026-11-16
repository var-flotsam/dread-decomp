// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): when either of the two states at
// +0x654/+0x658 is 1, plays the door's sound (path g_unk_7172731728) through the owner entity with a sound
// description kept in a function-local static. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The unit's own path id (set up by the unit's static initialiser).
static CFilePathStrId g_unk_7172731728;

UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// A sound description: the sound's id and path and its playback values.
struct UnkDoorSoundDesc_l8d6d38 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;
    CFilePathStrId path;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
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

    UnkDoorSoundDesc_l8d6d38()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), field_0x28(0.0f), field_0x2c(0.0f), field_0x30(0.0f), field_0x34(-1.0f),
          field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f),
          field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f),
          field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false),
          field_0x6e(false) {}
    ~UnkDoorSoundDesc_l8d6d38();  // fn_710034e0b8
};
static_assert(__builtin_offsetof(UnkDoorSoundDesc_l8d6d38, field_0x68) == 0x68, "layout");

struct UnkDoorLife_l8d6d38 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;
    u8 pad_0x38[0x654 - 0x38];
    s32 field_0x654;
    s32 field_0x658;
};

void fn_71008d6d38(UnkDoorLife_l8d6d38* self) {
    if (self->field_0x654 != 1 && self->field_0x658 != 1)
        return;
    static UnkDoorSoundDesc_l8d6d38 s_desc;
    s_desc.field_0x0 = 0;
    s_desc.path = g_unk_7172731728;
    s_desc.field_0x38 = 1.0f;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 1400.0f;
    s_desc.field_0x44 = 1.4f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;
    fn_7100d7c374(self->entity, &s_desc, &g_unk_7101d470e0, false, true, true);
}
