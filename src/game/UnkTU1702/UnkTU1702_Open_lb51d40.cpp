// Unit 1702 (tools/tu_map.py): opens the file screen. Hands the screen its root, and when the scenario
// differs from the screen's, starts the new-item pop-up timer if the profile asks for it and refreshes
// three of the screen's items; then fills the slot info and the title ("#GUI_FILEMENU_HEADER_" and the
// slot number). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU1702.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// An object reached through a reference that resolves it on first use (its vf_0x10).
template <class T>
struct UnkLazyRef_lb51d40 {
    void** vtable;
    T* object;

    T* Get() {
        if (!object)
            reinterpret_cast<void (*)(UnkLazyRef_lb51d40*)>(vtable[0x10 / 8])(this);
        return object;
    }
};

// One of the screen's items, refreshed through its vf_0x230.
struct UnkScreenItem_lb51d40 {
    void** vtable;

    void Refresh() { reinterpret_cast<void (*)(UnkScreenItem_lb51d40*)>(vtable[0x230 / 8])(this); }
};

struct UnkSlotOwner_lb51d40 {
    UnkLazyRef_lb51d40<UnkGuiDisplayObject> root;
    f32 timer;  // 0x10: the new-item pop-up's, -1 while stopped
};

struct UnkFileScreen_lb51d40 {
    u8 pad_0x0[0x268];
    UnkGuiDisplayObject* root;  // 0x268
    u8 pad_0x270[0xc];
    s32 field_0x27c;
    u8 pad_0x280[0x8];
    CRntString scenario;  // 0x288
    u8 pad_0x2b8[0x508 - 0x2b8];
    UnkLazyRef_lb51d40<UnkScreenItem_lb51d40> field_0x508;
    UnkLazyRef_lb51d40<UnkScreenItem_lb51d40> field_0x518;
    UnkLazyRef_lb51d40<UnkScreenItem_lb51d40> field_0x528;
};
static_assert(__builtin_offsetof(UnkFileScreen_lb51d40, field_0x528) == 0x528, "UnkFileScreen_lb51d40 layout");

// GUI::CSlotInfoComposition, as far as this reads it.
struct UnkSlotInfo_lb51d40 : UnkGuiDisplayObject {
    u8 pad_0x198[0x230 - 0x198];
    u32 slot;  // 0x230: the slot's index
};
static_assert(__builtin_offsetof(UnkSlotInfo_lb51d40, slot) == 0x230, "UnkSlotInfo_lb51d40 layout");

// The game manager's current scenario name.
struct UnkGameManagerScenario_lb51d40 {
    u8 pad_0x0[0x7f0];
    CRntString scenario;  // 0x7f0
};

void fn_7100b7af7c(UnkSlotInfo_lb51d40* self);

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

// The id of the profile section that holds the new-item flag.
static inline const CStrId& NewItemSectionId() {
    return GameManagerIdTable().field_0x1be8;
}

// The display object at `path` below `root` when it is of the class `Getter` returns, or null.
template <auto Getter>
static inline UnkGuiDisplayObject* FindChild(UnkGuiDisplayObject* root, const CRntString& path) {
    UnkGuiDisplayObject* obj = fn_71011d9508(root, reinterpret_cast<const char* const*>(&path));
    if (obj && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Getter())))
        obj = nullptr;
    return obj;
}

void fn_7100b51d40(UnkSlotOwner_lb51d40* self, UnkFileScreen_lb51d40* screen) {
    screen->root = self->root.Get();
    self->timer = -1.0f;
    CRntString scenario = reinterpret_cast<UnkGameManagerScenario_lb51d40*>(g_unk_7101d47310)->scenario;
    bool changed;
    {
        CRntString current = screen->scenario;
        changed = CRntString_KeysDiffer(&current, &scenario);
    }
    if (changed) {
        CBlackboard* profile = &g_unk_7101d47310->field_0x820;
        bool show_new_item = false;
        {
            const CStrId& section_id = NewItemSectionId();
            const UnkQualifiedId& name = Qid(g_unk_717275ad68);
            if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
                fn_710006d11c(section, &name.id, &show_new_item);
        }
        if (show_new_item) {
            self->timer = 0.0f;
            fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_7172764720);
        }
        screen->field_0x508.Get()->Refresh();
        screen->field_0x518.Get()->Refresh();
        screen->field_0x528.Get()->Refresh();
    }
    screen->field_0x27c = 3;

    UnkSlotInfo_lb51d40* slot_info =
        static_cast<UnkSlotInfo_lb51d40*>(FindChild<fn_7100b79b74>(self->root.object, CRntString("Content.SlotInfoComposition")));
    if (!slot_info)
        return;
    fn_7100b7af7c(slot_info);
    UnkGuiDisplayObject* title = FindChild<fn_71011e8dfc>(self->root.object, CRntString("Content.CLabel-Title"));
    if (!title)
        return;
    CRntString key("#GUI_FILEMENU_HEADER_");
    fn_710007f634(&key, slot_info->slot + 1);
    CRntString text(key.data ? key.data : g_unk_710162e2ed);
    fn_71011ec424(title, text);
}
