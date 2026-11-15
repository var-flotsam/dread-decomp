// The CMinimapData unit (unit 2563): moves a layer into the entry that a keyed array holds under a string id,
// if there is one. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkLayerAssign {
    base::global::CRntVector<void*> list0;
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    f32 min0[2];
    f32 max0[2];
    u32 field_0x40;
    base::global::CRntVector<void*> list1;
    base::global::CRntVector<void*> list2;
    u32 field_0x98, field_0x9c, field_0xa0, field_0xa4;
};
static_assert(sizeof(UnkLayerAssign) == 0xa8, "layout");

struct UnkKeyedLayers {
    UnkLayerAssign* items;
    CStrId* keys;  // +0x8
    u32 count;     // +0x10
};

void fn_7100e5ef38(UnkKeyedLayers* self, const CStrId* key, UnkLayerAssign* layer) {
    u64 n = self->count;
    u64 i = 0;
    for (; i < n; ++i)
        if (self->keys[i].data == key->data)
            break;
    UnkLayerAssign* found = &self->items[i];
    if (found == &self->items[self->count])
        return;
    found->field_0x28 = layer->field_0x28;
    found->field_0x2c = layer->field_0x2c;
    found->field_0x2d = true;
    found->list0 = static_cast<base::global::CRntVector<void*>&&>(layer->list0);
    found->field_0x40 = layer->field_0x40;
    found->list1 = static_cast<base::global::CRntVector<void*>&&>(layer->list1);
    found->list2 = static_cast<base::global::CRntVector<void*>&&>(layer->list2);
    found->field_0x98 = layer->field_0x98;
    found->field_0x9c = layer->field_0x9c;
    found->field_0xa0 = layer->field_0xa0;
    found->field_0xa4 = layer->field_0xa4;
}
