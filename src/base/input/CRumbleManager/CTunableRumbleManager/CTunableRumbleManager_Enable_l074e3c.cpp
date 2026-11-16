// The rumble manager's unit (tools/tu_map.py unit 51, base::input::CRumbleManager::CTunableRumbleManager):
// sets the enabled flag at +0x2209 and, when it is cleared (disabled), fades out (0.2 s) every effect playing on any
// channel. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/input/UnkRumbleManager.h"
#include "game/UnkTU50.h"
#include "types.h"

// A channel mask whose constructor isn't constexpr, so its statics are guarded.
struct UnkRumbleChannels_l074e3c {
    u32 bits;
    UnkRumbleChannels_l074e3c() : bits(0) {}
    UnkRumbleChannels_l074e3c(u32 b) : bits(b) {}
    UnkRumbleChannels_l074e3c(const UnkRumbleChannels_l074e3c& o) : bits(o.bits) {}
};

inline const UnkRumbleChannels_l074e3c& UnkRumbleChannelsNone_l074e3c() {
    static UnkRumbleChannels_l074e3c s_none asm("g_unk_7101d58738");
    return s_none;
}

inline const UnkRumbleChannels_l074e3c& UnkRumbleChannelsAll_l074e3c() {
    static UnkRumbleChannels_l074e3c s_all asm("g_unk_7101d58748")(~UnkRumbleChannelsNone_l074e3c().bits & 0x1f);
    return s_all;
}

struct UnkRumbleEffect_l074e3c {
    u8 pad_0x0[0x54];
    u32 channel;  // 0x54
};

struct UnkTunableRumbleManager_l074e3c {
    u8 pad_0x0[0x2158];
    base::global::CRntVector<UnkRumbleEffect_l074e3c*> effects;  // 0x2158
    u8 pad_0x2180[0x2209 - 0x2180];
    bool enabled;  // 0x2209
};
static_assert(__builtin_offsetof(UnkTunableRumbleManager_l074e3c, enabled) == 0x2209, "layout");

// The manager as this function reads it (base/input/UnkRumbleManager.h spells out only its constructor's view).
void fn_7100074e3c(base::input::UnkRumbleManager* manager, bool enabled) {
    auto* self = reinterpret_cast<UnkTunableRumbleManager_l074e3c*>(manager);
    if (self->enabled == enabled)
        return;
    self->enabled = enabled;
    if (enabled)
        return;
    const UnkRumbleChannels_l074e3c channels = UnkRumbleChannelsAll_l074e3c();
    UnkRumbleEffect_l074e3c** it = self->effects.data;
    UnkRumbleEffect_l074e3c** end = it + self->effects.count;
    for (; it != end; ++it) {
        UnkRumbleEffect_l074e3c* effect = *it;
        u32 bit = 1u << effect->channel;
        if (channels.bits & bit)
            fn_7100072328(effect, 0.2f, 0.0f);
    }
}
