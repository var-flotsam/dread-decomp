// CGrapplePointComponent's unit: forwards to the virtual at slot 0x188, which returns a structure in
// memory. Names are placeholders.
#include "types.h"

struct UnkGrapplePointResult_A5_b65 {
    u64 field_0x0[4];
};

struct UnkGrapplePointVirtual_A5_b65 {
    virtual void vf_0x0();
};

typedef UnkGrapplePointResult_A5_b65 (*UnkGrapplePointFn_A5_b65)(UnkGrapplePointVirtual_A5_b65*);

UnkGrapplePointResult_A5_b65 fn_7100822c94(UnkGrapplePointVirtual_A5_b65* self) {
    return (*reinterpret_cast<UnkGrapplePointFn_A5_b65**>(self))[0x188 / 8](self);
}
