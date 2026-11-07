// CPlayerMovement's unit (tools/tu_map.py): the low bit of the first word of the result (returned in
// memory) of the virtual at +0x3d8. Names and layouts are placeholders.
#include "types.h"

struct UnkMovementResult_b45a3 {
    u32 flag : 1;
    u32 rest : 31;

    UnkMovementResult_b45a3(const UnkMovementResult_b45a3& other);
};

struct UnkMovementOwner_b45a3;
struct UnkMovementOwnerVtbl_b45a3 {
    void* slots[0x3d8 / 8];
    UnkMovementResult_b45a3 (*vf_0x3d8)(UnkMovementOwner_b45a3* self, const void* arg);
};
struct UnkMovementOwner_b45a3 {
    UnkMovementOwnerVtbl_b45a3* vtable;
};

// The callers' `arg` (x1) reaches the virtual untouched (tools/forwarded_args.py).
bool fn_71010dca80(UnkMovementOwner_b45a3* self, const void* arg) {
    return self->vtable->vf_0x3d8(self, arg).flag;
}
