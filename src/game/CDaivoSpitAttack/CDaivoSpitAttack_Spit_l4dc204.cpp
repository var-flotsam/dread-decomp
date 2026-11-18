// CDaivoSpitAttack's unit (tools/tu_map.py unit 761): the spit. Spawns the spit actor (the definition named
// by g_unk_71726d93f8); when it is an entity, places it 50 units ahead of the Daivo's mouth along its
// facing, sets its movement going, hands its controller three of the AI component's tracked parts, adds it
// to the game manager's list and launches it from there. The attack is marked done (+0x88) either way.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CDaivoSpitAttack.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CScenario.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU365.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
struct UnkGrappleAnswer_b50;
struct UnkSelf;
struct UnkDaivoController_b46a1;

// The spit actor's definition.
extern CStrId g_unk_71726d93f8 __attribute__((visibility("hidden")));
extern u32 g_unk_7172a0ba4c;
// The mouth's point on the Daivo's model.
extern const u64 g_unk_7101649be0;

void fn_71004dc590(UnkSelf* self, UnkWeakTarget* other);
void fn_71004e092c(UnkDaivoController_b46a1* controller, const CVector3D* position, const CVector3D* forward,
                   const CVector3D* side);
struct UnkPathNodeOwnerA;
void fn_7101059f8c(UnkPathNodeOwnerA* nodes);

struct UnkSpitName_l4dc204 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkSpitDefType_l4dc204 {
    u32 id;
};

struct UnkSpitMovement_l4dc204 {
    void** vtable;
    u8 pad_0x8[0x108];
    u32 field_0x110;
    u32 field_0x114;
    u32 field_0x118;

    void Start() { reinterpret_cast<void (*)(UnkSpitMovement_l4dc204*)>(vtable[0x48 / 8])(this); }
};

struct UnkSpit_l4dc204 {
    void** vtable;
    u8 pad_0x8[0x228];
    UnkSpitMovement_l4dc204* movement;  // 0x230

    void SetPosition(const CVector3D* position, bool flag) {
        reinterpret_cast<void (*)(UnkSpit_l4dc204*, const CVector3D*, bool)>(vtable[0xd0 / 8])(this, position, flag);
    }
};

struct UnkSpitController_l4dc204 {
    u8 pad_0x0[0x80];
    void* path;  // 0x80
    u8 pad_0x88[0x4f0];
    u64 field_0x578;
    u64 field_0x580;
};

// The AI component's tracked parts.
struct UnkDaivoAI_l4dc204 {
    u8 pad_0x0[0x1350];
    u8 part_0x1350[0x13470];
    u8 part_0x147c0[0x40];
    u8 part_0x14800[0x40];
};
static_assert(__builtin_offsetof(UnkDaivoAI_l4dc204, part_0x14800) == 0x14800, "layout");

struct UnkPartLink_l4dc204 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkPathHolder_l4dc204 {
    u8 pad_0x0[0x30];
    u8 path[1];  // 0x30
};

struct UnkTracker_l4dc204 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkDaivoSpitAttack_l4dc204 : CAttack {
    u8 pad_0x28[0x8];
    u8 nodes[0x58];  // 0x30, logic path nodes cleared by fn_7101059f8c
    bool done;       // 0x88
};
static_assert(__builtin_offsetof(UnkDaivoSpitAttack_l4dc204, done) == 0x88, "layout");

// The object the part's link holds, if any.
static inline void* LinkedObject(void* part) {
    auto* link = reinterpret_cast<UnkPartLink_l4dc204*>(*fn_71002a76a4(static_cast<UnkBreakablePart*>(part)));
    return link ? link->object : nullptr;
}

void fn_71004dc204(UnkDaivoSpitAttack_l4dc204* self) {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkSpitName_l4dc204* name = reinterpret_cast<UnkSpitName_l4dc204*>(g_unk_71726d93f8.data);
    ++name->use_count;
    const char* path = name->text;
    UnkSpitDefType_l4dc204 type;
    type.id = g_unk_7172a0ba4c;
    auto* spawned = reinterpret_cast<UnkReflObject*>(fn_710119438c(
        factory, path, &g_unk_71726d93f8, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!spawned || !fn_710009d50c(spawned->vf_0x0(), fn_7100d70c20())) {
        self->done = true;
        return;
    }
    self->done = true;
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71004dec74()))
        ai = nullptr;
    auto* spit = reinterpret_cast<UnkSpit_l4dc204*>(spawned);
    auto* controller = reinterpret_cast<UnkSpitController_l4dc204*>(fn_71004dc4b4(spit));
    fn_71004dc590(reinterpret_cast<UnkSelf*>(ai), reinterpret_cast<UnkWeakTarget*>(spit));

    CVector3D position;
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;
    fn_710117364c(self->field_0x8->field_0x228, &g_unk_7101649be0,
                  reinterpret_cast<UnkGrappleAnswer_b50*>(&position));
    position.z = 0.0f;
    const UnkMatrix44* xform = reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38);
    const UnkMatrix44& m = fn_710002614c(xform);
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    CVector3D forward = {fx, fy, fz};
    const UnkMatrix44& n = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38));
    f32 sx = n.m[1];
    f32 sy = n.m[5];
    CVector3D side = {sx, sy, 0.0f};
    // Flattened to the plane.
    forward.z = 0.0f;
    CVector3D offset = {forward.x * 50.0f, forward.y * 50.0f, forward.z * 50.0f};
    position.x = offset.x + position.x;
    position.y = offset.y + position.y;
    position.z = offset.z + position.z;
    spit->SetPosition(&position, true);

    UnkSpitMovement_l4dc204* movement = spit->movement;
    movement->field_0x118 = 4;
    movement->field_0x110 = 10;
    movement->field_0x114 = 5;
    fn_7101059f8c(reinterpret_cast<UnkPathNodeOwnerA*>(self->nodes));
    auto* daivo = reinterpret_cast<UnkDaivoAI_l4dc204*>(ai);
    auto* holder = reinterpret_cast<UnkPathHolder_l4dc204*>(
        fn_7100382150(static_cast<UnkActor*>(LinkedObject(daivo->part_0x1350))));
    controller->path = holder->path;
    controller->field_0x578 =
        reinterpret_cast<UnkTracker_l4dc204*>(fn_710028aca8(LinkedObject(daivo->part_0x147c0)))->field_0x30;
    controller->field_0x580 =
        reinterpret_cast<UnkTracker_l4dc204*>(fn_710028aca8(LinkedObject(daivo->part_0x14800)))->field_0x30;
    movement->Start();
    fn_71011a2770(g_unk_7101d47310->field_0x608, spit);
    fn_71004e092c(reinterpret_cast<UnkDaivoController_b46a1*>(controller), &position, &forward, &side);
}
