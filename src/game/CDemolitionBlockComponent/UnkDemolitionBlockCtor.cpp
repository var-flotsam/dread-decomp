// Constructor of a component in the CDemolitionBlockComponent unit, built on the activatable
// component of fn_7100878d24 (inlined; src/game/CCharClassActivatableComponent/): its own vtable,
// the member at +0x80 (fn_71007b15f4), a listener at +0xc0 and two empty arrays (step 10, flags
// 0xf, default allocator), then the post-construction virtual call (slot 3). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

struct UnkStateObject;
void fn_71007b15f4(char* self);
extern void* g_unk_7101a7ac20[];
extern void* g_unk_7101a70380[];
extern void* g_unk_7101a22d48[];

struct UnkDemolitionBlockComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    u32 field_0x7c;
    u8 field_0x80[0x40];
    void** field_0xc0;
    void* field_0xc8;
    void* field_0xd0;
    void* field_0xd8;
    void* field_0xe0;
    void* field_0xe8;
    u32 field_0xf0;
    bool field_0xf4;
    u32 field_0xf8;
    base::UnkAllocator* field_0x100;
    void* field_0x108;
    void* field_0x110;
    u32 field_0x118;
    bool field_0x11c;
    u32 field_0x120;
    base::UnkAllocator* field_0x128;
};

void fn_71007b1848(UnkDemolitionBlockComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    self->vtable = g_unk_7101a70380 + 2;
    fn_71007b15f4(reinterpret_cast<char*>(self->field_0x80));
    self->field_0xc0 = g_unk_7101a22d48 + 2;
    self->field_0xc8 = nullptr;
    self->field_0xd0 = nullptr;
    self->field_0xd8 = nullptr;
    self->field_0xe0 = nullptr;
    self->field_0xe8 = nullptr;
    self->field_0xf0 = 10;
    self->field_0xf4 = true;
    self->field_0xf8 = 0xf;
    self->field_0x100 = fn_7100083990();
    self->field_0x108 = nullptr;
    self->field_0x110 = nullptr;
    self->field_0x118 = 10;
    self->field_0x11c = true;
    self->field_0x120 = 0xf;
    self->field_0x128 = fn_7100083990();
    reinterpret_cast<void (*)(UnkDemolitionBlockComponent*)>(self->vtable[3])(self);
}
