// UnkTU1376 (tools/tu_map.py, unit 1376; near CDoorLifeComponent): whether the door may close. Not while the
// player stands in its frame or its timer runs, nor while the player or a listed actor fails fn_71008da2e4 or
// (doors that check it) stands within the tuned radius; then not while the owner's collision shape still
// touches something (two tree queries). Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

namespace base::reflection {
struct UnkTunable;
}
extern base::reflection::UnkTunable g_unk_7172731368;
extern CStrId g_unk_7172731720 __attribute__((visibility("hidden")));
extern void* g_unk_7101a7eae0[];

struct UnkTree;
struct UnkBvhCollider;
bool fn_7100d5bfbc(UnkTree* self, u32 mask, UnkBvhCollider* shape, void* arg, u32 arg2);

// The door's tunables: the radius the player and listed actors keep it open within.
struct UnkDoorTunables_l8d4e14 {
    u8 pad_0x0[0x28];
    f32 radius;  // 0x28
};

struct UnkDoorConfig_l8d4e14 {
    u8 pad_0x0[0x124];
    f32 min_half_size;  // 0x124
    bool ignore_radius;  // 0x128
};

struct UnkDoorOwner_l8d4e14 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkDoorLife_l8d4e14 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d4e14* owner;  // 0x30
    u8 pad_0x38[0x644 - 0x38];
    s32 field_0x644;
    s32 field_0x648;
    u8 pad_0x64c[0x698 - 0x64c];
    CVector3D position;  // 0x698
    f32 width;           // 0x6a4
    f32 height;          // 0x6a8
    u8 pad_0x6ac[0x9f8 - 0x6ac];
    UnkDoorConfig_l8d4e14* config;  // 0x9f8
    u8 pad_0xa00[0xaf8 - 0xa00];
    u8 timer[0x8];  // 0xaf8
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d4e14, timer) == 0xaf8, "layout");

bool fn_71008da2e4(UnkDoorLife_l8d4e14* self, UnkActor* actor);

struct UnkListedLink_l8d4e14 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkListedItemView_l8d4e14 {
    u8 pad_0x0[0x10];
    UnkListedLink_l8d4e14* link;  // 0x10
};

static inline UnkListedLink_l8d4e14* ListedLink_l8d4e14(UnkListedItem* item) {
    return item ? reinterpret_cast<UnkListedItemView_l8d4e14*>(item)->link : nullptr;
}

static inline f32 DistanceSq_l8d4e14(const UnkActor* actor, const CVector3D& p) {
    f32 dx = actor->field_0x44 - p.x;
    f32 dy = actor->field_0x54 - p.y;
    f32 dz = actor->field_0x64 - p.z;
    return dx * dx + dy * dy + dz * dz;
}

// A hit of the tree query: two counted references and the contact.
struct __attribute__((packed)) UnkDoorHit_l8d4e14 {
    UnkRefObject* object;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};

// The query's hit list (vtable g_unk_7101a7eae0; its destructor is fn_71008da4f0).
struct __attribute__((packed, aligned(8))) UnkDoorHits_l8d4e14 {
    void** vtable;
    u32 count;
    UnkDoorHit_l8d4e14 hits[1];

    UnkDoorHits_l8d4e14() {
        vtable = g_unk_7101a7eae0 + 2;
        count = 0;
    }
    ~UnkDoorHits_l8d4e14() {
        vtable = g_unk_7101a7eae0 + 2;
        for (u64 i = 0; i < count; i++) {
            Release(hits[i].field_0x8);
            Release(hits[i].object);
        }
        count = 0;
    }
    static void Release(UnkRefObject* p) {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

// Whether the shape touches anything of the given kinds.
static inline bool Touches_l8d4e14(UnkTree* tree, u32 mask, UnkCollisionObject* object) {
    UnkDoorHits_l8d4e14 hits;
    return fn_7100d5bfbc(tree, mask, reinterpret_cast<UnkBvhCollider*>(object), &hits, 0);
}

bool fn_71008d4e14(UnkDoorLife_l8d4e14* self) {
    CVector2D door;
    CVector2D at;
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        fn_710007b430(&door, &self->position);
        {
            CVector3D pos = {player->field_0x44, player->field_0x54, player->field_0x64};
            fn_710007b430(&at, &pos);
        }
        f32 dx = fabsf(door.x - at.x);
        f32 dy = fabsf(door.y - at.y);
        f32 half_w = self->width * 0.5f;
        f32 half_h = self->height * 0.5f;
        f32 min = self->config->min_half_size;
        half_w = min < half_w ? half_w : min;
        half_h = min < half_h ? half_h : min;
        // Both sides are tested (the original computes both before branching).
        if ((dx < half_w) & (dy < half_h))
            return false;
        if (fn_710002fb70(self->timer))
            return false;
        if (!fn_71008da2e4(self, player))
            return false;
        if (!self->config->ignore_radius && (self->field_0x644 != 0) != (self->field_0x648 != 0)) {
            f32 radius = reinterpret_cast<const UnkDoorTunables_l8d4e14*>(&g_unk_7172731368)->radius;
            if (radius > 0.0f) {
                f32 radius_sq = radius * radius;
                if (DistanceSq_l8d4e14(player, self->position) <= radius_sq)
                    return false;
                base::global::CRntVector<UnkListedItem*>* list = fn_7100552d7c();
                for (u32 i = 0; i < list->count; i++) {
                    UnkListedLink_l8d4e14* link = ListedLink_l8d4e14(list->data[i]);
                    if (link && DistanceSq_l8d4e14(link->actor, self->position) <= radius_sq)
                        return false;
                }
            }
        }
    }

    base::global::CRntVector<UnkListedItem*>* list = fn_7100552d7c();
    for (u32 i = 0; i < list->count; i++) {
        UnkListedLink_l8d4e14* link = ListedLink_l8d4e14(list->data[i]);
        if (link && !fn_71008da2e4(self, link->actor))
            return false;
    }

    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return true;
    UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_7172731720);
    if (!object)
        return true;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (Touches_l8d4e14(tree, 0x10000000, object))
        return false;
    return !Touches_l8d4e14(tree, 0x2000, object);
}
