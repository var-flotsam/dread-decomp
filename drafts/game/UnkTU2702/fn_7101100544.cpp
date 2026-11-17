// CAIComponent unit (tools/tu_map.py unit 2702): warns that a blockable attack is coming: plays the effect
// the owner's CCharClassTryToLaunchBlockableAttackWarningTrack names (or "blockableattackwarning") and the
// blockable-attack audio preset. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkWorld;
struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c45c(UnkEntityWithSound* self, void* a, void* b, bool c);

struct UnkAIOwner_l100544 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x58];
    UnkWorld* fx;  // 0x280
};

struct UnkAITunables_l100544 {
    u8 pad_0x0[0x438];
    f32 warning_volume;  // 0x438
};

struct UnkAI_l100544 {
    u8 pad_0x0[0x30];
    UnkAIOwner_l100544* owner;  // 0x30
    u8 pad_0x38[0x1610 - 0x38];
    UnkAITunables_l100544* tunables;  // 0x1610
    u8 pad_0x1618[0x1451c - 0x1618];
    u32 field_0x1451c;
};

// An audio preset: its ids and the resource they resolve to. Unload() drops the resource before the
// ids go.
struct UnkPresetSound_l100544 {
    CStrId type;
    CFilePathStrId path;
    UnkCountedResource* resource;

    UnkPresetSound_l100544() : type(g_unk_7101d470e0), path(g_unk_7101d470e8), resource(nullptr) {}

    UnkStrIdPathResource* Resource() { return reinterpret_cast<UnkStrIdPathResource*>(this); }
    bool IsSet() const {
        return type.data != &g_unk_7101d47108->empty && path.data != &g_unk_7101d47200->empty;
    }
};

static void Unload(UnkPresetSound_l100544* sound) {
    fn_7100027044(sound->Resource());
    if (sound->resource && --sound->resource->ref_count == 0)
        fn_710001737c(sound->resource);
}

struct UnkSoundParams_l100544 {
    f32 volume;
    f32 field_0x4;
    bool field_0x8;
};

void fn_7101100544(UnkAI_l100544* self) {
    UnkAIOwner_l100544* owner = self->owner;
    UnkWorld* fx = owner->fx;
    UnkAnimationComponent* animation = owner->animation;
    if (fx && animation) {
        UnkTrackQuery query = {fn_7100c54e58(), nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        UnkTrackRecord* record = query.result;
        u64 effect = record && record->sources.data != &g_unk_7101d47108->empty
                         ? record->sources.data->Key()
                         : fn_7100001570("blockableattackwarning", 22);
        fn_710114fe8c(fx, effect, 1, 1);
    }

    static f32 s_volume asm("g_unk_7172a062c4") = self->tunables->warning_volume;
    UnkPresetSound_l100544 sound;
    if (!sound.IsSet()) {
        static CRntString s_preset asm("g_unk_7172a063d0")("audiopreset:system/snd/presets/gui/blockable");
        fn_71000267f8(sound.Resource(), UnkPathIdsFromText(s_preset).ids);
    }
    if (sound.IsSet()) {
        UnkSoundParams_l100544 params;
        params.volume = s_volume;
        params.field_0x4 = -1.0f;
        params.field_0x8 = true;
        fn_7100d7c45c(reinterpret_cast<UnkEntityWithSound*>(self->owner), &sound, &params, false);
    }
    self->field_0x1451c = 0;
    Unload(&sound);
}
