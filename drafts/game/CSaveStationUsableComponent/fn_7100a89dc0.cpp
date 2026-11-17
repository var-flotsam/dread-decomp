// CSaveStationUsableComponent's unit (tools/tu_map.py): unless its sound (+0x540) still plays, plays the
// char class's sound (+0x188) on the entity with a shared set of play parameters, keeps the handle; then
// adds an id to a user still alive (+0xe8) and clears the word at +0x558. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSound;
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;

    UnkSound* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
};

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

extern const u64 g_unk_710164b5b8;

// The parameters a sound is played with: its ids and the volume, pitch and distance settings.
struct UnkSoundPlayParams_la89dc0 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;  // 0x18
    CFilePathStrId path;  // 0x20
    u64 field_0x28;
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

    UnkSoundPlayParams_la89dc0()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), field_0x28(0), field_0x30(0.0f), field_0x34(-1.0f), field_0x38(1.0f),
          field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f),
          field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f),
          field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundPlayParams_la89dc0() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundPlayParams_la89dc0, field_0x6c) == 0x6c, "");

struct UnkSaveStationClass_la89dc0 {
    u8 pad_0x0[0x188];
    CFilePathStrId sound;  // 0x188
};

struct UnkSaveStation_la89dc0 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x18];
    UnkEntityWithSound* entity;  // 0x30
    u8 pad_0x38[0xb0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x450];
    UnkSoundHandle sound;  // 0x540
    u8 pad_0x550[0x8];
    u32 field_0x558;

    UnkSaveStationClass_la89dc0* CharClass() {
        base::reflection::UnkReflObject* object = char_class;
        return fn_710009d50c(object->vf_0x0(), fn_7100a8886c())
                   ? reinterpret_cast<UnkSaveStationClass_la89dc0*>(object)
                   : nullptr;
    }
};

void fn_7100a89dc0(UnkSaveStation_la89dc0* self) {
    if (!self->sound.Get() && self->CharClass()->sound.data != &g_unk_7101d47200->empty) {
        static UnkSoundPlayParams_la89dc0 s_params;
        s_params.field_0x0 = 0;
        s_params.path = self->CharClass()->sound;
        s_params.field_0x38 = 0.6f;
        s_params.field_0x3c = 500.0f;
        s_params.field_0x40 = 3000.0f;
        s_params.field_0x44 = 1.0f;
        s_params.field_0x6c = true;
        s_params.field_0x6d = true;
        s_params.field_0x4c = 0.45f;
        self->sound = fn_7100d7c374(self->entity, &s_params, &g_unk_7101d470e0, false, true, true);
    }
    if (self->user && self->user->target)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b5b8, false);
    self->field_0x558 = 0;
}
