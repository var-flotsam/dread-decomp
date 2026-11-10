// CCharClassSetRelativeCameraAnimationBaseEvent's unit (tools/tu_map.py): the event record's copy assignment:
// a u32, a file path id (+0x10), 12 bytes, a string id, a byte, two string ids, 21 plain bytes, a string id
// and a u32. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkCameraEvent_b0141 {
    u8 pad_0x0[8];
    u32 field_0x8;
    CFilePathStrId path;  // 0x10
    u64 field_0x18;
    u32 field_0x20;
    u8 unk_0x24[4];
    CStrId id_0x28;
    u8 field_0x30;
    u8 unk_0x31[7];
    CStrId id_0x38;
    CStrId id_0x40;
    u8 data_0x48[0x15];
    u8 unk_0x5d[3];
    CStrId id_0x60;
    u32 field_0x68;
};

UnkCameraEvent_b0141* fn_7100bdb724(UnkCameraEvent_b0141* self, const UnkCameraEvent_b0141* other_) {
    const UnkCameraEvent_b0141& other = *other_;
    self->field_0x8 = other.field_0x8;
    if (self->path.data != other.path.data) {
        base::global::UnkStringData* d = other.path.data;
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (self->path.data != &pool->empty) {
            --self->path.data->ref_count;
            if (self->path.data->ref_count == 0)
                fn_71000803a4(pool, self->path.data);
        }
        self->path.data = d;
    }
    u64 v18 = other.field_0x18;
    u32 v20 = other.field_0x20;
    self->field_0x20 = v20;
    self->field_0x18 = v18;
    self->id_0x28 = other.id_0x28;
    self->field_0x30 = other.field_0x30;
    self->id_0x38 = other.id_0x38;
    self->id_0x40 = other.id_0x40;
    __builtin_memcpy(self->data_0x48, other.data_0x48, sizeof(self->data_0x48));
    self->id_0x60 = other.id_0x60;
    self->field_0x68 = other.field_0x68;
    return self;
}
