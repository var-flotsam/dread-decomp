// behaviortree::CXParasiteChooseWanderPositionAction's unit (tools/tu_map.py unit 1094): the start. For an
// X-parasite AI with its char class, takes the actor's position as the centre (the stored one while the
// "found" flag is set or no position is stored; kept inside the camera area's box, shrunk by the char class's
// margin), then picks the next diagonal angle with some jitter and stores a point at the wander distance
// along it plus a far point on the same ray, and clears the flag. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1091.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

#include <math.h>

extern "C" int rand();

using base::math::CVector3D;
using base::math::operator+;
using base::math::operator*;
using base::reflection::UnkReflObject;

struct UnkBehaviorName_ovb0265;

bool fn_7100394bdc(UnkSelf* behavior, UnkBehaviorName_ovb0265* name, bool* out, const base::global::CStrId* section,
                   bool global);
void fn_7100395600(UnkSelf* behavior, void* record, const CVector3D* value);

struct UnkAngles4 {
    float v[4];
};
extern UnkAngles4 g_unk_717270ae88;

struct UnkWanderNames_l6e1500 {
    u8 point[8];
    u8 flag[8];
};
extern UnkWanderNames_l6e1500 g_unk_717270aee0 __attribute__((visibility("hidden")));
extern u8 g_unk_717270aeb0[];

struct UnkWanderCharClass_l6e1500 : UnkReflObject {
    u8 pad_0x8[0x524];
    f32 margin;  // 0x52c
};

struct UnkWanderAI_l6e1500 : UnkReflObject {
    u8 pad_0x8[8];
    UnkReflObject* charclass;  // 0x10
    u8 pad_0x18[0x146f8];
    u32 counter;  // 0x14710
};
static_assert(__builtin_offsetof(UnkWanderAI_l6e1500, counter) == 0x14710, "layout");

struct UnkWanderBox_l6e1500 {
    CVector3D min;
    CVector3D max;
};

struct UnkWanderBounds_l6e1500 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkWanderBox_l6e1500 Box(const void* arg);
    u8 field_0x8[8];
};

struct UnkWanderArea_l6e1500 {
    u8 pad_0x0[0x158];
    UnkWanderBounds_l6e1500* bounds;  // 0x158
};

struct UnkWanderNode_l6e1500 {
    u8 pad_0x0[0x10];
    UnkWanderArea_l6e1500* area;  // 0x10
};

struct UnkWanderHolder_l6e1500 {
    u8 pad_0x0[0xc38];
    UnkWanderNode_l6e1500* node;  // 0xc38
};

struct UnkWander_l6e1500 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0xc];
    f32 distance;  // 0x2c
};

void fn_71006e1500(UnkWander_l6e1500* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* object = static_cast<UnkReflObject*>(self->actor->field_0x240);
    if (!object || !fn_710009d50c(object->vf_0x0(), fn_71006e5f28()))
        return;
    UnkWanderAI_l6e1500* ai = static_cast<UnkWanderAI_l6e1500*>(object);
    UnkReflObject* cc = ai->charclass;
    if (!cc)
        return;
    bool is_cc = fn_710009d50c(cc->vf_0x0(), fn_71006e48f4());
    UnkWanderCharClass_l6e1500* charclass = is_cc ? static_cast<UnkWanderCharClass_l6e1500*>(cc) : nullptr;
    if (!is_cc)
        return;
    CVector3D pos;
    f32 x = self->actor->field_0x44;
    f32 y = self->actor->field_0x54;
    f32 z = self->actor->field_0x64;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    bool found = false;
    fn_7100394bdc(reinterpret_cast<UnkSelf*>(self), reinterpret_cast<UnkBehaviorName_ovb0265*>(&g_unk_717270aee0.flag),
                  &found, &g_unk_7101d470e0, false);
    if (found || !CBehavior_LoadVector3DValue(reinterpret_cast<UnkSelf*>(self), g_unk_717270aea0, &pos)) {
        UnkWanderHolder_l6e1500* holder = static_cast<UnkWanderHolder_l6e1500*>(fn_7100defcec(g_unk_7101d47310));
        if (holder) {
            UnkWanderNode_l6e1500* node = holder->node;
            UnkWanderArea_l6e1500* area = reinterpret_cast<UnkWanderArea_l6e1500*>(node);
            if (node)
                area = node->area;
            UnkWanderBounds_l6e1500* bounds = area->bounds;
            UnkWanderBox_l6e1500 box = bounds->Box(&bounds->field_0x8);
            f32 margin = charclass->margin + 100.0f;
            if (box.max.x - pos.x < margin)
                pos.x = box.max.x - margin;
            else if (pos.x - box.min.x < margin)
                pos.x = margin + box.min.x;
            if (box.max.y - pos.y < margin)
                pos.y = box.max.y - margin;
            else if (pos.y - box.min.y < margin)
                pos.y = margin + box.min.y;
        }
        fn_7100395600(reinterpret_cast<UnkSelf*>(self), g_unk_717270aea0, &pos);
    }
    u32 index = ai->counter & 3;
    f32 base = g_unk_717270ae88.v[index];
    f32 jitter = (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * 20.0f + -10.0f;
    f32 angle = (f32)((f64)(base + jitter) * 3.14159265358979323846 / 180.0);
    ai->counter = index + 1;
    f32 cosine = cosf(angle);
    f32 sine = sinf(angle);
    f32 distance = self->distance;
    if (distance < 0.0f)
        distance = static_cast<UnkWanderCharClass_l6e1500*>(cc)->margin;
    f32 reach = distance + ((f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * 0.7f + -0.35f);
    CVector3D dir = {cosine, sine, 0.0f};
    CVector3D near = pos + dir * reach;
    f32 step = near.x < pos.x ? reach * -0.25f : reach * 0.25f;
    near.x = near.x + step;
    CVector3D far = dir * 10000.0f + pos;
    fn_7100395600(reinterpret_cast<UnkSelf*>(self), &g_unk_717270aee0.point, &near);
    fn_7100395600(reinterpret_cast<UnkSelf*>(self), g_unk_717270aeb0, &far);
    fn_7100395474(reinterpret_cast<UnkSelf*>(self), &g_unk_717270aee0.flag, false);
}
