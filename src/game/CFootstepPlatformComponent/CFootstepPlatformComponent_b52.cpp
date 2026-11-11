// CFootstepPlatformComponent (unit 1255): the constructor. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a73a70[];  // the component's vtables
extern void* g_unk_7101a20998[];  // the lists' base vtable
extern void* g_unk_7101a33fa0[];  // the lists' vtable

struct UnkStateObject;

struct UnkFootstepList {
    void** vtable;
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x1c;
    u32 flags;
    base::UnkAllocator* allocator;
    u32 field_0x30;
    void* field_0x38;

    UnkFootstepList() {
        vtable = g_unk_7101a20998 + 2;
        data = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        field_0x1c = true;
        flags = 0xf;
        allocator = fn_7100083990();
        field_0x30 = 0;
        vtable = g_unk_7101a33fa0 + 2;
        field_0x38 = nullptr;
    }
};

struct UnkFootstepPlatformComponent {
    void** vtable;
    u8 unk_0x8[0x38];
    void** vtable_0x40;
    UnkFootstepList list_a;  // 0x48
    UnkFootstepList list_b;  // 0x88
    bool field_0xc8;
    u32 field_0xcc;
    bool field_0xd0;
    f32 field_0xd4;
    f32 field_0xd8;
    CStrId id_a;  // 0xe0
    CStrId id_b;  // 0xe8
};

void fn_71007e7794(UnkFootstepPlatformComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a73a70 + 2;
    self->vtable_0x40 = g_unk_7101a73a70 + 0x34;
    new (&self->list_a) UnkFootstepList();
    new (&self->list_b) UnkFootstepList();
    self->field_0xc8 = false;
    self->field_0xcc = 0;
    self->field_0xd0 = false;
    self->field_0xd4 = 0.0f;
    self->field_0xd8 = 1.5f;
    new (&self->id_a) CStrId(g_unk_7101d470e0);
    new (&self->id_b) CStrId(g_unk_7101d470e0);
}
