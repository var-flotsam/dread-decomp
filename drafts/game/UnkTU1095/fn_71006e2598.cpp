// behaviortree::CXParasiteGoToPointAction's unit (tools/tu_map.py unit 1095): the start: takes the
// tunables the action leaves unset from the X parasite's char class and, when it zigzags, fills its
// path with points alternating to each side of the line to the target. Names and layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkStruct_0x68;

struct UnkXParasiteCharClass_l6e2598 : UnkReflObject {
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual f32 vf_0xb0(UnkActor* actor);

    u8 pad_0x8[0x50c];
    f32 field_0x514;
    f32 field_0x518;
    f32 field_0x51c;
    f32 field_0x520;
    u8 field_0x524;
    u8 pad_0x525[3];
    f32 field_0x528;
};
static_assert(__builtin_offsetof(UnkXParasiteCharClass_l6e2598, field_0x528) == 0x528, "layout");

struct UnkXParasiteAI_l6e2598 : UnkReflObject {
    u8 pad_0x8[8];
    UnkReflObject* charclass;  // 0x10
};

struct UnkXParasiteGoToPointAction_l6e2598 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x21];
    bool zigzag;  // 0x41
    u8 pad_0x42[2];
    UnkVec3f target;     // 0x44
    UnkVec3f direction;  // 0x50
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    u8 field_0x70;
    f32 field_0x74;
    base::global::CRntVector<UnkVec3f> path;  // 0x78
};
static_assert(__builtin_offsetof(UnkXParasiteGoToPointAction_l6e2598, path) == 0x78, "layout");

// The actor's position.
static inline UnkVec3f Position(const UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

bool fn_71006e2b9c(UnkXParasiteGoToPointAction_l6e2598* self);

void fn_71006e2598(UnkXParasiteGoToPointAction_l6e2598* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* object = static_cast<UnkReflObject*>(self->actor->field_0x240);
    if (!object || !fn_710009d50c(object->vf_0x0(), fn_71006e5f28()))
        return;
    UnkReflObject* cc = static_cast<UnkXParasiteAI_l6e2598*>(object)->charclass;
    if (!cc)
        return;
    UnkXParasiteCharClass_l6e2598* charclass =
        fn_710009d50c(cc->vf_0x0(), fn_71006e48f4()) ? static_cast<UnkXParasiteCharClass_l6e2598*>(cc) : nullptr;
    if (!charclass)
        return;
    if (!fn_71006e2b9c(self))
        return;

    self->target.z = 0.0f;
    self->field_0x5c = self->field_0x5c < 0.0f ? charclass->vf_0xb0(self->actor) : self->field_0x5c;
    self->field_0x60 = self->field_0x60 < 0.0f ? charclass->field_0x514 : self->field_0x60;
    self->field_0x64 = self->field_0x64 < 0.0f ? charclass->field_0x518 : self->field_0x64;
    self->field_0x68 = self->field_0x68 < 0.0f ? charclass->field_0x520 : self->field_0x68;
    self->field_0x6c = self->field_0x6c < 0.0f ? charclass->field_0x51c : self->field_0x6c;
    self->field_0x70 = charclass->field_0x524;
    self->field_0x74 = self->field_0x74 < 0.0f ? charclass->field_0x528 : self->field_0x74;

    self->direction = self->target - Position(self->actor);
    self->path.Clear();
    if (!self->zigzag)
        return;

    f32 distance = Length(self->direction);
    f32 spacing = distance > 3000.0f ? 1000.0f : 400.0f;
    f32 offset = distance > 3000.0f ? 80.0f : 30.0f;
    self->direction = Normalized(self->direction);

    s32 steps = (s32)(distance / spacing);
    // The side of the line the first point goes to is random; each next one goes to the other.
    UnkVec3f side(self->direction.y, -self->direction.x, 0.0f);
    f32 random = (rand() & 0x7fff) * (1.0 / 32767.0);
    if (steps > 1) {
        if (random < 0.5f)
            side = -side;
        for (s32 i = 1; i < steps; ++i) {
            UnkVec3f along = Position(self->actor) + self->direction * spacing * (f32)i;
            self->path.PushBackCopy(side * offset + along);
            side = -side;
        }
    }
    self->path.PushBackCopy(self->target);

    if (self->path.count != 0) {
        self->target = self->path[0];
        self->path.EraseAt(0);
        self->direction = self->target - Position(self->actor);
    }
}
