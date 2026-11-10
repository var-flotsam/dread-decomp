// CGoliathAttack's unit (tools/tu_map.py): whether the attack may start. The base check
// (fn_7100360b84) must pass, the actor must have a target (fn_7100d7c990) and the game manager's
// collision tracker (+0x608, its +0x820) must exist. The actor's AI component (+0x240), taken as a
// CBaseBigFistAIComponent (null when it isn't one), must then fail fn_71003e6c04 for the tunable
// distance (g_unk_71726e5e70, +0x30); the answer is fn_71003e60ac's for it. Names and layouts are
// placeholders. Then the attack's shot (fn_710058f1c8): a shot description is built
// (fn_71009f35b0), given the Goliath AI's shot path (vtable slot 0x850) when the AI component is a
// CGoliathAIComponent, fired for the owner the link at +0x68 holds (fn_71009f5470) and destroyed.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkBigFistAI_gla;
bool fn_71003e6c04(UnkBigFistAI_gla* self, f32 distance);
bool fn_71003e60ac(UnkBigFistAI_gla* self, s32* out);

struct UnkGoliathTunables_gla {
    u8 pad_0x0[0x30];
    f32 distance;
};
extern UnkGoliathTunables_gla g_unk_71726e5e70;

static inline UnkBigFistAI_gla* BigFistAI_gla(UnkActor* actor) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(actor->field_0x240);
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), fn_71003e53c4()) ? reinterpret_cast<UnkBigFistAI_gla*>(ai) : nullptr;
}

bool fn_710058e488(CAttack* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(self, arg, mode);
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (!ok || !target)
        return false;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub || !sub->field_0x820)
        return false;
    f32 distance = g_unk_71726e5e70.distance;
    if (fn_71003e6c04(BigFistAI_gla(self->field_0x8), distance))
        return false;
    s32 out = 0;
    return fn_71003e60ac(BigFistAI_gla(self->field_0x8), &out);
}

// The shot description fn_71009f35b0 builds (0x210 bytes, its file path at +0x1d8) and
// fn_71003cc474 destroys.
struct UnkShockWaveMember;
void fn_71009f35b0(char* self);
void fn_71003cc474(UnkShockWaveMember* self);
void fn_71009f5470(char* self, void* owner);

// The Goliath AI's vtable slot 0x850 gives the path of its shot.
struct UnkGoliathAI_gla {
    void** vtable;
    const base::global::CFilePathStrId* ShotPath() {
        return reinterpret_cast<const base::global::CFilePathStrId* (*)(UnkGoliathAI_gla*)>(vtable[0x850 / 8])(this);
    }
};

struct UnkGoliathLink_gla {
    u8 pad_0x0[0x10];
    void* owner;
};

struct UnkGoliathAttack_gla : CAttack {
    u8 pad_0x28[0x40];
    UnkGoliathLink_gla* link;  // 0x68
};

void fn_710058f1c8(UnkGoliathAttack_gla* self) {
    void* owner = self->link ? self->link->owner : nullptr;
    alignas(8) char shot[0x210];
    fn_71009f35b0(shot);
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005908ac())) {
        base::global::CFilePathStrId* path = reinterpret_cast<base::global::CFilePathStrId*>(shot + 0x1d8);
        *path = base::global::CFilePathStrId(*reinterpret_cast<UnkGoliathAI_gla*>(ai)->ShotPath());
    }
    fn_71009f5470(shot, owner);
    fn_71003cc474(reinterpret_cast<UnkShockWaveMember*>(shot));
}
