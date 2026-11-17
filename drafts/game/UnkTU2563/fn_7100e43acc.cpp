// CMinimapData's unit (2563): rebuilds the map from each breakable vignette's owner id to the ids of the
// doors and the other keyed rectangles whose centres lie inside the vignette's shape.
// Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkNoArg.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChangeStageNavMeshItemComponent.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::UnkIterValue;
using base::UnkStrIdVec;
using base::global::CRntSmallDictionary;
using base::global::CStrId;
using base::math::CVector2D;
using base::reflection::UnkReflObject;

// A 0x60-byte record with its 2D bounds at 8, destroyed out of line.
struct UnkRect60_le43acc {
    u64 field_0x0;
    CVector2D min;
    CVector2D max;
    u8 unk_0x18[0x48];

    ~UnkRect60_le43acc() { fn_7100e4a1d0(reinterpret_cast<UnkRecord60*>(this)); }
};
static_assert(sizeof(UnkRect60_le43acc) == 0x60, "UnkRect60_le43acc layout");

// A 0x28-byte record with its 2D bounds at 8 and a string id at 0x18.
struct UnkRect28_le43acc {
    u64 field_0x0;
    CVector2D min;
    CVector2D max;
    CStrId field_0x18;
    u64 field_0x20;
};
static_assert(sizeof(UnkRect28_le43acc) == 0x28, "UnkRect28_le43acc layout");

struct UnkShape_le43acc {
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
    virtual bool Contains(const CVector2D* point);  // 0x50
};

struct UnkOwner_le43acc {
    u8 unk_0x0[0x30];
    CStrId id;
};

struct UnkVignette_le43acc : UnkReflObject {
    u8 unk_0x8[0x10];
    UnkOwner_le43acc* owner;  // 0x18
    u8 unk_0x20[0x10];
    UnkShape_le43acc* shape;  // 0x30
};

struct UnkHandle_le43acc {
    u8 unk_0x0[0x10];
    UnkReflObject* target;
};
struct UnkHandleList_le43acc {
    UnkHandle_le43acc** data;
    u32 count;
};
UnkHandleList_le43acc* fn_7100914230(UnkNoArg);

using UnkIdLists_le43acc = CRntSmallDictionary<CStrId, UnkStrIdVec>;

struct UnkMinimapRects_le43acc {
    u8 pad_0x0[0x6140];
    CRntSmallDictionary<CStrId, UnkRect60_le43acc> field_0x6140;
    CRntSmallDictionary<CStrId, UnkRect28_le43acc> field_0x6168;
    CRntSmallDictionary<CStrId, UnkRect28_le43acc> field_0x6190;
    CRntSmallDictionary<CStrId, UnkRect28_le43acc> field_0x61b8;
    CRntSmallDictionary<CStrId, UnkRect28_le43acc> field_0x61e0;
    CRntSmallDictionary<CStrId, UnkRect28_le43acc> field_0x6208;
    u8 pad_0x6230[0xf0];
    UnkIdLists_le43acc field_0x6320;
};
static_assert(__builtin_offsetof(UnkMinimapRects_le43acc, field_0x6208) == 0x6208, "layout");
static_assert(__builtin_offsetof(UnkMinimapRects_le43acc, field_0x6320) == 0x6320, "layout");

// Adds an empty id list under `key` and returns it.
inline UnkStrIdVec& AddIdList(UnkIdLists_le43acc& lists, const CStrId& key) {
    base::UnkAllocator* allocator = fn_7100083990();
    if (lists.count == lists.capacity)
        fn_71009ba838(&lists, lists.count + lists.step);
    new (&lists.keys[lists.count]) CStrId(key);
    UnkStrIdVec& ids = lists.values[lists.count];
    ids.data = nullptr;
    ids.count = 0;
    ids.capacity = 0;
    ids.step = 10;
    ids.field_0x14 = true;
    ids.flags = 0xf;
    ids.allocator = allocator;
    u32 n = ++lists.count;
    UnkStrIdVec* end = &lists.values[n];
    return n ? end[-1] : *end;
}

