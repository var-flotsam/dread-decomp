// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): applies a sub-area's settings;
// for the current sub-area without the flag, finds the sub-area whose polygon holds the main camera's
// boundary position and makes it current. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkMask.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2611.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::global::CStrId;

namespace base::reflection {
struct UnkReflObject;
}

struct CSubAreaManager;
struct UnkSubAreaEntry_lee2aa0;
struct UnkSubAreaMgr_lee2aa0;
struct UnkObjMgrB_b189;
struct UnkRecB_b189;

base::reflection::UnkReflObject* fn_7100d786c0(void* entity);  // its "CAMERA" component
UnkSubAreaEntry_lee2aa0* CSubAreaManager_FindSubArea(UnkSubAreaMgr_lee2aa0* self, const CStrId* id);
UnkRecB_b189* fn_7100ee2d84(UnkObjMgrB_b189* manager, const CStrId* id);
void CSubAreaManager_SetCurrentSubArea(CSubAreaManager* self, UnkSubArea* area, bool force, bool flag);

// A shape whose kind 0x10 is the polygon one; the polygon's area is at +0xa8.
struct UnkShape_ed550 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();  // the shape's kind
    u8 pad_0x8[0xa0];
    u8 field_0xa8[1];  // the polygon area
};

struct UnkActor_ed550 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();  // flags
    u8 pad_0x8[0x18];
    CStrId field_0x20;  // its name
    u8 pad_0x28[0x130];
    UnkShape_ed550* field_0x158;
};
static_assert(__builtin_offsetof(UnkActor_ed550, field_0x158) == 0x158, "UnkActor_ed550 layout");

struct UnkActorRef_ed550 {
    u8 pad_0x0[0x10];
    UnkActor_ed550* actor;
};

struct UnkActorList_ed550 {
    u8 pad_0x0[0x8];
    UnkActorRef_ed550** items;
    u32 count;
};

struct UnkCameraComponent_ed550 {
    u8 pad_0x0[0xc38];
    UnkActorRef_ed550* boundary;
};

struct UnkBoundaryCtrl_ed550 : UnkCameraCtrl {
    u8 pad_0x28[0x210];
    base::math::CVector3D field_0x238;  // its position
};
static_assert(__builtin_offsetof(UnkBoundaryCtrl_ed550, field_0x238) == 0x238, "UnkBoundaryCtrl_ed550 layout");

struct UnkSubAreaEntry_ed550 {
    u8 pad_0x0[0x105];
    bool field_0x105;
};

struct UnkSubAreaMgr_ed550 {
    u8 pad_0x0[0x3f0];
    UnkSubArea* field_0x3f0;  // the current sub-area
    u8 pad_0x3f8[0x538];
    UnkActorList_ed550* field_0x930;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_ed550, field_0x930) == 0x930, "UnkSubAreaMgr_ed550 layout");

// The main camera entity, or null.
static inline void* MainCamera() {
    UnkGameManager* gm = g_unk_7101d47310;
    u32 kind_value = g_unk_71727bebb8;
    u64 name = fn_7100001570("MainCamera", 10);
    alignas(8) u32 kind = kind_value;  // stored after the hash, in an 8-byte slot
    return fn_7100deba8c(gm, &name, &kind);
}

void fn_7100eed550(UnkSubAreaMgr_ed550* self, UnkSubArea* area, const UnkMask& mask, bool a, bool b) {
    if (self->field_0x3f0 != area) {
        fn_7100fac120(area, mask, a, b, self->field_0x3f0, false);
        return;
    }
    if (a) {
        fn_7100fac120(area, mask, true, b, nullptr, false);
        return;
    }

    void* camera = MainCamera();
    if (!camera)
        return;
    base::reflection::UnkReflObject* component = fn_7100d786c0(camera);
    if (!component)
        return;
    UnkCameraCtrl* ctrl = fn_71010e2080(component);
    if (!ctrl || !fn_710009d50c(ctrl->vf_0x0(), fn_7100763c7c()))
        return;
    UnkActorRef_ed550* ref = reinterpret_cast<UnkCameraComponent_ed550*>(component)->boundary;
    if (!ref)
        return;
    UnkActor_ed550* boundary = ref->actor;
    if (!boundary)
        return;

    CStrId boundary_name(boundary->field_0x20);
    base::math::CVector2D pos;
    fn_710007b430(&pos, &static_cast<UnkBoundaryCtrl_ed550*>(ctrl)->field_0x238);

    UnkActor_ed550* found = nullptr;
    UnkActorList_ed550* list = self->field_0x930;
    for (UnkActorRef_ed550 **it = list->items, **end = it + list->count; it != end; ++it) {
        UnkActor_ed550* actor = *it ? (*it)->actor : nullptr;
        u32 flags = actor->vf_0x58();
        if (actor == boundary || !(flags & 4))
            continue;
        UnkShape_ed550* shape = actor->field_0x158;
        UnkShape_ed550* polygon = shape->vf_0x18() == 0x10 ? shape : nullptr;
        if (!fn_7100d65fb0(polygon->field_0xa8, &pos))
            continue;
        UnkSubAreaEntry_lee2aa0* entry =
            CSubAreaManager_FindSubArea(reinterpret_cast<UnkSubAreaMgr_lee2aa0*>(self), &actor->field_0x20);
        if (entry && !reinterpret_cast<UnkSubAreaEntry_ed550*>(entry)->field_0x105)
            found = actor;
    }

    if (UnkRecB_b189* next = fn_7100ee2d84(reinterpret_cast<UnkObjMgrB_b189*>(self), &found->field_0x20))
        CSubAreaManager_SetCurrentSubArea(reinterpret_cast<CSubAreaManager*>(self), reinterpret_cast<UnkSubArea*>(next), false, false);
}
