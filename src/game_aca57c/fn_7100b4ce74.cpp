// More menu GUI setup functions (compare fn_7100b4d0ec): each looks up a named composition
// through the GUI registry, keeps the object (dropping the temporary handle), configures it
// (including its vf_0x78) and either hands it to the current main menu (or to nothing when no
// main menu is active) or, for screens set up once, reports whether it was found.
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

static inline void CallVf78(void* composition, s32 a, bool b) {
    void** vtable = *static_cast<void***>(composition);
    reinterpret_cast<void (*)(void*, s32, bool)>(vtable[0x78 / 8])(composition, a, b);
}

template <s32 Value>
__attribute__((always_inline)) static inline bool SetUpForMenu(UnkMenuScreen* self, const char* name) {
    void*& composition = self->composition;
    composition = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
    if (composition) {
        fn_71011cdce8(composition, 0);
        fn_71011cf2a4(composition, 0);
        fn_71011cf364(composition, Value);
        CallVf78(composition, -1500, true);
        fn_71011cde44(composition, 2);
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        if (menu)
            menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
        fn_71011d968c(menu, composition);
    }
    return true;
}

// NON_MATCHING(minor): the original addresses the slot through a pointer to it (x19 + 8, written
// back) and sets the return value before restoring the registers.
bool fn_7100b4ce74(UnkMenuScreen* self) { return SetUpForMenu<0>(self, "ChozoArchivesGalleryComposition"); }
// NON_MATCHING(minor): the original addresses the slot through a pointer to it (x19 + 8, written
// back) and sets the return value before restoring the registers.
bool fn_7100b4f2cc(UnkMenuScreen* self) { return SetUpForMenu<-1>(self, "ExtrasMenuComposition"); }
// NON_MATCHING(minor): the original addresses the slot through a pointer to it (x19 + 8, written
// back) and sets the return value before restoring the registers.
bool fn_7100b4fa9c(UnkMenuScreen* self) { return SetUpForMenu<0>(self, "GammaAdjustComposition"); }

template <s32 Value, s32 Vf78>
__attribute__((always_inline)) static inline bool SetUpOnce(UnkMenuScreen* self, const char* name) {
    if (self->composition)
        return false;
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
    if (!self->composition)
        return false;
    fn_71011cdce8(self->composition, 0);
    fn_71011cf2a4(self->composition, 0);
    fn_71011cf364(self->composition, Value);
    CallVf78(self->composition, Vf78, true);
    fn_71011cde44(self->composition, 2);
    return true;
}

bool fn_7100b49730(UnkMenuScreen* self) { return SetUpOnce<-1, -2000>(self, "ChozoArchivesInspectorComposition"); }
bool fn_7100b517ec(UnkMenuScreen* self) { return SetUpOnce<-1, -2000>(self, "PopUpComposition"); }
bool fn_7100b519bc(UnkMenuScreen* self) { return SetUpOnce<0, -1500>(self, "SlotOptionsComposition"); }
