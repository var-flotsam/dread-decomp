// CBreakableVignetteComponent's unit (tools/tu_map.py): the component's update. After the base one
// (fn_710024fb24), an armed (+0x3a) vignette that isn't broken yet (+0x39) breaks when the player's
// position (the translation column of fn_7100dec788's transform, projected to 2D by fn_710007b430)
// is inside its shape (+0x30, vtable slot 0x50): the break of fn_710076351c
// (src/game/CBreakableVignetteComponent/CBreakableVignetteComponent_Break_A5_b66.cpp), inlined.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CActorComponent.h"
#include "game/CBreakableVignetteComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_7100219c6c(UnkRenderManager*, bool, u64, u32, bool);

struct UnkPlayerTransform_bvc {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkVignetteShape_bvc {
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
    virtual bool vf_0x50(const CVector2D& point);  // whether the point is inside
};

struct UnkKeyed_bvc {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkBreakableVignette_A5_b66;

struct UnkBreakableVignette_bvc {
    u8 pad_0x0[0x18];
    u64 field_0x18;
    u8 pad_0x20[0x10];
    UnkVignetteShape_bvc* shape;  // 0x30
    u8 pad_0x38;
    bool broken;  // 0x39
    bool armed;   // 0x3a
    u8 pad_0x3b[5];
    UnkKeyed_bvc* field_0x40;
    u8 pad_0x48[0x18];
    u8 field_0x60[1];
};

static inline void Break_bvc(UnkBreakableVignette_bvc* self) {
    if (self->broken)
        return;
    self->broken = true;
    auto show = reinterpret_cast<u32 (*)(UnkRenderManager*, bool, u64, u32, bool)>(fn_7100219c6c);
    while (show(g_unk_7101d47300, !self->broken, self->field_0x40->key, 0, false) == 2) {
    }
    fn_7100762f64(reinterpret_cast<UnkBreakableVignette_A5_b66*>(self));
    u64 value = self->field_0x18;
    fn_710076371c(self->field_0x60, &value);
}

void fn_7100763414(UnkBreakableVignette_bvc* self) {
    fn_710024fb24(self);
    if (self->broken || !self->armed)
        return;
    UnkPlayerTransform_bvc* player = reinterpret_cast<UnkPlayerTransform_bvc*>(fn_7100dec788(g_unk_7101d47310));
    CVector2D point;
    UnkVignetteShape_bvc* shape = self->shape;
    {
        // The translation column, projected to 2D.
        f32 x = player->x;
        f32 y = player->y;
        f32 z = player->z;
        CVector3D position;
        position.x = x;
        position.y = y;
        position.z = z;
        fn_710007b430(&point, &position);
    }
    if (shape->vf_0x50(point))
        Break_bvc(self);
}
