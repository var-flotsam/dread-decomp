// CItemLifeComponent's unit (tools/tu_map.py): the life component's constructor (fn_71008e04e8) and its
// update (fn_71008e1174), which steps a colour towards its goal and hands it to a target object. Names
// and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector4D.h"
#include "types.h"

void fn_710115f7bc(void* self);  // as CPowerBombBlockLifeComponent.cpp declares it
// base::refl_StrIdListCtor<..>, instantiated in src/clones (UnkClone_fn_71008e266c).
void fn_71008e266c(char* self);
extern void* g_unk_7101a7f028[];
extern void* g_unk_7101a22d48[];

struct UnkItemLifeCtor_ovb0114 {
    void** vtable;
    u8 pad_0x8[0x238];
    u32 field_0x240;
    u8 pad_0x244[0x4];
    u64 field_0x248;
    u64 field_0x250;
    char list[0x38];  // 0x258
    base::UnkVector<void*> lists[3];  // 0x290, flags 0xf
    bool field_0x308;
    u8 pad_0x309[0xf];
    void* inner_vtable;  // 0x318
    u64 field_0x320;
    u64 field_0x328;
    u64 field_0x330;
    u64 field_0x338;
    u32 field_0x340;
    u8 pad_0x344[0x4];
    u64 field_0x348;
    u64 field_0x350;
    u64 field_0x358;
    u64 field_0x360;
    u64 field_0x368;
    u64 field_0x370;
    u64 field_0x378;
    u64 field_0x380;
};
static_assert(__builtin_offsetof(UnkItemLifeCtor_ovb0114, field_0x380) == 0x380, "layout");

struct UnkLifeComponent_A5_b71;

// The life component's own constructor (the base of the item and door shield life components): after
// the base constructor (fn_710115f7bc) it sets the vtable, clears the fields at +0x240, builds the
// string-id list at +0x258 (fn_71008e266c), three engine arrays (flags 0xf), a flag, and the object at
// +0x318 (vtable g_unk_7101a22d48, a count of 1 at +0x340).
void fn_71008e04e8(UnkLifeComponent_A5_b71* component) {
    UnkItemLifeCtor_ovb0114* self = reinterpret_cast<UnkItemLifeCtor_ovb0114*>(component);
    fn_710115f7bc(self);
    self->vtable = &g_unk_7101a7f028[2];
    self->field_0x240 = 0;
    self->field_0x248 = 0;
    self->field_0x250 = 0;
    fn_71008e266c(self->list);
    for (u32 i = 0; i < 3; ++i)
        new (&self->lists[i]) base::UnkVector<void*>(0xf);
    self->field_0x308 = false;
    self->inner_vtable = reinterpret_cast<char*>(g_unk_7101a22d48) + 0x10;
    self->field_0x320 = 0;
    self->field_0x328 = 0;
    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->field_0x340 = 1;
    self->field_0x348 = 0;
    self->field_0x350 = 0;
    self->field_0x358 = 0;
    self->field_0x360 = 0;
    self->field_0x368 = 0;
    self->field_0x370 = 0;
    self->field_0x378 = 0;
    self->field_0x380 = 0;
}

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkCommanderLife_A5_b63;
struct UnkColorLerp_A4_b75;
void fn_7101160078(UnkCommanderLife_A5_b63* self, f32 dt);
UnkVector4D fn_71008c6b0c(UnkColorLerp_A4_b75* self, f32 dt);

struct UnkItemLifeTint_ovb0114 {
    u8 pad_0x0[0x30];
    CVector4D colour;  // 0x30
};

// The object the colour is applied to.
struct UnkItemLifeTarget_ovb0114 {
    u8 pad_0x0[0x240];
    CVector4D colour;  // 0x240
    u8 pad_0x250[0x90];
    struct {
        u8 pad_0x0[0x30];
        CVector4D* colour;  // 0x30
    }* field_0x2e0;
};

struct UnkItemLifeUpdate_ovb0114 {
    u8 pad_0x0[0x348];
    UnkItemLifeTarget_ovb0114* target;  // 0x348
    CVector4D current;                  // 0x350, the start of a colour lerp (fn_71008c6b0c)
    CVector4D start;                    // 0x360
    CVector4D goal;                     // 0x370
};

static inline bool Near(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

// The same test with the two bounds compared in the other order (as the original does for w).
static inline bool NearW(f32 a, f32 b) {
    return a + 1e-5f >= b && a - 1e-5f <= b;
}

// After the base update: while the colour differs from its goal, steps it (fn_71008c6b0c) and gives
// it to the target object (and to the target's tint at +0x2e0, when there is one).
void fn_71008e1174(UnkItemLifeUpdate_ovb0114* self, f32 dt) {
    fn_7101160078(reinterpret_cast<UnkCommanderLife_A5_b63*>(self), dt);
    if (Near(self->current.x, self->goal.x) && Near(self->current.y, self->goal.y) &&
        Near(self->current.z, self->goal.z) && NearW(self->current.w, self->goal.w))
        return;
    if (!self->target)
        return;
    fn_71008c6b0c(reinterpret_cast<UnkColorLerp_A4_b75*>(&self->current), dt);
    UnkItemLifeTarget_ovb0114* target = self->target;
    UnkVector4D colour(*static_cast<UnkVector4D*>(&self->current));
    fn_710007b518(&target->colour, &colour);
    if (target->field_0x2e0)
        *target->field_0x2e0->colour = colour;
}
