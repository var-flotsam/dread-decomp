// CSwarmAttackComponent unit (tools/tu_map.py, the name from reflection): fn_7100746460 scales a damage
// request with a request at +0x98 by the swarm's controller (+0x140 -> +0x10, its component at +0x10 cast to
// CCharClassSwarmControllerComponent): the amount (fn_71008c1538) becomes the class's per-member value
// (vf_0x58) times the members fn_710074b41c counts within the class's radius (vf_0x50) of the request's
// point (+0x6c), then the value at
// +0xf8 clamped between the class's vf_0x40 and vf_0x48; the controller gets the point's projection
// (vf_0x198). Then the base handler (fn_7101136bb4) runs. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);

struct UnkSwarmClass_A3_b72 {
    void** vtable;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkSwarmClass_A3_b72*)>(vtable[0])(this);
    }
    f32 vf_0x40() { return reinterpret_cast<f32 (*)(UnkSwarmClass_A3_b72*)>(vtable[0x40 / 8])(this); }
    f32 vf_0x48() { return reinterpret_cast<f32 (*)(UnkSwarmClass_A3_b72*)>(vtable[0x48 / 8])(this); }
    f32 vf_0x50() { return reinterpret_cast<f32 (*)(UnkSwarmClass_A3_b72*)>(vtable[0x50 / 8])(this); }
    f32 vf_0x58() { return reinterpret_cast<f32 (*)(UnkSwarmClass_A3_b72*)>(vtable[0x58 / 8])(this); }
};

struct UnkSwarmController_A3_b72 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkSwarmClass_A3_b72* klass;  // 0x10

    void vf_0x198(CVector2D* point) {
        reinterpret_cast<void (*)(UnkSwarmController_A3_b72*, CVector2D*)>(vtable[0x198 / 8])(this, point);
    }
};

struct UnkSwarmHolder_A3_b72 {
    u8 pad_0x0[0x10];
    UnkSwarmController_A3_b72* controller;  // 0x10
};

struct UnkSwarmRequest_A3_b72 {
    u8 pad_0x0[0x6c];
    CVector3D point;  // 0x6c
    u8 pad_0x78[0x20];
    void* field_0x98;
    u8 pad_0xa0[0x58];
    f32 field_0xf8;
};

struct UnkSwarmAttack_A3_b72 {
    u8 pad_0x0[0x140];
    UnkSwarmHolder_A3_b72* holder;  // 0x140
};

u32 fn_710074b41c(UnkSwarmController_A3_b72* controller, const CVector3D* point, f32 radius);
void fn_7101136bb4(UnkSwarmAttack_A3_b72* self, void* a1, UnkSwarmRequest_A3_b72* request, void* a3, void* a4);

static inline __attribute__((always_inline)) UnkSwarmController_A3_b72* Controller_A3_b72(
    UnkSwarmAttack_A3_b72* self) {
    return self->holder ? self->holder->controller : nullptr;
}

void fn_7100746460(UnkSwarmAttack_A3_b72* self, void* a1, UnkSwarmRequest_A3_b72* request, void* a3, void* a4) {
    if (request->field_0x98 && self->holder && self->holder->controller) {
        UnkSwarmClass_A3_b72* klass = self->holder->controller->klass;
        UnkSwarmClass_A3_b72* cast =
            klass && fn_710009d50c(klass->vf_0x0(), fn_7100733464()) ? klass : nullptr;
        UnkSwarmController_A3_b72* controller = Controller_A3_b72(self);
        f32 count = fn_710074b41c(controller, &request->point, cast->vf_0x50());
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), cast->vf_0x58() * count);
        f32 lo = cast->vf_0x40();
        f32 hi = cast->vf_0x48();
        f32 value = request->field_0xf8;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), value <= lo ? lo : (value >= hi ? hi : value));
        UnkSwarmController_A3_b72* target = Controller_A3_b72(self);
        CVector2D point;
        fn_710007b430(&point, &request->point);
        target->vf_0x198(&point);
    }
    fn_7101136bb4(self, a1, request, a3, a4);
}
