// GUI::CMainMenu's unit (tools/tu_map.py): the menu's destructor (fn_7100b34700). Its vtable and the cleared
// count of the pointer list at +0x2b8, then the members in reverse: five small listeners (vtable
// g_unk_7101a9c360, one word) from +0x548 down to +0x508, a reference pair at +0x4e8 (vtable
// g_unk_7101a9cf08, as fn_7100b4edcc in src/game/UnkUnit1694RefPair_Dtor_b44.cpp destroys it), another
// listener at +0x4d8, the members at +0x3e0 (fn_7100b49a64), listeners at +0x3d0, +0x3c0, +0x3b0, +0x398, +0x350
// and +0x340, the member at +0x2f8 (fn_7100b52460), a listener at +0x2e0, the pointer list, the string at
// +0x288 and the two callbacks at +0x248 and +0x230 (each leaving its deferred set, fn_71004f3ea4); then the
// display object's destructor (fn_71011d8e90). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

extern char g_unk_7101a9c0f0[];  // the menu's vtable
extern char g_unk_7101a9c360[];  // the listeners' vtable
extern char g_unk_7101a9cf08[];  // the reference pair's vtable

struct UnkMainMenuMember3e0_ovb0126;
void fn_7100b49a64(UnkMainMenuMember3e0_ovb0126* self);
struct UnkMainMenuMember2f8_ovb0126;
void fn_7100b52460(UnkMainMenuMember2f8_ovb0126* self);

struct UnkMainMenuListener_ovb0126 {
    char* vtable;
    u64 field_0x8;
    __attribute__((always_inline)) ~UnkMainMenuListener_ovb0126() {
        vtable = g_unk_7101a9c360 + 0x10;
        field_0x8 = 0;
    }
};

struct UnkMainMenuRefObj_ovb0126 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();  // 0x10
    union {
        s32 ref_count;
    };
};

struct UnkMainMenuRef_ovb0126 {
    UnkMainMenuRefObj_ovb0126* ptr;
    __attribute__((always_inline)) ~UnkMainMenuRef_ovb0126() {
        if (UnkMainMenuRefObj_ovb0126* p = ptr) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkMainMenuRefPair_ovb0126 : UnkMainMenuListener_ovb0126 {
    UnkMainMenuRef_ovb0126 field_0x10;
    UnkMainMenuRef_ovb0126 field_0x18;
    __attribute__((always_inline)) ~UnkMainMenuRefPair_ovb0126() { vtable = g_unk_7101a9cf08 + 0x10; }
};

struct UnkMainMenuMember3e0Holder_ovb0126 {
    u8 storage[0xf8];
    __attribute__((always_inline)) ~UnkMainMenuMember3e0Holder_ovb0126() {
        fn_7100b49a64(reinterpret_cast<UnkMainMenuMember3e0_ovb0126*>(this));
    }
};

struct UnkMainMenuMember2f8Holder_ovb0126 {
    u8 storage[0x48];
    __attribute__((always_inline)) ~UnkMainMenuMember2f8Holder_ovb0126() {
        fn_7100b52460(reinterpret_cast<UnkMainMenuMember2f8_ovb0126*>(this));
    }
};

struct UnkMainMenuString_ovb0126 {
    u8 storage[0x30];
    __attribute__((always_inline)) ~UnkMainMenuString_ovb0126() {
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
};

struct UnkMainMenuPtrList_ovb0126 : base::UnkVector<void*> {
    __attribute__((always_inline)) ~UnkMainMenuPtrList_ovb0126() { Free(); }
};

struct UnkMainMenuCallback_ovb0126 {
    base::UnkDeferredSet<void>* set;
    void* owner;
    void (*fn)();
    __attribute__((always_inline)) ~UnkMainMenuCallback_ovb0126() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        fn = nullptr;
        owner = nullptr;
    }
};

struct UnkMainMenuBase_ovb0126 {
    char* vtable;
    u8 pad_0x8[0x228];
    __attribute__((always_inline)) ~UnkMainMenuBase_ovb0126() {
        fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(this));
    }
};

struct UnkMainMenu_ovb0126 : UnkMainMenuBase_ovb0126 {
    UnkMainMenuCallback_ovb0126 field_0x230;
    UnkMainMenuCallback_ovb0126 field_0x248;
    u8 pad_0x260[0x28];
    UnkMainMenuString_ovb0126 field_0x288;
    UnkMainMenuPtrList_ovb0126 field_0x2b8;
    UnkMainMenuListener_ovb0126 field_0x2e0;
    u8 pad_0x2f0[0x8];
    UnkMainMenuMember2f8Holder_ovb0126 field_0x2f8;
    UnkMainMenuListener_ovb0126 field_0x340;
    UnkMainMenuListener_ovb0126 field_0x350;
    u8 pad_0x360[0x38];
    UnkMainMenuListener_ovb0126 field_0x398;
    u8 pad_0x3a8[0x8];
    UnkMainMenuListener_ovb0126 field_0x3b0;
    UnkMainMenuListener_ovb0126 field_0x3c0;
    UnkMainMenuListener_ovb0126 field_0x3d0;
    UnkMainMenuMember3e0Holder_ovb0126 field_0x3e0;
    UnkMainMenuListener_ovb0126 field_0x4d8;
    UnkMainMenuRefPair_ovb0126 field_0x4e8;
    UnkMainMenuListener_ovb0126 field_0x508;
    UnkMainMenuListener_ovb0126 field_0x518;
    UnkMainMenuListener_ovb0126 field_0x528;
    UnkMainMenuListener_ovb0126 field_0x538;
    UnkMainMenuListener_ovb0126 field_0x548;

    __attribute__((always_inline)) ~UnkMainMenu_ovb0126() {
        vtable = g_unk_7101a9c0f0 + 0x10;
        field_0x2b8.count = 0;
    }
};
static_assert(__builtin_offsetof(UnkMainMenu_ovb0126, field_0x2b8) == 0x2b8, "layout");
static_assert(__builtin_offsetof(UnkMainMenu_ovb0126, field_0x3e0) == 0x3e0, "layout");
static_assert(__builtin_offsetof(UnkMainMenu_ovb0126, field_0x548) == 0x548, "layout");

void fn_7100b34700(UnkMainMenu_ovb0126* self) {
    self->~UnkMainMenu_ovb0126();
}
