// From unit 50 of tools/tu_map.py (0x7100071dac-0x7100072564, no reflected class): sets an object up
// from a description: its file-path id (base/global/CFilePathStrId.h) at +0x40, a word at +0x68, a
// count at +0x54 (0x7fffffff stands for none, stored as 0), fourteen words copied to +0x48 and
// +0x74-0x90 (three of them also spread over +0x8-0x30), two flags, and the rest reset. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id with CStrId's copy assignment (base/global/CStrId.h), on the file-path pool.
struct UnkFilePathIdRef : base::global::UnkFilePathStrIdBase {
    UnkFilePathIdRef& operator=(const UnkFilePathIdRef& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkUnit50Desc {
    UnkFilePathIdRef path;  // 0x0
    u64 field_0x8;
    s32 count;  // 0x10
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    bool field_0x40;
    bool field_0x41;
};

struct UnkUnit50Object {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u8 pad_0x20[8];
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u8 pad_0x34[0xc];
    UnkFilePathIdRef path;  // 0x40
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    s32 count;  // 0x54
    u8 pad_0x58[8];
    u64 field_0x60;
    u64 field_0x68;
    u8 pad_0x70[4];
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    f32 field_0x94;
    u32 field_0x98;
    bool field_0x9c;
    bool field_0x9d;
    bool field_0x9e;
    bool field_0x9f;
    bool field_0xa0;
    bool field_0xa1;
    bool field_0xa2;
    bool field_0xa3;
};
static_assert(__builtin_offsetof(UnkUnit50Object, field_0xa3) == 0xa3, "UnkUnit50Object layout");

void fn_7100071ef0(UnkUnit50Object* self, const UnkUnit50Desc* desc) {
    self->path = desc->path;
    self->field_0x68 = desc->field_0x8;
    self->count = desc->count == 0x7fffffff ? 0 : desc->count;
    self->field_0x48 = desc->field_0x14;
    self->field_0x4c = desc->field_0x18;
    self->field_0x50 = desc->field_0x1c;
    self->field_0x74 = desc->field_0x20;
    self->field_0x78 = desc->field_0x24;
    self->field_0x7c = desc->field_0x28;
    self->field_0x80 = desc->field_0x2c;
    self->field_0x84 = desc->field_0x30;
    self->field_0x88 = desc->field_0x34;
    self->field_0x8c = desc->field_0x38;
    self->field_0x90 = desc->field_0x3c;
    self->field_0xa2 = desc->field_0x40;
    self->field_0xa1 = desc->field_0x41;
    self->field_0x94 = -1.0f;
    self->field_0x98 = 0;
    self->field_0x9c = true;
    self->field_0x9d = false;
    self->field_0x9e = false;
    self->field_0x9f = false;
    self->field_0xa0 = false;
    self->field_0xa3 = false;
    self->field_0x60 = 0;
    self->field_0x0 = 0;
    self->field_0x8 = self->field_0x7c;
    self->field_0xc = self->field_0x88;
    self->field_0x18 = self->field_0x80;
    self->field_0x1c = self->field_0x80;
    self->field_0x10 = 0;
    self->field_0x14 = self->field_0x80;
    self->field_0x30 = self->field_0x84;
    self->field_0x2c = self->field_0x84;
    self->field_0x28 = self->field_0x84;
}
