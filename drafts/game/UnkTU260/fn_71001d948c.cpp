// Unit 260 (render passes, no reliable name): the render owner's constructor: five reserved arrays, default
// colours and factors, the empty bind lists, fourteen pass setups, and the screen size it reads into the unit's
// globals. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "base/tunable/CTunable.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

extern char g_unk_7101a1e8b0[];
extern u32 g_unk_716e595ff4;
extern u32 g_unk_716e595ff8;
extern u32 g_unk_716e596050[16] __attribute__((visibility("hidden")));

void fn_710014e6cc(void* self);
void fn_710014dc2c(void* self);
void fn_710014dd8c(void* self);
void fn_710014deec(void* self);
void fn_710014f71c(void* self);
void fn_7100150398(void* self);
void fn_7100150960(void* self);
void fn_710014ffd0(void* self);
void fn_710014e83c(void* self);
void fn_710014e07c(void* self);
void fn_710014e364(void* self);
void fn_710014e53c(void* self);
void fn_710014f454(void* self);

// A count, packed 8-byte entries and a trailing pointer; constructed empty.
struct UnkBindList_1d948c {
    u32 count;
    u8 items[0x80];
    void* tail;
};
static_assert(sizeof(UnkBindList_1d948c) == 0x90, "UnkBindList_1d948c layout");

// A growable array that starts with room for 200 entries (allocation flags 0x38).
struct UnkReservedArray_1d948c : base::global::CRntVector<void*> {
    UnkReservedArray_1d948c() : CRntVector(0x38) { Reserve(200); }
};

struct UnkRenderOwner_1d948c {
    void* vtable;  // 0x0
    UnkReservedArray_1d948c arrays[5];  // 0x8
    u64 field_0xd0;  // 0xd0
    u32 field_0xd8;  // 0xd8
    u8 pad_0xdc[0x4];
    u64 field_0xe0;  // 0xe0
    u32 field_0xe8;  // 0xe8
    u8 pad_0xec[0x4];
    f32 matrix_0xf0[24];  // 0xf0
    u64 field_0x150;  // 0x150
    f32 field_0x158;  // 0x158
    u32 field_0x15c;  // 0x15c
    CVector4D color_0x160;  // 0x160
    f32 field_0x170[4];  // 0x170
    f32 field_0x180;  // 0x180
    f32 field_0x184;  // 0x184
    f32 field_0x188;  // 0x188
    f32 field_0x18c;  // 0x18c
    f32 field_0x190[5];  // 0x190
    UnkVector4D vector_0x1a4;  // 0x1a4
    u8 pad_0x1b4[0x4];
    u32 field_0x1b8;  // 0x1b8
    UnkVector4D vector_0x1bc;  // 0x1bc
    u8 pad_0x1cc[0x4];
    UnkBindList_1d948c list_0x1d0;  // 0x1d0
    u8 pad_0x260[0x18];
    UnkBindList_1d948c list_0x278;  // 0x278
    UnkBindList_1d948c list_0x308;  // 0x308
    u8 pad_0x398[0x88];
    u64 field_0x420;  // 0x420
    u64 field_0x428;  // 0x428
    u32 field_0x430;  // 0x430
    f32 field_0x434;  // 0x434
    f32 field_0x438;  // 0x438
    u8 field_0x43c;  // 0x43c
    u8 pad_0x43d[0x4b];
    u64 slot_indices_0x488;  // 0x488
    u8 pad_0x490[0x10];
    u64 field_0x4a0;  // 0x4a0
    u64 field_0x4a8;  // 0x4a8
    u8 setup_0x4b0[0xc10];  // 0x4b0
    u8 setup_0x10c0[0xc10];  // 0x10c0
    u8 setup_0x1cd0[0xc10];  // 0x1cd0
    u8 setup_0x28e0[0xc18];  // 0x28e0
    u8 setup_0x34f8[0xc18];  // 0x34f8
    u8 setup_0x4110[0xc18];  // 0x4110
    u8 setup_0x4d28[0xc10];  // 0x4d28
    u8 setup_0x5938[0xc10];  // 0x5938
    u8 setup_0x6548[0xc10];  // 0x6548
    u8 setup_0x7158[0xc10];  // 0x7158
    u8 setup_0x7d68[0xc18];  // 0x7d68
    u8 setup_0x8980[0xc10];  // 0x8980
    u8 setup_0x9590[0xc10];  // 0x9590
    u8 setup_0xa1a0[0xc18];  // 0xa1a0
    u32 field_0xadb8;  // 0xadb8
    u8 pad_0xadbc[0x84];
    u64 field_0xae40;  // 0xae40
    u64 field_0xae48;  // 0xae48
    u8 flag_0xae50;  // 0xae50
    u8 flag_0xae51;  // 0xae51
    bool flag_0xae52;  // 0xae52
    u8 flag_0xae53;  // 0xae53
    u8 flag_0xae54;  // 0xae54
};
static_assert(__builtin_offsetof(UnkRenderOwner_1d948c, list_0x308) == 0x308, "UnkRenderOwner_1d948c layout");
static_assert(__builtin_offsetof(UnkRenderOwner_1d948c, field_0x420) == 0x420, "UnkRenderOwner_1d948c layout");
static_assert(__builtin_offsetof(UnkRenderOwner_1d948c, setup_0x4b0) == 0x4b0, "UnkRenderOwner_1d948c layout");
static_assert(__builtin_offsetof(UnkRenderOwner_1d948c, field_0xadb8) == 0xadb8, "UnkRenderOwner_1d948c layout");
static_assert(__builtin_offsetof(UnkRenderOwner_1d948c, flag_0xae54) == 0xae54, "UnkRenderOwner_1d948c layout");

