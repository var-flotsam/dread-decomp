// CModelInstanceComponent unit (tools/tu_map.py): a component destructor that releases its render object
// (+0x50, 0xffffffff when none) from the render manager, then its file path id (+0x48), then runs the base
// component destructor. Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a83cd0[];  // the component's vtable

void fn_710021949c(UnkRenderManager* self, u64 object, u32 arg2, u32 arg3);

struct UnkModelInstanceComponent_b53 : UnkBaseComponent {
    u8 pad_0x40[0x8];
    UnkStringData* field_0x48;  // CFilePathStrId
    u64 field_0x50;             // render object, 0xffffffff when none
};

void fn_7100936cc0(UnkModelInstanceComponent_b53* self) {
    self->vtable = g_unk_7101a83cd0 + 2;
    if (self->field_0x50 != 0xffffffffu) {
        fn_710021949c(g_unk_7101d47300, self->field_0x50, 0, 0);
        fn_7100219e10(g_unk_7101d47300, self->field_0x50);
        self->field_0x50 = 0xffffffffu;
    }
    if (UnkStringPool* pool = g_unk_7101d47200) {
        UnkStringData** id = &self->field_0x48;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47200;
            }
        }
        *id = &pool->empty;
    }
    fn_71010c4194(self);
}
