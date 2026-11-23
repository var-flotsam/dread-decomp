// Unit 1694 (tools/tu_map.py): sets the difficulty selector up. Reads whether hard and expert mode are
// unlocked from the user data, fades the panels, describes the two difficulty buttons (greyed out while
// locked), selects the default entry and titles the screen after the current difficulty. Names and
// layouts are placeholders.
#include "../../../src/game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU57.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkList_ovb0059;
void fn_71012015a0(UnkList_ovb0059* self, const CStrId* id);
base::global::CRntString fn_7100df703c(u32 kind);
extern UnkStateKey g_unk_71729f0178;
void fn_71011cf960(UnkGuiDisplayObject* self, f32 value);

// The names of the difficulty list, its default entry and its hard and expert buttons.
extern CStrId g_unk_71727645a8;
extern CStrId g_unk_71727645b0;
extern CStrId g_unk_71727645b8;
extern CStrId g_unk_71727645c8;

struct UnkDifficultySelector_lb4e5b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // builds the root

    UnkGuiDisplayObject* root;  // 0x8

    UnkGuiDisplayObject* Root() {
        if (!root)
            vf_0x10();
        return root;
    }
};

struct UnkDifficultyOwner_lb4e5b0 {
    u8 pad_0x0[0x268];
    UnkGuiDisplayObject* root;  // 0x268
};

struct UnkDifficultyButton_lb4e5b0 {
    u8 pad_0x0[0x2a0];
    bool locked;  // 0x2a0
    u8 pad_0x2a1[0xf];
    CRntString description;  // 0x2b0
};

struct UnkDifficultyList_lb4e5b0 {
    u8 pad_0x0[0x262];
    bool field_0x262;
    bool field_0x263;
};

// The game manager's current difficulty name.
struct UnkGameManagerDifficulty_lb4e5b0 {
    u8 pad_0x0[0x7f0];
    CRntString difficulty;  // 0x7f0
};

struct UnkLockedColor_lb4e5b0 {
    f32 r, g, b, a;
    UnkLockedColor_lb4e5b0(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

// The object when it is of the class `Type` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Type)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(o->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Type()))
               ? o
               : nullptr;
}

static inline UnkDifficultyButton_lb4e5b0* AsButton(UnkGuiDisplayObject* o) {
    return reinterpret_cast<UnkDifficultyButton_lb4e5b0*>(o);
}

// Selects the list's entry named `id` (taken by value).
static inline void SelectEntry(UnkGuiDisplayObject* list, CStrId id) {
    fn_71012015a0(reinterpret_cast<UnkList_ovb0059*>(list), &id);
}

static inline bool Equal(const CRntString& a, const CRntString& b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&a), reinterpret_cast<const UnkCompared_0x30*>(&b));
}

void fn_7100b4e5b0(UnkDifficultySelector_lb4e5b0* self, UnkDifficultyOwner_lb4e5b0* owner) {
    owner->root = self->Root();
    // Whether hard and expert mode are unlocked: flags of the user data's "USER_DATA" section.
    bool hard_unlocked = false;
    {
        CBlackboard* data = fn_710120384c(GetOwner());
        CStrId section_name("USER_DATA", true);
        CStrId key("HardModeUnlocked", true);
        if (CBlackboard::CSection* section = fn_7101047270(data, section_name))
            fn_710006d11c(section, &key, &hard_unlocked);
    }
    bool expert_unlocked = false;
    {
        CBlackboard* data = fn_710120384c(GetOwner());
        CStrId section_name("USER_DATA", true);
        CStrId key("ExpertModeUnlocked", true);
        if (CBlackboard::CSection* section = fn_7101047270(data, section_name))
            fn_710006d11c(section, &key, &expert_unlocked);
    }

    UnkGuiDisplayObject* content = As<fn_71011d7b84>(fn_71011d98ec(self->root, CStrId("Content", true).data->Key()));
    UnkGuiDisplayObject* modal = As<fn_71011d7b84>(fn_71011d98ec(self->root, CStrId("ModalPanel", true).data->Key()));
    UnkGuiDisplayObject* window = fn_71011d98ec(self->root, CStrId("Window", true).data->Key());
    UnkGuiDisplayObject* list = As<fn_71011fe904>(fn_71011d98ec(self->root, g_unk_71727645a8.data->Key()));
    if (content && modal && window && list) {
        fn_71011cf960(modal, 0.92f);
        fn_71011cf960(window, 0.92f);
        fn_71011cf960(content, 1.0f);
        fn_71011ccd20(list, 0.255f);
        UnkGuiDisplayObject* hard = As<fn_7100b02b50>(fn_71011d94d4(list, g_unk_71727645b8));
        UnkGuiDisplayObject* expert = As<fn_7100b02b50>(fn_71011d94d4(list, g_unk_71727645c8));
        fn_71011cdc30(hard, true);
        fn_71011cf2a4(hard, true);
        fn_71011cdc30(expert, true);
        fn_71011cf2a4(expert, true);
        fn_710007e77c(&AsButton(hard)->description, hard_unlocked ? "#GUI_DIFSELECTOR_LABEL_DESCRIPTOR_HARD_UNLOCKED"
                                                                  : "#GUI_GAMEMODE_HARD_LOCKED_DESC");
        fn_710007e77c(&AsButton(expert)->description, expert_unlocked ? "#GUI_DIFSELECTOR_LABEL_DESCRIPTOR_EXPERT"
                                                                      : "#GUI_GAMEMODE_DREAD_LOCKED_DESC");
        UnkLockedColor_lb4e5b0 grey(0x1.468bd4p-2f, 0x1.acbbc8p-2f, 0x1.c71c74p-2f, 1.0f);
        if (!hard_unlocked) {
            unk_gui::fn_71011cfc24(hard, reinterpret_cast<const unk_gui::UnkColor&>(grey), true);
            AsButton(hard)->locked = true;
        }
        if (!expert_unlocked) {
            unk_gui::fn_71011cfc24(expert, reinterpret_cast<const unk_gui::UnkColor&>(grey), true);
            AsButton(expert)->locked = true;
        }
    }
    SelectEntry(list, g_unk_71727645b0);
    UnkDifficultyList_lb4e5b0* list_flags = reinterpret_cast<UnkDifficultyList_lb4e5b0*>(list);
    list_flags->field_0x262 = true;
    list_flags->field_0x263 = true;
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self->root);

    UnkGuiDisplayObject* root = self->root;
    UnkGuiDisplayObject* title;
    {
        CRntString path("Content.CLabel-Title");
        title = As<fn_71011e8dfc>(fn_71011d9508(root, &path.data));
    }
    if (title) {
        const CRntString& current =
            reinterpret_cast<UnkGameManagerDifficulty_lb4e5b0*>(g_unk_7101d47310)->difficulty;
        u32 level;
        if (Equal(current, fn_7100df703c(0)))
            level = 1;
        else if (Equal(current, fn_7100df703c(1)))
            level = 2;
        else if (Equal(current, fn_7100df703c(2)))
            level = 3;
        else
            level = 1;
        CRntString key("#GUI_DIFSELECTOR_LABEL_TITLE_DIFSELECT_");
        fn_710007f634(&key, level);
        CRntString text(key.data ? key.data : g_unk_710162e2ed);
        fn_71011ec424(title, text);
    }
}
