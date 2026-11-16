// CFreezeRoomComponent (tools/tu_map.py unit 1537): fn_7100a22cf4, the constructor. On the damage
// trigger base (fn_7100a45a30): the vtable, an empty enter sound path and handle, the environment ids
// ("ICEZONE"), the trigger's name "Freeze Zone" and its flags. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStrId;

struct UnkBaseDamageTrigger3;
void fn_7100a45a30(UnkBaseDamageTrigger3* self);

extern char g_unk_7101a8d410[];  // vtable

struct UnkFreezeRoom_la22cf4 {
    void* vtable;
    u8 pad_0x8[0xadc - 0x8];
    s32 field_0xadc;
    bool field_0xae0;
    bool field_0xae1;
    u8 pad_0xae2[0xae9 - 0xae2];
    bool field_0xae9;
    u8 pad_0xaea[0xb74 - 0xaea];
    bool field_0xb74;
    bool field_0xb75;
    bool field_0xb76;
    bool field_0xb77;
    UnkStrId name;  // 0xb78
    u8 pad_0xb80[0xbe0 - 0xb80];
    bool field_0xbe0;
    u8 pad_0xbe1[0xc1c - 0xbe1];
    s32 field_0xc1c;
    CFilePathStrId enter_path;  // 0xc20
    UnkSoundHandle enter_sound;  // 0xc28
    UnkStrId environment;        // 0xc38
    CStrId field_0xc40;
    bool field_0xc48;
};
static_assert(__builtin_offsetof(UnkFreezeRoom_la22cf4, field_0xc48) == 0xc48, "layout");

static inline void InitEmpty(base::global::UnkStrIdBase* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

static inline void InitEmpty(CFilePathStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    id->data = &g_unk_7101d47200->empty;
}

void fn_7100a22cf4(UnkFreezeRoom_la22cf4* self) {
    fn_7100a45a30(reinterpret_cast<UnkBaseDamageTrigger3*>(self));
    self->vtable = g_unk_7101a8d410 + 0x10;
    self->field_0xc1c = 0;
    InitEmpty(&self->enter_path);
    self->enter_sound.ref = nullptr;
    self->enter_sound.serial = -1;
    InitEmpty(&self->environment);
    InitEmpty(&self->field_0xc40);
    self->name = UnkStrId("Freeze Zone", true);
    self->field_0xb74 = false;
    self->field_0xb75 = false;
    self->field_0xb76 = false;
    self->field_0xb77 = true;
    self->field_0xae1 = true;
    self->field_0xadc = 2;
    self->field_0xbe0 = true;
    self->field_0xae9 = true;
    self->field_0xc40 = g_unk_7101d470e0;
    self->environment = UnkStrId("ICEZONE", true);
    self->field_0xc48 = true;
}
