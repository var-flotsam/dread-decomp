// behaviortree::CTakumakuChaseAction's unit (tools/tu_map.py unit 1076): the start (fn_71006d1260): the
// base start, the actor's Takumaku AI (+0x30) and its char class (+0x38), the actor's position (+0x40) and
// the char class's direction (+0x4c), which sets or clears the AI's flag at +0x14758. Names and layouts
// are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkStruct_0x68;

struct UnkTakumakuCharClass_ovb0125 : UnkReflObject {
    u8 pad_0x8[0x634];
    s32 direction;  // 0x63c
};

struct UnkTakumakuAI_ovb0125 : UnkReflObject {
    u8 pad_0x8[0x1608];
    UnkReflObject* charclass;  // 0x1610
    u8 pad_0x1618[0x13140];
    bool field_0x14758;
};
static_assert(__builtin_offsetof(UnkTakumakuAI_ovb0125, field_0x14758) == 0x14758, "layout");

struct UnkTakumakuChaseAction_ovb0125 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkTakumakuAI_ovb0125* ai;                // 0x30
    UnkTakumakuCharClass_ovb0125* charclass;  // 0x38
    UnkVec3f start;  // 0x40
    s32 direction;   // 0x4c
};

void fn_71006d1260(UnkTakumakuChaseAction_ovb0125* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* object = self->actor ? static_cast<UnkReflObject*>(self->actor->field_0x240) : nullptr;
    if (!object || !fn_710009d50c(object->vf_0x0(), fn_71006d3ba8())) {
        self->ai = nullptr;
        self->charclass = nullptr;
        return;
    }
    self->ai = static_cast<UnkTakumakuAI_ovb0125*>(object);
    UnkReflObject* cc = self->ai->charclass;
    if (!cc) {
        self->charclass = nullptr;
        return;
    }
    bool is_cc = fn_710009d50c(cc->vf_0x0(), fn_71006d2174());
    UnkTakumakuAI_ovb0125* ai = self->ai;
    UnkTakumakuCharClass_ovb0125* charclass = is_cc ? static_cast<UnkTakumakuCharClass_ovb0125*>(cc) : nullptr;
    self->charclass = charclass;
    if ((charclass == nullptr) | (ai == nullptr))
        return;
    self->start = UnkVec3f(self->actor->field_0x44, self->actor->field_0x54, self->actor->field_0x64);
    s32 direction = charclass->direction;
    self->direction = direction;
    if (direction == -1 || direction >= 1)
        ai->field_0x14758 = false;
    else if (direction == 0)
        ai->field_0x14758 = true;
}
