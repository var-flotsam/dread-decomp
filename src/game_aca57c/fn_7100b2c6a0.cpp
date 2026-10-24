// Menu GUI setup functions: each looks up a named composition through the GUI registry, keeps
// the object at `Offset` (dropping the temporary handle, or keeping the handle itself),
// configures it and hands it to the menu itself (compare fn_7100b4d0ec, which hands it to the
// main menu). Some also call the composition's vf_0x78.
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CClass;

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

extern void* g_unk_717271e8f8;
UnkGuiHandle fn_71011b69d8(void* registry, const CRntString& name);
void fn_71011cdce8(void* composition, s32 value);
void fn_71011cf2a4(void* composition, s32 value);
void fn_71011cf364(void* composition, s32 value);
void fn_71011cde44(void* composition, s32 value);
void fn_71011d968c(void* menu, void* composition);

template <u32 Offset>
struct UnkMenu {
    u8 unk_0x0[Offset];
    void* composition;
};

// vf_0x78 of a composition (called with -1500 and true by the variant below).
static inline void CallVf78(void* composition, s32 a, bool b) {
    void** vtable = *static_cast<void***>(composition);
    reinterpret_cast<void (*)(void*, s32, bool)>(vtable[0x78 / 8])(composition, a, b);
}

template <u32 Offset, s32 Value, bool Vf78 = false>
__attribute__((always_inline)) static inline void SetUp(UnkMenu<Offset>* self, const char* name) {
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
    void* const& composition = self->composition;
    if (composition) {
        fn_71011cdce8(composition, 0);
        fn_71011cf2a4(composition, 0);
        fn_71011cf364(composition, Value);
        if (Vf78)
            CallVf78(self->composition, -1500, true);
        fn_71011cde44(composition, 2);
        fn_71011d968c(self, composition);
    }
}

// NON_MATCHING(minor): the original keeps the slot's address in a register (x20) instead of
// addressing it from the menu.
void fn_7100b2c6a0(UnkMenu<0x290>* self) { SetUp<0x290, -1>(self, "ConfigMenuComposition"); }
// NON_MATCHING(minor): as fn_7100b2c6a0.
void fn_7100b2c784(UnkMenu<0x2c8>* self) { SetUp<0x2c8, -1>(self, "ConfigMenuBossRush"); }
// NON_MATCHING(minor): as fn_7100b2c6a0.
void fn_7100b2c964(UnkMenu<0x2a0>* self) { SetUp<0x2a0, 0>(self, "SystemMenuComposition"); }
// NON_MATCHING(minor): as fn_7100b2c6a0.
void fn_7100b2cc40(UnkMenu<0x2c0>* self) { SetUp<0x2c0, -1>(self, "SamusMenuComposition"); }
// NON_MATCHING(minor): as fn_7100b2c6a0.
void fn_7100b2c868(UnkMenu<0x298>* self) { SetUp<0x298, 0, true>(self, "GammaAdjustComposition"); }
// NON_MATCHING(minor): as fn_7100b2c6a0.
void fn_7100b2ca48(UnkMenu<0x2a8>* self) { SetUp<0x2a8, 0, true>(self, "AmiiboComposition"); }
// NON_MATCHING(minor): as fn_7100b2c6a0.
void fn_7100b2cb44(UnkMenu<0x2b0>* self) { SetUp<0x2b0, -1, true>(self, "ControlsMenuComposition"); }

// The mission menu (0x490) and the power description (0x488) are kept as handles (assigned by
// fn_7100b2c458) and set up the same way through them, once; the power description also gets
// vf_0x78(-2000, true).
template <u32 Offset>
struct UnkHandleMenu {
    u8 unk_0x0[Offset];
    UnkGuiHandle menu;
};

void fn_7100b2c458(UnkGuiHandle* dst, const UnkGuiHandle& src);
void fn_71011cef74(void* composition, void* owner);

template <u32 Offset, bool Vf78>
__attribute__((always_inline)) static inline void SetUpHandle(UnkHandleMenu<Offset>* self, const char* name) {
    if (self->menu.Get())
        return;
    UnkGuiHandle& menu = self->menu;
    fn_7100b2c458(&menu, fn_71011b69d8(g_unk_717271e8f8, CRntString(name)));
    if (!menu.Get())
        return;
    fn_71011cdce8(menu.Get(), 0);
    fn_71011cf2a4(menu.Get(), 0);
    fn_71011cf364(menu.Get(), -1);
    if (Vf78)
        CallVf78(menu.ptr->target, -2000, true);
    fn_71011cde44(menu.Get(), 2);
    fn_71011cef74(menu.Get(), self);
    fn_71011d968c(self, self->menu.Get());
}

void fn_7100b2c2bc(UnkHandleMenu<0x488>* self) { SetUpHandle<0x488, true>(self, "PowerDescComposition"); }
void fn_7100b2cd24(UnkHandleMenu<0x490>* self) { SetUpHandle<0x490, false>(self, "MissionMenuComposition"); }

// The icons HUD: the looked-up object is kept at 0x2d0 only as a GUI::CHUD (else null) and set
// up through that slot.
struct UnkGuiObject {
    void** vtable;

    const CClass* Type() { return reinterpret_cast<const CClass* (*)(UnkGuiObject*)>(vtable[0])(this); }
};
const CClass* fn_71011dce90();  // GUI::CHUD::refl_GetClass

void fn_7100b2c58c(UnkMenu<0x2d0>* self) {
    UnkGuiObject* obj =
        static_cast<UnkGuiObject*>(fn_71011b69d8(g_unk_717271e8f8, CRntString("IconsHudComposition")).Get());
    if (!obj)
        return;
    self->composition = fn_710009d50c(obj->Type(), fn_71011dce90()) ? obj : nullptr;
    fn_71011cdce8(self->composition, 0);
    fn_71011cf2a4(self->composition, 0);
    fn_71011cf364(self->composition, 0);
    fn_71011cde44(self->composition, 2);
    fn_71011cef74(self->composition, self);
    fn_71011d968c(self, self->composition);
}
