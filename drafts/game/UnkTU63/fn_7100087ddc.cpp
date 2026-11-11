#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7101d592c8;

struct UnkRegistryString_b0248 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkRegistryKey_b0248 {
    UnkRegistryString_b0248* id;
    UnkRegistryString_b0248* path;  // 0x8
};

class UnkRegistryHandler_b0248 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual u32 vf_0x38(UnkRegistryString_b0248* const* path);
};

struct UnkRegistryNode_b0248 {
    UnkRegistryString_b0248* key;
    UnkRegistryHandler_b0248* value;  // 0x8
    u8 pad_0x10[0x10];
    UnkRegistryNode_b0248* next;  // 0x20
};

struct UnkRegistry_b0248 {
    u8 pad_0x0[0x18];
    UnkRegistryNode_b0248** buckets;  // 0x18
    u32 count;                        // 0x20
    u32 capacity;                     // 0x24
    u8 pad_0x28[0x30];
    UnkRegistryHandler_b0248* default_a;  // 0x58
    void* context;                        // 0x60
    void (*on_error)(void* context, UnkRegistryHandler_b0248* handler, u32 result);  // 0x68

    __attribute__((always_inline)) UnkRegistryHandler_b0248** Slot(UnkRegistryString_b0248* const* pkey) {
        auto* empty = reinterpret_cast<UnkRegistryString_b0248*>(g_unk_7101d592c8.data);
        UnkRegistryString_b0248* key = *pkey;
        if (key != empty) {
            if (count == 0)
                return nullptr;
            u32 i = ((u32)(key->hash >> 32) ^ (u32)key->hash) % capacity;
            UnkRegistryNode_b0248** bucket = &buckets[i];
            if (!bucket)
                return nullptr;
            for (UnkRegistryNode_b0248* n = *bucket; n; n = n->next) {
                if (n->key == key)
                    return &n->value;
            }
            return nullptr;
        }
        return &default_a;
    }

    __attribute__((always_inline)) bool Done(UnkRegistryHandler_b0248* handler, u32 result) {
        if (result == 0)
            return true;
        if (result != 1 && on_error)
            on_error(context, handler, result);
        return false;
    }
};

bool fn_7100087ddc(UnkRegistry_b0248* self, const UnkRegistryKey_b0248* key) {
    UnkRegistryHandler_b0248** slot = self->Slot(&key->id);
    if (!slot || !*slot)
        return false;
    UnkRegistryHandler_b0248* h = *slot;
    return self->Done(h, h->vf_0x38(&key->path));
}
