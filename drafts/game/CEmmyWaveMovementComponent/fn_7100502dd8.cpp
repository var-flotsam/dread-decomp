// CEmmyWaveMovementComponent's unit: the update step. After the base movement update it flags the wave as
// blocked when the owner leaves any area of the central unit's shape, stops it, and kills the owner when the
// wave has passed its end heading towards the player. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkMovementComponent.h"
#include "game/UnkTU108.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100ddb860(UnkUnit2547* self);
struct UnkBossOwner_b49;
bool fn_7100d7b184(UnkBossOwner_b49* owner);

// Something placed in the world: its position is the translation column of its transform.
struct UnkPlaced_l502dd8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x54 - 0x48];
    f32 y;  // 0x54
    u8 pad_0x58[0x64 - 0x58];
    f32 z;  // 0x64

    CVector3D Position() const { return {x, y, z}; }
};

struct UnkWaveMovement_l502dd8 {
    u8 pad_0x0[0x30];
    UnkPlaced_l502dd8* owner;  // 0x30
    u8 pad_0x38[0x40 - 0x38];
    CVector3D direction;  // 0x40
    u8 pad_0x4c[0x5d8 - 0x4c];
    u32 blocked;  // 0x5d8
    u8 pad_0x5dc[0x5e4 - 0x5dc];
    bool ignore_areas;  // 0x5e4
    u8 pad_0x5e5[0x5f0 - 0x5e5];
    CVector2D end;  // 0x5f0
};

void fn_7100502a30(UnkWaveMovement_l502dd8* self);

struct UnkArea_l502dd8 {
    u8 pad_0x0[0x48];
};

struct UnkAreaShape_l502dd8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetKind();  // 0x18
    u8 pad_0x8[0xa8 - 0x8];
    base::global::CRntVector<UnkArea_l502dd8> areas;  // 0xa8
};

struct UnkAreaTracker_l502dd8 {
    u8 pad_0x0[0x30];
    UnkAreaShape_l502dd8* shape;  // 0x30
};

struct UnkAreaTarget_l502dd8 {
    u8 pad_0x0[0x10];
    void* actor;  // 0x10
};

struct UnkCentralUnit_l502dd8 {
    u8 pad_0x0[0x5a8];
    u8 area_part[1];  // 0x5a8
};

static inline UnkDoorRegistry_b61* Registry_l502dd8() {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    return reg;
}

static inline void Flatten(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

void fn_7100502dd8(UnkWaveMovement_l502dd8* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    if (auto* unit = reinterpret_cast<UnkCentralUnit_l502dd8*>(
            fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(Registry_l502dd8())))) {
        auto* target = reinterpret_cast<UnkAreaTarget_l502dd8*>(
            *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(unit->area_part)));
        if (target && target->actor) {
            if (auto* tracker = reinterpret_cast<UnkAreaTracker_l502dd8*>(fn_710028aca8(target->actor))) {
                UnkAreaShape_l502dd8* shape = tracker->shape;
                if (shape && shape->GetKind() == 0x10) {
                    for (UnkArea_l502dd8* area = shape->areas.data; area != shape->areas.data + shape->areas.count;
                         ++area) {
                        CVector2D pos;
                        Flatten(&pos, self->owner->Position());
                        if (!fn_7100116de0(reinterpret_cast<UnkEntry*>(area), &pos, 0, 1, 1.0f) && !self->ignore_areas)
                            self->blocked = 1;
                    }
                }
            }
        }
    }
    if (!self->blocked) {
        if (fn_7100ddb860(reinterpret_cast<UnkUnit2547*>(Registry_l502dd8())))
            fn_7100232c68(self->owner);
    }
    if (self->blocked)
        fn_7100502a30(self);
    CVector2D direction;
    Flatten(&direction, self->direction);
    CVector2D pos;
    Flatten(&pos, self->owner->Position());
    if ((self->end.x - pos.x) * direction.x + (self->end.y - pos.y) * direction.y < 0.0f) {
        CVector2D heading;
        Flatten(&heading, self->direction);
        auto* player = reinterpret_cast<UnkPlaced_l502dd8*>(fn_7100dec788(g_unk_7101d47310));
        CVector3D p = player->Position();
        CVector3D o = self->owner->Position();
        CVector2D to_player;
        Flatten(&to_player, {p.x - o.x, p.y - o.y, p.z - o.z});
        if (heading.x * to_player.x + heading.y * to_player.y < 0.0f &&
            fn_7100d7b184(reinterpret_cast<UnkBossOwner_b49*>(self->owner)))
            fn_7100232c68(self->owner);
    }
}