void fn_71001d948c(UnkRenderOwner_1d948c* self, u32 value, bool flag) {
    self->vtable = g_unk_7101a1e8b0 + 0x10;
    new (&self->arrays[0]) UnkReservedArray_1d948c();
    new (&self->arrays[1]) UnkReservedArray_1d948c();
    new (&self->arrays[2]) UnkReservedArray_1d948c();
    new (&self->arrays[3]) UnkReservedArray_1d948c();
    new (&self->arrays[4]) UnkReservedArray_1d948c();

    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
    self->field_0xe0 = 0;
    self->field_0xe8 = 0;
    for (u32 i = 0; i < 24; i++)
        self->matrix_0xf0[i] = 0.0f;
    self->matrix_0xf0[9] = 1.0f;
    self->matrix_0xf0[15] = 1.0f;
    self->field_0x150 = 0;

    self->color_0x160.x = 0.2f;
    self->color_0x160.y = 0.2f;
    self->color_0x160.z = 0.2f;
    self->color_0x160.w = 1.0f;
    self->field_0x170[0] = 1.0f;
    self->field_0x170[1] = 1.0f;
    self->field_0x170[2] = 0.5f;
    self->field_0x170[3] = 0.5f;
    self->field_0x180 = 6.6f;
    self->field_0x184 = 1.0f;
    self->field_0x188 = 0.3f;
    self->field_0x18c = 0.675f;
    self->field_0x190[0] = 0.0f;
    self->field_0x190[1] = 1.0f;
    self->field_0x190[2] = 1.0f;
    self->field_0x190[3] = 1.0f;
    self->field_0x190[4] = 1.0f;
    new (&self->vector_0x1a4) UnkVector4D();
    new (&self->vector_0x1bc) UnkVector4D();
    {
        CVector4D zero = {0.0f, 0.0f, 0.0f, 0.0f};
        fn_710007b518(&self->vector_0x1a4, &zero);
    }
    self->field_0x1b8 = 0;
    {
        CVector4D value_0x1bc = {0.13f, 0.569f, 0.0f, 0.0f};
        fn_710007b518(&self->vector_0x1bc, &value_0x1bc);
    }

    self->list_0x1d0.count = 0;
    self->list_0x1d0.tail = nullptr;
    self->list_0x278.count = 0;
    self->list_0x278.tail = nullptr;
    self->list_0x308.count = 0;
    self->list_0x308.tail = nullptr;
    self->field_0x420 = 0;
    self->field_0x428 = 0;
    self->field_0x430 = 0;
    self->field_0x434 = 1.0f;
    self->field_0x438 = 1.0f;
    self->field_0x43c = 0;
    self->slot_indices_0x488 = 0;
    self->field_0x4a0 = 0;
    self->field_0x4a8 = 0;

    fn_710014e6cc(self->setup_0x4b0);
    fn_710014dc2c(self->setup_0x10c0);
    fn_710014dd8c(self->setup_0x1cd0);
    fn_710014deec(self->setup_0x28e0);
    fn_710014f71c(self->setup_0x34f8);
    fn_7100150398(self->setup_0x4110);
    fn_7100150960(self->setup_0x4d28);
    fn_710014ffd0(self->setup_0x5938);
    fn_710014e83c(self->setup_0x6548);
    fn_710014dc2c(self->setup_0x7158);
    fn_710014e07c(self->setup_0x7d68);
    fn_710014e364(self->setup_0x8980);
    fn_710014e53c(self->setup_0x9590);
    fn_710014f454(self->setup_0xa1a0);

    self->field_0xadb8 = 0;
    self->field_0xae40 = 0;
    self->field_0xae48 = 0;
    self->flag_0xae50 = 0;
    self->flag_0xae51 = 0;
    self->flag_0xae53 = 1;
    self->flag_0xae54 = 0;
    self->field_0x158 = 1.0f;
    self->field_0x15c = value;
    self->flag_0xae52 = flag;

    // The screen size, in pixels and as floats.
    g_unk_716e595ff4 = fn_7100132f80();
    g_unk_716e595ff8 = fn_7100132f90();
    f32* size = reinterpret_cast<f32*>(g_unk_716e596050);
    size[0] = static_cast<f32>(g_unk_716e595ff4);
    size[1] = static_cast<f32>(g_unk_716e595ff8);
}
