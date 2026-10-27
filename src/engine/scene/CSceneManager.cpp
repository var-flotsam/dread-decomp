// engine::scene::CSceneManager (0xff0 bytes; the name comes from its reflection initialiser,
// fn_71001fe540): the scene items by id (an array of pointers whose accessor, fn_71001fc54c, is a
// shared out-of-line copy in src/clones/), a stack of up to 100 entry arrays at +0x48 with its depth
// at +0xfe8, and the object at +0x28 that draws them. The members are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

// A scene item (the arrays hold pointers to them): its id first.
struct UnkSceneItem {
    s32 id;
    u8 pad_0x4[0xd4];
    f32 field_0xd8;
    f32 field_0xdc;
    f32 field_0xe0;
};

struct UnkClone_fn_71001fc54c;
using UnkSceneSlot = UnkCloneElem<UnkClone_fn_71001fc54c, 8>;
extern template UnkSceneSlot& base::UnkVector<UnkSceneSlot>::GrowAt(u32);

// A rectangle of the display and a depth range.
struct UnkSceneRect {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    f32 field_0x10;
    f32 field_0x14;
};

// An entry of the stacked arrays (0x28 bytes): the id of the item it is for, first.
struct UnkSceneEntry {
    u64 item_id;
    s32 field_0x8;
    UnkSceneRect rect;
    bool field_0x24;
};
static_assert(sizeof(UnkSceneEntry) == 0x28, "UnkSceneEntry placeholder layout");

// What draws the scene (at +0x28): vf_0x20 draws a rectangle.
struct UnkSceneColor {
    f32 r;
    f32 g;
    f32 b;

    UnkSceneColor(f32 r, f32 g, f32 b) : r(r), g(g), b(b) {}
};
struct UnkSceneDrawer {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(void* a, void* b, const UnkSceneRect& rect, bool c, bool d, const UnkSceneColor& color, f32 e);
};

// One of the stacked arrays.
struct UnkSceneLayer : base::UnkVector<UnkSceneEntry> {
    UnkSceneLayer() : base::UnkVector<UnkSceneEntry>(0xf) {}
};

struct UnkSceneManager {
    base::UnkVector<UnkSceneSlot> items{0x39};
    UnkSceneDrawer* field_0x28 = nullptr;
    bool field_0x30 = true;
    u8 pad_0x31[0x7];
    u64 field_0x38 = 0;
    u64 field_0x40 = 0;
    UnkSceneLayer layers[100];
    u32 layer_count = 0;
};
static_assert(__builtin_offsetof(UnkSceneManager, layers) == 0x48, "UnkSceneManager placeholder layout");
static_assert(__builtin_offsetof(UnkSceneManager, layer_count) == 0xfe8, "UnkSceneManager placeholder layout");

static inline UnkSceneItem*& ItemAt(UnkSceneManager* self, u32 index) {
    return *reinterpret_cast<UnkSceneItem**>(self->items.GrowAt(index).data);
}

// Sets the three floats at +0xd8 of the item `id` names; false when there is none.
static inline UnkSceneItem* Find(UnkSceneManager* self, s32 id) {
    for (UnkSceneSlot *it = self->items.data, *end = it + self->items.count; it != end; ++it) {
        UnkSceneItem* item = *reinterpret_cast<UnkSceneItem**>(it->data);
        if (item->id == id)
            return item;
    }
    return nullptr;
}

bool fn_71001fc844(UnkSceneManager* self, s32 id, f32 x, f32 y, f32 z) {
    UnkSceneItem* found = Find(self, id);
    if (!found)
        return false;
    found->field_0xd8 = x;
    found->field_0xdc = y;
    found->field_0xe0 = z;
    return true;
}

// The flag at +0x142 (the declaration is src/game_s1/fn_71001fcd4c.cpp's, over its item view).
struct UnkItem;
bool fn_7100200144(UnkItem* item);
void fn_7100200160(UnkSceneItem* item, f32 dt);

// Advances every item whose flag at +0x142 is set by `dt` (nothing below 1e-6).
void fn_71001fcca0(UnkSceneManager* self, f32 dt) {
    if (dt < 1e-6f)
        return;
    s32 n = self->items.count;
    for (s32 i = 0; i < n; ++i) {
        if (ItemAt(self, i) && fn_7100200144(reinterpret_cast<UnkItem*>(ItemAt(self, i))))
            fn_7100200160(ItemAt(self, i), dt);
    }
}

void fn_71001cb588(UnkGlobal_710b372a98* self);
UnkSceneEntry* fn_71001fca5c(base::UnkVector<UnkSceneEntry>* self, u32 index);  // grows to `index` like GrowAt
bool fn_7100200158(UnkSceneItem* item);  // the flag at +0x141
void fn_710020017c(UnkSceneItem* item, const UnkSceneEntry* entry);
u32 fn_7100132f80();  // the display width
u32 fn_7100132f90();  // the display height

