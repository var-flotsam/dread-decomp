// CFrozenComponent's unit (tools/tu_map.py unit 1260): the frozen component's constructor. After the
// component base constructor it sets the vtable, a listener, four numbered stages, a timer-like member,
// three engine arrays (flags 0xf) and a callback link back to itself. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CComponent.h"
#include "game/CFrozenComponent.h"
#include "types.h"

extern void* g_unk_7101a74318[];
void fn_71007fb5f8(void* owner);

// One of the four stages (ids 1 to 4).
struct UnkFrozenStage_l7f8bc0 {
    void** vtable;
    u64 field_0x8;
    u32 id;
    u8 pad_0x14[0x4];
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;

    explicit UnkFrozenStage_l7f8bc0(u32 id)
        : field_0x8(0), field_0x18(0), field_0x20(0), field_0x28(2),
          field_0x2c(1), field_0x30(0), field_0x38(0), field_0x40(0), field_0x48(0), field_0x50(0),
          field_0x58(0), field_0x60(0), field_0x68(0) {
        vtable = &g_unk_7101a74318[2];
        this->id = id;
    }
};
static_assert(sizeof(UnkFrozenStage_l7f8bc0) == 0x70, "layout");

struct UnkFrozenTimer_l7f8bc0 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    u8 pad_0x24[0x80];
    u32 field_0xa4;

    UnkFrozenTimer_l7f8bc0()
        : vtable(g_unk_7101a743c8 + 2), field_0x8(0), field_0x10(0),
          field_0x18(0), field_0x20(0), field_0xa4(5) {}
};
static_assert(sizeof(UnkFrozenTimer_l7f8bc0) == 0xa8, "layout");

struct UnkFrozenCtor_l7f8bc0 {
    void** vtable;
    u8 pad_0x8[0x38];
    u64 field_0x40;
    void** listener_vtable;  // 0x48
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    UnkFrozenStage_l7f8bc0 stages[4];  // 0x68
    UnkFrozenTimer_l7f8bc0 timer;       // 0x228
    u16 field_0x2d0;
    u8 pad_0x2d2[0x6];
    u64 field_0x2d8;
    base::global::CRntVector<void*> lists[3];  // 0x2e0
    bool field_0x358;
    u8 pad_0x359[0x7];
    u64 field_0x360;
    UnkFrozenCtor_l7f8bc0* owner;  // 0x368
    void (*callback)(void*);      // 0x370
    u64 field_0x378;
    u64 field_0x380;
    u64 field_0x388;
    u64 field_0x390;
    u8 pad_0x398[0x4];
    u32 field_0x39c[5];
};
static_assert(__builtin_offsetof(UnkFrozenCtor_l7f8bc0, timer) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkFrozenCtor_l7f8bc0, lists) == 0x2e0, "layout");
static_assert(__builtin_offsetof(UnkFrozenCtor_l7f8bc0, field_0x39c) == 0x39c, "layout");

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_71007f8bc0(void* component) {
    UnkFrozenCtor_l7f8bc0* self = static_cast<UnkFrozenCtor_l7f8bc0*>(component);
    fn_71010c4148(static_cast<UnkStateObject*>(component));
    self->vtable = &g_unk_7101a740d0[2];
    self->field_0x40 = 0;
    self->listener_vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a22d48) + 0x10);
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    new (&self->stages[0]) UnkFrozenStage_l7f8bc0(1);
    new (&self->stages[1]) UnkFrozenStage_l7f8bc0(2);
    new (&self->stages[2]) UnkFrozenStage_l7f8bc0(3);
    new (&self->stages[3]) UnkFrozenStage_l7f8bc0(4);
    new (&self->timer) UnkFrozenTimer_l7f8bc0();
    self->field_0x2d0 = 0;
    self->field_0x2d8 = 0;
    for (u32 i = 0; i < 3; ++i)
        new (&self->lists[i]) base::global::CRntVector<void*>(0xf);
    self->field_0x358 = false;
    self->field_0x360 = 0;
    self->owner = self;
    self->callback = &fn_71007fb5f8;
    self->field_0x378 = 0;
    self->field_0x380 = 0;
    self->field_0x388 = 0;
    self->field_0x390 = 0;
    for (u32 i = 0; i < 5; ++i)
        self->field_0x39c[i] = 0;
}
