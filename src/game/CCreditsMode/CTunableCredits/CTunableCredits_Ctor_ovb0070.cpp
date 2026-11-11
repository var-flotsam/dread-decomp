// CCreditsMode::CTunableCredits's unit (tools/tu_map.py unit 2724): the constructor of the credits
// object whose destructors are fn_7101185dd4 (CTunableCredits_Dtor_b44.cpp) and fn_7101185ea8. Names
// and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/CCreditsMode/CTunableCredits.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "game/UnkTU2725.h"
#include "types.h"

using base::global::CRntString;

struct UnkMusicTrigger;

struct UnkCreditsNode_ovb0070;

struct UnkCreditsCtor_ovb0070 {
    void** vtable;
    CRntString name;  // 0x8
    bool field_0x38;
    bool field_0x39;
    u8 pad_0x3a[0x16];
    f32 time;   // 0x50
    u32 flags;  // 0x54
    f32 wait;   // 0x58
    f32 field_0x5c;
    void* objects[4];  // 0x60
    u64 field_0x80;
    u8 pad_0x88[0x8];
    void** list_vtable;  // 0x90: the list of lines
    u32 count;
    UnkCreditsNode_ovb0070* head;
    UnkCreditsNode_ovb0070* tail;
    base::UnkAllocator* allocator;
    u32 list_flags;  // 0xb8
    u8 pad_0xbc[0x14];
    u8 member[0x9a0];  // 0xd0
    void* field_0xa70;
    void* field_0xa78;
    u16 field_0xa80;
};
static_assert(__builtin_offsetof(UnkCreditsCtor_ovb0070, field_0xa80) == 0xa80, "layout");

void fn_7101183f84(UnkCreditsCtor_ovb0070* self) {
    fn_7101186780(reinterpret_cast<UnkMusicTrigger*>(self));
    self->vtable = &g_unk_7101cb4ef8[2];
    self->time = 0.0f;
    self->flags = 1;
    self->wait = 0.0f;
    self->field_0x5c = 0.0f;
    self->objects[0] = nullptr;
    self->objects[1] = nullptr;
    self->objects[2] = nullptr;
    self->objects[3] = nullptr;
    self->field_0x80 = 0;
    self->list_vtable = &g_unk_7101cb5010[2];
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = fn_7100083990();
    self->list_flags = 0x13;
    fn_71009ca9fc(self->member, "CreditsMode", 0xc);
    self->field_0xa70 = nullptr;
    self->field_0xa78 = nullptr;
    self->field_0xa80 = 0;
    fn_710007e77c(&self->name, "CREDITS");
    self->field_0x38 = true;
    self->field_0x39 = true;
}
