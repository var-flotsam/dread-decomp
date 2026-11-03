// CSpitclawkAIComponent unit (tools/tu_map.py unit 1067). Its destructors: the vtable, the array of
// counted references at +0x146f8 (each dropped, then the array freed), then the base destructor
// fn_710067c3f8; fn_71006c8a28 also frees the object. Names and layouts are placeholders.
#include "base/UnkVector.h"
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

extern void* g_unk_7101a65cd8[];

struct UnkSpitclawkAIComponent_b49a3 {
    void** vtable;
    u8 pad_0x8[0x146f0];
    base::UnkVector<UnkRefSlot_b49a3> refs;  // 0x146f8
};

void fn_710067c3f8(UnkSpitclawkAIComponent_b49a3* self);

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkSpitclawkAIComponent_b49a3* self) {
    self->vtable = g_unk_7101a65cd8 + 2;
    self->refs.Free();
    fn_710067c3f8(self);
}

void fn_71006c8970(UnkSpitclawkAIComponent_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_71006c8a28(UnkSpitclawkAIComponent_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
