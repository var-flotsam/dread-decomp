// Unit 2726 (gui/scripts/msemenu_ingame.lua): the player's death as the in-game menu handles it. It fades the
// music out, plays a sound, stops the rumble, asks the script for the death overrides and tells the player's
// animation and timeline about the death. Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "game/UnkMask.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU56.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::Key;
using unk_prop_bag::UnkPropBag;

// Calls the script function `name`, collecting what it returns in `results`.
bool fn_7100dfb758(UnkPropBag* results, const char* name, const char* fmt, ...);
void fn_7100108b1c(UnkSoundMgr* manager);
void fn_710010825c(UnkSoundMgr* manager, const u32* id, f32 time);

struct UnkIngameMenu_l18cdfc {
    u8 pad_0x0[0x260];
    u32 field_0x260;
    f32 field_0x264;
};

struct UnkPlayer_l18cdfc {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkGameManagerView_l18cdfc {
    u8 pad_0x0[0x29a0];
    UnkWeakHandle* player;  // 0x29a0
};

// A value a script call returned: where it is in the bag's stream, the hash of its name and its type.
struct UnkScriptValue_l18cdfc {
    u64 offset;
    u64 key;
    s32 type;  // 'b' for a bool
};

struct UnkZero_l18cdfc {
    u32 value;
    UnkZero_l18cdfc() : value(0) {}
};

// Every music channel: the complement of a zero, limited to nine bits.
inline const UnkZero_l18cdfc& MusicChannelsNone_l18cdfc() {
    static UnkZero_l18cdfc s_value;
    return s_value;
}
// Every rumble channel: the complement of a zero, limited to five bits.
inline const UnkZero_l18cdfc& RumbleChannelsNone_l18cdfc() {
    static UnkZero_l18cdfc s_value;
    return s_value;
}
inline u32 RumbleChannelsAll_l18cdfc() {
    static u32 s_mask = ~RumbleChannelsNone_l18cdfc().value & 0x1f;
    return s_mask;
}

static inline UnkScriptValue_l18cdfc* Values_l18cdfc(UnkPropBag& results) {
    return reinterpret_cast<UnkScriptValue_l18cdfc*>(results.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkScriptValue_l18cdfc* FindValue_l18cdfc(UnkPropBag& results, const CStrId& id) {
    UnkScriptValue_l18cdfc* values = Values_l18cdfc(results);
    u64 key = id.data->Key();
    UnkIter<UnkScriptValue_l18cdfc> first{values};
    UnkIter<UnkScriptValue_l18cdfc> last{&values[results.values.count]};
    return std::find_if(first, last, [key](const UnkScriptValue_l18cdfc& v) { return v.key == key; }).p;
}

void fn_710118cdfc(UnkIngameMenu_l18cdfc* self) {
    self->field_0x260 = 7;
    self->field_0x264 = 0.4f;

    void* music = g_unk_71726bc560;
    if (!music) {
        music = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(music);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(music);
    }
    fn_7100fd7b44(static_cast<UnkMusicMgr_b0019*>(music), UnkMask(2), UnkMaskOf<UnkMusicMaskTag>::All(), 0.4f);

    fn_7100108b1c(g_unk_7101d4d820);
    alignas(8) u32 sound = 0xef;
    fn_710010825c(g_unk_7101d4d820, &sound, 0.2f);

    base::input::UnkRumbleManager* rumble = g_unk_7101d47348;
    if (!rumble) {
        rumble = static_cast<base::input::UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(rumble);
        g_unk_7101d47348 = rumble;
    }
    alignas(8) u32 channels = RumbleChannelsAll_l18cdfc();
    fn_7100074f94(rumble, &channels);

    bool overrides = true;
    UnkPropBag results;
    if (fn_7100dfb758(&results, "CurrentScenario.GetOnDeathOverrides", g_unk_710162e2ed)) {
        UnkScriptValue_l18cdfc* it = FindValue_l18cdfc(results, Key(0x1218));
        if (it != &Values_l18cdfc(results)[results.values.count] && it->type == 'b') {
            fn_710007c00c(&results.lock, it->offset, 0);
            fn_710007c70c(&results.lock, &overrides, 1);
        }
    }

    UnkGameManagerView_l18cdfc* manager = reinterpret_cast<UnkGameManagerView_l18cdfc*>(g_unk_7101d47310);
    UnkPlayer_l18cdfc* player = reinterpret_cast<UnkPlayer_l18cdfc*>(manager->player->target);
    void* animation = player->animation;
    u64 hash = Key(0x1d78).data->Key();
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg type;
    type.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &g_unk_7172769740, &type);
    u64 death = fn_7100001570("death", 5);
    fn_7100d7c94c(reinterpret_cast<UnkActor*>(player), &death);
}
