// CSamusMovementState::CParams's unit (tools/tu_map.py): a constructor (the destructor is
// fn_71009b0800): an empty reflected value, two groups of three empty arrays and a flag, cleared
// state and the link at +0x168. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "types.h"

extern void* g_unk_7101a87850[];

// Three arrays and a flag (twice in the member at +0x48).
struct UnkSamusParamsArrays_l9b06dc {
    base::global::CRntVector<void*> lists[3];
    bool field_0x78;
};

struct UnkSamusParams_l9b06dc {
    void** vtable;
    u64 field_0x8;
    u32 field_0x10;
    base::reflection::UnkReflValue value;  // 0x18
    u8 pad_0x30[0x18];
    UnkSamusParamsArrays_l9b06dc arrays[2];  // 0x48
    f32 field_0x148;
    void* field_0x150;
    bool field_0x158;
    bool field_0x159;
    void* field_0x160;
    void* link_vtable;  // 0x168
    u64 field_0x170;
    u64 field_0x178;
    u64 field_0x180;
};
static_assert(__builtin_offsetof(UnkSamusParams_l9b06dc, arrays) == 0x48, "UnkSamusParams_l9b06dc layout");
static_assert(__builtin_offsetof(UnkSamusParams_l9b06dc, field_0x148) == 0x148, "UnkSamusParams_l9b06dc layout");
static_assert(__builtin_offsetof(UnkSamusParams_l9b06dc, field_0x180) == 0x180, "UnkSamusParams_l9b06dc layout");

void fn_71009b06dc(UnkSamusParams_l9b06dc* self) {
    self->value.field_0x0 = nullptr;
    self->value.field_0x14 = 1;
    self->field_0x10 = 0xf;
    self->value.type = nullptr;
    self->value.kind = 2;
    self->vtable = &g_unk_7101a87850[2];
    self->field_0x8 = 0;
    for (UnkSamusParamsArrays_l9b06dc& group : self->arrays) {
        for (base::global::CRntVector<void*>& list : group.lists)
            new (&list) base::global::CRntVector<void*>(0xf);
        group.field_0x78 = false;
    }
    self->field_0x148 = 0.0f;
    self->field_0x159 = false;
    self->field_0x150 = nullptr;
    self->field_0x160 = nullptr;
    self->field_0x158 = false;
    self->link_vtable = &g_unk_7101a22d48[2];
    self->field_0x170 = 0;
    self->field_0x178 = 0;
    self->field_0x180 = 0;
    self->field_0x10 = 0xf;
}
