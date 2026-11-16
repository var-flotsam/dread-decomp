// CHeatRoomComponent unit (tools/tu_map.py unit 1538): the component's constructor: the damage
// trigger base (fn_7100a45a30), the vtable, an empty list of parts, the sound path and two
// environment ids, no sound, then the trigger's name "Heat Zone" and settings. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStrId;

struct UnkBaseDamageTrigger3;

struct UnkHeatPart_la261fc;

struct UnkHeatRoom_la261fc {
    char* vtable;
    u8 pad_0x8[0xadc - 0x8];
    u32 field_0xadc;
    u8 pad_0xae0[0x1];
    bool field_0xae1;
    u8 pad_0xae2[0xae9 - 0xae2];
    bool field_0xae9;
    u8 pad_0xaea[0xb74 - 0xaea];
    u32 field_0xb74;
    UnkStrId name;  // 0xb78
    u8 pad_0xb80[0xbe0 - 0xb80];
    bool field_0xbe0;
    u8 pad_0xbe1[0xc24 - 0xbe1];
    u32 field_0xc24;
    CRntVector<UnkHeatPart_la261fc> parts;  // 0xc28
    CFilePathStrId sound_path;              // 0xc50
    CStrId environment;                     // 0xc58
    CStrId previous_environment;            // 0xc60
    bool field_0xc68;
    UnkSoundHandle sound;  // 0xc70
    bool field_0xc80;
    bool field_0xc81;
};
static_assert(__builtin_offsetof(UnkHeatRoom_la261fc, sound) == 0xc70, "layout");
static_assert(__builtin_offsetof(UnkHeatRoom_la261fc, field_0xc80) == 0xc80, "layout");

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100a261fc(UnkHeatRoom_la261fc* self) {
    fn_7100a45a30(reinterpret_cast<UnkBaseDamageTrigger3*>(self));
    self->vtable = g_unk_7101a8d620 + 0x10;
    self->field_0xc24 = 0;
    new (&self->parts) CRntVector<UnkHeatPart_la261fc>(0xf);
    new (&self->sound_path) CFilePathStrId();
    InitEmptyStrId(&self->environment);
    InitEmptyStrId(&self->previous_environment);
    self->sound.ref = nullptr;
    self->sound.serial = -1;
    self->field_0xc80 = false;
    self->field_0xc81 = false;
    self->name = UnkStrId("Heat Zone", true);
    self->field_0xb74 = 0x20000;
    self->field_0xae1 = true;
    self->field_0xadc = 2;
    self->field_0xbe0 = true;
    self->field_0xae9 = true;
    self->previous_environment = g_unk_7101d470e0;
    self->field_0xc80 = false;
    self->field_0xc81 = false;
    self->field_0xc68 = true;
}
