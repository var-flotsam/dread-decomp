// CGrapplePointComponent unit (tools/tu_map.py unit 1278). fn_7100822278: the destructor; as the
// activatable component's (CDemolitionBlockComponent_Dtor_b44.cpp): the vtable, the string-id table at
// +0x58 reset and its arrays freed, the string id at +0x50 released, then the counted reference at
// +0x40 dropped and the component destructor fn_71010c4194. Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a75660[];

struct UnkGrapplePointComponent_b49a3 : UnkBaseComponent {
    UnkRefObject* ref;  // 0x40
    u8 pad_0x48[0x8];
    UnkStringData* id;  // 0x50, a CStrId
    UnkStrIdTable table;  // 0x58
};

void fn_7100822278(UnkGrapplePointComponent_b49a3* self) {
    self->vtable = g_unk_7101a75660 + 2;
    fn_71004f11f8(&self->table);
    if (self->table.keys)
        self->table.allocator->vf_0x48(self->table.keys);
    if (self->table.values)
        self->table.allocator->vf_0x48(self->table.values);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->id;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    if (UnkRefObject* p = self->ref) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_71010c4194(self);
}
