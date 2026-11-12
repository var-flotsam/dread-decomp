// GUI::CChozoGalleryBase unit (tools/tu_map.py, unit 1656), continued from
// src/game/GUI/CChozoGalleryBase_Selection_A5_b74.cpp (same layout). fn_7100af820c is the constructor: base
// constructor fn_71011d8d24, vtable g_unk_7101a98af8, the watch callback bound to fn_7100af9cd0, the entry
// array (+0x258) and the revealed-index array (+0x280). The rest: the event handler (fn_7100af8450), confirm
// (fn_7100af8cf4), next and previous visible entry (fn_7100af9108, fn_7100af94e4) and the reveal queue's step
// (fn_7100af999c). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

void fn_7100af9cd0();
extern void* g_unk_7101a98af8[];

struct UnkGalleryItem_A5_b75 {
    virtual void vf_0x0();
    void vf_0x1e8(const char* name, bool b) {
        reinterpret_cast<void (*)(UnkGalleryItem_A5_b75*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, name, b);
    }
    u8 pad_0x8[0x130 - 0x8];
    CStrId id;
};

struct UnkGalleryEntry_A5_b75 {
    UnkGalleryItem_A5_b75* item;
    bool hidden;
};

struct UnkGallery_A5_b75 {
    void** vtable;
    u8 pad_0x8[0x230 - 0x8];
    base::UnkDeferredSet<void>* watch;
    void* watch_object;
    void (*watch_fn)();
    u32 count;
    u32 columns;
    u32 selected;
    u8 field_0x254;
    u8 field_0x255;
    base::UnkVector<UnkGalleryEntry_A5_b75> entries;
    base::UnkVector<u32> revealed;
    f32 field_0x2a0;
    UnkReflObject* inspector;  // GUI::CArchivesInspectorComposition, found through the main menu
};

void fn_7100af820c(UnkGallery_A5_b75* self) {
    fn_71011d8d24(self);
    self->vtable = &g_unk_7101a98af8[2];
    self->watch = nullptr;
    self->watch_object = self;
    self->watch_fn = fn_7100af9cd0;
    self->count = 0;
    self->columns = 0;
    self->field_0x254 = 0;
    self->field_0x255 = 0;
    new (&self->entries) base::UnkVector<UnkGalleryEntry_A5_b75>(0xf);
    new (&self->revealed) base::UnkVector<u32>(0xf);
    self->field_0x2a0 = -1.0f;
    self->inspector = nullptr;
}

// fn_7100af999c: the revealed entry loses its selection and leaves the queue (+0x280); the next one is revealed
// (fn_7100af8648). With the queue empty the gallery closes (vf_0x270) and, for the archives composition, the main
// menu either shows its rewards (state 0xb and a render-pass parameter) or drops the gallery's key and selects the
// first entry.
struct UnkMenuPart_A5_b75 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};
struct UnkMainMenu_A5_b75 {
    u8 pad_0x0[0x528];
    u8 part[8];  // UnkMenuPart_A5_b75
    UnkReflObject* rewards;
    u8 pad_0x538[0x550 - 0x538];
    UnkReflObject* inspector;
};
struct UnkRewards_A5_b75 {
    u8 pad_0x0[0x288];
    u32 count;
};

UnkReflObject* fn_7100e033a8(UnkGameManager* manager);
void fn_7100b35704(UnkReflObject* menu, u32 value, bool flag);
struct UnkGallery_A5_b74;  // the layout in CChozoGalleryBase_Selection_A5_b74.cpp
void fn_7100af8648(UnkGallery_A5_b74* self);
extern CStrId g_unk_717275ce20 __attribute__((visibility("hidden")));
extern CStrId g_unk_717275ce28 __attribute__((visibility("hidden")));

