// CCollisionComponent's constructors, without (fn_710086016c) and with (fn_71008667fc) the
// post-construction virtual call (slot 3): the component base constructor fn_71010c4148, the
// primary and secondary vtables (+0x0, +0x40), the member at +0x48 (fn_710086023c with 1, 5, 7),
// two empty arrays (step 10, flags 7, default allocator), a word 0x100 at +0xc8, a third array at
// +0xd0, a cleared half-word at +0x100, a listener at +0x108 and a cleared flag at +0x128. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCollisionComponent.h"
#include "game/CComponent.h"
#include "types.h"

struct UnkStateObject;
void fn_710086023c(char* self, u32 a, u32 b, u32 c);

struct UnkCollisionComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    void** field_0x40;
    u8 field_0x48[0x28];
    void* field_0x70;
    void* field_0x78;
    u32 field_0x80;
    bool field_0x84;
    u32 field_0x88;
    base::UnkAllocator* field_0x90;
    void* field_0x98;
    void* field_0xa0;
    u32 field_0xa8;
    bool field_0xac;
    u32 field_0xb0;
    base::UnkAllocator* field_0xb8;
    void* field_0xc0;
    u32 field_0xc8;
    void* field_0xd0;
    void* field_0xd8;
    void* field_0xe0;
    void* field_0xe8;
    u32 field_0xf0;
    u32 field_0xf4;
    base::UnkAllocator* field_0xf8;
    u16 field_0x100;
    void** field_0x108;
    void* field_0x110;
    void* field_0x118;
    void* field_0x120;
    bool field_0x128;
};

static inline __attribute__((always_inline)) void InitCollision(UnkCollisionComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a79c50 + 2;
    self->field_0x40 = g_unk_7101a79c50 + 55;
    fn_710086023c(reinterpret_cast<char*>(self->field_0x48), 1, 5, 7);
    self->field_0x70 = nullptr;
    self->field_0x78 = nullptr;
    self->field_0x80 = 10;
    self->field_0x84 = true;
    self->field_0x88 = 7;
    self->field_0x90 = fn_7100083990();
    self->field_0x98 = nullptr;
    self->field_0xa0 = nullptr;
    self->field_0xa8 = 10;
    self->field_0xac = true;
    self->field_0xb0 = 7;
    self->field_0xb8 = fn_7100083990();
    self->field_0xc0 = nullptr;
    self->field_0xc8 = 0x100;
    self->field_0xd0 = nullptr;
    self->field_0xd8 = nullptr;
    self->field_0xe0 = nullptr;
    self->field_0xe8 = nullptr;
    self->field_0xf0 = 10;
    self->field_0xf4 = 7;
    self->field_0xf8 = fn_7100083990();
    self->field_0x100 = 0;
    self->field_0x108 = g_unk_7101a22d48 + 2;
    self->field_0x110 = nullptr;
    self->field_0x118 = nullptr;
    self->field_0x120 = nullptr;
    self->field_0x128 = false;
}

void fn_710086016c(UnkCollisionComponent* self) {
    InitCollision(self);
}

void fn_71008667fc(UnkCollisionComponent* self) {
    InitCollision(self);
    reinterpret_cast<void (*)(UnkCollisionComponent*)>(self->vtable[3])(self);
}
