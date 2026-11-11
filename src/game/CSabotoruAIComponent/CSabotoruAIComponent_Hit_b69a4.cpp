// CSabotoruAIComponent unit (tools/tu_map.py, the name from reflection): fn_710067a06c fills a damage request
// after the base attack's (fn_7101105c90): when the actor's spawn point (+0x398, its component at +0x10) is a
// CSabotoruSpawnPointComponent whose link (+0x3b8, fn_71003b0c4c) names an actor, the request's direction
// (+0x4c) is -x or +x as that actor lies right or left of the given one, with its flag (+0x49) set and 1.0 at
// +0x50. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

// Declared as src/game/CKraidAIComponent/CKraidAIComponent_2.cpp does.
struct UnkKraidAI2;
struct UnkKraidRequest;

// Declared as src/game/CGobblerAIComponent/CGobblerAIComponent_Spawn_b60.cpp does.
struct UnkZiplineLinkTarget_b49;
struct UnkZiplineLink_b49;

extern const base::math::CVector3D g_unk_710164a4f8 __attribute__((visibility("hidden")));  // {-1, 0, 0}
extern const base::math::CVector3D g_unk_710164a504 __attribute__((visibility("hidden")));  // {1, 0, 0}

struct UnkSabotoruLinkTarget_b69a4 {
    u8 pad_0x0[0x10];
    UnkActor* actor;
};

struct UnkSabotoruSpawnPoint_b69a4 {
    void** vtable;
    u8 pad_0x8[0x3b0];
    u8 link[0x8];  // 0x3b8

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkSabotoruSpawnPoint_b69a4*)>(vtable[0])(this);
    }
    UnkSabotoruLinkTarget_b69a4* Target() {
        return *reinterpret_cast<UnkSabotoruLinkTarget_b69a4**>(
            fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(link)));
    }
};

struct UnkSabotoruSpawnHolder_b69a4 {
    u8 pad_0x0[0x10];
    UnkSabotoruSpawnPoint_b69a4* component;
};

struct UnkSabotoruActor_b69a4 {
    u8 pad_0x0[0x398];
    UnkSabotoruSpawnHolder_b69a4* spawn;
};

struct UnkSabotoruAI_b69a4 {
    u8 pad_0x0[0x30];
    UnkSabotoruActor_b69a4* actor;
};

struct UnkSabotoruRequest_b69a4 {
    u8 pad_0x0[0x49];
    bool has_direction;
    u8 pad_0x4a[2];
    base::math::CVector2D direction;  // 0x4c
};

// Sets the direction from the projection of a 3D one.
static inline __attribute__((always_inline)) void SetDirection_b69a4(UnkSabotoruRequest_b69a4* request,
                                                                      const base::math::CVector3D* v) {
    base::math::CVector2D direction;
    fn_710007b430(&direction, v);
    request->direction.x = direction.x;
    request->direction.y = direction.y;
}

void fn_710067a06c(UnkSabotoruAI_b69a4* self, UnkActor* other, UnkSabotoruRequest_b69a4* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), other, reinterpret_cast<UnkKraidRequest*>(request));
    UnkSabotoruSpawnHolder_b69a4* spawn = self->actor->spawn;
    if (!spawn)
        return;
    UnkSabotoruSpawnPoint_b69a4* point = spawn->component;
    if (!point || !fn_710009d50c(point->vf_0x0(), fn_7100a1b7b4()))
        return;
    UnkSabotoruLinkTarget_b69a4* target = point->Target();
    if (!target || !target->actor)
        return;
    // Set in each branch (the original's stores stay apart from the scale written after them).
    if (point->Target()->actor->field_0x44 > other->field_0x44)
        SetDirection_b69a4(request, &g_unk_710164a4f8);
    else
        SetDirection_b69a4(request, &g_unk_710164a504);
    request->has_direction = true;
    request->direction.y = 1.0f;
}
