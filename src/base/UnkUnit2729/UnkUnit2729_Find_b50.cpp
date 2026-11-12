// From unit 2729 of tools/tu_map.py (0x7101192c5c-0x7101196284): a cache of counted objects keyed by
// a resource pointer, in a chained hash map (buckets through fn_71011955d4) hashed by the two halves
// of the 64-bit id at +0x30 of the key. A miss builds the object (fn_7100255ce8) and adds it
// (fn_7101193cb4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CActorDef.h"
#include "game/UnkTU2729.h"
#include "types.h"

struct UnkKey_71011934b0 {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkCounted_71011934b0 {
    u8 pad_0x0[0x54];
    u32 ref_count;
};

struct UnkNode_71011934b0 {
    UnkKey_71011934b0* key;
    UnkCounted_71011934b0* value;
    UnkNode_71011934b0* next;
};

struct UnkCache_71011934b0 {
    u8 pad_0x0[0xa0];
    u8 map[0x8];  // +0xa0
    u32 count;    // +0xa8
    u32 buckets;  // +0xac
};

// A counted reference to a cached object (returned in memory: it has a destructor).
struct UnkRef_71011934b0 {
    UnkCounted_71011934b0* ptr;
    // Empty: the word is cleared as an integer.
    UnkRef_71011934b0() { *reinterpret_cast<u64*>(&ptr) = 0; }
    UnkRef_71011934b0(UnkCounted_71011934b0* p) {
        ++p->ref_count;
        ptr = p;
    }
    ~UnkRef_71011934b0();
};

char** fn_71011955d4(char*, u32);

UnkRef_71011934b0 fn_71011934b0(UnkCache_71011934b0* self, UnkKey_71011934b0* const* key) {
    UnkCounted_71011934b0* value = nullptr;
    if (self->count) {
        u32 b = ((*key)->id_lo ^ (*key)->id_hi) % self->buckets;
        if (b < self->count) {
            UnkNode_71011934b0* node =
                *reinterpret_cast<UnkNode_71011934b0**>(fn_71011955d4(reinterpret_cast<char*>(self->map), b));
            for (; node; node = node->next) {
                if (node->key == *key) {
                    value = node->value;
                    break;
                }
            }
        }
    }
    if (!value) {
        value = fn_7100255ce8(g_unk_7101d470e0, key, 0);
        if (!value)
            return UnkRef_71011934b0();
        fn_7101193cb4(self, value, key);
    }
    return UnkRef_71011934b0(value);
}
