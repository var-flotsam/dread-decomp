// CCameraBoundaryCtrl unit: whether the camera moves to another sub-area. The controlled object's current
// sub-area (its weak reference at +0xc38, or the one under `pos` when it has none) is kept unless the
// manager is following a named area already, the point the player (or its morph-ball camera track) is at
// still lies in the area's polygons, the manager holds the switch, or the player is held by another actor;
// otherwise the next sub-area is looked up and taken over, and the transition between the two names is
// asked for. `left` is set when no next sub-area is found while one is current. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CGrabComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

// As src/game_aca57c/fn_7100d78378.cpp defines it.
template <u32 Offset>
struct UnkWeakRefAt {
    u8 pad_0x0[Offset];
    UnkWeakHandle* ref;
};
void fn_71010e6038(UnkWeakRefAt<0xc30>* self, UnkWeakTarget* other);
void fn_71010e6158(UnkWeakRefAt<0xc38>* self, UnkWeakTarget* other);

struct UnkRecB_b189;
struct UnkObjMgrB_b189;
UnkRecB_b189* fn_7100ee2d84(UnkObjMgrB_b189* manager, const CStrId* id);
struct UnkAreaObject_b189;
struct UnkAreaMgr_b189;
UnkAreaObject_b189* fn_7100eeb730(UnkAreaMgr_b189* self, const base::math::CVector2D* pos);
struct UnkManager;

// The sub-area's collision shape; kind 0x10 is a polygon collection.
struct UnkBoundaryShape_l766a68 {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 polygons[0x8];  // 0xa8

    u32 Kind() { return reinterpret_cast<u32 (*)(UnkBoundaryShape_l766a68*)>(vtable[0x18 / 8])(this); }
};

struct UnkBoundaryArea_l766a68 {
    void** vtable;
    u8 pad_0x8[0x18];
    CStrId name;  // 0x20
    u8 pad_0x28[0x8];
    void* field_0x30;
    u8 pad_0x38[0x120];
    UnkBoundaryShape_l766a68* shape;  // 0x158

    u32 Kind() { return reinterpret_cast<u32 (*)(UnkBoundaryArea_l766a68*)>(vtable[0x58 / 8])(this); }
};

// Whether the sub-area is hidden (+0x105) in the manager's records.
struct UnkBoundaryRecord_l766a68 {
    u8 pad_0x0[0x105];
    bool hidden;  // 0x105
};

struct UnkSubAreaManager_l766a68 {
    u8 pad_0x0[0x930];
    void* field_0x930;
    u8 pad_0x938[0x98];
    CStrId followed;  // 0x9d0
};

bool fn_7100eeb244(UnkSubAreaManager_l766a68* self);
UnkBoundaryArea_l766a68* fn_7100ee2928(UnkSubAreaManager_l766a68* self, const CStrId* name);
UnkBoundaryArea_l766a68* fn_7100ee29a0(UnkSubAreaManager_l766a68* self, const CVector2D* pos, u32 kind,
                                       UnkBoundaryArea_l766a68* current);
u32 fn_7100eeb11c(UnkSubAreaManager_l766a68* self, const CStrId* from, const CStrId* to);

struct UnkBoundaryObject_l766a68 {
    u8 pad_0x0[0xc38];
    UnkWeakHandle* area;  // 0xc38
};

struct UnkBoundaryOwner_l766a68 {
    u8 pad_0x0[0x10];
    UnkBoundaryObject_l766a68* object;
};

struct UnkCameraBoundaryCtrl_l766a68 {
    u8 pad_0x0[0xb0];
    UnkBoundaryOwner_l766a68* owner;
};

struct UnkBoundaryGrab_l766a68 {
    u8 pad_0x0[0xc8];
    UnkWeakHandle* field_0xc8;
    UnkWeakHandle* field_0xd0;
};

struct UnkBoundaryPlayer_l766a68 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1a8];
    void* field_0x210;
    u8 pad_0x218[0x8];
    UnkAnimationComponent* animation;  // 0x220
    void* model;  // 0x228
    u8 pad_0x230[0x30];
    UnkBoundaryGrab_l766a68* grab;  // 0x260
};

static inline UnkSubAreaManager_l766a68* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkSubAreaManager_l766a68*>(manager);
}

static inline UnkBoundaryArea_l766a68* AreaOf(UnkWeakHandle* ref) {
    return ref ? reinterpret_cast<UnkBoundaryArea_l766a68*>(ref->target) : nullptr;
}

