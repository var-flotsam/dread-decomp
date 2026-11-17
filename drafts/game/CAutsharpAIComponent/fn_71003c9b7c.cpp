// CAutsharpAIComponent unit (tools/tu_map.py unit 572): fn_71003c9b7c lets go of the wall: unless `check`
// asks to stay while the target plays the release animation or something tagged is in reach (a circle
// query of the collision tree at the actor), it clears the hold flag and switches the collider. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU437.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCollisionContainer;
void fn_71008635a4(UnkCollisionContainer* self, const CStrId* id, const u32* extra);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bee48;  // the collider to switch to
__attribute__((visibility("hidden"))) extern const u8 g_unk_7101649264[];  // the query circle's source
extern void* g_unk_7101a2e230[];  // vtable of the reach query's visitor

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkAutsharpTag_l3c9b7c {
    u8 pad_0x0[0x18];
    u64 hash;  // 0x18
};

struct UnkAutsharpTagged_l3c9b7c {
    u8 pad_0x0[0x164];
    u32 tag_count;  // 0x164
    UnkAutsharpTag_l3c9b7c* tags[1];  // 0x168
};

struct UnkAutsharpHitSource_l3c9b7c : UnkRefObject {
    UnkAutsharpTagged_l3c9b7c* object;  // 0x10
};
static_assert(sizeof(UnkRefObject) == 0x10, "UnkRefObject size");

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

#pragma pack(push, 4)
struct UnkAutsharpHit_l3c9b7c {
    UnkAutsharpHitSource_l3c9b7c* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

// The hits of one query (vtable g_unk_7101a23c58).
struct UnkAutsharpHitList_l3c9b7c {
    void** vtable;
    u32 count;
    UnkAutsharpHit_l3c9b7c items[64];  // 0xc

    UnkAutsharpHitList_l3c9b7c() {
        vtable = g_unk_7101a23c58 + 2;
        count = 0;
    }
    ~UnkAutsharpHitList_l3c9b7c() {
        vtable = g_unk_7101a23c58 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items[i].second);
            ReleaseRef(items[i].first);
        }
    }
};

// The query's visitor (vtable g_unk_7101a2e230): the hits, and where the query was made from.
struct UnkAutsharpHitVisitor_l3c9b7c : UnkAutsharpHitList_l3c9b7c {
    CVector3D origin;  // 0x120c
    u64 field_0x1218;
    u32 field_0x1220;

    UnkAutsharpHitVisitor_l3c9b7c(const CVector3D& from) : origin(from), field_0x1218(0), field_0x1220(0) {
        vtable = g_unk_7101a2e230 + 2;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkAutsharpHit_l3c9b7c) == 0x48, "record");
static_assert(__builtin_offsetof(UnkAutsharpHitVisitor_l3c9b7c, origin) == 0x120c, "visitor");

// The owner as this unit reads it (UnkActor, with the collision container at +0x2a0).
struct UnkAutsharpOwner_l3c9b7c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b0];
    void* movement;  // 0x218
    u8 pad_0x220[0x80];
    UnkCollisionContainer* collision;  // 0x2a0
};
static_assert(__builtin_offsetof(UnkAutsharpOwner_l3c9b7c, collision) == 0x2a0, "owner");

struct UnkAutsharpAI_l3c9b7c {
    u8 pad_0x0[0x30];
    UnkAutsharpOwner_l3c9b7c* owner;  // 0x30
    u8 pad_0x38[0x14728];
    CVector3D facing;  // 0x14760
    u8 pad_0x1476c[0x14];
    bool field_0x14780;
    u8 pad_0x14781[0x28];
    bool holding;  // 0x147a9
};
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c9b7c, facing) == 0x14760, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c9b7c, holding) == 0x147a9, "layout");

static inline CVector3D Pos(const UnkAutsharpOwner_l3c9b7c* a) {
    return {a->x, a->y, a->z};
}

static inline bool HasTag(const UnkAutsharpTagged_l3c9b7c* object) {
    for (u32 i = 0; i < object->tag_count; i++) {
        if (object->tags[i]->hash == 0xeca5efb02baf9968)
            return true;
    }
    return false;
}

// Clears the hold and switches the collider.
static void LetGo(UnkAutsharpAI_l3c9b7c* self, UnkCollisionContainer* collision) {
    self->holding = false;
    u32 flags = 0x100;
    fn_71008635a4(collision, &g_unk_71726bee48, &flags);
}

void fn_71003c9b7c(UnkAutsharpAI_l3c9b7c* self, bool check) {
    if (!check || self->field_0x14780) {
        LetGo(self, self->owner->collision);
        return;
    }
    auto* anim = static_cast<UnkAnimationComponent*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x220);
    if (!anim || fn_7101177ae8(anim, 0xf4061ff25a2163d6)->data->Key() != 0x34fb0ed603175553) {
        // Stay while something tagged is within the actor's reach ahead of it.
        bool found = false;
        {
            UnkAutsharpOwner_l3c9b7c* owner = self->owner;
            CVector3D pos = Pos(owner);
            CVector3D ahead = self->facing + pos;
            UnkLavaDropsShape_A2_b71 shape(g_unk_7101649264, fn_7100977b20(owner->movement));
            UnkMatrix44 xf;
            xf.m[1] = 0.0f;
            xf.m[2] = 0.0f;
            xf.m[4] = 0.0f;
            xf.m[6] = 0.0f;
            xf.m[8] = 0.0f;
            xf.m[9] = 0.0f;
            xf.m[12] = 0.0f;
            xf.m[13] = 0.0f;
            xf.m[14] = 0.0f;
            xf.m[15] = 1.0f;
            xf.m[10] = 1.0f;
            xf.m[5] = 1.0f;
            xf.m[0] = 1.0f;
            xf.m[3] = pos.x;
            xf.m[7] = pos.y;
            xf.m[11] = pos.z;
            UnkAutsharpHitVisitor_l3c9b7c hits(pos);
            UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
            CVector3D delta = ahead - pos;
            CVector2D dir;
            fn_710007b430(&dir, &delta);
            fn_7100d5b484(tree, 0x20, reinterpret_cast<UnkShape*>(&shape), reinterpret_cast<const UnkMtx44*>(&xf), &hits,
                          0);
            for (UnkAutsharpHit_l3c9b7c *it = hits.items, *end = it + hits.count; it != end; ++it) {
                if (HasTag(it->first->object)) {
                    found = true;
                    break;
                }
            }
        }
        if (!found)
            return;
    }
    self->holding = false;
    if (UnkCollisionContainer* collision = self->owner->collision) {
        u32 flags = 0x100;
        fn_71008635a4(collision, &g_unk_71726bee48, &flags);
    }
}
