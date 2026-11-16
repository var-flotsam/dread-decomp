// Unit 2744 (gui/scripts/guimanager.lua): a keyed array of the GUI's render layers grows to a new
// capacity; the layers are copied over and released, the keys moved. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkGuiRenderLayer;
void fn_71011f5eb4(UnkGuiRenderLayer* self);

// A render layer as the array holds it (game/GUI/UnkGuiRenderLayer.cpp releases it).
struct UnkGuiLayerSlot {
    CStrId id;
    u32 layer;
    u8 pad_0xc[0xc];
    char* item;
    bool attached;
    u8 pad_0x21[0x1f];
    u32 field_0x40;
};
static_assert(sizeof(UnkGuiLayerSlot) == 0x48, "UnkGuiLayerSlot layout");

using UnkGuiLayerSlotArrays = base::global::CRntSmallDictionary<CStrId, UnkGuiLayerSlot>;

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71011bbb24(UnkGuiLayerSlotArrays* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkGuiLayerSlot* values = static_cast<UnkGuiLayerSlot*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkGuiLayerSlot), 8, self->flags));
    CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
    if (self->values) {
        UnkGuiLayerSlot* src = self->values;
        UnkGuiLayerSlot* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkGuiLayerSlot(*src);
            fn_71011f5eb4(reinterpret_cast<UnkGuiRenderLayer*>(src));
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        CStrId* src = self->keys;
        CStrId* dst = keys;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            dst->data = src->data;
            src->data = &g_unk_7101d47108->empty;
            src->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}
