// CCharClassBillboardComponent unit: fn_710072f1a4 is a component's constructor on the component base
// (fn_71011acd10): the vtable, its default distances and scales, empty file-path and string ids, and
// two growable arrays (allocation flags 0xf). Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStruct_71008bca30;
struct UnkItemBase_71011a91c4;
extern char g_unk_7101a6d578[];

struct UnkBillboardComponent_l72f1a4 {
    void* vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    u32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    u8 pad_0x64[0x4];
    CFilePathStrId field_0x68;
    CFilePathStrId field_0x70;
    u32 field_0x78;
    u32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    f32 field_0x9c;
    u32 field_0xa0;
    u8 pad_0xa4[0x4];
    CStrId field_0xa8;
    CStrId field_0xb0;
    f32 field_0xb8;
    u8 pad_0xbc[0x4];
    CStrId field_0xc0;
    f32 field_0xc8;
    bool field_0xcc;
    u8 pad_0xcd[0x3];
    CStrId field_0xd0;
    CStrId field_0xd8;
    base::global::CRntVector<UnkItemBase_71011a91c4*> field_0xe0;
    base::global::CRntVector<UnkItemBase_71011a91c4*> field_0x108;
    CStrId field_0x130;
    f32 field_0x138;
    u8 pad_0x13c[0x4];
    CFilePathStrId field_0x140;
    f32 field_0x148;
    f32 field_0x14c;
    CStrId field_0x150;
    u32 field_0x158;
    f32 field_0x15c;
    f32 field_0x160;
};
static_assert(__builtin_offsetof(UnkBillboardComponent_l72f1a4, field_0x160) == 0x160, "layout");

void fn_710072f1a4(UnkBillboardComponent_l72f1a4* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->field_0x58 = 0;
    self->vtable = g_unk_7101a6d578 + 0x10;
    self->field_0x5c = 20.0f;
    self->field_0x60 = 10.0f;
    new (&self->field_0x68) CFilePathStrId("", true);
    self->field_0x70.data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x70.data = &g_unk_7101d47200->empty;
    self->field_0x78 = 1;
    self->field_0x7c = 1;
    self->field_0x80 = 1.0f;
    self->field_0x84 = 1.0f;
    self->field_0x88 = 1.0f;
    self->field_0x8c = 0;
    self->field_0x90 = 0;
    self->field_0x94 = 0;
    self->field_0x98 = 0;
    self->field_0x9c = 1.0f;
    self->field_0xa0 = 0;
    new (&self->field_0xa8) CStrId("", true);
    new (&self->field_0xb0) CStrId("", true);
    self->field_0xb8 = 10.0f;
    new (&self->field_0xc0) CStrId("", true);
    self->field_0xc8 = 30.0f;
    self->field_0xcc = false;
    new (&self->field_0xd0) CStrId("", true);
    new (&self->field_0xd8) CStrId("", true);
    new (&self->field_0xe0) base::global::CRntVector<UnkItemBase_71011a91c4*>(0xf);
    new (&self->field_0x108) base::global::CRntVector<UnkItemBase_71011a91c4*>(0xf);
    new (&self->field_0x130) CStrId("", true);
    self->field_0x138 = 0.2f;
    new (&self->field_0x140) CFilePathStrId("", true);
    self->field_0x148 = 300.0f;
    self->field_0x14c = 1000.0f;
    new (&self->field_0x150) CStrId("", true);
    self->field_0x158 = 1;
    self->field_0x15c = 500.0f;
    self->field_0x160 = 2000.0f;
    self->field_0x18 = 2500;
}
