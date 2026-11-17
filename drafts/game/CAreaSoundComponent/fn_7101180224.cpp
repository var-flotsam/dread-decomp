// CAreaSoundComponent's unit (tools/tu_map.py): the constructor. After the base constructor it
// sets the vtables (the primary and the listener's at +0xad0), three empty sound handles, three
// empty sound paths and the volume defaults, then empties the second handle again.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

void fn_7100718184(void* self);
extern char g_unk_7101cb48a0[];  // CAreaSoundComponent's vtables

// A sound handle that starts empty (no block, serial -1).
struct UnkAreaSoundHandle_l180224 : UnkSoundHandle {
    UnkAreaSoundHandle_l180224() {
        ref = nullptr;
        serial = -1;
    }
    UnkAreaSoundHandle_l180224& operator=(const UnkSoundHandle& other) {
        UnkSoundHandle::operator=(other);
        return *this;
    }
};

struct UnkAreaSoundCtor_l180224 {
    void* vtable;
    u8 unk_0x8[0xac8];
    void* listener_vtable;      // 0xad0
    UnkAreaSoundHandle_l180224 handles[3];  // 0xad8
    CFilePathStrId paths[3];    // 0xb08
    u8 unk_0xb20[0x14];
    f32 field_0xb34;
    f32 field_0xb38;
    f32 field_0xb3c;
    f32 field_0xb40;
    f32 field_0xb44;
    f32 field_0xb48;
    f32 field_0xb4c;
    f32 field_0xb50;
    f32 field_0xb54;
    f32 field_0xb58;
    f32 field_0xb5c;
    f32 field_0xb60;
    f32 field_0xb64;
};
static_assert(__builtin_offsetof(UnkAreaSoundCtor_l180224, field_0xb64) == 0xb64, "layout");

void fn_7101180224(UnkAreaSoundCtor_l180224* self) {
    fn_7100718184(self);
    self->vtable = g_unk_7101cb48a0 + 0x10;
    self->listener_vtable = g_unk_7101cb48a0 + 0x220;
    for (int i = 0; i < 3; ++i)
        new (&self->handles[i]) UnkAreaSoundHandle_l180224();
    for (int i = 0; i < 3; ++i)
        new (&self->paths[i]) CFilePathStrId();
    self->field_0xb34 = 1.0f;
    self->field_0xb38 = 1.0f;
    self->field_0xb3c = 0.2f;
    self->field_0xb40 = 0.2f;
    self->field_0xb44 = 1.0f;
    self->field_0xb48 = 1.0f;
    self->field_0xb4c = 1.0f;
    self->field_0xb50 = 0.2f;
    self->field_0xb54 = 0.2f;
    self->field_0xb58 = 1.0f;
    self->field_0xb5c = 1.0f;
    self->field_0xb60 = 0.2f;
    self->field_0xb64 = 0.2f;
    self->handles[1] = g_unk_7108b6c460;
}
