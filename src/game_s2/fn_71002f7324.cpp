// Looks up an entry of a hash table of reference-counted objects, keyed by the object at +0x8 of `key`
// (its hash is the xor of two words at +0x30), and returns a counted reference to it. A missing entry is
// created through the table's vtable slot at +0x48.
#include "base/UnkCache.h"
#include "types.h"

struct UnkRefCounted {
    u8 pad_0x0[0x54];
    s32 field_0x54;
};

struct UnkRef {
    UnkRefCounted* field_0x0;
    ~UnkRef() {}
};

struct UnkKeyBody {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkKey {
    u8 pad_0x0[8];
    UnkKeyBody* field_0x8;
};

struct UnkNode {
    UnkKeyBody* field_0x0;
    UnkRefCounted* field_0x8;
    UnkNode* field_0x10;
};

struct UnkTable {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual UnkRefCounted* vf_0x48(UnkKey* key, u32 a, void* b);
    u8 field_0x8[8];
    u32 field_0x10;
    u32 field_0x14;
};

UnkNode** fn_71002f7a2c(void* buckets, u32 index);
void fn_7100017390(UnkRefCounted* object, base::UnkLoadRequest* visitor);

UnkRef fn_71002f7324(UnkTable* self, UnkKey* key, u32 a, void* b) {
    UnkRefCounted* found;
    if (self->field_0x10) {
        u32 h = key->field_0x8->field_0x30 ^ key->field_0x8->field_0x34;
        u32 index = h % self->field_0x14;
        if (index < self->field_0x10) {
            UnkNode* node = *fn_71002f7a2c(self->field_0x8, index);
            while (node) {
                if (node->field_0x0 == key->field_0x8) {
                    found = node->field_0x8;
                    base::UnkLoadRequest visitor = {g_unk_7101a19610 + 0x10, 0, a, b};
                    fn_7100017390(found, &visitor);
                    goto have;
                }
                node = node->field_0x10;
            }
        }
    }
    found = self->vf_0x48(key, a, b);
have:
    UnkRef result = {nullptr};
    if (found) {
        ++found->field_0x54;
        result.field_0x0 = found;
    }
    return result;
}
