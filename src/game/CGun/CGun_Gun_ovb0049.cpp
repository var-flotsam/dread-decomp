// CGun (tools/tu_map.py unit 1302; the name from reflection): the gun's constructor (fn_7100835410) and the
// creation of one of its billboards (fn_71008366fc). Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU272.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char g_unk_7101a77980[];

// One of the gun's 150 colour slots.
struct UnkGunSlot_ovb0049 {
    u64 field_0x0;
    f32 field_0x8;
    f32 color[4];  // 0xc
    s32 field_0x1c;  // unused padding in the constructor
    s32 field_0x20;
    bool field_0x24;
};
static_assert(sizeof(UnkGunSlot_ovb0049) == 0x28, "slot layout");

struct UnkGunCtor_ovb0049 {
    void** vtable;
    u8 pad_0x8[0x8];
    CStrId field_0x10;
    void* field_0x18;
    CStrId field_0x20;
    u8 field_0x28[0x10];
    s32 field_0x38;
    s32 field_0x3c;
    s32 field_0x40;
    s32 field_0x44;
    u8 pad_0x48[0x4];
    bool field_0x4c;
    u8 pad_0x4d[0x3];
    CStrId field_0x50;
    CStrId field_0x58;
    u64 field_0x60;
    s64 field_0x68;
    CStrId field_0x70;
    bool field_0x78;
    u8 pad_0x79[0x7];
    u64 field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    s64 field_0x98;
    s64 field_0xa0;
    s64 field_0xa8;
    s64 field_0xb0;
    u64 field_0xb8;
    s32 field_0xc0;
    bool field_0xc4;
    u8 pad_0xc5[0x3];
    u64 field_0xc8;
    u64 field_0xd0;
    u64 field_0xd8;
    UnkGunSlot_ovb0049 slots[150];  // 0xe0
    bool field_0x1850;
    u8 pad_0x1851[0x3];
    f32 field_0x1854[4];
    u8 pad_0x1864[0x4];
    CFilePathStrId field_0x1868;
    s32 field_0x1870;
    s32 sound_id;  // 0x1874
    s32 field_0x1878;
};
static_assert(__builtin_offsetof(UnkGunCtor_ovb0049, field_0x1850) == 0x1850, "layout");
static_assert(__builtin_offsetof(UnkGunCtor_ovb0049, field_0x1878) == 0x1878, "layout");

static inline u64 EmptyHash() {
    return fn_7100001570(g_unk_710162e2ed, 0);
}

void fn_7100835410(UnkGunCtor_ovb0049* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a77980 + 0x10);
    new (&self->field_0x10) CStrId(g_unk_7101d470e0);
    self->field_0x18 = nullptr;
    new (&self->field_0x20) CStrId(g_unk_7101d470e0);
    fn_71002ef5bc(self->field_0x28);
    self->field_0x38 = 0;
    self->field_0x3c = -1;
    self->field_0x40 = -1;
    self->field_0x44 = 0;
    self->field_0x4c = false;
    new (&self->field_0x50) CStrId(g_unk_710162e2ed, true);
    new (&self->field_0x58) CStrId(g_unk_710162e2ed, true);
    self->field_0x60 = EmptyHash();
    self->field_0x68 = -1;
    new (&self->field_0x70) CStrId(g_unk_710162e2ed, true);
    self->field_0x78 = false;
    self->field_0x80 = EmptyHash();
    self->field_0x88 = EmptyHash();
    self->field_0x90 = EmptyHash();
    self->field_0x98 = -1;
    self->field_0xa0 = -1;
    self->field_0xa8 = -1;
    self->field_0xb0 = -1;
    self->field_0xc4 = false;
    for (UnkGunSlot_ovb0049* slot = self->slots; slot != self->slots + 150; ++slot) {
        slot->field_0x0 = 0;
        slot->field_0x8 = 0.0f;
        slot->color[0] = 1.0f;
        slot->color[1] = 1.0f;
        slot->color[2] = 1.0f;
        slot->color[3] = 1.0f;
        slot->field_0x20 = 0;
        slot->field_0x24 = false;
    }
    self->field_0x1850 = false;
    self->field_0x1854[0] = 1.0f;
    self->field_0x1854[1] = 1.0f;
    self->field_0x1854[2] = 1.0f;
    self->field_0x1854[3] = 1.0f;
    new (&self->field_0x1868) CFilePathStrId(g_unk_710162e2ed, true);
    self->field_0x1870 = 0;
    self->sound_id = -1;
    self->field_0x1878 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
}

