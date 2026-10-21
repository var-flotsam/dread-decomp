// Destructor of a GUI class: releases a refcounted pointer, unregisters from a deferred set,
// releases four string ids, then runs the base destructor.
#include "base/UnkDestructors.h"

struct UnkTag_71011bf1a4;
extern char g_unk_7101cb5908[];
void fn_71011d8e90(char* self);
void fn_71004f3ea4(void* set, void* item);

template <>
struct base::UnkDtorTraits<UnkTag_71011bf1a4> {
    static char* vtable() { return g_unk_7101cb5908; }
    static void base(char* p) { fn_71011d8e90(p); }
};

// A registration in a deferred set (the set pointer, then two cleared words).
struct UnkSetEntry_71011bf1a4 {
    void* set;
    void* field_0x8;
    void* field_0x10;
    ~UnkSetEntry_71011bf1a4() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

__attribute__((flatten)) void fn_71011bf1a4(char* self) {
    base::refl_Dtor<UnkTag_71011bf1a4, base::UnkRefPtrAt<0x270>, base::UnkMemberAt<UnkSetEntry_71011bf1a4, 0x258>,
                    base::UnkStrIdAt<0x248>, base::UnkStrIdAt<0x240>, base::UnkStrIdAt<0x238>,
                    base::UnkStrIdAt<0x230>>(self);
}
