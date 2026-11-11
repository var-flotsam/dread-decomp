// A function in the GUI::CMapMenuController unit: fn_7100b420b8 runs and clears the pending callback at
// +0xb88 (object) / +0xb90 (function) with bit 0 of `flag`, clears +0x234, moves to the entry the object at
// +0x928 names (its +0x10; 0x7fffffff without one, fn_7100b3e5ac), drops the key at 0x7172763d98 from the
// GUI root's list and puts the in-game menu back in state 1 (fn_7100b2eaac). Names and layouts are
// placeholders.
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1689.h"
#include "types.h"

using base::global::CStrId;

struct UnkStateOwner;
extern CStrId g_unk_7172763d98 __attribute__((visibility("hidden")));

struct UnkMapMenuEntry {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkMapMenuCallback {
    void* object;
    void (*function)(void* object, bool flag);
};

struct UnkMapMenuControllerClose {
    u8 pad_0x0[0x234];
    u32 field_0x234;
    u8 pad_0x238[0x928 - 0x238];
    UnkMapMenuEntry* field_0x928;
    u8 pad_0x930[0xb88 - 0x930];
    UnkMapMenuCallback callback;  // 0xb88
};

void fn_7100b420b8(UnkMapMenuControllerClose* self, u32 flag) {
    self->callback.function(self->callback.object, flag & 1);
    UnkMapMenuCallback none = {nullptr, nullptr};
    self->callback = none;
    self->field_0x234 = 0;
    UnkMapMenuEntry* entry = self->field_0x928;
    fn_7100b3e5ac(self, entry ? entry->field_0x10 : 0x7fffffff, 0);
    fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172763d98);
    fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(fn_7100deac84(g_unk_7101d47310)), 1, true, true);
}
