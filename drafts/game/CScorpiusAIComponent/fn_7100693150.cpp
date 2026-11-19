// CScorpiusAIComponent unit: handles an event after the base handler (fn_71003ad45c). On the STAGE_2 or
// STAGE_3 id it registers with the lazily created AI manager and hands it the stage's linked target;
// when the manager does not take it, it unregisters, plays the stage's cutscene message and enters the
// stage (fn_71006935a4 or fn_7100692c8c). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/CScorpiusAIComponent.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU437.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkAmiiboComposition_laf0d38;

// A function bound to its object, called when the message closes. It is passed by value; its copy
// constructor is out of line, so it goes in memory.
struct UnkAmiiboCallback_laf0d38 {
    UnkAmiiboComposition_laf0d38* object;
    void (*function)(UnkAmiiboComposition_laf0d38*);

    UnkAmiiboCallback_laf0d38(UnkAmiiboComposition_laf0d38* object, void (*function)(UnkAmiiboComposition_laf0d38*))
        : object(object), function(function) {}
    UnkAmiiboCallback_laf0d38(const UnkAmiiboCallback_laf0d38& other);
};

// What the cutscene message calls when it closes.
void fn_71006986f8(UnkAmiiboComposition_laf0d38* self);

struct UnkScorpiusGuiRef_l693150 {
    u8 pad_0x0[0x10];
    UnkAmiiboMessages_laf0d38* messages;  // 0x10
};

// The manager as seen here: the list its listeners join at +0x24a8.
struct UnkScorpiusManager_l693150 {
    u8 pad_0x0[0x24a8];
    u8 listeners[0x10];  // 0x24a8
};

struct UnkScorpiusCounted_l693150 : base::UnkRefCounted {
    UnkWeakTarget* target;  // 0x10
};

// A link to a target, resolved from the source on first use (state 1 -> 2).
struct UnkScorpiusLazyLink_l693150 {
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
        UnkScorpiusCounted_l693150* p = static_cast<UnkScorpiusCounted_l693150*>(handle.ptr);
        return p ? p->target : nullptr;
    }
};

struct UnkScorpiusEvent_l693150 {
    u8 pad_0x0[0x10];
    base::global::UnkStringData* id;  // 0x10
};

struct UnkScorpiusAI_l693150 {
    u8 pad_0x0[0x14d90];
    UnkScorpiusLazyLink_l693150 stage2_target;  // 0x14d90
    UnkScorpiusLazyLink_l693150 stage3_target;  // 0x14dd0
    u8 pad_0x14e10[0x40];
    UnkLinkList* stage2_listener;  // 0x14e50
    u8 pad_0x14e58[0x10];
    UnkLinkList* stage3_listener;  // 0x14e68
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l693150, stage3_target) == 0x14dd0, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l693150, stage2_listener) == 0x14e50, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l693150, stage3_listener) == 0x14e68, "");

void fn_7100692c8c(UnkScorpiusAI_l693150* self, s32 value, bool flag);

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

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

static inline void Unlink(UnkLinkList** slot) {
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
}

static inline void Listen(UnkLinkList** slot) {
    UnkManager_71726af468* manager = Manager();
    Unlink(slot);
    UnkLinkList* list =
        reinterpret_cast<UnkLinkList*>(reinterpret_cast<UnkScorpiusManager_l693150*>(manager)->listeners);
    *slot = list;
    fn_71002af070(list, slot);
}

static inline bool Hand(UnkScorpiusLazyLink_l693150* link, f32 time) {
    return fn_7100da9018(reinterpret_cast<UnkTimedRef*>(Manager()), link->Target(), time);
}

// The message box that shows cutscene titles, or null.
static inline UnkAmiiboMessages_laf0d38* CutsceneMessages() {
    UnkScorpiusGuiRef_l693150* gui = static_cast<UnkScorpiusGuiRef_l693150*>(g_unk_7101d47310->field_0x2258);
    return gui ? gui->messages : nullptr;
}

static inline UnkAmiiboComposition_laf0d38* AsCallbackObject(UnkScorpiusAI_l693150* self) {
    return reinterpret_cast<UnkAmiiboComposition_laf0d38*>(self);
}

void fn_7100693150(UnkScorpiusAI_l693150* self, const UnkScorpiusEvent_l693150* event) {
    fn_7100fdbd84(MusicManager(), 1);
    fn_71003ad45c(reinterpret_cast<UnkSelf*>(self), event);
    if (event->id == g_unk_7172703ef8.data) {
        Listen(&self->stage2_listener);
        if (Hand(&self->stage2_target, 0.26f))
            return;
        Unlink(&self->stage2_listener);
        if (UnkAmiiboMessages_laf0d38* messages = CutsceneMessages())
            fn_7100b5fd9c(messages, CStrId("#CUT_SCORPIUS_STAGE_2", true),
                          UnkAmiiboCallback_laf0d38(AsCallbackObject(self), fn_71006986f8), true, false);
        fn_71006935a4(reinterpret_cast<UnkScorpiusAI_l697ce0*>(self), 1, false);
    } else if (event->id == g_unk_7172703f00.data) {
        Listen(&self->stage3_listener);
        if (Hand(&self->stage3_target, 0.13f))
            return;
        Unlink(&self->stage3_listener);
        if (UnkAmiiboMessages_laf0d38* messages = CutsceneMessages())
            fn_7100b5fd9c(messages, CStrId("#CUT_SCORPIUS_STAGE_3", true),
                          UnkAmiiboCallback_laf0d38(AsCallbackObject(self), fn_71006986f8), true, false);
        fn_7100692c8c(self, 1, true);
    }
}
