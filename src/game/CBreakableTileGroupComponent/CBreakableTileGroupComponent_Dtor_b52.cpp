// The breakable tile group component's destructor (the constructor is fn_71007584c0 in
// CBreakableTileGroupComponent_b46.cpp): the string-id table at +0xd0 (base/UnkStrIdTable.h) is reset
// and its arrays freed, the string id at +0xc8 released, the link at +0xa8 dropped from its list
// (fn_71002b46ec), the word array at +0x78 freed and the member at +0x50 destroyed (fn_71007586b0)
// before the base destructor (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char g_unk_7101a6e3d8[];

struct UnkLinkList;
void fn_71002b46ec(UnkLinkList* list, UnkLinkList** slot);
void fn_71007586b0(void* member);

struct UnkBreakableTileGroup_b52 : UnkBaseComponent {
    u8 pad_0x40[0x10];
    u8 field_0x50[0x28];
    base::UnkVector<u32> list_0x78;
    u64 field_0xa0;
    UnkLinkList* link;  // 0xa8
    void* link_owner;
    void* link_callback;
    u64 field_0xc0;
    UnkStringData* field_0xc8;  // CStrId
    UnkStrIdTable field_0xd0;
};

void fn_71007585b4(UnkBreakableTileGroup_b52* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a6e3d8 + 0x10);
    fn_71004f11f8(&self->field_0xd0);
    if (self->field_0xd0.keys)
        self->field_0xd0.allocator->vf_0x48(self->field_0xd0.keys);
    if (self->field_0xd0.values)
        self->field_0xd0.allocator->vf_0x48(self->field_0xd0.values);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->field_0xc8;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    UnkLinkList** slot = &self->link;
    if (*slot) {
        fn_71002b46ec(*slot, slot);
        *slot = nullptr;
    }
    self->link_owner = nullptr;
    self->link_callback = nullptr;
    self->list_0x78.Free();
    fn_71007586b0(self->field_0x50);
    fn_71010c4194(self);
}
