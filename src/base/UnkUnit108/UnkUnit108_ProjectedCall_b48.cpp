// Unit 108: projects a 3D point onto its x/y plane, maps it through a held callable
// (std::function-like, its target at +0x20) and returns the result with z = 0.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkVec3Result.h"

using base::math::CVector2D;
using base::math::CVector3D;

[[noreturn]] void fn_71001228e0();  // throws bad_function_call

struct UnkVec2Callable {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual UnkVec2Result Call(const CVector2D& p);
};

struct UnkVec2MapperOwner {
    u8 pad_0x0[0x20];
    UnkVec2Callable* target;  // the std::function's target pointer
};

// The lambda clone's call operator: its one capture (+0x8) is the owner of the function.
struct UnkVec2MapperLambda {
    void* vtable;
    UnkVec2MapperOwner* owner;
};

struct UnkProjectedXY : CVector2D {
    explicit UnkProjectedXY(const CVector3D& v) { fn_710007b430(this, &v); }
};

inline UnkVec2Result Invoke(UnkVec2MapperOwner* f, const CVector2D& p) {
    if (f->target == nullptr)
        fn_71001228e0();
    return f->target->Call(p);
}

UnkVec3Result fn_7100122dc8(UnkVec2MapperLambda* self, const CVector3D& p) {
    UnkVec2MapperOwner* f = self->owner;
    UnkVec2Result r = Invoke(f, UnkProjectedXY(p));
    return UnkVec3Result(r.x, r.y, 0.0f);
}
