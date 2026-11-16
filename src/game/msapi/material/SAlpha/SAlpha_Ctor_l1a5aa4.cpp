// msapi::material::SAlpha's unit (tools/tu_map.py unit 229): a constructor over the base object
// (fn_7100023838): three empty string ids, an index of -1, an empty file path, and scales of one.
// Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a1e328[];

struct UnkMaterialDef_l1a5aa4 {
    void** vtable;
    UnkStringData* ids[3];  // 0x8
    s32 index;  // 0x20
    u8 pad_0x24[4];
    UnkStringData* path;  // 0x28
    u8 pad_0x30[0x28];
    f32 field_0x58;
    u8 pad_0x5c[4];
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    u32 field_0x70;
    u8 pad_0x74[4];
    u64 field_0x78;
    u64 field_0x80;
};
static_assert(__builtin_offsetof(UnkMaterialDef_l1a5aa4, field_0x80) == 0x80, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

static inline void MakeEmptyPath(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    *id = &g_unk_7101d47200->empty;
}

void fn_71001a5aa4(UnkMaterialDef_l1a5aa4* self) {
    fn_7100023838(self);
    self->vtable = g_unk_7101a1e328 + 2;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    self->index = -1;
    MakeEmptyPath(&self->path);
    self->field_0x58 = 16.0f;
    self->field_0x60 = 1.0f;
    self->field_0x64 = 1.0f;
    self->field_0x68 = 0.0f;
    self->field_0x6c = 0.0f;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
}
