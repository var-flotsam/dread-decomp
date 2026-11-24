#pragma once

#include "base/global/CRntVector.h"
#include "game/SLogicPathNode.h"
#include "game/UnkPathPointEntry.h"

struct UnkPathPointEntryArray {
    UnkPathPointEntry* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    void Reallocate(u32 new_capacity) {
        UnkPathPointEntry* p = static_cast<UnkPathPointEntry*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkPathPointEntry), 8, flags));
        if (data) {
            UnkPathPointEntry* src = data;
            UnkPathPointEntry* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                dst->vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
                dst->field_0x8 = src->field_0x8;
                dst->field_0xc = src->field_0xc;
                dst->tail.field_0x0 = src->tail.field_0x0;
                dst->tail.field_0x4 = src->tail.field_0x4;
                dst->field_0x18 = src->field_0x18;
                src->Destroy();
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};
