// CAccessPointComponent's unit (tools/tu_map.py unit 1581): the access point's use step. Keeps a weak handle
// to the user (+0xe8); with the question flag (+0x590) set it asks the save-station question with the
// callbacks fn_7100a74680 and fn_7100a74684, otherwise runs slot 0x298 or 0x2a8 as slot 0x188 answers.
// Always returns false. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkAp_la70710;
void fn_7100a74680(UnkAp_la70710* self);
void fn_7100a74684(UnkAp_la70710* self);

// A callback: an object and a function to call on it.
struct UnkApCallback_la70710 {
    UnkAp_la70710* object;
    void (*function)(UnkAp_la70710*);
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;

struct UnkGuiRef_la70710 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

struct UnkAp_la70710 {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x590 - 0xf0];
    bool ask;  // 0x590

    template <class R>
    R CallSlot(u32 offset) {
        return reinterpret_cast<R (*)(UnkAp_la70710*)>(vtable[offset / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkAp_la70710, ask) == 0x590, "layout");

bool fn_7100a70710(UnkAp_la70710* self, UnkWeakTarget* user) {
    if (!user)
        return false;
    AssignRef(&self->user, fn_7100d78378(user));
    if (self->ask) {
        UnkGuiRef_la70710* ref = static_cast<UnkGuiRef_la70710*>(g_unk_7101d47310->field_0x2258);
        if (!ref)
            return false;
        UnkGuiComposition_ovb0090* gui = ref->composition;
        if (!gui)
            return false;
        CStrId back("#GUI_B_BACK", true);
        CStrId accept("#GUI_A_ACCEPT", true);
        CStrId text("#GUI_SAVESTATION_QUESTION", true);
        UnkApCallback_la70710 accept_callback = {self, fn_7100a74680};
        UnkApCallback_la70710 back_callback = {self, fn_7100a74684};
        fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
        return false;
    }
    if (self->CallSlot<bool>(0x188))
        self->CallSlot<void>(0x298);
    else
        self->CallSlot<void>(0x2a8);
    return false;
}