bool fn_7100af999c(UnkGallery_A5_b75* self) {
    u32 index = *self->revealed.data;
    self->entries.data[index].item->vf_0x1e8("OnSelectionLost", false);
    if (self->revealed.count != 0) {
        __builtin_memcpy(self->revealed.data, self->revealed.data + 1, (u64)(self->revealed.count - 1) * 4);
        if (--self->revealed.count != 0) {
            fn_7100af8648(reinterpret_cast<UnkGallery_A5_b74*>(self));
            return true;
        }
    }
    reinterpret_cast<void (*)(UnkGallery_A5_b75*)>(self->vtable[0x270 / 8])(self);
    UnkReflObject* obj = reinterpret_cast<UnkReflObject*>(self);
    if (fn_710009d50c(obj->vf_0x0(), fn_7100af6844())) {
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        if (menu)
            menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
        UnkMainMenu_A5_b75* m = reinterpret_cast<UnkMainMenu_A5_b75*>(menu);
        reinterpret_cast<UnkMenuPart_A5_b75*>(m->part)->vf_0x10();
        UnkReflObject* rewards = m->rewards;
        auto* r = reinterpret_cast<UnkRewards_A5_b75*>(
            fn_710009d50c(rewards->vf_0x0(), fn_7100b1c740()) ? rewards : nullptr);
        if (r->count != 0) {
            fn_7100b35704(menu, 0xb, true);
            f32 one = 1.0f;
            fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654ull, &one, 2);
        } else {
            fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_717275ce20);
            self->entries.data[0].item->vf_0x1e8("OnMemorySelected", false);
        }
    }
    return true;
}

// fn_7100af8450: the gallery's event handler. After the container's own (fn_71011d99e4), an event of kind 2
// (+0x20) opens the gallery: the archives inspector (cached at +0x2a8) is told whether +0x255 is clear
// (fn_7100af2d7c), the selection goes back to 0 with every entry deselected, then either the next queued entry
// is revealed (registering the gallery's key) or the first one is selected.
struct UnkGuiEvent_b49 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};
void fn_71011d99e4(UnkGuiDisplayObject* self, UnkGuiEvent_b49* event);
void fn_7100af2d7c(UnkReflObject* inspector, bool flag);

static inline __attribute__((always_inline)) UnkReflObject* AsInspector_A5_b75(UnkReflObject* o) {
    return fn_710009d50c(o->vf_0x0(), fn_7100af1f50()) ? o : nullptr;
}

void fn_7100af8450(UnkGallery_A5_b75* self, UnkGuiEvent_b49* event) {
    fn_71011d99e4(reinterpret_cast<UnkGuiDisplayObject*>(self), event);
    if (event->kind != 2)
        return;
    UnkReflObject* inspector = self->inspector;
    if (!inspector) {
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
        UnkReflObject* o = reinterpret_cast<UnkMainMenu_A5_b75*>(menu)->inspector;
        inspector = o && AsInspector_A5_b75(o) ? o : nullptr;
        self->inspector = inspector;
    }
    fn_7100af2d7c(inspector ? AsInspector_A5_b75(inspector) : nullptr, self->field_0x255 == 0);
    self->selected = 0;
    for (UnkGalleryEntry_A5_b75 *it = self->entries.data, *end = it + self->entries.count; it != end;) {
        UnkGalleryEntry_A5_b75* e = it++;
        bool hidden = e->hidden;
        UnkGalleryItem_A5_b75* item = e->item;
        if (hidden) {
            if (item->id.data == g_unk_717275ce28.data)
                item->vf_0x1e8("SecretMemory_Hidden", false);
            else
                item->vf_0x1e8("Memory_Hidden", false);
        } else {
            item->vf_0x1e8("OnSelectionLost", false);
        }
    }
    if (self->revealed.count != 0) {
        fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_717275ce20);
        fn_7100af8648(reinterpret_cast<UnkGallery_A5_b74*>(self));
    } else {
        self->entries.data[0].item->vf_0x1e8("OnMemorySelected", false);
    }
    self->field_0x2a0 = -1.0f;
}

// fn_7100af8cf4: the gallery's confirm. After the base handler (fn_71011cf70c), and only with the inspector
// found: a hidden entry just gets the refused sound; otherwise the confirm sound plays and the inspector is
// shown the entry's "Label" text (+0x218) with the path "<image>/<image>" built from its "Image" child's
// texture name (+0x1d8, empty when none). The secret entry also goes through vf_0x268.
struct UnkPooledStrId;
void fn_710007eb20(CRntString* self, const UnkPooledStrId* id);
void fn_710007f564(void*, const char*);
struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f01a0;
extern UnkEnumType g_unk_71729f01a8;
extern UnkEnumType g_unk_71729f0220;

