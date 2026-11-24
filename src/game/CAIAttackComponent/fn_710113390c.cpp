// CAIAttackComponent unit: the constructor. Runs the attack component base constructor, sets the
// vtables, three named keys, the two timers to -1, an empty list with block size 5 and the empty string
// id in three members, then a listener. Names and layouts are placeholders.
#include <new>

#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAutclastChargeAttack.h"
#include "game/UnkTU604.h"
#include "types.h"

using base::global::CStrId;

namespace base {
struct IAllocator;
}
struct UnkAttackComponent_c3;
extern void* g_unk_7101cb2ab0[];

void fn_7101135f04(UnkAttackComponent_c3* self);
void fn_710113596c();

struct UnkAttackPool_l13390c {
    u64 field_0x0;
    u64 field_0x8;
    u8 list_0x10[0x70];
    u8 field_0x80;
    u8 pad_0x81[3];
    u32 field_0x84[13];

    UnkAttackPool_l13390c() {
        field_0x0 = 0;
        field_0x8 = 0;
        fn_71003fa9fc(list_0x10, 5, 0, 0x13, nullptr);
        field_0x80 = 1;
        for (u32& v : field_0x84)
            v = 0;
    }
};
static_assert(sizeof(UnkAttackPool_l13390c) == 0xb8, "layout");

struct UnkAIAttackComponent_l13390c {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    u8 pad_0x48[0xf0];
    base::UnkNamedEntry key_0x138;
    base::UnkNamedEntry key_0x168;
    u32 field_0x198;
    u8 pad_0x19c[4];
    void* field_0x1a0;
    u8 field_0x1a8;
    u8 pad_0x1a9[3];
    f32 field_0x1ac;
    f32 field_0x1b0;
    u8 pad_0x1b4[4];
    UnkAttackPool_l13390c pool;  // 0x1b8
    base::UnkNamedEntry key_0x270;
    s32 field_0x2a0;
    u8 pad_0x2a4[4];
    CStrId id_0x2a8;
    CStrId id_0x2b0;
    CStrId id_0x2b8;
    u8 field_0x2c0;
    u8 pad_0x2c1[7];
    u64 field_0x2c8;
    u8 field_0x2d0;
    u8 pad_0x2d1[7];
    base::UnkNamedEntry key_0x2d8;
    void** field_0x308;
    void* field_0x310;
    void (*field_0x318)();
    u64 field_0x320;
    u64 field_0x328;
    u64 field_0x330;
    u64 field_0x338;
    u64 field_0x340;
};
static_assert(__builtin_offsetof(UnkAIAttackComponent_l13390c, key_0x2d8) == 0x2d8, "layout");
static_assert(__builtin_offsetof(UnkAIAttackComponent_l13390c, field_0x308) == 0x308, "layout");
static_assert(__builtin_offsetof(UnkAIAttackComponent_l13390c, key_0x270) == 0x270, "layout");

void fn_710113390c(UnkAIAttackComponent_l13390c* self) {
    fn_7101135f04(reinterpret_cast<UnkAttackComponent_c3*>(self));
    self->vtable = g_unk_7101cb2ab0 + 2;
    self->vtable_0x40 = g_unk_7101cb2ab0 + 62;
    CRntString_ConstructFromCString(&self->key_0x138, g_unk_710162e2ed);
    CRntString_ConstructFromCString(&self->key_0x168, g_unk_710162e2ed);
    self->field_0x198 = 0;
    self->field_0x1a0 = nullptr;
    self->field_0x1a8 = 0;
    self->field_0x1ac = -1.0f;
    self->field_0x1b0 = -1.0f;
    new (&self->pool) UnkAttackPool_l13390c();
    CRntString_ConstructFromCString(&self->key_0x270, g_unk_710162e2ed);
    self->field_0x2a0 = -1;
    new (&self->id_0x2a8) CStrId(base::global::CStrId::Empty());
    new (&self->id_0x2b0) CStrId(base::global::CStrId::Empty());
    new (&self->id_0x2b8) CStrId(base::global::CStrId::Empty());
    self->field_0x2c0 = 0;
    self->field_0x2c8 = 0;
    self->field_0x2d0 = 0;
    CRntString_ConstructFromCString(&self->key_0x2d8, g_unk_710162e2ed);
    self->field_0x308 = g_unk_7101a2aef8 + 2;
    self->field_0x328 = 0;
    self->field_0x338 = 0;
    self->field_0x320 = 0;
    self->field_0x340 = 0;
    self->field_0x330 = 0;
    self->field_0x310 = self;
    self->field_0x318 = fn_710113596c;
}
