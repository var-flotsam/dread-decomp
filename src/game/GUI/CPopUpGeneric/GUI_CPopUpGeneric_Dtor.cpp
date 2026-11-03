// GUI::CPopUpGeneric (unit 1709 of tools/tu_map.py, the name from reflection): the destructor.
// Two registrations in deferred sets (+0x3c0, +0x3a8) are dropped, the member at +0x2a8 is
// destroyed (fn_7100b61264), then two id pairs (+0x290, +0x278, fn_7100026f70) and two refcounted
// pointers (+0x240, +0x238); last the display object's destructor runs (fn_71011d8e90).
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "game/UnkGuiDisplayObject.h"

struct UnkTag_7100b6117c;
extern char g_unk_7101a9da40[];  // the pop-up's vtable

struct UnkIdPair;
void fn_7100026f70(UnkIdPair*);
struct UnkPopUpMember_7100b61264;
void fn_7100b61264(UnkPopUpMember_7100b61264* self);

template <>
struct base::UnkDtorTraits<UnkTag_7100b6117c> {
    static char* vtable() { return g_unk_7101a9da40; }
    static void base(char* p) { fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(p)); }
};

// A registration in a deferred set (the set pointer, then two cleared words).
struct UnkSetEntry_7100b6117c {
    base::UnkDeferredSet<void>* set;
    void* field_0x8;
    void* field_0x10;
    ~UnkSetEntry_7100b6117c() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

// A member destroyed by an out-of-line function.
template <class T, void (*Destroy)(T*), u32 Offset>
struct UnkCalledAt_7100b6117c {
    static void Release(char* self) { Destroy(reinterpret_cast<T*>(self + Offset)); }
};

__attribute__((flatten)) void fn_7100b6117c(char* self) {
    base::refl_Dtor<UnkTag_7100b6117c, base::UnkMemberAt<UnkSetEntry_7100b6117c, 0x3c0>,
                    base::UnkMemberAt<UnkSetEntry_7100b6117c, 0x3a8>,
                    UnkCalledAt_7100b6117c<UnkPopUpMember_7100b61264, fn_7100b61264, 0x2a8>,
                    UnkCalledAt_7100b6117c<UnkIdPair, fn_7100026f70, 0x290>,
                    UnkCalledAt_7100b6117c<UnkIdPair, fn_7100026f70, 0x278>, base::UnkRefPtrAt<0x240>,
                    base::UnkRefPtrAt<0x238>>(self);
}
