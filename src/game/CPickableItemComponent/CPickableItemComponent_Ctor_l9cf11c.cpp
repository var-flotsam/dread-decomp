// Unit 1493 (CPickableItemComponent): the component's constructor: the base component's, then empty
// string ids and strings, a small dictionary, a string-id list, a property member, a callback bound to
// the component and six arrays. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CPickableItemComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPickableComponent;
void fn_71009ccf7c(UnkPickableComponent* self);  // CPickableComponent's constructor
void fn_71009d3cf8(char* self);   // base::refl_StrIdListCtor<..>, instantiated in src/clones
void fn_71009d42cc();

// Three arrays and a flag.
struct UnkListSet_l9cf11c {
    CRntVector<void*> lists[3];
    bool flag;
};

static inline void Construct(UnkListSet_l9cf11c* set) {
    for (CRntVector<void*>& list : set->lists)
        new (&list) CRntVector<void*>(0xf);
    set->flag = false;
}

struct UnkPickableItem_l9cf11c {
    void** vtable;
    u8 pad_0x8[0x90];
    UnkStringData* field_0x98;
    UnkStringData* field_0xa0;
    f32 field_0xa8;
    f32 field_0xac;
    u32 kind;  // 0xb0
    u8 pad_0xb4[4];
    CStrId field_0xb8;
    CRntString field_0xc0;
    CRntString field_0xf0;
    UnkStringData* start_point;  // 0x120
    bool field_0x128;
    bool field_0x129;
    bool field_0x12a;
    u8 pad_0x12b;
    f32 time_to_can_be_picked;  // 0x12c
    u64 field_0x130;
    CRntSmallDictionary<void*, void*> field_0x138;
    bool field_0x160;
    u8 pad_0x161[7];
    u8 field_0x168[0x38];  // string-id list
    u8 field_0x1a0[0x10];  // property member
    bool field_0x1b0;
    bool field_0x1b1;
    bool field_0x1b2[6];
    u64 field_0x1b8;
    UnkPickableItem_l9cf11c* owner;  // 0x1c0
    void (*callback)();               // 0x1c8
    UnkListSet_l9cf11c list_sets[2];  // 0x1d0
};
static_assert(__builtin_offsetof(UnkPickableItem_l9cf11c, field_0x160) == 0x160, "layout");
static_assert(__builtin_offsetof(UnkPickableItem_l9cf11c, list_sets) == 0x1d0, "layout");
static_assert(sizeof(UnkPickableItem_l9cf11c) == 0x2d0, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71009cf11c(UnkPickableItem_l9cf11c* self) {
    fn_71009ccf7c(reinterpret_cast<UnkPickableComponent*>(self));
    self->vtable = g_unk_7101a89358 + 2;
    MakeEmpty(&self->field_0x98);
    MakeEmpty(&self->field_0xa0);
    self->field_0xa8 = 0.0f;
    self->field_0xac = -1.0f;
    self->kind = 0;
    new (&self->field_0xb8) CStrId(g_unk_710162e2ed, true);
    new (&self->field_0xc0) CRntString(g_unk_710162e2ed);
    new (&self->field_0xf0) CRntString(g_unk_710162e2ed);
    MakeEmpty(&self->start_point);
    self->field_0x128 = true;
    self->field_0x129 = true;
    self->field_0x12a = false;
    self->time_to_can_be_picked = 0.0f;
    self->field_0x130 = 0;
    new (&self->field_0x138) CRntSmallDictionary<void*, void*>(2);
    self->field_0x160 = false;
    fn_71009d3cf8(reinterpret_cast<char*>(self->field_0x168));
    fn_71002ef5bc(self->field_0x1a0);
    self->field_0x1b0 = false;
    self->field_0x1b1 = true;
    for (bool& flag : self->field_0x1b2)
        flag = false;
    self->field_0x1b8 = 0;
    self->owner = self;
    self->callback = fn_71009d42cc;
    for (UnkListSet_l9cf11c& set : self->list_sets)
        Construct(&set);
}
