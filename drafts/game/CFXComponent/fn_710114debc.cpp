// CFXComponent unit (tools/tu_map.py unit 2712): the component's destructor. After fn_710114e130, the id
// array at +0x148 is deleted, the id list at +0x178 destroyed, the ids released, the value arrays at +0x78
// cleared and freed and the keyed ids at +0x50 destroyed, then the base. Names and layouts are placeholders.
#include "base/UnkKeyedStrIds.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkIdNode_0x18 {
    UnkStrIdTwoStep id;
    u64 field_0x8;
    UnkIdNode_0x18* next;
};

extern void* g_unk_7101cb38a8[];

// A list of id nodes (as fn_7101157b48's owners hold one) freed through its allocator; this component's
// copy frees the nodes before it restores the vtable and empties the list.
struct UnkFXIdList_l14debc {
    void** vtable;
    u32 count;
    UnkIdNode_0x18* head;
    UnkIdNode_0x18* tail;
    base::UnkAllocator* allocator;

    ~UnkFXIdList_l14debc() {
        for (UnkIdNode_0x18* n = head; n;) {
            UnkIdNode_0x18* next = n->next;
            n->id.~UnkStrIdTwoStep();
            allocator->vf_0x48(n);
            n = next;
        }
        vtable = &g_unk_7101cb38a8[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkFXIdValuesArrays;
void fn_7101157c08(UnkFXIdValuesArrays* self);
void fn_710114e4d8(UnkKeyedStrIds* self);

// The value arrays (fn_7101157c08 clears them) and the allocator that frees them.
struct UnkFXValueArrays_l14debc {
    void* records;
    void* ids;
    u32 count;
    u32 capacity;
    u8 pad_0x18[8];
    base::UnkAllocator* allocator;  // 0x20
};

extern void* g_unk_7101cb3728[];  // the component's vtable

struct UnkFXComponent_l14debc {
    void** vtable;
    u8 base_0x8[0x48];
    UnkKeyedStrIds keyed_ids;          // 0x50
    UnkFXValueArrays_l14debc values;   // 0x78
    u8 pad_0xa0[8];
    CFilePathStrId field_0xa8;
    u8 pad_0xb0[8];
    CStrId field_0xb8;
    u8 pad_0xc0[0x30];
    CFilePathStrId field_0xf0;
    u8 pad_0xf8[0x50];
    CStrId* ids;  // 0x148, from new[]
    u8 pad_0x150[0x28];
    UnkFXIdList_l14debc id_list;  // 0x178
};
static_assert(__builtin_offsetof(UnkFXComponent_l14debc, ids) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkFXComponent_l14debc, id_list) == 0x178, "layout");

void fn_710114e130(UnkFXComponent_l14debc* self);

void fn_710114debc(UnkFXComponent_l14debc* self) {
    self->vtable = g_unk_7101cb3728 + 2;
    fn_710114e130(self);
    if (self->ids) {
        delete[] self->ids;
        self->ids = nullptr;
    }
    self->id_list.~UnkFXIdList_l14debc();
    self->field_0xf0.~CFilePathStrId();
    self->field_0xb8.~CStrId();
    self->field_0xa8.~CFilePathStrId();
    UnkFXValueArrays_l14debc* values = &self->values;
    fn_7101157c08(reinterpret_cast<UnkFXIdValuesArrays*>(values));
    if (values->ids)
        values->allocator->vf_0x48(values->ids);
    if (values->records)
        values->allocator->vf_0x48(values->records);
    fn_710114e4d8(&self->keyed_ids);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
