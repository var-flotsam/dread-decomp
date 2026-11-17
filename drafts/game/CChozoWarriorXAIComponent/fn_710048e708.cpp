// CChozoWarriorXAIComponent unit: what the component does when its enemy is defeated. Unless the mode
// flag is set and while the mission log has no "#MLOG_DEFEAT_BOSS_CWXE_SHIP" entry, the defeat message of
// the current scenario goes to the game manager's callback set (+0x2128); then the music is told, the
// boss base handler runs and the music fades. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};

// The object behind the game manager's +0x608, as the scenario name is read from it.
struct UnkScenario_l48e708 {
    u8 pad_0x0[0x18];
    CStrId name;  // 0x18
};

struct UnkMlogMessage_l48e708 {
    u32 kind;
    CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    UnkMlogMessage_l48e708(CStrId t) : kind(0), text(t), args(0xc) {}
};

struct UnkMissionLog;
struct UnkStateOwner;
struct UnkMlogMessage_b42a1;
struct UnkSelf;

UnkMissionLog* fn_7100b2e338(UnkStateOwner* self);
bool fn_7100b59b18(UnkMissionLog* log, const CStrId& entry);
void fn_7100fdbd84(void* manager, u32 a);
void fn_710048ecbc(UnkSelf* self);

static inline void* MusicManager_l48e708() {
    if (!g_unk_71726bc560) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(m);
    }
    return g_unk_71726bc560;
}

// Whether the id names this string, counting the read.
inline bool NameIs_l48e708(const CStrId& id, const CRntString& str) {
    ++id.data->reads;
    return id.data->Key() == static_cast<u64>(str.key);
}

void fn_710048e708(void* self, void* arg) {
    if (!g_unk_71726bc570->field_0xb8) {
        UnkMissionLog* log = fn_7100b2e338(reinterpret_cast<UnkStateOwner*>(fn_7100deac84(g_unk_7101d47310)));
        if (log && !fn_7100b59b18(log, CStrId("#MLOG_DEFEAT_BOSS_CWXE_SHIP", true))) {
            if (auto* scenario = reinterpret_cast<UnkScenario_l48e708*>(g_unk_7101d47310->field_0x608)) {
                CStrId text(base::global::UnkStrIdInline(), nullptr, false);
                CStrId name = scenario->name;
                if (NameIs_l48e708(name, CRntString("s060_quarantine")))
                    text = CStrId("#MLOG_DEFEAT_BOSS_CWX_QUARANTINE", true);
                else if (NameIs_l48e708(name, CRntString("s050_forest")))
                    text = CStrId("#MLOG_DEFEAT_BOSS_CWX_FOREST", true);
                else if (NameIs_l48e708(name, CRntString("s010_cave")))
                    text = CStrId("#MLOG_DEFEAT_BOSS_CWX_CAVE", true);
                else if (NameIs_l48e708(name, CRntString("s080_shipyard")))
                    text = CStrId("#MLOG_DEFEAT_BOSS_CWX_SHIP", true);
                UnkMlogMessage_l48e708 message(text);
                fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                              reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
                message.args.Free();
            }
        }
    }
    fn_7100fdbd84(MusicManager_l48e708(), 0);
    fn_71003aea40(static_cast<UnkBossAIComponent*>(self), arg);
    fn_7100fdcf2c(MusicManager_l48e708(), 0, 0, 0.2f);
    fn_7100fdcf2c(MusicManager_l48e708(), 1, 0, 0.2f);
    fn_710048ecbc(static_cast<UnkSelf*>(self));
}
