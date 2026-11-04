// CEmmySancAIComponent's unit (tools/tu_map.py unit 793): when the owner's position lies outside the shape of
// the component found on the target the holder at +0x2a7a8 refers to, writes the tunable's u32 at +0x22c to
// `out`. In state 1 the holder is first given a counted reference built from the member at +0x2a778
// (fn_710023b580, as src/game/CBossAIComponent/fn_71003b0160.cpp) and the state moves to 2. Names and
// layouts are placeholders. The original reads the actor twice; a relaxed atomic load (a plain ldr) keeps
// the second read.
#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkTunable;

// As src/reflection/CCharClassEmmySancAIComponent.cpp declares it.
extern UnkTunable g_unk_71726dd3f0;

struct UnkRefl_fn_710028aca8;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710028aca8>(void*);

struct UnkSancShape_A3_b71 {
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
    virtual bool vf_0x50(const base::math::CVector2D* pos);
};

struct UnkSancShapeOwner_A3_b71 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkSancShapeHolder_A3_b71 : base::UnkRefCounted {
    void* actor;
};

struct UnkSancShapeComp_A3_b71 {
    u8 pad_0x0[0x30];
    UnkSancShapeOwner_A3_b71* owner;
    u8 pad_0x38[0x2a778 - 0x38];
    u8 field_0x2a778[0x28];
    u32 state;
    u8 pad_0x2a7a4[4];
    base::UnkRefHandle holder;
};

void fn_7100500048(UnkSancShapeComp_A3_b71* self, void*, u32* out) {
    if (self->state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x2a778);
            fn_7100234f78(&self->holder, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->state = 2;
    }
    auto* holder = static_cast<UnkSancShapeHolder_A3_b71*>(self->holder.ptr);
    if (!holder || !holder->actor)
        return;
    char* comp = reinterpret_cast<char*>(base::reflection::refl_FindTyped<UnkRefl_fn_710028aca8>(__atomic_load_n(&holder->actor, __ATOMIC_RELAXED)));
    if (!comp)
        return;
    UnkSancShape_A3_b71* shape = *reinterpret_cast<UnkSancShape_A3_b71**>(comp + 0x30);
    if (!shape)
        return;
    base::math::CVector2D pos;
    {
        base::math::CVector3D v = [self] {
            base::math::CVector3D r;
            r.x = self->owner->x;
            r.y = self->owner->y;
            r.z = self->owner->z;
            return r;
        }();
        fn_710007b430(&pos, &v);
    }
    if (!shape->vf_0x50(&pos))
        *out = *reinterpret_cast<u32*>(reinterpret_cast<char*>(&g_unk_71726dd3f0) + 0x22c);
}
