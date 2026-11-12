// GUI::CArchivesInspectorComposition's unit (tools/tu_map.py unit 1651, the name from reflection).
// fn_7100af2b94 opens the inspector: it is made active, shown and enabled and brought to the front of
// the GUI root, the main menu (when it is a GUI::CMainMenu-like class, fn_7100b34230) is disabled, the
// entry is set (without refreshing), the "OnShow" and "UI_Show" events are sent, the watch at +0x230 is
// dropped and the one at +0x248 moved to the set at +0x18 of the object at +0x30 of +0x2f0.
// fn_7100af2db4 sets an entry: the sprite (+0x2e0) gets the image id and the label (+0x2e8) the text, each refreshed
// (fn_71011cd5a0); the composition's key goes to the GUI root's key list (+0xca8) and +0x30c is set to
// 0.12. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkGuiSprite.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

using UnkGuiObj = UnkGuiDisplayObject;
void fn_71011cd5a0(UnkGuiObj* object);
extern CStrId g_unk_717275c5e0 __attribute__((visibility("hidden")));  // the composition's key

using base::reflection::UnkReflObject;

struct UnkArchivesWatched {
    u8 pad_0x0[0x30];
    char* field_0x30;  // its set at +0x18
};

struct UnkArchivesInspector {
    u8 pad_0x0[0x230];
    base::UnkDeferredSet<void>* watch_0x230;
    u8 pad_0x238[0x10];
    base::UnkDeferredSet<void>* watch_0x248;
    u8 pad_0x250[0x90];
    UnkGuiDisplayObject* sprite;  // 0x2e0
    UnkGuiDisplayObject* label;   // 0x2e8
    UnkArchivesWatched* field_0x2f0;
    u8 pad_0x2f8[0x10];
    bool field_0x308;
    u8 pad_0x309[3];
    f32 field_0x30c;

    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkArchivesInspector*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, name, flag);
    }
};
static_assert(__builtin_offsetof(UnkArchivesInspector, field_0x30c) == 0x30c, "layout");

void fn_7100af2b94(UnkArchivesInspector* self, const UnkGuiText& text, const CStrId& image) {
    fn_71011cdce8(self->Object(), true);
    fn_71011cdc30(self->Object(), true);
    fn_71011cf2a4(self->Object(), true);
    fn_71011b6bbc(g_unk_717271e8f8, self->Object(), false);
    if (UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310)) {
        if (fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
            fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(menu), false);
    }
    fn_71011f8bdc(self->sprite, image);
    fn_71011ec468(self->label, text, false);
    self->field_0x308 = true;
    self->SendEvent("OnShow", false);
    self->SendEvent("UI_Show", false);
    if (self->watch_0x230) {
        fn_71004f3ea4(self->watch_0x230, &self->watch_0x230);
        self->watch_0x230 = nullptr;
    }
    base::UnkDeferredSet<void>* current = self->watch_0x248;
    char* owner = self->field_0x2f0->field_0x30;
    if (current) {
        fn_71004f3ea4(current, &self->watch_0x248);
        self->watch_0x248 = nullptr;
    }
    self->watch_0x248 = reinterpret_cast<base::UnkDeferredSet<void>*>(owner + 0x18);
    fn_71004f3cf0(self->watch_0x248, &self->watch_0x248);
    fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_717275c5e0);
    self->field_0x30c = 0.12f;
}

void fn_7100af2db4(UnkArchivesInspector* self, const UnkGuiText& text, const CStrId& image) {
    fn_71011f8bdc(self->sprite, image);
    fn_71011cd5a0(self->sprite);
    fn_71011ec468(self->label, text, false);
    fn_71011cd5a0(self->label);
    fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_717275c5e0);
    self->field_0x30c = 0.12f;
}
