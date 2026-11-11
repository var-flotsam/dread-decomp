// CCharClassDeleteEntityEvent's unit (tools/tu_map.py unit 2230): runs the event on the context's
// actor. In the mode flagged at +0xb8 of g_unk_71726bc570, an actor whose AI component (+0x240) is a
// CChozoRobotSoldierAIComponent is deleted after 10 (fn_7100d7b054) unless the manager
// g_unk_71726af468 (created on first use) has a node at +0x228 (fn_7100475564). Otherwise a delay
// (+0xc) within 0.001 of zero deletes it now (fn_7100232c68), any other after that delay. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};
extern UnkModeFlags* g_unk_71726bc570;

struct UnkNodeSet695_b43a1;
base::reflection::UnkReflObject* fn_7100475564(UnkNodeSet695_b43a1* self);

struct UnkDeleteEntityActor {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;  // 0x240
};

struct UnkDeleteEntityContext {
    UnkDeleteEntityActor* actor;
};

struct UnkDeleteEntityEvent {
    u8 pad_0x0[0xc];
    f32 delay;  // 0xc
};

static inline UnkManager_71726af468* GetManager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

void fn_7100cb23c8(UnkDeleteEntityEvent* self, UnkDeleteEntityContext* context) {
    UnkDeleteEntityActor* actor = context->actor;
    if (!actor)
        return;
    if (g_unk_71726bc570->field_0xb8 && actor->ai &&
        fn_710009d50c(actor->ai->vf_0x0(), reinterpret_cast<const base::reflection::CType*>(fn_710046ea2c()))) {
        UnkManager_71726af468* manager = GetManager();
        if (!fn_7100475564(reinterpret_cast<UnkNodeSet695_b43a1*>(manager->pad_0x0 + 0x228))) {
            fn_7100d7b054(context->actor, 10.0f);
            return;
        }
    }
    f32 delay = self->delay;
    if (delay + 0.001f >= 0.0f && delay - 0.001f <= 0.0f)
        fn_7100232c68(context->actor);
    else
        fn_7100d7b054(context->actor, delay);
}
