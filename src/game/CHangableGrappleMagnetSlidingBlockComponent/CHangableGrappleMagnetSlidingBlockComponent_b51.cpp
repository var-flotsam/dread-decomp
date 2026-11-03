// CHangableGrappleMagnetSlidingBlockComponent: grapple attach test (unit 1281).
#include "types.h"

namespace {
struct UnkVObj {
    virtual void vf_0(); virtual void vf_1(); virtual void vf_2(); virtual void vf_3();
    virtual void vf_4(); virtual void vf_5(); virtual void vf_6(); virtual void vf_7();
    virtual void vf_8(); virtual void vf_9(); virtual void vf_10();
    virtual u32 vf_11();
};
}
struct UnkGrappleQuery { char* ctx; };

bool fn_7100825098(void* a, void* b, UnkGrappleQuery* q);

bool fn_710082456c(void* a, void* b, UnkGrappleQuery* q) {
    if (!fn_7100825098(a, b, q))
        return false;
    if (reinterpret_cast<UnkVObj*>(*(char**)(q->ctx + 0x10))->vf_11() & 0x20000)
        return true;
    return (reinterpret_cast<UnkVObj*>(*(char**)(q->ctx + 0x10))->vf_11() >> 11) & 1;
}