struct UnkText_A5_b75 {
    u8 unk_0x0[0x20];
    explicit UnkText_A5_b75(const void* other) { fn_7100066cac(this, other); }
    ~UnkText_A5_b75() { fn_7100066e44(this); }
};
struct UnkGuiLabel_A5_b75 {
    u8 pad_0x0[0x218];
    u8 text[0x20];
};
struct UnkGuiImage_A5_b75 {
    u8 pad_0x0[0x1d8];
    CStrId* texture;
};
// The name as a string (fn_710007eb20), destroyed as one.
struct UnkIdText_A5_b75 {
    union {
        CRntString str;
    };
    explicit UnkIdText_A5_b75(const CStrId* id) { fn_710007eb20(&str, reinterpret_cast<const UnkPooledStrId*>(id)); }
    ~UnkIdText_A5_b75() { fn_710007ecd4(&str); }
};
void fn_7100af2b94(UnkReflObject* inspector, const UnkText_A5_b75& text, const CStrId& path);

template <const base::reflection::CClass* (*Type)()>
static inline __attribute__((always_inline)) UnkGuiDisplayObject* AsKind_A5_b75(UnkGuiDisplayObject* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(o->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Type()))
               ? o
               : nullptr;
}

// Shows the entry in the inspector through `Send` (its "Label" text and "<image>/<image>"), then hands the
// secret entry to vf_0x268.
template <void (*Send)(UnkReflObject*, const UnkText_A5_b75&, const CStrId&)>
static inline __attribute__((always_inline)) void Show_A5_b75(UnkGallery_A5_b75* self,
                                                              UnkGalleryEntry_A5_b75& entry) {
    auto* label = reinterpret_cast<UnkGuiLabel_A5_b75*>(AsKind_A5_b75<fn_71011e8dfc>(
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(entry.item), CStrId("Label", true))));
    auto* image = reinterpret_cast<UnkGuiImage_A5_b75*>(AsKind_A5_b75<fn_71011f6890>(
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(entry.item), CStrId("Image", true))));
    UnkIdText_A5_b75 name(&static_cast<const CStrId&>(image->texture ? CStrId(*image->texture) : CStrId("", true)));
    CRntString path(name.str);
    fn_710007f564(&path, "/");
    fn_710007f3b4(&path, &name.str);
    Send(self->inspector, UnkText_A5_b75(label->text), CStrId(path.data ? path.data : "", true));
    if (entry.item->id.data == g_unk_717275ce28.data)
        reinterpret_cast<void (*)(UnkGallery_A5_b75*, UnkGalleryItem_A5_b75*)>(self->vtable[0x268 / 8])(self,
                                                                                                    entry.item);
}

void fn_7100af8cf4(UnkGallery_A5_b75* self) {
    fn_71011cf70c(self);
    if (!self->inspector)
        return;
    u32 selected = self->selected;
    UnkGalleryEntry_A5_b75& entry = self->entries.data[selected];
    if (entry.hidden) {
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01a0), self);
        return;
    }
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01a8), self);
    Show_A5_b75<fn_7100af2b94>(self, entry);
}

// fn_7100af9108: moves the selection to the next entry that isn't hidden (wrapping past `count`), deselecting
// the current one, shows it in the inspector (fn_7100af2db4) when there is one, and plays the move sound and
// "OnMemorySelected".
void fn_7100af2db4(UnkReflObject* inspector, const UnkText_A5_b75& text, const CStrId& path);

