// Unit 1698 (tools/tu_map.py, no name hint): unless the target is in mode 5 or 8, hands the target (+0x270)
// the composition's root, finds the root's "Content.ListComposition" GUI list and resets its two flags
// (+0x262 set in mode 4), then lets the target update (fn_7100b364dc) unless it is in mode 1. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CMainMenu.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CRntString;

struct UnkListComposition_lb51178 {
    void** vtable;
    UnkGuiDisplayObject* root;  // 0x8
};

struct UnkListTarget_lb51178 {
    u8 pad_0x0[0x270];
    UnkGuiDisplayObject* root;  // 0x270
    u8 pad_0x278[0x8];
    s32 mode;  // 0x280
};

struct UnkGuiList_lb51178 {
    u8 pad_0x0[0x262];
    bool field_0x262;
    bool field_0x263;
};

static inline UnkGuiDisplayObject* FindList(UnkGuiDisplayObject* root, const char* name) {
    UnkGuiDisplayObject* obj;
    {
        CRntString path(name);
        obj = fn_71011d9508(root, reinterpret_cast<const char* const*>(&path));
        if (obj && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                  reinterpret_cast<const base::reflection::CType*>(fn_71011fe904())))
            obj = nullptr;
    }
    return obj;
}

void AttachCompositionRootAndSetListCompositionFlags(UnkListComposition_lb51178* self, UnkListTarget_lb51178* target) {
    if (target->mode == 5 || target->mode == 8)
        return;
    target->root = self->root;
    auto* list = reinterpret_cast<UnkGuiList_lb51178*>(FindList(self->root, "Content.ListComposition"));
    if (list) {
        bool selected = target->mode == 4;
        list->field_0x263 = false;
        list->field_0x262 = selected;
    }
    if (target->mode != 1)
        fn_7100b364dc(reinterpret_cast<UnkMenuTarget*>(target));
}
