// base::animation::CAnimationBlendSkeletonTransformer's unit: calls the virtual at slot 0x40 with an
// empty shared pointer, 1, -1 and 0.0f. Names are placeholders.
#include <memory>

#include "types.h"

struct UnkBlendTransformerReset_b66a1 {
    void** vtable;
};

void fn_7100003d84(UnkBlendTransformerReset_b66a1* self) {
    typedef void (*Fn)(UnkBlendTransformerReset_b66a1*, const std::shared_ptr<void>&, u32, s64, f32);
    std::shared_ptr<void> none;
    reinterpret_cast<Fn>(self->vtable[0x40 / 8])(self, none, 1, -1, 0.0f);
}
