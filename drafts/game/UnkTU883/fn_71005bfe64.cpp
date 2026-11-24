// CHydrogigaAIComponent's unit (tools/tu_map.py unit 883): tells the music manager the fight is on, runs the
// base handler, links the component into the AI manager's list and hands the manager its target with a
// time (0.26 or 0.1 by the event's flags); when the manager refuses, unlinks and sends the owner's character
// class a request. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU437.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

struct UnkTimedRef;
struct UnkWeakTarget;
bool fn_7100da9018(UnkTimedRef* self, UnkWeakTarget* target, f32 time);
void fn_7100fdbd84(UnkMgr_0xf18* self, u32 flag);

struct UnkHydrogigaTargetHolder_l5bfe64 : base::UnkRefCounted {
    UnkWeakTarget* target;  // +0x10
};

struct UnkHydrogigaOwner_l5bfe64 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkHydrogigaEvent_l5bfe64 {
    u8 pad_0x0[0xb4];
    u16 flags;
};

struct UnkHydrogigaAI_l5bfe64 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_l5bfe64* owner;  // +0x30
    u8 pad_0x38[0x14da0 - 0x38];
    u8 source[0x28];  // +0x14da0
    u32 state;        // +0x14dc8
    u8 pad_0x14dcc[4];
    base::UnkRefHandle handle;  // +0x14dd0
    u8 pad_0x14dd8[0x15060 - 0x14dd8];
    UnkLinkList* link;  // +0x15060
};
static_assert(__builtin_offsetof(UnkHydrogigaAI_l5bfe64, link) == 0x15060, "layout");

static inline UnkMgr_0xf18* MusicManager() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    return static_cast<UnkMgr_0xf18*>(instance);
}

static inline UnkManager_71726af468* AIManager() {
    UnkManager_71726af468* instance = g_unk_71726af468;
    if (!instance) {
        instance = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(instance);
        g_unk_71726af468 = instance;
    }
    return instance;
}

static inline void Unlink(UnkLinkList** slot) {
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
}

static inline void Link(UnkLinkList** slot, UnkLinkList* list) {
    Unlink(slot);
    *slot = list;
    fn_71002af070(list, slot);
}

// The counted reference fn_7100353be8 returns, seen as a handle (the same pointer).
static inline const base::UnkRefHandle* AsHandle(const base::UnkRefPtrMember& reference) {
    return reinterpret_cast<const base::UnkRefHandle*>(&reference);
}

// The target behind the handle at +0x14dd0, resolved from +0x14da0 on first use.
static inline UnkHydrogigaTargetHolder_l5bfe64* Holder(UnkHydrogigaAI_l5bfe64* self) {
    if (self->state == 1) {
        {
            base::UnkRefPtrMember reference = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, AsHandle(reference));
        }
        self->state = 2;
    }
    return static_cast<UnkHydrogigaTargetHolder_l5bfe64*>(self->handle.ptr);
}

void fn_71005bfe64(UnkHydrogigaAI_l5bfe64* self, UnkHydrogigaEvent_l5bfe64* event) {
    fn_7100fdbd84(MusicManager(), 1);
    fn_71003aea40(reinterpret_cast<UnkBossAIComponent*>(self), event);
    f32 time = (event->flags & 0x504) ? 0.26f : 0.1f;
    Link(&self->link, reinterpret_cast<UnkLinkList*>(&AIManager()->field_0x330[0x24a8 - 0x330]));
    UnkManager_71726af468* manager = AIManager();
    UnkHydrogigaTargetHolder_l5bfe64* holder = Holder(self);
    bool accepted;
    if (holder)
        accepted = fn_7100da9018(reinterpret_cast<UnkTimedRef*>(manager), holder->target, time);
    else
        accepted = fn_7100da9018(reinterpret_cast<UnkTimedRef*>(manager), nullptr, time);
    if (accepted)
        return;
    Unlink(&self->link);
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkNone_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, 0x7531238cb72381d8, 0xbd794f4bf6d1d598, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
}
