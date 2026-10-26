// Five main-menu GUI setup functions: each looks up a named composition through the GUI
// registry, keeps the object (dropping the temporary handle), configures it and hands it to
// the current main menu (or to nothing when no main menu is active).
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkMenuScreen {
    void* vtable;
    UnkGuiDisplayObject* composition;
};

extern void* g_unk_717271e8f8;
void fn_71011cf364(void* composition, s32 value);
void fn_71011cde44(void* composition, s32 value);
UnkReflObject* fn_7100e033a8(UnkGameManager* manager);

template <s32 Value>
__attribute__((always_inline)) static inline bool SetUp(UnkMenuScreen* self, const char* name) {
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
    if (UnkGuiDisplayObject* composition = self->composition) {
        fn_71011cdce8(composition, 0);
        fn_71011cf2a4(self->composition, 0);
        fn_71011cf364(self->composition, Value);
        fn_71011cde44(self->composition, 2);
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        if (menu)
            menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
        fn_71011d968c(reinterpret_cast<UnkGuiDisplayObject*>(menu), self->composition);
    }
    return true;
}

// NON_MATCHING(minor): the epilogue is scheduled differently (the original sets the return
// value before restoring the registers).
bool fn_7100b4d0ec(UnkMenuScreen* self) { return SetUp<-1>(self, "CompanyNameComposition"); }
// NON_MATCHING(minor): the epilogue is scheduled differently (the original sets the return
// value before restoring the registers).
bool fn_7100b51320(UnkMenuScreen* self) { return SetUp<-1>(self, "MissionMenuComposition"); }
// NON_MATCHING(minor): the epilogue is scheduled differently (the original sets the return
// value before restoring the registers).
bool fn_7100b51c1c(UnkMenuScreen* self) { return SetUp<0>(self, "SlotSelectionComposition"); }
// NON_MATCHING(minor): the epilogue is scheduled differently (the original sets the return
// value before restoring the registers).
bool fn_7100b52550(UnkMenuScreen* self) { return SetUp<-1>(self, "GameTitleComposition"); }
// NON_MATCHING(minor): the epilogue is scheduled differently (the original sets the return
// value before restoring the registers).
bool fn_7100b5462c(UnkMenuScreen* self) { return SetUp<-1>(self, "WarningComposition"); }
