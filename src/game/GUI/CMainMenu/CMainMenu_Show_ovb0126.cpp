// GUI::CMainMenu's unit (tools/tu_map.py): fn_7100b363ac removes the menu's key (g_unk_7172763718) from the
// GUI root's key list (fn_710002ffb4). Unless the composition at +0x270 is the "slotselectioncomposition"
// and the state at +0x278 is 3, it plays the "Loop" animation of the object at +0x300 in state 1, shows the
// display object at +0x268 (fn_71011cf2a4) and, unless the flag at +0xb4 of the object at +0x540 is set,
// gives it the root's focus (fn_71011b6bbc). Returns true. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7172763718 __attribute__((visibility("hidden")));

struct UnkMainMenuComposition_ovb0126 {
    u8 pad_0x0[0x130];
    base::global::UnkStringData* name;
};

struct UnkMainMenuAnimated_ovb0126 {
    virtual void vf_0x0();
    void Play(const char* name, bool flag) {
        using Fn = void (*)(UnkMainMenuAnimated_ovb0126*, const char*, bool);
        (*reinterpret_cast<Fn**>(this))[0x1e8 / 8](this, name, flag);
    }
};

struct UnkMainMenuSettings_ovb0126 {
    u8 pad_0x0[0xb4];
    bool field_0xb4;
};

struct UnkMainMenu_ovb0126 {
    u8 pad_0x0[0x268];
    UnkGuiDisplayObject* display;  // 0x268
    UnkMainMenuComposition_ovb0126* composition;  // 0x270
    u32 state;  // 0x278
    u8 pad_0x27c[0x300 - 0x27c];
    UnkMainMenuAnimated_ovb0126* animated;  // 0x300
    u8 pad_0x308[0x540 - 0x308];
    UnkMainMenuSettings_ovb0126* settings;  // 0x540
};

// Whether the menu shows the slot selection composition in state 3.
static inline bool InSlotSelection(UnkMainMenu_ovb0126* self, UnkMainMenuComposition_ovb0126* composition) {
    CStrId name("slotselectioncomposition", true);
    return composition->name == name.data && self->state == 3;
}

bool fn_7100b363ac(UnkMainMenu_ovb0126* self) {
    fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172763718);
    if (UnkMainMenuComposition_ovb0126* composition = self->composition) {
        if (InSlotSelection(self, composition))
            return true;
    }
    if (self->state == 1)
        self->animated->Play("Loop", false);
    fn_71011cf2a4(self->display, true);
    if (!self->settings->field_0xb4)
        fn_71011b6bbc(g_unk_717271e8f8, self->display, false);
    return true;
}
