// CPositionalSoundComponent unit (tools/tu_map.py, the name from reflection): the constructor. The
// component base, the vtables, four empty sound paths, two empty sound handles, an empty id and the
// defaults; then every path in use is set empty again. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/CPositionalSoundComponent.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStateObject;

struct UnkPositionalSound_l14b5ec {
    void** vtable;
    u8 unk_0x8[0x38];
    void** vtable_0x40;
    s32 path_count;  // 0x48
    u8 unk_0x4c[4];
    CFilePathStrId paths[4];  // 0x50
    void* field_0x70;
    void* field_0x78;
    void* field_0x80;
    UnkSoundHandle handles[2];  // 0x88
    bool field_0xa8;
    bool field_0xa9;
    u8 unk_0xaa[6];
    CStrId field_0xb0;
    u32 field_0xb8;
    f32 field_0xbc;
    f32 field_0xc0;
    f32 field_0xc4;
    void* field_0xc8;
    u32 field_0xd0;
};
static_assert(__builtin_offsetof(UnkPositionalSound_l14b5ec, field_0xd0) == 0xd0, "layout");

void fn_710114b5ec(UnkPositionalSound_l14b5ec* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101cb3570 + 2;
    self->vtable_0x40 = g_unk_7101cb3570 + 52;
    self->path_count = 4;
    for (int i = 0; i < 4; ++i)
        new (&self->paths[i]) CFilePathStrId();
    self->field_0x70 = nullptr;
    self->field_0x78 = nullptr;
    self->field_0x80 = nullptr;
    self->handles[0] = g_unk_7108b6c460;
    self->handles[1] = g_unk_7108b6c460;
    self->field_0xa8 = false;
    self->field_0xa9 = false;
    new (&self->field_0xb0) CStrId("", true);
    self->field_0xb8 = 0;
    self->field_0xbc = 0.0f;
    self->field_0xc0 = 1.0f;
    self->field_0xc4 = 1.0f;
    self->field_0xc8 = nullptr;
    self->field_0xd0 = 0;
    for (int i = 0; i < self->path_count; ++i)
        self->paths[i] = CFilePathStrId("", true);
}
