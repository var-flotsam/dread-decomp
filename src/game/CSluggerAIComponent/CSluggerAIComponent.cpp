// CSluggerAIComponent: fn_71006c6a78 runs the base handler (fn_7101105e44), then, while holding a
// reference to the handle at +0x147a8, passes the event on (fn_71006c7d1c) to the component at
// +0x1f0 of the handle's target when that component is a CSluggerAcidBallMovementComponent.
// Returns the base handler's result. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkSluggerAI;
struct UnkSluggerEvent;

void fn_71006c7d1c(base::reflection::UnkReflObject* movement, UnkSluggerEvent* event);

// The handle's target (base::UnkRefCounted's layout); its owner is at +0x10.
struct UnkSluggerTarget : base::UnkRefCounted {
    char* field_0x10;
};

namespace {
struct UnkHeldRef {
    base::UnkRefCounted* ptr;
    explicit UnkHeldRef(base::UnkRefCounted* p) : ptr(p) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkHeldRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};
}  // namespace

// The part of the AI component this handler reads: the handle at +0x147a8.
struct UnkSluggerAI_71006c6a78 {
    u8 pad_0x0[0x147a8];
    base::UnkRefCounted* field_0x147a8;  // 0x147a8
};

u32 fn_71006c6a78(UnkSluggerAI* self, UnkSluggerEvent* event) {
    u32 handled = fn_7101105e44(self, event);
    UnkHeldRef ref(reinterpret_cast<UnkSluggerAI_71006c6a78*>(self)->field_0x147a8);
    if (ref.ptr) {
        if (char* owner = static_cast<UnkSluggerTarget*>(ref.ptr)->field_0x10) {
            if (base::reflection::UnkReflObject* movement =
                    *reinterpret_cast<base::reflection::UnkReflObject**>(owner + 0x1f0)) {
                if (fn_710009d50c(movement->vf_0x0(), fn_71006c7734()))
                    fn_71006c7d1c(movement, event);
            }
        }
    }
    return handled;
}
