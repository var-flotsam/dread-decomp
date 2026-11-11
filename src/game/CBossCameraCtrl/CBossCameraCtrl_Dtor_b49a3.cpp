// CBossCameraCtrl unit (tools/tu_map.py unit 1164). Its destructors: the vtable, the array of counted
// references at +0x2a0 (each dropped, then the array freed), then the base destructor fn_7100763f74;
// fn_710074f058 also frees the object. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A counted reference whose destructor keeps the pointer in a register (UnkRefPtr reloads it).
struct UnkRefSlot_b49a3 {
    UnkRefObject* ptr;
    ~UnkRefSlot_b49a3() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

extern void* g_unk_7101a6df08[];

struct UnkBossCameraCtrl_b49a3 {
    void** vtable;
    u8 pad_0x8[0x298];
    base::UnkVector<UnkRefSlot_b49a3> refs;  // 0x2a0
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkBossCameraCtrl_b49a3* self) {
    self->vtable = g_unk_7101a6df08 + 2;
    self->refs.Free();
    fn_7100763f74(self);
}

void fn_710074efb4(UnkBossCameraCtrl_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_710074f058(UnkBossCameraCtrl_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
