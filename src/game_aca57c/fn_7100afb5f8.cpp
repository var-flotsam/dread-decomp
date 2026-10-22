// Moves a callback (at 0x2a8) to the pop-up composition of the current main menu (its set at
// 0x260), when there is a main menu and its pop-up (at 0x540) is a GUI::CPopUpComposition.
// Three identical copies.
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflObject;

void fn_71004567dc(void* set, void** slot);  // unregisters
void fn_7100a58e80(void* set, void** slot);  // registers

struct UnkCallbackSlot {
    void* set;

    // Moves the callback to the set at `Offset` in `owner`.
    template <u32 Offset>
    __attribute__((always_inline)) void Watch(void* owner) {
        void** slot = &set;
        void* next = static_cast<char*>(owner) + Offset;
        if (*slot) {
            fn_71004567dc(*slot, slot);
            *slot = nullptr;
        }
        *slot = next;
        fn_7100a58e80(next, slot);
    }
};

struct UnkMainMenu {
    u8 unk_0x0[0x540];
    UnkReflObject* popup;
};

struct UnkPopUpListener {
    u8 unk_0x0[0x2a8];
    UnkCallbackSlot callback;
};

extern void* g_unk_7101d47310;
UnkReflObject* fn_7100e033a8(void* manager);
bool fn_710009d50c(const CType* type, const CType* base);
const CClass* fn_7100b34230();  // GUI::CMainMenu::refl_GetClass
const CClass* fn_7100b5e368();  // GUI::CPopUpComposition::refl_GetClass

__attribute__((always_inline)) static inline void WatchPopUp(UnkPopUpListener* self) {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (!menu || !fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
        return;
    UnkReflObject* popup = reinterpret_cast<UnkMainMenu*>(menu)->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    self->callback.Watch<0x260>(popup);
}

void fn_7100afb5f8(UnkPopUpListener* self) { WatchPopUp(self); }
void fn_7100b00e18(UnkPopUpListener* self) { WatchPopUp(self); }
void fn_7100b5dd28(UnkPopUpListener* self) { WatchPopUp(self); }
