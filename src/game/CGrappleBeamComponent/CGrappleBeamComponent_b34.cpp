#include "types.h"

struct UnkGrappleLink {
    u8 unk_0x0[0x10];
    void* target;
};

struct UnkGrappleVisitor {
    void** vtable;
};

struct CGrappleBeamComponent {
    u8 unk_0x0[0x230];
    UnkGrappleLink* link;
};

typedef void (*UnkGrappleVisit)(UnkGrappleVisitor*, void*, void*);

// Hands the grappled target (or null) and `arg` to the visitor's slot 0x180.
void fn_710081c124(CGrappleBeamComponent* self, void* arg, UnkGrappleVisitor* visitor) {
    void* target = self->link ? self->link->target : nullptr;
    reinterpret_cast<UnkGrappleVisit>(visitor->vtable[0x180 / 8])(visitor, target, arg);
}
