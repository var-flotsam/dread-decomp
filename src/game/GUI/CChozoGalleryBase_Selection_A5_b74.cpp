// GUI::CChozoGalleryBase unit (tools/tu_map.py, unit 1656): the gallery's selection. Entries (+0x258, 16 bytes:
// item and hidden flag) form a grid of `columns` (+0x24c) over `count` (+0x248); `selected` is at +0x250.
// fn_7100af8784 (after the base handler fn_71011d9a44) plays the selected item's "selection lost" animation
// through its vf_0x1e8 (hidden entries: "SecretMemory_Hidden" when the item's id (+0x130) is g_unk_717275ce28,
// else "Memory_Hidden"); the four moves (fn_7100af8850 left, fn_7100af8978 right, fn_7100af8aa4 up,
// fn_7100af8bd8 down) post the move sound (fn_7100fc9d78), deselect, move and play "OnMemorySelected";
// fn_7100af8648 reveals the entry *+0x280 and watches its "Background" child's set through +0x230.
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

struct UnkSmBase_A6_b62;
struct UnkSmArg_A6_b62;

extern CStrId g_unk_717275ce28 __attribute__((visibility("hidden")));

struct UnkGalleryItem_A5_b74 {
    virtual void vf_0x0();
    void vf_0x1e8(const char* name, bool b) {
        reinterpret_cast<void (*)(UnkGalleryItem_A5_b74*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, name, b);
    }
    u8 pad_0x8[0x130 - 0x8];
    CStrId id;
};

struct UnkGalleryEntry_A5_b74 {
    UnkGalleryItem_A5_b74* item;
    bool hidden;
};

struct UnkGalleryWatched_A5_b74 {
    u8 pad_0x0[0x30];
    char* field_0x30;  // its set at +0x18
};

struct UnkGallery_A5_b74 {
    u8 pad_0x0[0x230];
    base::UnkDeferredSet<void>* watch;
    u8 pad_0x238[0x248 - 0x238];
    u32 count;
    u32 columns;
    u32 selected;
    u8 field_0x254;
    UnkGalleryEntry_A5_b74* entries;
    u8 pad_0x260[0x280 - 0x260];
    u32* revealed;
};

struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkStateSink* g_unk_717275c380;
extern UnkEnumType g_unk_71729f0220;
extern UnkEnumType g_unk_71729f0228;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);

// The selected entry's "selection lost" animation (hidden entries show their hidden state instead).
static inline __attribute__((always_inline)) void Deselect_A5_b74(UnkGallery_A5_b74* self) {
    u32 selected = self->selected;
    UnkGalleryEntry_A5_b74& entry = self->entries[selected];
    bool hidden = entry.hidden;
    UnkGalleryItem_A5_b74* item = entry.item;
    if (hidden) {
        if (item->id.data == g_unk_717275ce28.data)
            item->vf_0x1e8("SecretMemory_Hidden", false);
        else
            item->vf_0x1e8("Memory_Hidden", false);
    } else {
        item->vf_0x1e8("OnSelectionLost", false);
    }
}

// Plays the move sound, deselects the current entry, then selects the one `next` gives.
template <typename F>
static inline __attribute__((always_inline)) void Move_A5_b74(UnkGallery_A5_b74* self, F next) {
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0220), self);
    Deselect_A5_b74(self);
    u32 selected = next(self->selected);
    self->selected = selected;
    self->entries[selected].item->vf_0x1e8("OnMemorySelected", false);
}

void fn_7100af8784(UnkGallery_A5_b74* self, UnkSmArg_A6_b62* arg) {
    fn_71011d9a44(reinterpret_cast<UnkSmBase_A6_b62*>(self), arg);
    Deselect_A5_b74(self);
}

void fn_7100af8850(UnkGallery_A5_b74* self) {
    u32 selected = self->selected;
    if (selected == 0 || selected == self->columns || selected == self->count)
        return;
    Move_A5_b74(self, [](u32 s) { return s - 1; });
}

void fn_7100af8978(UnkGallery_A5_b74* self) {
    u32 selected = self->selected;
    if (selected == self->columns - 1 || !((s32)selected < (s32)(self->count - 1)))
        return;
    Move_A5_b74(self, [](u32 s) { return s + 1; });
}

void fn_7100af8aa4(UnkGallery_A5_b74* self) {
    if ((s32)self->selected / (s32)self->columns == 0)
        return;
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0220), self);
    Deselect_A5_b74(self);
    u32 selected;
    if (self->selected == self->count)
        selected = 5;
    else
        selected = self->selected - self->columns;
    self->selected = selected;
    self->entries[selected].item->vf_0x1e8("OnMemorySelected", false);
}

void fn_7100af8bd8(UnkGallery_A5_b74* self) {
    u32 selected = self->selected;
    if (selected == self->count)
        return;
    s32 row = (s32)selected / (s32)self->columns;
    if ((row == 1) & (self->field_0x254 == 0))
        return;
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0220), self);
    Deselect_A5_b74(self);
    if ((row == 1) & (self->field_0x254 != 0))
        selected = self->count;
    else
        selected = self->columns + self->selected;
    self->selected = selected;
    self->entries[selected].item->vf_0x1e8("OnMemorySelected", false);
}

void fn_7100af8648(UnkGallery_A5_b74* self) {
    u32 index = *self->revealed;
    UnkGalleryEntry_A5_b74& entry = self->entries[index];
    entry.hidden = false;
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0228), self);
    UnkGalleryItem_A5_b74* item = entry.item;
    if (item->id.data == g_unk_717275ce28.data) {
        fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(item), true);
        entry.item->vf_0x1e8("SecretMemory_OnReveal", false);
    } else {
        item->vf_0x1e8("Memory_OnReveal", false);
    }
    UnkGuiDisplayObject* background =
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(entry.item), CStrId("Background", true));
    if (background) {
        if (self->watch) {
            fn_71004f3ea4(self->watch, &self->watch);
            self->watch = nullptr;
        }
        auto* watched = reinterpret_cast<UnkGalleryWatched_A5_b74*>(background);
        self->watch = reinterpret_cast<base::UnkDeferredSet<void>*>(watched->field_0x30 + 0x18);
        fn_71004f3cf0(self->watch, &self->watch);
    }
}
