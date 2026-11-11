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

u32 fn_71006c6a78(UnkSluggerAI* self, UnkSluggerEvent* event) {
    u32 handled = fn_7101105e44(self, event);
    UnkHeldRef ref(*reinterpret_cast<base::UnkRefCounted**>(reinterpret_cast<char*>(self) + 0x147a8));
    if (ref.ptr) {
        if (char* owner = *reinterpret_cast<char**>(reinterpret_cast<char*>(ref.ptr) + 0x10)) {
            if (base::reflection::UnkReflObject* movement =
                    *reinterpret_cast<base::reflection::UnkReflObject**>(owner + 0x1f0)) {
                if (fn_710009d50c(movement->vf_0x0(), fn_71006c7734()))
                    fn_71006c7d1c(movement, event);
            }
        }
    }
    return handled;
}
