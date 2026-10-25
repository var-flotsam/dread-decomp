// Five main-menu GUI setup functions: each looks up a named composition through the GUI
// registry, keeps the object (dropping the temporary handle), configures it and hands it to
// the current main menu (or to nothing when no main menu is active).
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflObject;

// A reference-counted weak handle to the looked-up object, released through vf_0x10.
struct UnkGuiHandleObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    void* target;
};

struct UnkGuiHandle {
    UnkGuiHandleObj* ptr;

    void* Get() const { return ptr ? ptr->target : nullptr; }
    ~UnkGuiHandle() {
        if (UnkGuiHandleObj* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkMenuScreen {
    void* vtable;
    void* composition;
};

extern void* g_unk_717271e8f8;
UnkGuiHandle fn_71011b69d8(void* registry, const CRntString& name);
void fn_71011cdce8(void* composition, s32 value);
void fn_71011cf2a4(void* composition, s32 value);
void fn_71011cf364(void* composition, s32 value);
void fn_71011cde44(void* composition, s32 value);
UnkReflObject* fn_7100e033a8(UnkGameManager* manager);
void fn_71011d968c(UnkReflObject* menu, void* composition);

template <s32 Value>
__attribute__((always_inline)) static inline bool SetUp(UnkMenuScreen* self, const char* name) {
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
    if (void* composition = self->composition) {
        fn_71011cdce8(composition, 0);
        fn_71011cf2a4(self->composition, 0);
        fn_71011cf364(self->composition, Value);
        fn_71011cde44(self->composition, 2);
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        if (menu)
            menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
        fn_71011d968c(menu, self->composition);
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
