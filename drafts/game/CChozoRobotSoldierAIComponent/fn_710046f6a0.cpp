// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): the start step. After the boss AI base's
// start, registers the component, resolves its links and probes the navigation mesh below each target into
// the hit array, resets the owner's slot colours, picks a random side and runs the boss intro when asked.
// Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CGameLink.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2564.h"
#include "game/UnkTU341.h"
#include "game/UnkTU55.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector4D;

struct CActor;
struct UnkCoreXAI_A2_b73;
struct UnkHolderOwner_b47;
struct UnkNodeSet_b43a1;
struct UnkVec3;
struct Vec2;
struct Result;

void fn_7100475394(UnkNodeSet_b43a1* self, u64 v);
void fn_71002c58cc(UnkHolderOwner_b47* self, CActor* target);
UnkNavMeshHit* fn_710046fb88(base::global::CRntVector<UnkNavMeshHit>* hits, u32 index);
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);
void fn_7100e01978(UnkGameManager* self);
void fn_71003aff64(UnkCoreXAI_A2_b73* self, u32 a, u32 b);

extern char g_unk_71726cca00[] __attribute__((visibility("hidden")));
__attribute__((visibility("hidden"))) extern u64 g_unk_71726cca20;

// The tunables the life component reads (g_unk_717272fd38): the value passed to the owner's part.
struct UnkRobotSoldierTunables_l46f6a0 {
    u8 pad_0x0[0xa0];
    f32 field_0xa0;
};

// The global mode flags: whether the boss intro runs.
struct UnkRobotSoldierModeFlags_l46f6a0 {
    u8 pad_0x0[0xb8];
    bool field_0xb8;
};

struct UnkRobotSoldierPart_l46f6a0;
struct UnkRobotSoldierPartVtable_l46f6a0 {
    void* pad_0x0[0x168 / 8];
    void (*vf_0x168)(UnkRobotSoldierPart_l46f6a0* self, bool enable, f32 value);
    void (*vf_0x170)(UnkRobotSoldierPart_l46f6a0* self, f32 value);
};
struct UnkRobotSoldierPart_l46f6a0 {
    const UnkRobotSoldierPartVtable_l46f6a0* vtable;
};

struct UnkRobotSoldierSlotCount_l46f6a0 {
    u8 pad_0x0[0x2c];
    u32 count;
};
struct UnkRobotSoldierSlots_l46f6a0 {
    u8 pad_0x0[0x78];
    UnkRobotSoldierSlotCount_l46f6a0* info;
};
struct UnkRobotSoldierModel_l46f6a0 {
    u8 pad_0x0[0x138];
    UnkRobotSoldierSlots_l46f6a0* slots;
};
struct UnkRobotSoldierRender_l46f6a0 {
    u8 pad_0x0[0x50];
    UnkRobotSoldierModel_l46f6a0* model;
};

struct UnkRobotSoldierOwner_l46f6a0 {
    u8 pad_0x0[0x30];
    u64 field_0x30;  // the scenario id
    u8 pad_0x38[0x1b8 - 0x38];
    UnkRobotSoldierPart_l46f6a0* part;  // 0x1b8
    u8 pad_0x1c0[0x228 - 0x1c0];
    UnkRobotSoldierRender_l46f6a0* render;  // 0x228
};

// The target's position as the probe reads it.
struct UnkRobotSoldierTarget_l46f6a0 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
};

struct UnkRobotSoldierPoint_l46f6a0 {
    f32 x;
    f32 y;
    UnkRobotSoldierPoint_l46f6a0(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// A segment with its direction (end - start).
struct UnkRobotSoldierSegment_l46f6a0 {
    UnkRobotSoldierPoint_l46f6a0 start;
    UnkRobotSoldierPoint_l46f6a0 end;
    UnkRobotSoldierPoint_l46f6a0 delta;
    UnkRobotSoldierSegment_l46f6a0(const UnkRobotSoldierPoint_l46f6a0& s, const UnkRobotSoldierPoint_l46f6a0& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkRobotSoldierNavResult_l46f6a0 {
    u16 cell;
    u8 side;
    UnkRobotSoldierPoint_l46f6a0 points[5];
    f32 distance;
    UnkRobotSoldierNavResult_l46f6a0()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

struct UnkHolder_l46f6a0 {
    u8 pad_0x0[0x68];
};

struct UnkChozoRobotSoldierAI_l46f6a0 {
    u8 pad_0x0[0x30];
    UnkRobotSoldierOwner_l46f6a0* owner;  // 0x30
    u8 pad_0x38[0x14b28 - 0x38];
    base::global::CRntVector<CGameLink<CActor>> links;  // 0x14b28
    base::global::CRntVector<UnkNavMeshHit> hits;       // 0x14b50
    u8 pad_0x14b78[0xc];
    u32 side;  // 0x14b84
    u8 pad_0x14b88[0x27628 - 0x14b88];
    UnkHolder_l46f6a0 holder_a;  // 0x27628
    UnkHolder_l46f6a0 holder_b;  // 0x27690
    UnkHolder_l46f6a0 holder_c;  // 0x276f8
    u8 pad_0x27760[0x30];
    bool field_0x27790;
};
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAI_l46f6a0, hits) == 0x14b50, "layout");
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAI_l46f6a0, side) == 0x14b84, "layout");
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAI_l46f6a0, field_0x27790) == 0x27790, "layout");

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

