// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee29a0 finds an object of
// the manager's list at +0x930 (other than `skip`) whose kind (vf_0x58) has a bit of `mask`, whose component
// at +0x158 is of kind 0x10 with a shape that contains `point`, and whose id names a sub-area (fn_7100ee2aa0)
// that is not locked (+0x105 clear); the last such object is returned, or null. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::global::CStrId;

struct UnkSubAreaMgr_lee2aa0;
struct UnkSubAreaEntry_lee2aa0;
UnkSubAreaEntry_lee2aa0* CSubAreaManager_FindSubArea(UnkSubAreaMgr_lee2aa0* self, const CStrId* id);

struct UnkShapeComponent_lee29a0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 pad_0x8[0xa0];
    u8 field_0xa8[1];
};

struct UnkObject_lee29a0 {
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
    virtual u32 vf_0x58();
    u8 pad_0x8[0x18];
    CStrId field_0x20;
    u8 pad_0x28[0x130];
    UnkShapeComponent_lee29a0* field_0x158;
};
static_assert(__builtin_offsetof(UnkObject_lee29a0, field_0x20) == 0x20, "layout");
static_assert(__builtin_offsetof(UnkObject_lee29a0, field_0x158) == 0x158, "layout");

struct UnkObjectNode_lee29a0 {
    u8 pad_0x0[0x10];
    UnkObject_lee29a0* object;
};

struct UnkObjectList_lee29a0 {
    u8 pad_0x0[8];
    UnkObjectNode_lee29a0** items;  // 0x8
    u32 count;                      // 0x10
};

struct UnkSubAreaMgr_lee29a0 {
    u8 pad_0x0[0x930];
    UnkObjectList_lee29a0* objects;
};

struct UnkLockedFlag_lee29a0 {
    u8 pad_0x0[0x105];
    bool locked;
};

UnkObject_lee29a0* fn_7100ee29a0(UnkSubAreaMgr_lee29a0* self, const base::math::CVector2D* point, u32 mask,
                                 UnkObject_lee29a0* skip) {
    UnkObject_lee29a0* result = nullptr;
    UnkObjectList_lee29a0* list = self->objects;
    UnkObjectNode_lee29a0** it = list->items;
    for (UnkObjectNode_lee29a0** end = it + list->count; it != end; ++it) {
        UnkObjectNode_lee29a0* node = *it;
        UnkObject_lee29a0* object = node ? node->object : nullptr;
        u32 kind = object->vf_0x58();
        if (object == skip)
            continue;
        if (!(kind & mask))
            continue;
        UnkShapeComponent_lee29a0* component = object->field_0x158;
        UnkShapeComponent_lee29a0* shape = component->vf_0x18() == 0x10 ? component : nullptr;
        if (!fn_7100d65fb0(shape->field_0xa8, point))
            continue;
        UnkLockedFlag_lee29a0* area = reinterpret_cast<UnkLockedFlag_lee29a0*>(
            CSubAreaManager_FindSubArea(reinterpret_cast<UnkSubAreaMgr_lee2aa0*>(self), &object->field_0x20));
        if (area && !area->locked)
            result = object;
    }
    return result;
}
