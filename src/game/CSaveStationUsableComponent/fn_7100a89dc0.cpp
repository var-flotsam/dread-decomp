// CSaveStationUsableComponent's unit (tools/tu_map.py): unless its sound (+0x540) still plays, plays the
// char class's sound (+0x188) on the entity with a shared set of play parameters, keeps the handle; then
// adds an id to a user still alive (+0xe8) and clears the word at +0x558. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The handle's view of the reference block.
struct UnkSoundRefView_la89dc0 {
    void* sound;
    s64 serial;
};

static inline void* Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_la89dc0* ref = reinterpret_cast<const UnkSoundRefView_la89dc0*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}


extern const u64 g_unk_710164b5b8;

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
    if (!Playing(self->sound) && self->CharClass()->sound.data != &g_unk_7101d47200->empty) {
        static UnkSoundDesc s_params;
        s_params.kind.field_0x0 = 0;
        s_params.path = self->CharClass()->sound;
        s_params.field_0x38 = 0.6f;
        s_params.field_0x3c = 500.0f;
        s_params.field_0x40 = 3000.0f;
        s_params.field_0x44 = 1.0f;
        s_params.field_0x6c = true;
        s_params.field_0x6d = true;
        s_params.field_0x4c = 0.45f;
        self->sound = fn_7100d7c374(self->entity, &s_params, &base::global::CStrId::Empty(), false, true, true);
    }
    if (self->user && self->user->target)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->user->target), &g_unk_710164b5b8, false);
    self->field_0x558 = 0;
}
