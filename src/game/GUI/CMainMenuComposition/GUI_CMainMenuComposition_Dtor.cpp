// GUI::CMainMenuComposition (unit 1688 of tools/tu_map.py, the name from reflection): the
// destructor and the deleting destructor. Three registrations in deferred sets (+0x260, +0x248,
// +0x230, last first) are dropped, then the display object's destructor runs (fn_71011d8e90).
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "game/UnkGuiDisplayObject.h"

struct UnkTag_7100b36bf8;
extern char g_unk_7101a9c3a8[];  // the composition's vtable

template <>
struct base::UnkDtorTraits<UnkTag_7100b36bf8> {
    static char* vtable() { return g_unk_7101a9c3a8; }
    static void base(char* p) { fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(p)); }
};

// A registration in a deferred set (the set pointer, then two cleared words).
struct UnkSetEntry_7100b36bf8 {
    base::UnkDeferredSet<void>* set;
    void* field_0x8;
    void* field_0x10;
    ~UnkSetEntry_7100b36bf8() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

__attribute__((flatten)) void fn_7100b36bf8(char* self) {
    base::refl_Dtor<UnkTag_7100b36bf8, base::UnkMemberAt<UnkSetEntry_7100b36bf8, 0x260>,
                    base::UnkMemberAt<UnkSetEntry_7100b36bf8, 0x248>,
                    base::UnkMemberAt<UnkSetEntry_7100b36bf8, 0x230>>(self);
}

__attribute__((flatten)) void fn_7100b36c88(char* self) {
    base::refl_DeletingDtor<UnkTag_7100b36bf8, base::UnkMemberAt<UnkSetEntry_7100b36bf8, 0x260>,
                            base::UnkMemberAt<UnkSetEntry_7100b36bf8, 0x248>,
                            base::UnkMemberAt<UnkSetEntry_7100b36bf8, 0x230>>(self);
}