// Hands each entry of the top array to its item (when the item's flag at +0x141 is set), then has
// the drawer, when there is one and it is enabled, cover the display in white.
void fn_71001fc940(UnkSceneManager* self) {
    fn_71001cb588(g_unk_710b372a98);
    base::UnkVector<UnkSceneEntry>* top = &self->layers[self->layer_count - 1];
    u32 n = top->count;
    for (u32 i = 0; i != n; ++i) {
        UnkSceneEntry* entry = fn_71001fca5c(top, i);
        UnkSceneItem* item = ItemAt(self, entry->item_id);
        if (item && fn_7100200158(item))
            fn_710020017c(item, entry);
    }
    if (self->field_0x28 && self->field_0x30) {
        UnkSceneRect rect;
        rect.x = 0;
        rect.y = 0;
        rect.width = fn_7100132f80();
        rect.height = fn_7100132f90();
        rect.field_0x10 = 0.0f;
        rect.field_0x14 = 1.0f;
        self->field_0x28->vf_0x20(nullptr, nullptr, rect, true, true, UnkSceneColor(1.0f, 1.0f, 1.0f), 0.0f);
    }
}

// The kinds of layer the manager knows (the arguments are placeholders).
struct UnkSceneLayerDesc {
    u32 field_0x0;
    u32 field_0x4;
    u32 index;
    u32 mask;
    bool field_0x10;
    bool field_0x11;
    bool field_0x12;
    bool field_0x13;
    bool field_0x14;
    bool field_0x15;
    bool field_0x16;
    bool field_0x17;
};
void fn_71001fc1c4(UnkSceneManager* self, const UnkSceneLayerDesc* desc);  // registers a layer kind
// Creates the drawer (src/base/UnkStruct3088Factory.cpp).
void* fn_7100150ff0(u32 id, bool small, bool large, bool none);
void fn_71001fe390(base::UnkVector<UnkSceneEntry>* self, const UnkSceneEntry* entry);  // PushBack
// Copy-assigns an entry array.
void fn_71001fdacc(base::UnkVector<UnkSceneEntry>* self, const base::UnkVector<UnkSceneEntry>* other);
extern UnkSceneManager* g_unk_716e598008 __attribute__((visibility("hidden")));  // the instance

// Registers a layer kind. The fields are set last to first (the closest order to the original's
// stores).
static inline void AddLayer(UnkSceneManager* self, u32 a, u32 b, u32 index, u32 mask, bool e0, bool e1, bool e2,
                            bool e3, bool e4, bool e5, bool e6, bool e7) {
    UnkSceneLayerDesc desc;
    desc.field_0x17 = e7;
    desc.field_0x16 = e6;
    desc.field_0x15 = e5;
    desc.field_0x14 = e4;
    desc.field_0x13 = e3;
    desc.field_0x12 = e2;
    desc.field_0x11 = e1;
    desc.field_0x10 = e0;
    desc.mask = mask;
    desc.index = index;
    desc.field_0x4 = b;
    desc.field_0x0 = a;
    fn_71001fc1c4(self, &desc);
}

// The constructor: the arrays, the drawer and the seven layer kinds (the first six take `flag`);
// then the manager becomes the instance and its first array gets the entries 0, 6 and 2, each over
// the whole display.
// NON_MATCHING(minor): the stores that fill the layer descriptors and the entry (all to the stack,
// before the call that reads them) come in another order, and the seventh descriptor's eight flag
// bytes are one 8-byte store (paired with the mask and index) where the original writes them as
// 4-, 1-, 2- and 1-byte stores; the same bytes reach every call (728 B original, 716 B here).
void fn_71001fbeec(UnkSceneManager* self, bool flag) {
    new (self) UnkSceneManager;
    self->field_0x28 = static_cast<UnkSceneDrawer*>(fn_7100150ff0(0, false, false, true));
    AddLayer(self, 0, 1, 0, 0x800, flag, true, true, true, true, true, true, true);
    AddLayer(self, 1, 3, 1, 0x800, flag, false, false, true, true, false, false, false);
    AddLayer(self, 0, 3, 2, 0x100, flag, false, false, true, true, false, false, false);
    AddLayer(self, 0, 4, 3, 0x80, flag, false, false, false, false, false, true, true);
    AddLayer(self, 1, 4, 4, 0x40, flag, false, false, false, false, false, true, true);
    AddLayer(self, 0, 2, 5, 0x800, flag, true, true, true, true, true, true, false);
    AddLayer(self, 0, 5, 6, 0x400, false, false, false, false, false, false, true, true);
    g_unk_716e598008 = self;
    u32 width = fn_7100132f80();
    u32 height = fn_7100132f90();
    base::UnkVector<UnkSceneEntry> entries(0xf);
    UnkSceneEntry entry;
    entry.item_id = 0;
    entry.field_0x8 = -1;
    entry.rect.x = 0;
    entry.rect.y = 0;
    entry.rect.width = width;
    entry.rect.height = height;
    entry.rect.field_0x10 = 0.0f;
    entry.rect.field_0x14 = 1.0f;
    entry.field_0x24 = false;
    fn_71001fe390(&entries, &entry);
    entry.item_id = 6;
    fn_71001fe390(&entries, &entry);
    entry.item_id = 2;
    fn_71001fe390(&entries, &entry);
    u32 depth = self->layer_count;
    self->layer_count = depth + 1;
    if (depth + 1 < 100)
        fn_71001fdacc(&self->layers[depth], &entries);
    entries.Free();
}
