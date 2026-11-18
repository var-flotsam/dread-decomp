// CChozoCommanderKiStrikeAttack's unit (tools/tu_map.py): grows a bounding box by the strike's point,
// taken from the Chozo Commander AI component (one x for each facing) and eased in from the start
// point over the attack's first 1.2 seconds. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU53.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkKiStrikeXY_l436c80 {
    f32 x, y;
};

struct UnkKiStrikeAI_l436c80 {
    u8 pad_0x0[0x15d1c];
    UnkKiStrikeXY_l436c80 spot;  // 0x15d1c: the strike's spot when the owner faces left
    f32 right_x;  // 0x15d24: its x when the owner faces right
};

struct UnkKiStrikePoint_l436c80 {
    f32 x, y, z;
};

struct UnkKiStrikeBox_l436c80 {
    UnkKiStrikePoint_l436c80 min;
    UnkKiStrikePoint_l436c80 max;
};

struct UnkKiStrike_l436c80 {
    void* vtable;
    UnkActor* owner;  // 0x8
    f32 time;  // 0x10
    u8 pad_0x14[0x44];
    u32 state;  // 0x58
    u8 pad_0x5c[0x10];
    UnkKiStrikePoint_l436c80 start;  // 0x6c
};

void fn_7100436c80(UnkKiStrike_l436c80* self, UnkKiStrikeBox_l436c80* box) {
    if (self->state == 5)
        return;
    UnkReflObject* obj = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_710044e708()))
        return;
    UnkKiStrikeAI_l436c80* ai = reinterpret_cast<UnkKiStrikeAI_l436c80*>(obj);
    UnkKiStrikeXY_l436c80 spot = ai->spot;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    if (m.m[2] > 0.0f)
        spot.x = ai->right_x;
    f32 t = self->time / 1.2f;
    t = t <= 0.0f ? 0.0f : (!(t >= 1.0f) ? t : 1.0f);
    t = fn_7100076020(t, "22");
    f32 x = spot.x, y = spot.y, z = 0.0f;
    if (t < 1.0f) {
        f32 s = 1.0f - t;
        x = x * t + s * self->start.x;
        y = t * y + s * self->start.y;
        z = t * z + s * self->start.z;
    }
    box->min.x = box->min.x < x ? box->min.x : x;
    box->min.y = box->min.y < y ? box->min.y : y;
    box->min.z = box->min.z < z ? box->min.z : z;
    box->max.x = box->max.x > x ? box->max.x : x;
    box->max.y = box->max.y > y ? box->max.y : y;
    box->max.z = box->max.z > z ? box->max.z : z;
}
