// CMinimapData's unit (tools/tu_map.py unit 2563, no reliable name): sets the record at an index in a
// base::global::CRntVector of 0x138-byte area records (destructor fn_7100e4e310 and move constructor fn_7100e51078,
// both out of line): the array grows to the index (moving the records over) and the record is copy-assigned
// member by member. Names and layouts are placeholders (siblings: src/game/CMinimapData/).
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CMinimapData.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

struct UnkPolyVector;
struct UnkArraysRecord;
void fn_7100e62e24(void*, u32);

// A moving magnet surface (include/game/CMinimapData.h).
using UnkRecord138_ovb0222 = CMinimapData::SMovingMagnetSurfaceData;

void fn_7100e62084(base::global::CRntVector<UnkRecord138_ovb0222>* self, const u32* indexp, const UnkRecord138_ovb0222* item) {
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
                        new (dst) UnkRecord138_ovb0222(static_cast<UnkRecord138_ovb0222&&>(*src));
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
    *reinterpret_cast<CStrId*>(&dst->sMoveEntityID) = *reinterpret_cast<const CStrId*>(&item->sMoveEntityID);
    dst->iEntityType = item->iEntityType;
    fn_7100e4e898(reinterpret_cast<UnkPolyVector*>(&dst->oPathLine.field_0x8),
                  reinterpret_cast<const UnkPolyVector*>(&item->oPathLine.field_0x8));
    dst->oPathLine.field_0x30 = item->oPathLine.field_0x30;
    dst->field_0x60 = item->field_0x60;
    dst->field_0x64 = item->field_0x64;
    dst->field_0x68 = item->field_0x68;
    dst->field_0x6c = item->field_0x6c;
    dst->field_0x70 = item->field_0x70;
    fn_7100e4f444(reinterpret_cast<UnkArraysRecord*>(&dst->field_0x78),
                  reinterpret_cast<const UnkArraysRecord*>(&item->field_0x78));
    fn_7100e4f444(reinterpret_cast<UnkArraysRecord*>(&dst->field_0xd8),
                  reinterpret_cast<const UnkArraysRecord*>(&item->field_0xd8));
}
