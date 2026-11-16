// CBossRushSurvivalConfiguration's unit (tools/tu_map.py unit 2544): the constructor of a boss-rush level's
// record (0xa8 bytes; its destructor is fn_7100db8468): empty ids, unset times and counts, and two item lists,
// the first holding the level's starting items (floor slide, and full special energy). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// An item and its amount.
struct UnkRushLevelItem_db8700 {
    UnkPropertyEx<>* item;
    f32 amount;
};

struct UnkRushLevel_db8700 {
    CStrId id_0x0;
    CStrId id_0x8;
    CStrId id_0x10;
    CStrId id_0x18;
    u64 field_0x20;
    CStrId id_0x28;
    CStrId id_0x30;
    CStrId id_0x38;
    f32 field_0x40;
    s32 field_0x44;
    s32 field_0x48;
    s32 field_0x4c;
    f32 field_0x50;
    CRntVector<UnkRushLevelItem_db8700> items;    // 0x58
    CRntVector<UnkRushLevelItem_db8700> items_2;  // 0x80
};
static_assert(__builtin_offsetof(UnkRushLevel_db8700, items) == 0x58, "layout");
static_assert(sizeof(UnkRushLevel_db8700) == 0xa8, "layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// Appends an item, growing the array to the next multiple of the step (the elements are copied one by one).
static inline void AddItem(CRntVector<UnkRushLevelItem_db8700>& items, const UnkRushLevelItem_db8700& value) {
    u32 n = items.count;
    u32 old_capacity = items.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && items.step != 0)
        new_capacity = (n / items.step + 1) * items.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkRushLevelItem_db8700* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkRushLevelItem_db8700*>(
            items.allocator->vf_0x40((u64)new_capacity * sizeof(UnkRushLevelItem_db8700), 8, items.flags));
        if (items.data) {
            UnkRushLevelItem_db8700* dst = p;
            UnkRushLevelItem_db8700* src = items.data;
            for (u32 i = 0; i < items.count; ++i)
                *dst++ = *src++;
            items.allocator->vf_0x48(items.data);
        }
        n = items.count;
        items.data = p;
        items.capacity = new_capacity;
    } else {
        p = items.data;
    }
    p[n] = value;
    ++items.count;
}

void fn_7100db8700(UnkRushLevel_db8700* self) {
    MakeEmpty(&self->id_0x0);
    MakeEmpty(&self->id_0x8);
    MakeEmpty(&self->id_0x10);
    MakeEmpty(&self->id_0x18);
    MakeEmpty(&self->id_0x28);
    MakeEmpty(&self->id_0x30);
    MakeEmpty(&self->id_0x38);
    self->field_0x40 = -1.0f;
    self->field_0x44 = -1;
    self->field_0x48 = -1;
    self->field_0x4c = -1;
    self->field_0x50 = -1.0f;
    new (&self->items) CRntVector<UnkRushLevelItem_db8700>(0xf);
    new (&self->items_2) CRntVector<UnkRushLevelItem_db8700>(0xf);
    AddItem(self->items, {&g_unk_717275aa18, 1.0f});
    AddItem(self->items, {&g_unk_717275a158, 1000.0f});
    AddItem(self->items, {&g_unk_717275a198, 1000.0f});
}
