// CScorpiusAIComponent unit: what the component does when Scorpius is defeated. It posts the "defeated"
// message, runs the base handler (fn_71003aea40), registers with the AI manager's two lists, starts the
// stage-3 target's cutscene unless it is loaded, and hands the manager the death target; when the
// manager does not take it, it unregisters, shows the death cutscene title and finishes (fn_7100694dc0).
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CBossAIComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU437.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkMlogMessage_b42a1 {
    u32 kind;
    base::global::CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_b42a1(const CStrId& t) : kind(0), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_b42a1() {
        args.Free();
    }
};

struct UnkMgr_0xf18;

struct UnkTimedRef;
struct UnkWeakTarget;

struct UnkObj_710119770c;

// A string id built from a pooled string (fn_7100000850, out of line).
struct UnkScorpiusTextId_l694ea8 : CStrId {
    UnkScorpiusTextId_l694ea8(const CRntString& str, bool flag) asm("fn_7100000850");
};

struct UnkScorpiusCounted_l694ea8 : base::UnkRefCounted {
    UnkWeakTarget* target;  // 0x10
};

static inline UnkWeakTarget* TargetOf(UnkScorpiusCounted_l694ea8* p) { return p ? p->target : nullptr; }

// A link to a target, resolved from the source on first use (state 1 -> 2).
struct UnkScorpiusLazyLink_l694ea8 {
    u8 pad_0x0[0x8];
    u8 source[0x28];  // 0x8
    u32 state;        // 0x30
    u8 pad_0x34[4];
    base::UnkRefHandle handle;  // 0x38

    UnkWeakTarget* Target() {
        if (state == 1) {
            {
                base::UnkRefPtrMember fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return TargetOf(static_cast<UnkScorpiusCounted_l694ea8*>(handle.ptr));
    }
};

struct UnkListenerList_l694ea8 {
    u8 raw[0x10];
};

// The manager as seen here: the two lists its listeners join.
struct UnkScorpiusManager_l694ea8 {
    u8 pad_0x0[0x2428];
    UnkListenerList_l694ea8 listeners_0x2428;
    u8 pad_0x2438[0x70];
    UnkListenerList_l694ea8 listeners_0x24a8;
};
static_assert(__builtin_offsetof(UnkScorpiusManager_l694ea8, listeners_0x24a8) == 0x24a8, "");

struct UnkScorpiusGuiRef_l694ea8 {
    u8 pad_0x0[0x10];
    void* messages;  // 0x10
};

// The component that holds the cutscene to play (+0x310).
struct UnkScorpiusCutscene_l694ea8 {
    u8 pad_0x0[0x310];
    CFilePathStrId path;  // 0x310
};

struct UnkScorpiusTargetActor_l694ea8 {
    u8 pad_0x0[0x200];
    UnkScorpiusCutscene_l694ea8* cutscene;  // 0x200
};

struct UnkScorpiusDefeat_l694ea8 {
    u8 pad_0x0[0x8];
    UnkScorpiusCounted_l694ea8* field_0x8;
    UnkScorpiusCounted_l694ea8* field_0x10;
};

struct UnkScorpiusAI_l694ea8 {
    u8 pad_0x0[0x14dd0];
    UnkScorpiusLazyLink_l694ea8 stage3_target;  // 0x14dd0
    UnkScorpiusLazyLink_l694ea8 death_target;   // 0x14e10
    u8 pad_0x14e50[0x18];
    UnkLinkList* stage3_listener;  // 0x14e68
    u8 pad_0x14e70[0x10];
    UnkLinkList* death_listener;  // 0x14e80
    u8 pad_0x14e88[0x10];
    UnkLinkList* death_listener2;  // 0x14e98
    u8 pad_0x14ea0[0x10];
    u8 field_0x14eb0[8];
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l694ea8, death_target) == 0x14e10, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l694ea8, death_listener) == 0x14e80, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l694ea8, field_0x14eb0) == 0x14eb0, "");

void fn_7101199768(void* manager, void* entry);