struct UnkBillboardFlags;
struct UnkGunTextureSet_ovb0049;
UnkBillboardFlags* fn_71001b7100(UnkBillboardFlags* self, bool value);
void fn_71001b7584(char* self, u32 mode);
void fn_71001b7550(char* self, u32 value);
void fn_71001b75e8(char* self, u32 value);

struct UnkGunBillboard_ovb0049 {
    u8 pad_0x0[0x284];
    f32 field_0x284;
    f32 field_0x288;
    f32 field_0x28c;
    u8 pad_0x290[0x64];
    s32 field_0x2f4;
    u8 pad_0x2f8[0x7a];
    bool field_0x372;
};

struct UnkGunBillboardItem_ovb0049 {
    u8 pad_0x0[0x18];
    u8 toggle[0xfb];  // 0x18
    u8 dirty;         // 0x113
    u8 pad_0x114[0x14];
    UnkGunBillboard_ovb0049* billboard;  // 0x128

    // Written through a plain byte pointer: the original reloads everything after it.
    void MarkDirtyByte() {
        u8* flag = &dirty;
        *flag = 1;
    }
    // Clears the word at +0x2f4 (written by offset: the billboard pointer is reloaded after it), then
    // passes the value on (fn_71001b7550).
    void SetField1dc(u32 value) {
        __builtin_memset(&billboard->field_0x2f4, 0, sizeof(s32));
        char* raw = Raw();
        MarkDirtyByte();
        fn_71001b7550(raw, value);
    }
    char* Raw() { return reinterpret_cast<char*>(billboard); }
};

struct UnkGunBillboardKey_ovb0049 {
    s32 kind;
    u8 index;
};

struct UnkGunBillboards_ovb0049 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    u8 pad_0xc5[0x3];
    UnkGunBillboardItem_ovb0049* items[3];  // 0xc8
};

UnkGunBillboardItem_ovb0049* fn_7100836ce8(UnkGunTextureSet_ovb0049* self, const void* path,
                                           UnkGunBillboardItem_ovb0049** out, const void* arg);

void fn_71008366fc(UnkGunBillboards_ovb0049* self, const void* path, u32 columns, u32 rows, u32 mode,
                   UnkGunBillboardKey_ovb0049 key, f32 size) {
    UnkGunBillboardItem_ovb0049** slot = &self->items[key.kind == 2 ? 2 : key.index];
    if (*slot)
        return;
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkGunBillboardItem_ovb0049* item = nullptr;
    fn_7100836ce8(reinterpret_cast<UnkGunTextureSet_ovb0049*>(manager->field_0x218), path, &item,
                  &g_unk_7101d470e0);
    fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(item), 0);
    *slot = item;
    item->billboard->field_0x284 = 1.0f / (f32)columns;
    item->billboard->field_0x288 = 1.0f / (f32)rows;
    item->MarkDirtyByte();
    (*slot)->SetField1dc(0x32);
    fn_71001b7584((*slot)->Raw(), mode);
    fn_71001f3224((*slot)->toggle, 1);
    (*slot)->billboard->field_0x372 = true;
    (*slot)->MarkDirtyByte();
    (*slot)->billboard->field_0x28c = size;
    (*slot)->MarkDirtyByte();
    fn_71001b7100(reinterpret_cast<UnkBillboardFlags*>((*slot)->billboard), true);
    fn_71001b75e8((*slot)->Raw(), 5);
    self->field_0xc4 = *slot != nullptr;
}
