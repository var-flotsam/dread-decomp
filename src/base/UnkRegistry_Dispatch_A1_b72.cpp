// Unit 63 of tools/tu_map.py (the registry of base/UnkRegistry.h): four calls that pick the handler
// for a string id and path pair and forward the path to one of its virtuals. The handler is the
// registry's default one (+0x58, or +0x50 for the last two) when the id is the empty string, or
// the value stored under the id in the registry's chained map (bucket = (high ^ low) of the
// string's 64-bit hash % capacity; next node at +0x20). A result of 0 is success; 1 is a quiet
// failure; any other result goes to the registry's error callback (+0x68, context at +0x60).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7101d592c8;  // the empty string id (as src/game/CGameManager.cpp declares it)

// A string's pooled data, as far as these calls read it: its 64-bit hash at +0x30.
struct UnkRegistryString_A1b72 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

// The pair passed in: a string id and a file-path id (UnkStrPair_0x10 of base/UnkRegistry.h).
struct UnkRegistryKey_A1b72 {
    UnkRegistryString_A1b72* id;
    UnkRegistryString_A1b72* path;  // 0x8
};

class UnkRegistryHandler_A1b72 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual u32 vf_0x40(UnkRegistryString_A1b72* const* path, void* a, bool b);
    virtual u32 vf_0x48(const u64* hash, bool b);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual u32 vf_0x88(UnkRegistryString_A1b72* const* path, void* a, void* b, bool c);
    virtual u32 vf_0x90(UnkRegistryString_A1b72* const* path);
};

struct UnkRegistryNode_A1b72 {
    UnkRegistryString_A1b72* key;
    UnkRegistryHandler_A1b72* value;  // 0x8
    u8 pad_0x10[0x10];
    UnkRegistryNode_A1b72* next;  // 0x20
};

struct UnkRegistry_A1b72 {
    u8 pad_0x0[0x18];
    UnkRegistryNode_A1b72** buckets;  // 0x18
    u32 count;                        // 0x20
    u32 capacity;                     // 0x24
    u8 pad_0x28[0x28];
    UnkRegistryHandler_A1b72* default_b;  // 0x50
    UnkRegistryHandler_A1b72* default_a;  // 0x58
    void* context;                        // 0x60
    void (*on_error)(void* context, UnkRegistryHandler_A1b72* handler, u32 result);  // 0x68

    template <bool B>
    __attribute__((always_inline)) UnkRegistryHandler_A1b72** Slot(UnkRegistryString_A1b72* const* pkey) {
        UnkRegistryString_A1b72* key = *pkey;
        auto* empty = reinterpret_cast<UnkRegistryString_A1b72*>(g_unk_7101d592c8.data);
        if (key != empty) {
            if (count == 0)
                return nullptr;
            u32 i = ((u32)(key->hash >> 32) ^ (u32)key->hash) % capacity;
            UnkRegistryNode_A1b72** bucket = &buckets[i];
            if (!bucket)
                return nullptr;
            for (UnkRegistryNode_A1b72* n = *bucket; n; n = n->next) {
                if (n->key == key)
                    return &n->value;
            }
            return nullptr;
        }
        return B ? &default_b : &default_a;
    }

    __attribute__((always_inline)) bool Done(UnkRegistryHandler_A1b72* handler, u32 result) {
        if (result == 0)
            return true;
        if (result != 1 && on_error)
            on_error(context, handler, result);
        return false;
    }
};

bool fn_7100087ecc(UnkRegistry_A1b72* self, const UnkRegistryKey_A1b72* key, void* a, bool b) {
    UnkRegistryHandler_A1b72** slot = self->Slot<false>(&key->id);
    if (!slot || !*slot)
        return false;
    UnkRegistryHandler_A1b72* h = *slot;
    return self->Done(h, h->vf_0x40(&key->path, a, b));
}

bool fn_7100087fc0(UnkRegistry_A1b72* self, const UnkRegistryKey_A1b72* key, bool b) {
    UnkRegistryHandler_A1b72** slot = self->Slot<false>(&key->id);
    if (!slot || !*slot)
        return false;
    UnkRegistryHandler_A1b72* h = *slot;
    u64 hash = key->path->hash;
    return self->Done(h, h->vf_0x48(&hash, b));
}

bool fn_71000880c0(UnkRegistry_A1b72* self, const UnkRegistryKey_A1b72* key, void* a, void* b, bool c) {
    UnkRegistryHandler_A1b72** slot = self->Slot<true>(&key->id);
    if (!slot || !*slot)
        return false;
    UnkRegistryHandler_A1b72* h = *slot;
    return self->Done(h, h->vf_0x88(&key->path, a, b, c));
}

bool fn_71000881b4(UnkRegistry_A1b72* self, const UnkRegistryKey_A1b72* key) {
    UnkRegistryHandler_A1b72** slot = self->Slot<true>(&key->id);
    if (!slot || !*slot)
        return false;
    UnkRegistryHandler_A1b72* h = *slot;
    return self->Done(h, h->vf_0x90(&key->path));
}
