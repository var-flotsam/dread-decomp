// CBossCameraCtrl unit (tools/tu_map.py unit 1164). Its destructors: the vtable, the array of counted
// references at +0x2a0 (each dropped, then the array freed), then the base destructor fn_7100763f74;
// fn_710074f058 also frees the object. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CBossCameraCtrl.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/UnkRefSlot_b49a3.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkBossCameraCtrl_b49a3 {
    void** vtable;
    u8 pad_0x8[0x298];
    base::global::CRntVector<UnkRefSlot_b49a3> refs;  // 0x2a0
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkBossCameraCtrl_b49a3* self) {
    self->vtable = g_unk_7101a6df08 + 2;
    self->refs.Free();
    fn_7100763f74(self);
}

void CBossCameraCtrl_Destroy(UnkBossCameraCtrl_b49a3* self) {
    Destroy_b49a3(self);
}

void CBossCameraCtrl_DeletingDestroy(UnkBossCameraCtrl_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
