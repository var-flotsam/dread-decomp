// CEmmyAIComponent's unit (tools/tu_map.py): whether `point` is within `radius` of the owner's (+0x30)
// position in the plane (fn_710007b430 drops z); with a negative radius, or when it is not, asks the
// part at +0x14780 (fn_7100573918 with 0.5 and 150). Names are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEmmyNearPart_A3_b64;
bool fn_7100573918(UnkEmmyNearPart_A3_b64* self, const CVector2D* point, f32 a, f32 b);

struct UnkEmmyNearAI_A3_b64 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14780 - 0x38];
    UnkEmmyNearPart_A3_b64* part() { return reinterpret_cast<UnkEmmyNearPart_A3_b64*>(reinterpret_cast<char*>(this) + 0x14780); }
};

static inline __attribute__((always_inline)) CVector3D Position_A3_b64(const UnkActor* a) {
    CVector3D v;
    v.x = a->field_0x44;
    v.y = a->field_0x54;
    v.z = a->field_0x64;
    return v;
}

template <class T>
static inline __attribute__((always_inline)) const T* Addr_A3_b64(const T& value) {
    return &value;
}

bool fn_710054b06c(UnkEmmyNearAI_A3_b64* self, const CVector2D* point, f32 radius) {
    if (radius >= 0.0f) {
        CVector2D at;
        fn_710007b430(&at, Addr_A3_b64(Position_A3_b64(self->owner)));
        f32 dx = at.x - point->x;
        f32 dy = at.y - point->y;
        if (dx * dx + dy * dy <= radius * radius)
            return true;
    }
    return fn_7100573918(self->part(), point, 0.5f, 150.0f);
}