// The "defeated" message log entry for `text`.
static inline UnkMlogMessage_b42a1 MlogMessage(CStrId text) { return UnkMlogMessage_b42a1(text); }

// Whether the cutscene manager has the cutscene at `path` loaded.
static inline bool IsCutsceneLoaded(void* cutscenes, CFilePathStrId path) {
    return fn_71011986e0(cutscenes, &path) != 0;
}

// The cutscene path as a string id (counting the read of its text).
static inline UnkScorpiusTextId_l694ea8 CutsceneId(const CFilePathStrId& path) {
    ++path.data->reads;
    return UnkScorpiusTextId_l694ea8(path.data->String(), true);
}

static inline void PlayCutscene(void* cutscenes, UnkScorpiusTextId_l694ea8 id) {
    fn_710119780c(static_cast<UnkObj_710119770c*>(cutscenes), id);
}
void fn_7100694dc0(UnkScorpiusAI_l694ea8* self);

// The music manager, created on first use.
static inline UnkMgr_0xf18* MusicManager() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    return static_cast<UnkMgr_0xf18*>(instance);
}

static inline UnkScorpiusManager_l694ea8* Manager() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return reinterpret_cast<UnkScorpiusManager_l694ea8*>(m);
}

static inline void Unlink(UnkLinkList** slot) {
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
}

static inline void Listen(UnkLinkList** slot, UnkListenerList_l694ea8 UnkScorpiusManager_l694ea8::*listeners) {
    UnkScorpiusManager_l694ea8* manager = Manager();
    Unlink(slot);
    UnkLinkList* list = reinterpret_cast<UnkLinkList*>(&(manager->*listeners));
    *slot = list;
    fn_71002af070(list, slot);
}

// The message box that shows cutscene titles, or null.
static inline void* CutsceneMessages() {
    UnkScorpiusGuiRef_l694ea8* gui = static_cast<UnkScorpiusGuiRef_l694ea8*>(g_unk_7101d47310->field_0x2258);
    return gui ? gui->messages : nullptr;
}

void fn_7100694ea8(UnkScorpiusAI_l694ea8* self, UnkScorpiusDefeat_l694ea8* defeat) {
    fn_7100fdbd84(MusicManager(), 1);
    UnkMlogMessage_b42a1 message = MlogMessage(CStrId("#MLOG_DEFEAT_BOSS_SCORPIUS", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128, &message);
    fn_71003aea40(reinterpret_cast<UnkBossAIComponent*>(self), defeat);
    f32 time = (TargetOf(defeat->field_0x8) || TargetOf(defeat->field_0x10)) ? 0.26f : 0.0f;

    Listen(&self->death_listener, &UnkScorpiusManager_l694ea8::listeners_0x2428);
    Listen(&self->death_listener2, &UnkScorpiusManager_l694ea8::listeners_0x24a8);

    UnkScorpiusTargetActor_l694ea8* actor =
        static_cast<UnkScorpiusTargetActor_l694ea8*>(fn_71004067a4(self->stage3_target.Target()));
    UnkScorpiusCutscene_l694ea8* cutscene = actor->cutscene;
    if (!IsCutsceneLoaded(g_unk_7101d47330, cutscene->path)) {
        Unlink(&self->stage3_listener);
        fn_7101199768(g_unk_7101d47330, self->field_0x14eb0);
        PlayCutscene(g_unk_7101d47330, CutsceneId(cutscene->path));
    }

    UnkScorpiusManager_l694ea8* manager = Manager();
    if (!fn_7100da9018(reinterpret_cast<UnkTimedRef*>(manager), self->death_target.Target(), time)) {
        Unlink(&self->death_listener);
        Unlink(&self->death_listener2);
        if (void* messages = CutsceneMessages()) {
            static CStrId s_dead("#CUT_SCORPIUS_DEAD", true);
            fn_7100b5fd6c(messages, &s_dead.data, &CStrId::Empty(), true, false);
        }
        fn_7100694dc0(self);
    }
}
