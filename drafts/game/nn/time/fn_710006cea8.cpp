// Unit 45 (tools/tu_map.py, named after nn::time): sets the sound holder's rumble flag and, when it
// changes, applies the "RumbleTunable" setting the user data blackboard saves (or the manager's
// current one) to the rumble manager. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "game/CBlackboard.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2768.h"
#include "game/nn/time.h"
#include "types.h"

using base::global::CStrId;
using base::input::UnkRumbleManager;

bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);
extern UnkRumbleManager* g_unk_7101d47348;

struct UnkSoundHolderRumble_l06cea8 {
    u8 pad_0x0[0x40];
    bool rumble;  // 0x40
};

// The manager's flags at +0x2208 (the header has them as one u16 for now): the second says whether
// rumble is enabled.
struct UnkRumbleManagerFlags_l06cea8 {
    u8 pad_0x0[0x2209];
    bool enabled;  // 0x2209
};

static inline UnkRumbleManager* RumbleManager() {
    UnkRumbleManager* manager = g_unk_7101d47348;
    if (!manager) {
        manager = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(manager);
        g_unk_7101d47348 = manager;
    }
    return manager;
}

void fn_710006cea8(UnkSoundHolder* holder_, bool value) {
    auto* holder = reinterpret_cast<UnkSoundHolderRumble_l06cea8*>(holder_);
    if (holder->rumble == value)
        return;
    holder->rumble = value;
    static CStrId s_section("USER_DATA", true);
    bool enabled = reinterpret_cast<UnkRumbleManagerFlags_l06cea8*>(RumbleManager())->enabled;
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    CBlackboard* blackboard = fn_710120384c(owner);
    if (fn_71010474f4(blackboard, s_section, CStrId("RumbleTunable", true))) {
        static CStrId s_name("RumbleTunable", true);
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, s_section))
            fn_710006d11c(section, &s_name, &enabled);
    }
    fn_7100074e3c(RumbleManager(), enabled);
}
