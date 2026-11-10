// CCharClassFanComponent's unit (tools/tu_map.py unit 1336): catching bombs in the fan's area. Every
// CBombMovement in the list fn_7100aa9ecc gives that is not yet caught (+0x488), not yet in the fan's list
// (+0xb80 count, +0xb88 head, +0xb90 tail) and whose owner's position lies in the area (vf_0x50 of the shape's
// +0x158) gets a node from the fan's allocator (+0xb98, flags +0xba0). The parameter type is that of
// src/game/CCharClassFanComponent/CFanComponent_Update_A4_b74.cpp's declaration. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

// As src/game/CCharClassFanComponent/CFanComponent_Update_A4_b74.cpp declares it.
struct UnkFan_A4_b74;

struct UnkFanArea_ovb0122 {
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
    virtual bool vf_0x50(const base::math::CVector2D* point);
};

struct UnkFanShape_ovb0122 {
    u8 pad_0x0[0x158];
    UnkFanArea_ovb0122* area;  // 0x158
};

struct UnkFanBombOwner_ovb0122 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkFanBomb_ovb0122 {
    u8 pad_0x0[0x30];
    UnkFanBombOwner_ovb0122* owner;  // 0x30
    u8 pad_0x38[0x450];
    bool field_0x488;
};
static_assert(__builtin_offsetof(UnkFanBomb_ovb0122, field_0x488) == 0x488, "layout");

struct UnkFanBombEntry_ovb0122 {
    u8 pad_0x0[0x10];
    UnkReflObject* movement;  // 0x10
};

struct UnkFanBombList_ovb0122 {
    UnkFanBombEntry_ovb0122** data;
    u32 count;
};

struct UnkFanNode_ovb0122 {
    UnkFanBomb_ovb0122* bomb;
    u32 field_0x8;
    UnkFanNode_ovb0122* next;
};

struct UnkFan_ovb0122 {
    u8 pad_0x0[0x88];
    UnkFanShape_ovb0122* shape;  // 0x88
    u8 pad_0x90[0xaf0];
    u32 count;                 // 0xb80
    UnkFanNode_ovb0122* head;  // 0xb88
    UnkFanNode_ovb0122* tail;  // 0xb90
    base::UnkAllocator* allocator;  // 0xb98
    u32 alloc_flags;                // 0xba0
};
static_assert(__builtin_offsetof(UnkFan_ovb0122, alloc_flags) == 0xba0, "layout");

UnkFanBombList_ovb0122* fn_7100aa9ecc();

static inline bool Contains_ovb0122(UnkFan_ovb0122* self, UnkFanBomb_ovb0122* bomb) {
    for (UnkFanNode_ovb0122* n = self->head; n; n = n->next) {
        if (n->bomb == bomb)
            return true;
    }
    return false;
}

void fn_710087ea38(UnkFan_A4_b74* fan) {
    UnkFan_ovb0122* self = reinterpret_cast<UnkFan_ovb0122*>(fan);
    if (!self->shape)
        return;
    UnkFanArea_ovb0122* area = self->shape->area;
    if (!area)
        return;
    UnkFanBombList_ovb0122* list = fn_7100aa9ecc();
    u32 count = list->count;
    if (!count)
        return;
    UnkFanBombEntry_ovb0122** it = list->data;
    UnkFanBombEntry_ovb0122** end = it + count;
    for (; it != end; ++it) {
        UnkFanBombEntry_ovb0122* entry = *it;
        if (!entry)
            continue;
        UnkReflObject* movement = entry->movement;
        if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_7100aa9174()))
            continue;
        UnkFanBomb_ovb0122* bomb = reinterpret_cast<UnkFanBomb_ovb0122*>(movement);
        if (bomb->field_0x488 || Contains_ovb0122(self, bomb))
            continue;
        UnkFanBombOwner_ovb0122* owner = bomb->owner;
        if (!owner)
            continue;
        base::math::CVector2D point;
        f32 x = owner->x;
        f32 y = owner->y;
        f32 z = owner->z;
        base::math::CVector3D pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        fn_710007b430(&point, &pos);
        if (!area->vf_0x50(&point))
            continue;
        UnkFanNode_ovb0122* node =
            static_cast<UnkFanNode_ovb0122*>(self->allocator->vf_0x40(0x18, 8, self->alloc_flags));
        if (!node)
            continue;
        node->bomb = bomb;
        node->field_0x8 = 0;
        node->next = nullptr;
        if (self->count == 0) {
            self->head = self->tail = node;
            self->count = 1;
        } else {
            self->tail->next = node;
            __builtin_memset(&node->next, 0, sizeof(node->next));
            self->tail = node;
            ++self->count;
        }
    }
}
