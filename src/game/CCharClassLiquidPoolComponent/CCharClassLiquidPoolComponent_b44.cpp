// CCharClassLiquidPoolComponent (tools/tu_map.py; the name from reflection): after the base step
// (fn_7100a45cac), passes vf_0x208 whether the pool is active (the game manager's flag at +0x2441,
// else its own at +0xc11), moves its registration (+0xbe8) to the game manager's deferred set at
// +0x1e28 and runs fn_7100900cf4. Names and layout are placeholders.
#include "base/UnkDeferredSet.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkLiquidPool_b44a2 {
    void** vtable;
    u8 pad_0x8[0xbe0];
    base::UnkDeferredSet<void>* field_0xbe8;  // the set it is registered in
    u8 pad_0xbf0[0x21];
    bool field_0xc11;
};

void fn_7100901144(UnkLiquidPool_b44a2* self) {
    fn_7100a45cac(self);
    bool active = reinterpret_cast<char*>(g_unk_7101d47310)[0x2441] ? true : self->field_0xc11;
    reinterpret_cast<void (*)(UnkLiquidPool_b44a2*, bool)>(self->vtable[0x208 / 8])(self, active);
    base::UnkDeferredSet<void>** slot = &self->field_0xbe8;
    char* manager = reinterpret_cast<char*>(g_unk_7101d47310);
    if (*slot) {
        fn_71004f3ea4(*slot, slot);
        *slot = nullptr;
    }
    *slot = reinterpret_cast<base::UnkDeferredSet<void>*>(manager + 0x1e28);
    fn_71004f3cf0(*slot, slot);
    fn_7100900cf4(self);
}