void fn_7100af9108(UnkGallery_A5_b75* self) {
    u32 current = self->selected;
    u32 count = self->count;
    u32 next = (s32)current >= (s32)count ? 0 : current + 1;
    UnkGalleryEntry_A5_b75* entries = self->entries.data;
    for (; next != current; ++next) {
        if ((s32)next > (s32)count)
            next = 0;
        if (!entries[next].hidden)
            break;
    }
    if (next == current)
        return;
    u64 index = next;  // the original keeps a zero-extended copy for the new entry
    UnkGalleryEntry_A5_b75& old = entries[current];
    bool hidden = old.hidden;
    UnkGalleryItem_A5_b75* item = old.item;
    if (hidden) {
        if (item->id.data == g_unk_717275ce28.data)
            item->vf_0x1e8("SecretMemory_Hidden", false);
        else
            item->vf_0x1e8("Memory_Hidden", false);
    } else {
        item->vf_0x1e8("OnSelectionLost", false);
    }
    UnkGalleryEntry_A5_b75& entry = self->entries.data[index];
    self->selected = next;
    if (self->inspector)
        Show_A5_b75<fn_7100af2db4>(self, entry);
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0220), self);
    entry.item->vf_0x1e8("OnMemorySelected", false);
}

// fn_7100af94e4: the same as fn_7100af9108 backwards (wrapping below 0 to `count`), except that the image's
// texture name is cut to what follows its "__" (in a string with 0x100 bytes of inline storage) before the path
// is built.
s32 fn_710007fb54(const CRntString* self, const char* str, u32 start);  // finds `str`
using UnkName_A5_b75 = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkNameFromId_A5_b75 : UnkName_A5_b75 {
    __attribute__((always_inline)) explicit UnkNameFromId_A5_b75(const CStrId& id) {
        fn_710007eb6c(Str(), &id);
    }
};
struct UnkNamePart_A5_b75 : UnkName_A5_b75 {
    __attribute__((always_inline)) UnkNamePart_A5_b75(const char* str, u32 length) {
        fn_710007e7f8(Str(), str, length);
    }
};

void fn_7100af94e4(UnkGallery_A5_b75* self) {
    u32 current = self->selected;
    u32 prev;
    UnkGalleryEntry_A5_b75* entries;
    if ((s32)current >= 1) {
        entries = self->entries.data;
        asm("" : "+r"(entries));
        prev = current - 1;
    } else {
        prev = self->count;
        if (prev == current)
            return;
        entries = self->entries.data;
    }
    for (;;) {
        if ((s32)prev < 0)
            prev = self->count;
        if (!entries[prev].hidden)
            break;
        if (--prev == current)
            return;
    }
    if (prev == current)
        return;
    u64 index = prev;
    UnkGalleryEntry_A5_b75& old = entries[current];
    bool hidden = old.hidden;
    UnkGalleryItem_A5_b75* item = old.item;
    if (hidden) {
        if (item->id.data == g_unk_717275ce28.data)
            item->vf_0x1e8("SecretMemory_Hidden", false);
        else
            item->vf_0x1e8("Memory_Hidden", false);
    } else {
        item->vf_0x1e8("OnSelectionLost", false);
    }
    UnkGalleryEntry_A5_b75& entry = self->entries.data[index];
    self->selected = prev;
    if (self->inspector) {
        auto* label = reinterpret_cast<UnkGuiLabel_A5_b75*>(AsKind_A5_b75<fn_71011e8dfc>(
            fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(entry.item), CStrId("Label", true))));
        auto* image = reinterpret_cast<UnkGuiImage_A5_b75*>(AsKind_A5_b75<fn_71011f6890>(
            fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(entry.item), CStrId("Image", true))));
        UnkNameFromId_A5_b75 name(image->texture ? CStrId(*image->texture) : CStrId("", true));
        u32 start = fn_710007fb54(name.Str(), "__", 0) + 1;
        u32 length = name.Str()->length - start;
        const char* rest = name.Str()->data + start;
        fn_710007eaa0(name.Str(), UnkNamePart_A5_b75(rest, length).Str());
        UnkName_A5_b75 path(name);
        fn_710007f564(path.Str(), "/");
        fn_710007f3b4(path.Str(), name.Str());
        fn_7100af2db4(self->inspector, UnkText_A5_b75(label->text),
                      CStrId(path.Str()->data ? path.Str()->data : "", true));
        if (entry.item->id.data == g_unk_717275ce28.data)
            reinterpret_cast<void (*)(UnkGallery_A5_b75*, UnkGalleryItem_A5_b75*)>(self->vtable[0x268 / 8])(
                self, entry.item);
    }
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0220), self);
    entry.item->vf_0x1e8("OnMemorySelected", false);
}