// Whether an actor other than the player holds it.
static inline bool HeldByOther(UnkBoundaryPlayer_l766a68* player) {
    if (!player->grab || !fn_710113d83c(player->grab))
        return false;
    UnkBoundaryGrab_l766a68* grab = player->grab;
    void* other = grab->field_0xd0 ? grab->field_0xd0->target : nullptr;
    if (!other || other == player)
        other = grab->field_0xc8 ? grab->field_0xc8->target : nullptr;
    return other && other != player;
}

bool fn_7100766a68(UnkCameraBoundaryCtrl_l766a68* self, const CVector3D* pos, bool* left) {
    *left = false;
    UnkSubAreaManager_l766a68* manager = SubAreaManager();
    UnkBoundaryArea_l766a68* area = AreaOf(self->owner->object->area);
    CStrId name = area ? CStrId(area->name) : CStrId(g_unk_7101d470e0);
    if (area) {
        auto* record = reinterpret_cast<UnkBoundaryRecord_l766a68*>(
            fn_7100ee2d84(reinterpret_cast<UnkObjMgrB_b189*>(manager), &area->name));
        if (!record || record->hidden)
            area = nullptr;
    } else {
        CVector2D at;
        fn_710007b430(&at, pos);
        area = reinterpret_cast<UnkBoundaryArea_l766a68*>(
            fn_7100eeb730(reinterpret_cast<UnkAreaMgr_b189*>(manager), &at));
        UnkBoundaryOwner_l766a68* owner = self->owner;
        fn_71010e6158(reinterpret_cast<UnkWeakRefAt<0xc38>*>(owner ? owner->object : nullptr),
                      reinterpret_cast<UnkWeakTarget*>(area));
    }

    UnkBoundaryPlayer_l766a68* player =
        reinterpret_cast<UnkBoundaryPlayer_l766a68*>(fn_7100dec788(g_unk_7101d47310));
    CVector2D target;
    fn_710007b430(&target, pos);
    if (player) {
        {
            f32 x = player->x;
            f32 y = player->y;
            f32 z = player->z;
            CVector2D flat;
            CVector3D at;
            at.x = x;
            at.y = y;
            at.z = z;
            fn_710007b430(&flat, &at);
            target.x = flat.x;
            target.y = flat.y;
        }
        if (player->field_0x210) {
            if (UnkAnimationComponent* animation = player->animation) {
                if (fn_710117b348(animation, fn_7100c3f96c()) && player->model) {
                    CVector3D track;
                    track.x = 0.0f;
                    track.y = 0.0f;
                    track.z = 0.0f;
                    u64 id = 0xd280200dfa7467a3;
                    if (fn_710117364c(player->model, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(&track))) {
                        CVector2D flat;
                        fn_710007b430(&flat, &track);
                        target.x = flat.x;
                        target.y = flat.y;
                    }
                }
            }
        }
    }

    if (fn_7100eeb244(manager)) {
        if (manager->followed == name)
            return false;
    } else {
        if (!manager->field_0x930)
            return false;
        if (area && area->field_0x30 && area->Kind() == 4) {
            UnkBoundaryShape_l766a68* shape = area->shape;
            UnkBoundaryShape_l766a68* polygons = shape->Kind() == 0x10 ? shape : nullptr;
            if (fn_7100d65fb0(polygons->polygons, &target))
                return false;
        }
    }

    UnkBoundaryArea_l766a68* next = nullptr;
    if (!fn_7100eef674(SubAreaManager()) && !(player && HeldByOther(player))) {
        if (fn_7100eeb244(manager))
            next = fn_7100ee2928(manager, &manager->followed);
        else
            next = fn_7100ee29a0(manager, &target, 4, AreaOf(self->owner->object->area));
    }
    UnkBoundaryObject_l766a68* object = self->owner ? self->owner->object : nullptr;
    if (next) {
        fn_71010e6038(reinterpret_cast<UnkWeakRefAt<0xc30>*>(object),
                      reinterpret_cast<UnkWeakTarget*>(AreaOf(self->owner->object->area)));
        UnkBoundaryOwner_l766a68* owner = self->owner;
        fn_71010e6158(reinterpret_cast<UnkWeakRefAt<0xc38>*>(owner ? owner->object : nullptr),
                      reinterpret_cast<UnkWeakTarget*>(next));
        return fn_7100eeb11c(manager, &name, &next->name) == 1;
    }
    if (AreaOf(object->area))
        *left = true;
    return false;
}
