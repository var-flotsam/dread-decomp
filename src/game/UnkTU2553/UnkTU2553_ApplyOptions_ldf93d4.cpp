// Unit 2553 (CGameManager::CTunableGameManager): reads three option values from the game manager's
// blackboard and applies them: the sound volume, the music manager's level and the rumble switch.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/snd/CSoundSystem.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CStrId;

// The option properties this reads.

struct UnkOptionsManager_ldf93d4 {
    u8 pad_0x0[0x820];
    CBlackboard options;  // 0x820
};

// Reads the int `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadInt_ldf93d4(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def, s32* out) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395a30(s, &qid.id, out);
}

static inline void* MusicManager_ldf93d4() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

struct UnkMusicVtable_ldf93d4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(f32 value);
};

static inline base::input::UnkRumbleManager* RumbleManager_ldf93d4() {
    base::input::UnkRumbleManager* manager = g_unk_7101d47348;
    if (!manager) {
        manager = static_cast<base::input::UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(manager);
        g_unk_7101d47348 = manager;
    }
    return manager;
}

void fn_7100df93d4(UnkGameManager* manager) {
    auto* self = reinterpret_cast<UnkOptionsManager_ldf93d4*>(manager);
    CBlackboard* blackboard = &self->options;

    s32 volume = 10;
    ReadInt_ldf93d4(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x40a], g_unk_7172759b10,
                    &volume);
    f32 level = (volume - 1) * 0.1f;
    fn_710010806c(g_unk_7101d4d820, level);
    static_cast<UnkMusicVtable_ldf93d4*>(MusicManager_ldf93d4())->vf_0x10(0.0f);

    s32 music = 10;
    ReadInt_ldf93d4(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x40a], g_unk_7172759bd0,
                    &music);

    s32 rumble = 2;
    ReadInt_ldf93d4(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x40a], g_unk_7172759b90,
                    &rumble);
    fn_7100074e3c(RumbleManager_ldf93d4(), rumble == 2);
}
