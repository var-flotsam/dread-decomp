// GUI::CScene3D's unit: the constructor of the 3D model scene. It runs the base constructor, sets the
// vtable and fills the members with their defaults. Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "types.h"

extern void* g_unk_7101a9e968[];  // the scene's vtable
extern void* g_unk_7101a9ebb8[];  // the node list's vtable
extern u32 g_unk_710164d27c;
extern u32 g_unk_710164d278;
extern u32 g_unk_710164d274;

struct __attribute__((packed)) UnkWords_lb72380 {
    u64 value;
};

struct UnkScene3DCtor_lb72380 {
    void** vtable;
    u8 pad_0x8[0x280];
    u8 name[0x30];  // 0x288
    s32 frame;      // 0x2b8
    u8 pad_0x2bc[0x24];
    u8 field_0x2e0[0x30];
    u32 field_0x310;
    u8 field_0x314[0x18];
    u8 pad_0x32c[0xc];
    u8 field_0x338[0x30];
    u8 pad_0x368[0x8];
    void** list_vtable;  // 0x370
    u32 list_count;      // 0x378
    u8 pad_0x37c[0x4];
    void* list_head;  // 0x380
    void* list_tail;  // 0x388
    base::UnkAllocator* allocator;  // 0x390
    u32 field_0x398;
    u8 pad_0x39c[0x4];
    u32 field_0x3a0;
    u32 field_0x3a4;
    u32 field_0x3a8;
    UnkWords_lb72380 field_0x3ac;
    UnkWords_lb72380 field_0x3b4;
    f32 field_0x3bc;
    f32 field_0x3c0;
    f32 field_0x3c4;
    u8 pad_0x3c8[0x8];
    u8 maps[0x98];  // 0x3d0
    u16 field_0x468;
    u8 pad_0x46a[0x6];
    u8 item[0x38];  // 0x470
    u8 pad_0x4a8[0x968];
    void* field_0xe10;
    u8 field_0xe18[0x30];
    u64 field_0xe48;
    u16 field_0xe50;
};
static_assert(__builtin_offsetof(UnkScene3DCtor_lb72380, field_0xe50) == 0xe50, "layout");
static_assert(__builtin_offsetof(UnkScene3DCtor_lb72380, field_0xe18) == 0xe18, "layout");
static_assert(__builtin_offsetof(UnkScene3DCtor_lb72380, maps) == 0x3d0, "layout");

void fn_71011f3c4c(void* self);  // the base constructor
void fn_710007e6c4(void* self);  // an empty CRntString
void fn_7100b724a4(void* maps);

void fn_7100b72380(UnkScene3DCtor_lb72380* self) {
    fn_71011f3c4c(self);
    self->vtable = &g_unk_7101a9e968[2];
    fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(self->name), g_unk_710162e2ed);
    self->frame = 0x7fffffff;
    fn_710007e6c4(self->field_0x2e0);
    u32 value = g_unk_710164d27c;
    self->field_0x310 = value;
    __builtin_memset(self->field_0x314, 0, 0x18);
    fn_710007e6c4(self->field_0x338);
    self->list_vtable = &g_unk_7101a9ebb8[2];
    self->list_count = 0;
    self->list_head = nullptr;
    self->list_tail = nullptr;
    self->allocator = fn_7100083990();
    self->field_0x3a8 = value;
    self->field_0x398 = 0x13;
    self->field_0x3a0 = g_unk_710164d278;
    self->field_0x3a4 = g_unk_710164d274;
    __builtin_memset(&self->field_0x3b4, 0, 8);
    self->field_0x3bc = 0.0f;
    __builtin_memset(&self->field_0x3ac, 0, 8);
    self->field_0x3c0 = -1.0f;
    self->field_0x3c4 = -500.0f;
    fn_7100b724a4(self->maps);
    self->field_0x468 = 0;
    fn_71009ca9fc(self->item, "GUIScene3D", 0x330c00);
    self->field_0xe10 = nullptr;
    fn_710007e6c4(self->field_0xe18);
    self->field_0xe48 = 0x3f80000000000000ull;
    self->field_0xe50 = 0x100;
}
