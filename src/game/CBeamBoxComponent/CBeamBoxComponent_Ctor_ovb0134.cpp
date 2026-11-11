// CBeamBoxComponent's unit (tools/tu_map.py unit 1355): fn_71008a5fb0 is the component's constructor.
// After the life component base constructor (fn_71008e04e8) it sets the vtable, an empty engine
// array (+0x388, flags 0xf), the member at +0x3b0 (fn_71008a99e4), two listeners (+0x3e8, +0x408),
// its tunables and state, the callback to fn_71008ac44c (+0x468) and an empty string id (+0x478).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CItemLifeComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkLifeComponent_A5_b71;
extern void* g_unk_7101a7d4e0[];  // the class's vtable
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a77000[];
void fn_71008a99e4(void* self);

struct UnkBeamBox_ovb0134;
void fn_71008ac44c(UnkBeamBox_ovb0134* self);

struct UnkBeamBoxListener_ovb0134 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkBeamBox_ovb0134 {
    void** vtable;
    u8 pad_0x8[0x380];
    base::UnkVector<void*> list;  // 0x388
    u8 field_0x3b0[0x38];
    UnkBeamBoxListener_ovb0134 listener_0x3e8;
    UnkBeamBoxListener_ovb0134 listener_0x408;
    u64 field_0x428;
    f32 field_0x430;
    u32 field_0x434;
    bool field_0x438;
    bool field_0x439;
    bool field_0x43a;
    f32 field_0x43c;
    u64 field_0x440;
    u64 field_0x448;
    u64 field_0x450;
    u64 field_0x458;
    u64 field_0x460;
    UnkBeamBox_ovb0134* callback_object;  // 0x468
    void (*callback)(UnkBeamBox_ovb0134*);  // 0x470
    union {
        CStrId id;  // 0x478
    };
};
static_assert(__builtin_offsetof(UnkBeamBox_ovb0134, id) == 0x478, "layout");

void fn_71008a5fb0(UnkBeamBox_ovb0134* self) {
    fn_71008e04e8(reinterpret_cast<UnkLifeComponent_A5_b71*>(self));
    self->vtable = &g_unk_7101a7d4e0[2];
    new (&self->list) base::UnkVector<void*>(0xf);
    fn_71008a99e4(self->field_0x3b0);
    self->listener_0x3e8.vtable = &g_unk_7101a22d48[2];
    self->listener_0x3e8.field_0x8 = 0;
    self->listener_0x3e8.field_0x10 = 0;
    self->listener_0x3e8.field_0x18 = 0;
    self->listener_0x408.vtable = &g_unk_7101a77000[2];
    self->listener_0x408.field_0x8 = 0;
    self->listener_0x408.field_0x10 = 0;
    self->listener_0x408.field_0x18 = 0;
    self->field_0x428 = 0;
    self->field_0x430 = 0.3f;
    self->field_0x434 = 0;
    self->field_0x438 = false;
    self->field_0x439 = true;
    self->field_0x43a = false;
    self->field_0x43c = 1.0f;
    self->field_0x440 = 0;
    self->field_0x448 = 0;
    self->field_0x450 = 0;
    self->field_0x458 = 0;
    self->field_0x460 = 0;
    self->callback_object = self;
    self->callback = fn_71008ac44c;
    // The empty id: the pool is made first if there is none.
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
}
