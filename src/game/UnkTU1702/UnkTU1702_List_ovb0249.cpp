// Unit 1702 (tools/tu_map.py): sets up a screen from the game manager's string at +0x7f0: copies it into the
// screen's (+0x288) and, unless the screen's mode (+0x280) is 8, remembers the parent object (+0x270), finds the list
// "Content.ListComposition" below it and sets its two flags (+0x262 unless the mode is 3, +0x263) before fn_7100b364dc.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CMainMenu.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "types.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTUProperty.h"

using base::global::CRntString;

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
struct UnkMenuTarget;

struct UnkListScreen_b0249 {
    u8 pad_0x0[0x270];
    UnkGuiObj* parent;  // 0x270
    u8 pad_0x278[8];
    s32 mode;  // 0x280
    u8 pad_0x284[4];
    CRntString text;  // 0x288
};

struct UnkListOwner_b0249 {
    u8 pad_0x0[8];
    UnkGuiObj* field_0x8;
};

struct UnkListFlags_b0249 : UnkGuiObj {
};

void fn_7100b52284(UnkListOwner_b0249* owner, UnkListScreen_b0249* screen) {
    // Lives to the end of the function, but is destroyed straight after the assignment.
    union Holder {
        CRntString str;
        Holder(const CRntString& s) : str(s) {}
        ~Holder() {}
    } copy(*reinterpret_cast<const CRntString*>(reinterpret_cast<const char*>(g_unk_7101d47310) + 0x7f0));
    fn_710007eaa0(&screen->text, &copy.str);
    copy.str.~CRntString();
    if (screen->mode == 8)
        return;
    // a char store: the parent is read again afterwards
    __builtin_memcpy(&screen->parent, &owner->field_0x8, sizeof(screen->parent));
    UnkGuiObj* root = owner->field_0x8;
    UnkGuiObj* list;
    {
        CRntString path("Content.ListComposition");
        list = fn_71011d9508(root, reinterpret_cast<const char* const*>(&path));
        if (list && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(list->GetClass()),
                                   reinterpret_cast<const base::reflection::CType*>(fn_71011fe904())))
            list = nullptr;
    }
    if (list) {
        u8* p = reinterpret_cast<u8*>(list);
        p[0x262] = screen->mode != 3;
        p[0x263] = 1;
    }
    fn_7100b364dc(reinterpret_cast<UnkMenuTarget*>(screen));
}

// Unit 1702 (tools/tu_map.py): a timer (+0x10, counting up from when it is started, -1 while stopped) that, once it
// reaches one second, stops, clears the profile's "ShowNewItemPopUp" flag, saves the profile, removes a key from the
// GUI root's list and shows the pop-up "#GUI_FILEMENU_PROMPT_NEWCHOZOMEMORIES" on the other object's pop-up
// composition (+0x540). Names and layouts are placeholders.

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};

extern UnkPropertyEx<> g_unk_717275ad68;  // "ShowNewItemPopUp"
extern CStrId g_unk_7172764720 __attribute__((visibility("hidden")));

static inline const CStrId& IdTableKey(u32 offset) {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}

// The property's qualified id (game/UnkGameBlackboard.h), built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = reinterpret_cast<UnkQualifiedId&>(item.field_0x10);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, reinterpret_cast<UnkIdSource*>(&item), g_unk_7101d470e0);
    return qid;
}

// Stores `value` under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetBool_b0249(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                 const bool& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = const_cast<bool*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

struct UnkPopUpTimer_b0249 {
    u8 pad_0x0[0x10];
    f32 timer;  // 0x10
};

struct UnkPopUpOwner_b0249 {
    u8 pad_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

void fn_7100b5206c(UnkPopUpTimer_b0249* self, UnkPopUpOwner_b0249* owner, f32 dt) {
    if (!(self->timer >= 0.0f))
        return;
    self->timer += dt;
    if (!(self->timer >= 1.0f))
        return;
    self->timer = -1.0f;
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* profile = reinterpret_cast<CBlackboard*>(reinterpret_cast<char*>(gm) + 0x820);
    SetBool_b0249(profile, IdTableKey(0x1be8), Qid(g_unk_717275ad68), false);
    {
        UnkProfileSaveArg arg = {nullptr, nullptr};
        fn_7100df99f8(g_unk_7101d47310, 0, &arg, 0);
    }
    fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172764720);
    UnkReflObject* popup = owner->popup;
    if (popup)
        popup = fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()) ? popup : nullptr;
    CStrId id("#GUI_FILEMENU_PROMPT_NEWCHOZOMEMORIES", true);
    fn_7100b5fd6c(popup, &id.data, &g_unk_7101d470e0, true, false);
}
