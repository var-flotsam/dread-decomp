// GUI::CSlotSelectedComposition unit (tools/tu_map.py, unit 1720): the "NewContent" / "NoContent" state, set
// from the main menu's ending-rewards (+0x530) and Chozo archives (+0x520) compositions: either one's count
// (+0x288) being non-zero means new content. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSlotSelected_ovb0113 {
    void** vtable;
    void SetState(const char* name, bool b) {
        reinterpret_cast<void (*)(UnkSlotSelected_ovb0113*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, b);
    }
};

struct UnkMainMenu_ovb0113 {
    u8 pad_0x0[0x520];
    UnkReflObject* archives;  // 0x520
    u8 pad_0x528[0x530 - 0x528];
    UnkReflObject* rewards;  // 0x530
};

struct UnkContentList_ovb0113 {
    u8 pad_0x0[0x288];
    u32 count;  // 0x288
};

__attribute__((always_inline)) bool fn_7100b7cd98(UnkSlotSelected_ovb0113* self) {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (menu && fn_710009d50c(menu->vf_0x0(), fn_7100b34230())) {
        UnkMainMenu_ovb0113* m = reinterpret_cast<UnkMainMenu_ovb0113*>(menu);
        UnkReflObject* rewards = m->rewards;
        auto* r = reinterpret_cast<UnkContentList_ovb0113*>(
            fn_710009d50c(rewards->vf_0x0(), fn_7100b1c740()) ? rewards : nullptr);
        u32 a = r->count;
        UnkReflObject* archives = m->archives;
        auto* c = reinterpret_cast<UnkContentList_ovb0113*>(
            fn_710009d50c(archives->vf_0x0(), fn_7100af6844()) ? archives : nullptr);
        if ((c->count | a) != 0)
            self->SetState("NewContent", false);
        else
            self->SetState("NoContent", false);
    }
    return true;
}

// fn_7100b7cad8: the event handler. After the container's own (fn_71011d99e4), and only while +0x230 is
// empty: the "Content.NewContent" sprite and its glow are both found, placed (vf_0x78 with -1400 and -1300)
// and the content state is set as above.
struct UnkGuiEvent_b49;
using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);

struct UnkSlotSelectedBody_ovb0113 {
    u8 pad_0x0[0x230];
    void* field_0x230;
};

static inline UnkGuiDisplayObject* FindSprite_ovb0113(UnkGuiDisplayObject* self, const char* name) {
    UnkGuiDisplayObject* obj;
    {
        base::global::CRntString path(name);
        obj = fn_71011d9508(self, reinterpret_cast<const char* const*>(&path));
        if (obj && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                  reinterpret_cast<const base::reflection::CType*>(fn_71011f6890())))
            obj = nullptr;
    }
    return obj;
}

static inline void Place_ovb0113(UnkGuiDisplayObject* obj, s32 v) {
    reinterpret_cast<void (*)(UnkGuiDisplayObject*, s32, bool)>((*reinterpret_cast<void***>(obj))[0x78 / 8])(
        obj, v, false);
}

void fn_7100b7cad8(UnkSlotSelected_ovb0113* self, UnkGuiEvent_b49* event) {
    UnkGuiDisplayObject* obj = reinterpret_cast<UnkGuiDisplayObject*>(self);
    fn_71011d99e4(obj, event);
    if (reinterpret_cast<UnkSlotSelectedBody_ovb0113*>(self)->field_0x230)
        return;
    UnkGuiDisplayObject* sprite = FindSprite_ovb0113(obj, "Content.NewContent");
    UnkGuiDisplayObject* glow = FindSprite_ovb0113(obj, "Content.NewContent_glow");
    if (!sprite || !glow)
        return;
    Place_ovb0113(sprite, -1400);
    Place_ovb0113(glow, -1300);
    fn_7100b7cd98(self);
}
