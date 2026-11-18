// GUI::CButton unit: the button's accept. After the display object's handler, a button inside a GUI::CList
// tells the list and plays the list sound; otherwise it plays the accept sound (unless muted), sends
// "OnAccept", and when its target answers to "OnAccept" watches the target's set, else calls vf_0x268.
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/TypeObject.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CButton.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2747.h"
#include "game/UnkTUInit.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkEnumType;
extern UnkEnumType g_unk_71729f01a0;
extern UnkEnumType g_unk_71729f01a8;

// The class name and the class type after it.
struct UnkButtonClassData_l1c1530 {
    CStrId name;
    u8 unk_0x8[0x10];
    UnkReflType type;  // 0x18
};

void fn_710120186c(UnkGuiDisplayObject* list);

// What the target hands out to be watched: a set at +0x18.
struct UnkButtonWatched_l1bf3cc {
    u8 pad_0x0[0x18];
    base::UnkDeferredSet<void> watchers;
};

struct UnkButtonTarget_l1bf3cc {
    void** vtable;
    u8 pad_0x8[0x18];
    CStrId name;  // 0x20

    // The name's key, counting the read.
    s64 Key() const {
        ++name.data->reads;
        return name.data->String().key;
    }
    UnkButtonWatched_l1bf3cc* vf_0x1f8() {
        return reinterpret_cast<UnkButtonWatched_l1bf3cc* (*)(UnkButtonTarget_l1bf3cc*)>(vtable[0x1f8 / 8])(this);
    }
};

struct UnkButtonAccept_l1bf3cc {
    void** vtable;
    u8 pad_0x8[0x118];
    UnkGuiDisplayObject* parent;  // 0x120
    u8 pad_0x128[0x128];
    bool muted;  // 0x250
    u8 pad_0x251[0x7];
    base::UnkDeferredSet<void>* watch;  // 0x258
    u8 pad_0x260[0x18];
    UnkButtonTarget_l1bf3cc* target;  // 0x278
    u8 pad_0x280[0x20];
    bool in_list;  // 0x2a0

    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkButtonAccept_l1bf3cc*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, flag);
    }
    void vf_0x268() { reinterpret_cast<void (*)(UnkButtonAccept_l1bf3cc*)>(vtable[0x268 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkButtonAccept_l1bf3cc, in_list) == 0x2a0, "layout");

void fn_71011bf3cc(UnkButtonAccept_l1bf3cc* self) {
    fn_71011cf70c(reinterpret_cast<UnkGallery_A5_b75*>(self));
    if (self->in_list) {
        if (UnkGuiDisplayObject* parent = self->parent) {
            const base::reflection::CClass* cls = parent->GetClass();
            if (fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls),
                              reinterpret_cast<const base::reflection::CType*>(fn_71011fe904())))
                fn_710120186c(parent);
        }
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01a0), self);
        return;
    }
    if (!self->muted)
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01a8), self);
    self->SendEvent("OnAccept", false);
    if (UnkButtonTarget_l1bf3cc* target = self->target) {
        bool accepted;
        {
            CRntString event("OnAccept");
            accepted = target->Key() == event.key;
        }
        if (accepted) {
            fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_7172a0d340.name);
            base::UnkDeferredSet<void>** watch = &self->watch;
            UnkButtonWatched_l1bf3cc* watched = self->target->vf_0x1f8();
            if (*watch) {
                fn_71004f3ea4(*watch, watch);
                *watch = nullptr;
            }
            *watch = &watched->watchers;
            fn_71004f3cf0(*watch, watch);
            return;
        }
    }
    self->vf_0x268();
}
