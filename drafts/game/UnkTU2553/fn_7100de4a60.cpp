// Unit 2553 (CGameManager::CTunableGameManager): whether the in-game mode is active and one of four
// inputs is pressed. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2726.h"
#include "game/UnkTU2730.h"
#include "game/nn/time.h"
#include "types.h"

struct UnkTU2553Mode {
    u8 pad_0x0[8];
    base::global::CRntString name;
    bool field_0x38;
    u8 pad_0x39[2];
    bool field_0x3b;
};

struct UnkTU2553Input {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28();
    virtual bool IsPressed(u32 button, u32 mask);
};

static inline UnkSoundHolder* SoundHolder_e4a60() {
    if (!g_unk_7101d47340) {
        auto* h = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(h);
        g_unk_7101d47340 = h;
    }
    return g_unk_7101d47340;
}

bool fn_7100de4a60() {
    auto* mode = static_cast<UnkTU2553Mode*>(g_unk_7101d47310->field_0x498);
    if (!mode || !CRntString_EqualsCString(&mode->name, "INGAME") || !mode->field_0x38 || mode->field_0x3b)
        return false;
    if (fn_7101196fd8(g_unk_7101d47330))
        return false;
    if (!fn_710118a8d0(reinterpret_cast<UnkPlacedObj*>(mode)))
        return false;
    auto* input = *reinterpret_cast<UnkTU2553Input**>(SoundHolder_e4a60());
    if (!input)
        return false;
    if (!input->vf_0x28())
        return false;
    return input->IsPressed(0x20, 0xfd) || input->IsPressed(0x10, 0xfd) || input->IsPressed(0x40, 0xfd) ||
           input->IsPressed(0x80, 0xfd);
}
