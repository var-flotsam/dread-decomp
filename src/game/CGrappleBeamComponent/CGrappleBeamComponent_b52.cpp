// CGrappleBeamComponent (unit of tools/tu_map.py hint CGrappleBeamComponent): attaching the beam to a
// grapple point. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkGrappleBeamComponent {
    u8 unk_0x0[0x548];
    UnkWeakHandle* point;  // 0x548
};

typedef void (*UnkGrappleNotify)(UnkWeakTarget* target, UnkGrappleBeamComponent* beam);

// Remembers the point (through its weak handle) and, while it is alive, tells it (vtable slot 0x198).
void fn_7100818bc0(UnkGrappleBeamComponent* self, UnkWeakTarget* point) {
    UnkWeakHandle* handle = GetWeakHandle(point);
    AssignRef(&self->point, handle);
    if (handle && handle->target) {
        UnkWeakTarget* target = handle->target;
        (*reinterpret_cast<UnkGrappleNotify**>(target))[0x198 / 8](target, self);
    }
}
