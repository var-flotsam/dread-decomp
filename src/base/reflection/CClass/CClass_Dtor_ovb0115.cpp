// base::reflection::CClass's unit (unit 72 of tools/tu_map.py): CClass's destructor. The function names
// (+0x118, string ids released in two steps) and the functions (+0x110, 0xb8 bytes each, destroyed through
// their vtable) are freed, then the member fields (+0xe8, keys +0xf0; records destroyed by fn_71000e70c4),
// each array through its allocator; then the CType part (fn_710009cc04). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CType.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a1b2e0[];  // CClass's vtable

struct UnkRecordB0_b44a1;

struct UnkClassFunction_ovb0115 {
    virtual void vf_0x0();
    u8 pad_0x8[0xb0];
};

struct UnkClassField_ovb0115 {
    u8 pad_0x0[0xb0];
};

struct UnkClassDesc_ovb0115 {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkClassField_ovb0115* fields;  // 0xe8
    u64* field_keys;                // 0xf0
    u32 field_count;                // 0xf8
    u8 pad_0xfc[0xc];
    base::UnkAllocator* field_allocator;  // 0x108
    UnkClassFunction_ovb0115* functions;  // 0x110
    UnkStringData** function_names;       // 0x118
    u32 function_count;                   // 0x120
    u8 pad_0x124[0xc];
    base::UnkAllocator* function_allocator;  // 0x130
};

// CClass's destructor: frees the functions and their names, then the fields, then the CType part.
void fn_7100096604(UnkClassDesc_ovb0115* self) {
    self->vtable = &g_unk_7101a1b2e0[2];
    if (self->function_names && self->function_count) {
        for (u64 i = 0; i < self->function_count; ++i) {
            UnkStringPool* pool = g_unk_7101d47108;
            if (!pool)
                continue;
            UnkStringData** names = self->function_names;
            UnkStringData* data = names[i];
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (names[i]->ref_count == 0) {
                    fn_71000803a4(pool, names[i]);
                    pool = g_unk_7101d47108;
                }
            }
            names[i] = &pool->empty;
        }
    }
    if (self->functions) {
        for (u32 i = 0; i < self->function_count; ++i)
            self->functions[i].vf_0x0();
    }
    self->function_count = 0;
    if (self->function_names)
        self->function_allocator->vf_0x48(self->function_names);
    if (self->functions)
        self->function_allocator->vf_0x48(self->functions);
    if (self->fields) {
        for (u32 i = 0; i < self->field_count; ++i)
            fn_71000e70c4(reinterpret_cast<UnkRecordB0_b44a1*>(&self->fields[i]));
    }
    self->field_count = 0;
    if (self->field_keys)
        self->field_allocator->vf_0x48(self->field_keys);
    if (self->fields)
        self->field_allocator->vf_0x48(self->fields);
    fn_710009cc04(reinterpret_cast<char*>(self));
}
