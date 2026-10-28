// Destructor of a holder of a string id (+0x0) and an owned node (+0x8): the node's own object at +0x30 is
// released through its vtable slot 0x10, the node's string id at +0x8 is released, the node is freed
// (fn_7100083c00) and the holder's id is released like CStrId's destructor does.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkOwned {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkNode {
    u8 pad_0x0[0x8];
    CStrId field_0x8;
    u8 pad_0x10[0x20];
    UnkOwned* field_0x30;
};

struct UnkHolder {
    CStrId field_0x0;
    UnkNode* field_0x8;
};

void fn_7100083c00(UnkNode* node);

void fn_71002edacc(UnkHolder* self) {
    UnkNode* node = self->field_0x8;
    self->field_0x8 = nullptr;
    if (node) {
        UnkOwned* owned = node->field_0x30;
        node->field_0x30 = nullptr;
        if (owned)
            owned->vf_0x10();
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (node->field_0x8.data && node->field_0x8.data != &pool->empty) {
                --node->field_0x8.data->ref_count;
                if (node->field_0x8.data->ref_count == 0)
                    fn_71000803a4(pool, node->field_0x8.data);
            }
        }
        fn_7100083c00(node);
    }
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (self->field_0x0.data && self->field_0x0.data != &pool->empty) {
            --self->field_0x0.data->ref_count;
            if (self->field_0x0.data->ref_count == 0) {
                fn_71000803a4(pool, self->field_0x0.data);
                pool = g_unk_7101d47108;
            }
        }
        self->field_0x0.data = &pool->empty;
    }
}
