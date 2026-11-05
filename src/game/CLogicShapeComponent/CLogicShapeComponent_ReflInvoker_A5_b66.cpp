// CLogicShapeComponent's unit: a reflected invoker without arguments: unless the game manager says
// no (fn_7100deabe4), gives the shape at +0x30 (its vf_0x48) the 2D point of the owner's position
// (+0x18, fn_710007b430). Answers the empty status {0, 0, 2, 1} (CEmmyValveComponent_ReflInvoker_A5_b65.cpp).
// Names are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflValue;

struct UnkLogicShape_A5_b66 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(const CVector2D* point);
};

struct UnkLogicShapeComponent_A5_b66 {
    u8 pad_0x0[0x18];
    UnkActor* owner;               // 0x18
    u8 pad_0x20[0x10];
    UnkLogicShape_A5_b66* shape;  // 0x30
};

struct UnkLogicShapePosition_A5_b66 {
    f32 x, y, z;
};

UnkReflValue fn_71009158dc(void*, char* obj, UnkReflValue* const*) {
    UnkLogicShapeComponent_A5_b66* self = *reinterpret_cast<UnkLogicShapeComponent_A5_b66**>(obj + 8);
    if (!fn_7100deabe4(g_unk_7101d47310)) {
        if (UnkLogicShape_A5_b66* shape = self->shape) {
            UnkActor* actor = self->owner;
            f32 x = actor->field_0x44;
            f32 y = actor->field_0x54;
            f32 z = actor->field_0x64;
            UnkLogicShapePosition_A5_b66 pos;
            pos.x = x;
            pos.y = y;
            pos.z = z;
            CVector2D point;
            fn_710007b430(&point, reinterpret_cast<const CVector3D*>(&pos));
            shape->vf_0x48(&point);
        }
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
