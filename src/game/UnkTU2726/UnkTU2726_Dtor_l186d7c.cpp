// Unit 2726 (gui/scripts/msemenu_ingame.lua): the in-game menu's destructor. It unregisters the two
// render items the menu put on layer 2, unmounts its packs, drops its handles, empties its three arrays and
// its callback, and destroys its strings before the music trigger base. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101cb50f8[];
// The two render items the in-game menu shows on layer 2.
__attribute__((visibility("hidden"))) extern UnkItemBase_71011a91c4* g_unk_7172a0b8f0[2];

void fn_7101187e2c();
void fn_7100e02984(UnkGameManager* manager);

// A callback registered with a deferred set: unregistered when destroyed.
struct UnkIngameCallback_l186d7c {
    base::UnkDeferredSet<void>* set;
    void* object;
    void (*fn)();

    ~UnkIngameCallback_l186d7c() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        fn = nullptr;
        object = nullptr;
    }
};

struct UnkIngameMenu_l186d7c {
    void** vtable;
    u8 pad_0x8[0x50];
    CRntString field_0x58;
    CRntString field_0x88;
    CRntString field_0xb8;
    u8 pad_0xe8[0x8];
    UnkWeakHandle* field_0xf0;
    UnkWeakHandle* field_0xf8;
    UnkWeakHandle* field_0x100;
    u8 field_0x108[0xb0];
    UnkIngameCallback_l186d7c callback;  // 0x1b8
    base::global::CRntVector<void**> lists[3];  // 0x1d0
    u8 pad_0x248[0x10];
    UnkWeakHandle* field_0x258;
};
static_assert(__builtin_offsetof(UnkIngameMenu_l186d7c, field_0x258) == 0x258, "layout");

static inline void Release(UnkWeakHandle* handle) {
    if (handle && --handle->ref_count == 0)
        handle->vf_0x10();
}

void fn_7101186d7c(UnkIngameMenu_l186d7c* self) {
    self->vtable = &g_unk_7101cb50f8[2];
    for (UnkItemBase_71011a91c4*& item : g_unk_7172a0b8f0) {
        fn_71002199d8(g_unk_7101d47300, item, 2);
        item = nullptr;
    }
    fn_7101187e2c();
    fn_7100e02984(g_unk_7101d47310);
    Release(self->field_0x258);
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    self->callback.~UnkIngameCallback_l186d7c();
    fn_710002f67c(self->field_0x108);
    Release(self->field_0x100);
    Release(self->field_0xf8);
    Release(self->field_0xf0);
    CRntString_Destroy(&self->field_0xb8);
    CRntString_Destroy(&self->field_0x88);
    CRntString_Destroy(&self->field_0x58);
    fn_71011867d4(self);
}