// Whether the centre of a record's bounds lies inside `shape`.
template <class Rect>
inline bool CentreInside(UnkShape_le43acc* shape, const Rect& rect) {
    CVector2D centre;
    centre.x = (rect.min.x + rect.max.x) * 0.5f;
    centre.y = (rect.min.y + rect.max.y) * 0.5f;
    return shape->Contains(&centre);
}

void fn_7100e43acc(UnkMinimapRects_le43acc* self) {
    fn_71009ba68c(reinterpret_cast<UnkNavMeshItemMember*>(&self->field_0x6320));
    UnkHandleList_le43acc* list = fn_7100914230(UnkNoArg());
    for (UnkHandle_le43acc **h = list->data, **end = list->data + list->count; h != end; ++h) {
        UnkHandle_le43acc* handle = *h;
        if (!handle)
            continue;
        UnkReflObject* target = handle->target;
        if (!target || !fn_710009d50c(target->vf_0x0(), fn_710076238c()))
            continue;
        UnkVignette_le43acc* vignette = static_cast<UnkVignette_le43acc*>(target);
        UnkStrIdVec& ids = AddIdList(self->field_0x6320, vignette->owner->id);
        UnkShape_le43acc* shape = vignette->shape;
        // The key of every record whose centre lies inside the shape: the doors, then the other tables
        // through one iterator.
        {
            UnkIterValue<UnkRect60_le43acc, CStrId> it;
            CRntSmallDictionary<CStrId, UnkRect60_le43acc>& doors = self->field_0x6140;
            for (it.SetRef(doors.keys, doors.values); it.ref.index != &doors.keys[doors.count];
                 ++it.ref.index, ++it.ref.element)
                if (CentreInside(shape, it.Element()))
                    ids.PushBack(it.Index());
        }
        UnkIterValue<UnkRect28_le43acc, CStrId> it;
        CRntSmallDictionary<CStrId, UnkRect28_le43acc>& rects_6168 = self->field_0x6168;
        for (it.SetRef(rects_6168.keys, rects_6168.values); it.ref.index != &rects_6168.keys[rects_6168.count];
             ++it.ref.index, ++it.ref.element)
            if (CentreInside(shape, it.Element()))
                ids.PushBack(it.Index());
        CRntSmallDictionary<CStrId, UnkRect28_le43acc>& rects_6190 = self->field_0x6190;
        for (it.SetRef(rects_6190.keys, rects_6190.values); it.ref.index != &rects_6190.keys[rects_6190.count];
             ++it.ref.index, ++it.ref.element)
            if (CentreInside(shape, it.Element()))
                ids.PushBack(it.Index());
        CRntSmallDictionary<CStrId, UnkRect28_le43acc>& rects_61b8 = self->field_0x61b8;
        for (it.SetRef(rects_61b8.keys, rects_61b8.values); it.ref.index != &rects_61b8.keys[rects_61b8.count];
             ++it.ref.index, ++it.ref.element)
            if (CentreInside(shape, it.Element()))
                ids.PushBack(it.Index());
        CRntSmallDictionary<CStrId, UnkRect28_le43acc>& rects_61e0 = self->field_0x61e0;
        for (it.SetRef(rects_61e0.keys, rects_61e0.values); it.ref.index != &rects_61e0.keys[rects_61e0.count];
             ++it.ref.index, ++it.ref.element)
            if (CentreInside(shape, it.Element()))
                ids.PushBack(it.Index());
        CRntSmallDictionary<CStrId, UnkRect28_le43acc>& rects_6208 = self->field_0x6208;
        for (it.SetRef(rects_6208.keys, rects_6208.values); it.ref.index != &rects_6208.keys[rects_6208.count];
             ++it.ref.index, ++it.ref.element)
            if (CentreInside(shape, it.Element()))
                ids.PushBack(it.Index());
    }
}
