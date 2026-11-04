// CSideEnemyMovement's unit: forwards to the object's vtable slot 0x210 (a result returned in memory). Names are placeholders.
#include "types.h"

// The result is returned through memory (x8).
struct UnkSideEnemyResult_b65a6 {
    u64 words[3];
};

struct UnkSideEnemyMovement_b65a6 {
    virtual void vf_0x0();
    UnkSideEnemyResult_b65a6 vf_0x210() {
        return (*reinterpret_cast<UnkSideEnemyResult_b65a6 (***)(UnkSideEnemyMovement_b65a6*)>(this))[0x210 / 8](this);
    }
};

UnkSideEnemyResult_b65a6 fn_71009b6c44(UnkSideEnemyMovement_b65a6* self) { return self->vf_0x210(); }
