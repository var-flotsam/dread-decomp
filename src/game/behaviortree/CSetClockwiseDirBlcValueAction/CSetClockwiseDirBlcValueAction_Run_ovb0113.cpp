// behaviortree::CSetClockwiseDirBlcValueAction's unit (tools/tu_map.py, unit 460): the direction from the actor
// (+0x18) to its target (fn_7100d7c990) is tested against the polygon (+0x138) of the actor's floor collision
// (+0x1f0, then +0x128, +0x10, +0x158; a polygon collection is type 16) by fn_71001186d8; results 0 and 2 count
// as clockwise. When the blackboard property (+0x30) is set (fn_7100394e2c), it is given +0x38 with that sign
// (fn_71003954fc). Running (5) then, failed (6) otherwise. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkTU109.h"
#include "types.h"

// Signature as src/game/behaviortree/CStopWatchAction/fn_7100392704.cpp declares it (it returns whether the
// property is set).
struct UnkSelf;
void fn_7100394e2c(UnkSelf* self, void* record, f32* out);
void fn_71003954fc(UnkSelf* behavior, void* record, f32 value);
struct UnkPool_0x48;
struct UnkStruct_0x48;

struct UnkDirVec2_ovb0113 {
    f32 x, y;
};
s32 fn_71001186d8(UnkStruct_0x48* polygon, u16 index, const UnkDirVec2_ovb0113* dir);

struct UnkCollection_ovb0113 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetType();
    u8 pad_0x8[0xa0];
    u8 polygons[1];  // 0xa8
};

struct UnkFloorShape_ovb0113 {
    u8 pad_0x0[0x158];
    UnkCollection_ovb0113* collection;
};
struct UnkFloorOwner_ovb0113 {
    u8 pad_0x0[0x10];
    UnkFloorShape_ovb0113* shape;
};
struct UnkFloor_ovb0113 {
    u8 pad_0x0[0x128];
    UnkFloorOwner_ovb0113* owner;
    u8 pad_0x130[0x8];
    union {
        u32 polygon;  // 0x138
        u16 polygon_index;
    };
};
struct UnkFloorActor_ovb0113 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0x1f0 - 0x58];
    UnkFloor_ovb0113* floor;  // 0x1f0
};

struct UnkClockwiseAction_ovb0113 {
    u8 pad_0x0[0x18];
    UnkActor* actor;
    u8 pad_0x20[0x10];
    u8 property[8];  // 0x30
    f32 value;       // 0x38
};

s32 fn_71003746d8(UnkClockwiseAction_ovb0113* self) {
    UnkFloorActor_ovb0113* target = reinterpret_cast<UnkFloorActor_ovb0113*>(fn_7100d7c990(self->actor));
    UnkFloorActor_ovb0113* actor = reinterpret_cast<UnkFloorActor_ovb0113*>(self->actor);
    UnkFloor_ovb0113* floor = actor->floor;
    if (!floor || !floor->owner || !floor->owner->shape)
        return 6;
    UnkCollection_ovb0113* collection = floor->owner->shape->collection;
    f32 tx = target->x;
    f32 ty = target->y;
    f32 ax = actor->x;
    f32 ay = actor->y;
    if (collection->GetType() != 16 || !collection)
        return 6;
    tx -= ax;
    ty -= ay;
    UnkStruct_0x48* polygon =
        fn_710012431c(reinterpret_cast<UnkPool_0x48*>(collection->polygons), floor->polygon);
    u16 index = floor->polygon_index;
    UnkDirVec2_ovb0113 dir;
    dir.x = tx;
    dir.y = ty;
    s32 side = fn_71001186d8(polygon, index, &dir);
    f32 out;
    UnkSelf* s = reinterpret_cast<UnkSelf*>(self);
    if (!reinterpret_cast<bool (*)(UnkSelf*, void*, f32*)>(fn_7100394e2c)(s, self->property, &out))
        return 6;
    f32 sign = (side == 0 || side == 2) ? 1.0f : -1.0f;
    fn_71003954fc(s, self->property, sign * self->value);
    return 5;
}