static inline UnkManager_71726af468* Registry() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

void fn_710046f6a0(UnkChozoRobotSoldierAI_l46f6a0* self) {
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    if (self->field_0x27790) {
        if (UnkRobotSoldierPart_l46f6a0* part = self->owner->part) {
            const UnkRobotSoldierTunables_l46f6a0* tunables =
                reinterpret_cast<const UnkRobotSoldierTunables_l46f6a0*>(&g_unk_717272fd38);
            f32 value = tunables->field_0xa0;
            part->vtable->vf_0x170(part, value);
            part = self->owner->part;
            value = tunables->field_0xa0;
            part->vtable->vf_0x168(part, true, value);
        }
    }
    fn_7100de9540(g_unk_7101d47310, reinterpret_cast<const u64*>(g_unk_71726cca00),
                  reinterpret_cast<UnkWeakTarget*>(self));
    fn_7100475394(reinterpret_cast<UnkNodeSet_b43a1*>(Registry()->pad_0x0 + 0x228),
                  reinterpret_cast<u64>(self->owner));

    self->hits.Reserve(self->links.count);
    for (u32 i = 0; i < self->links.count; ++i) {
        CGameLink<CActor>* link = reinterpret_cast<CGameLink<CActor>*>(
            fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(&self->links), i));
        if (link->state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(&link->callbacks);
                fn_7100234f78(&link->handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            link->state = 2;
        }
        CActor* target = link->Target();
        if (!target)
            continue;
        fn_71002c58cc(reinterpret_cast<UnkHolderOwner_b47*>(&self->holder_a), target);
        fn_71002c58cc(reinterpret_cast<UnkHolderOwner_b47*>(&self->holder_b), target);
        fn_71002c58cc(reinterpret_cast<UnkHolderOwner_b47*>(&self->holder_c), target);

        UnkNavMeshHit hit;
        const UnkRobotSoldierTarget_l46f6a0* pos = reinterpret_cast<const UnkRobotSoldierTarget_l46f6a0*>(target);
        UnkRobotSoldierSegment_l46f6a0 segment(UnkRobotSoldierPoint_l46f6a0(pos->x + 0.0f, pos->y + 5.0f),
                                               UnkRobotSoldierPoint_l46f6a0(pos->x + 0.0f, pos->y + -5.0f));
        UnkRobotSoldierNavResult_l46f6a0 result;
        if (fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result),
                          0.0f, 0, -1.0f)) {
            hit.cell = result.cell;
            hit.side = result.side;
            fn_710007b444(&hit.point, reinterpret_cast<const UnkVec3*>(&result.points[0]));
        }
        UnkNavMeshHit* slot = fn_710046fb88(&self->hits, i);
        slot->cell = hit.cell;
        slot->side = hit.side;
        slot->point.x = hit.point.x;
        slot->point.y = hit.point.y;
    }

    if (UnkRobotSoldierRender_l46f6a0* render = self->owner->render) {
        if (UnkRobotSoldierModel_l46f6a0* model = render->model) {
            for (u32 i = 0; i < model->slots->info->count; ++i) {
                CVector4D colour;
                colour.x = 1.0f;
                colour.y = 1.0f;
                colour.z = 1.0f;
                colour.w = 1.0f;
                fn_71001c287c(model->slots, i, 0, &colour, 1);
                colour.w = !self->field_0x27790 ? 0.0f : 1.0f;
                fn_71001c2764(reinterpret_cast<UnkSlots*>(model->slots), i, 0, &colour, 1, true);
            }
        }
    }

    self->side = static_cast<f32>((rand() & 0x7fff) * (1.0 / 32767.0)) <= 0.5f;

    const base::global::CStrId* scenario = fn_7100de9bf8(g_unk_7101d47310);
    if (scenario->data == g_unk_71727c8018.data && self->owner->field_0x30 == g_unk_71726cca20)
        fn_7100e01978(g_unk_7101d47310);
    if (reinterpret_cast<UnkRobotSoldierModeFlags_l46f6a0*>(g_unk_71726bc570)->field_0xb8)
        fn_71003aff64(reinterpret_cast<UnkCoreXAI_A2_b73*>(self), 1, 1);
}
