// CChozombieFXComponent unit (tools/tu_map.py unit 1270). Its destructors: the vtable, the render object at +0x2d8 taken off the render manager
// (fn_71002199d8, layer 0) and cleared, the word at +0x50 cleared, the file-path id at +0x48 reset to
// the pool's empty id (released when its count drops to zero), then the component destructor
// fn_71010c4194; fn_7100806fc0 also frees the object. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a74b88[];

// A render object: its item for the render manager follows the vtable.
struct UnkFxRenderObject_b49a3 {
    void** vtable;
    u8 item[0x8];
};

struct UnkChozombieFXComponent_b49a3 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkStringData* path;  // 0x48, a CFilePathStrId
    u32 field_0x50;
    u8 pad_0x54[0x284];
    UnkFxRenderObject_b49a3* item;  // 0x2d8
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkChozombieFXComponent_b49a3* self) {
    self->vtable = g_unk_7101a74b88 + 2;
    if (UnkFxRenderObject_b49a3* item = self->item) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
        self->item = nullptr;
    }
    self->field_0x50 = 0;
    if (UnkStringPool* pool = g_unk_7101d47200) {
        UnkStringData** path = &self->path;
        if (*path && *path != &pool->empty) {
            --(*path)->ref_count;
            if ((*path)->ref_count == 0) {
                fn_71000803a4(pool, *path);
                pool = g_unk_7101d47200;
            }
        }
        *path = &pool->empty;
    }
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

void fn_7100806f1c(UnkChozombieFXComponent_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_7100806fc0(UnkChozombieFXComponent_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
