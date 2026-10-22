// Four menu GUI setup functions: each looks up a named composition through the GUI registry,
// keeps the object at `Offset` (dropping the temporary handle), configures it and hands it to
// the menu itself (compare fn_7100b4d0ec, which hands it to the main menu).
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

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

template <u32 Offset, s32 Value>
__attribute__((always_inline)) static inline void SetUp(UnkMenu<Offset>* self, const char* name) {
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
    void* const& composition = self->composition;
    if (composition) {
        fn_71011cdce8(composition, 0);
        fn_71011cf2a4(composition, 0);
        fn_71011cf364(composition, Value);
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
