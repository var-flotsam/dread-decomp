// CMinimapData's unit (tools/tu_map.py unit 2563, no reliable name): sets the record at an index in a
// base::UnkVector of 0x138-byte area records (destructor fn_7100e4e310 and move constructor fn_7100e51078,
// both out of line): the array grows to the index (moving the records over) and the record is copy-assigned
// member by member. Names and layouts are placeholders (siblings: src/game/CMinimapData/).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

struct UnkPolyVector;
struct UnkArraysRecord;
void fn_7100e62e24(void*, u32);

struct alignas(8) UnkRecord138_ovb0222 {
    u8 name[8];  // a string id
    u32 field_0x8;
    u8 unk_0xc[4];
    u8 field_0x10[0x8];
    u8 field_0x18[0x28];  // an array of virtual elements, assigned out of line
    u8 field_0x40;
    u8 unk_0x41[0x1f];
    u8 field_0x60;
    u8 unk_0x61[3];
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u8 unk_0x74[4];
    u8 field_0x78[0x60];  // two paths, assigned out of line
    u8 field_0xd8[0x60];
    ~UnkRecord138_ovb0222() { fn_7100e4e310(reinterpret_cast<char*>(this)); }
};
static_assert(sizeof(UnkRecord138_ovb0222) == 0x138, "layout");

void fn_7100e62084(base::UnkVector<UnkRecord138_ovb0222>* self, const u32* indexp, const UnkRecord138_ovb0222* item) {
    u32 index = *indexp;
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                UnkRecord138_ovb0222* p = static_cast<UnkRecord138_ovb0222*>(
                    self->allocator->vf_0x40((u64)grown * sizeof(UnkRecord138_ovb0222), 8, self->flags));
                UnkRecord138_ovb0222* src = self->data;
                if (src) {
                    UnkRecord138_ovb0222* dst = p;
                    for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                        fn_7100e51078(dst, src);
                        src->~UnkRecord138_ovb0222();
                    }
                    self->allocator->vf_0x48(self->data);
                }
                self->data = p;
                self->capacity = grown;
            }
        }
        fn_7100e62e24(self, index + 1);
    }
    UnkRecord138_ovb0222* dst = &self->data[index];
    *reinterpret_cast<CStrId*>(dst->name) = *reinterpret_cast<const CStrId*>(item->name);
    dst->field_0x8 = item->field_0x8;
    fn_7100e4e898(reinterpret_cast<UnkPolyVector*>(dst->field_0x18),
                  reinterpret_cast<const UnkPolyVector*>(item->field_0x18));
    dst->field_0x40 = item->field_0x40;
    dst->field_0x60 = item->field_0x60;
    dst->field_0x64 = item->field_0x64;
    dst->field_0x68 = item->field_0x68;
    dst->field_0x6c = item->field_0x6c;
    dst->field_0x70 = item->field_0x70;
    fn_7100e4f444(reinterpret_cast<UnkArraysRecord*>(dst->field_0x78),
                  reinterpret_cast<const UnkArraysRecord*>(item->field_0x78));
    fn_7100e4f444(reinterpret_cast<UnkArraysRecord*>(dst->field_0xd8),
                  reinterpret_cast<const UnkArraysRecord*>(item->field_0xd8));
}
