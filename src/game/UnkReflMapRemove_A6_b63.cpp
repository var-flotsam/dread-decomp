// Copy of fn_7101004edc (src/game/UnkReflMapRemove_b41.cpp) in unit 2604 (no name hints): look the string-id
// argument up in a map of the object at 8 and erase it with the map's own erase (fn_7100f8ede4).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkKeyData_A6_b63r {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

// A map node: the key, the value, then the next node at NextOffset.
template <u32 NextOffset>
struct UnkMapNode_A6_b63 {
    UnkKeyData_A6_b63r* key;
    u8 value[NextOffset - 8];
    UnkMapNode_A6_b63* next;
};

template <u32 NextOffset>
struct UnkStrIdMap_A6_b63 {
    u8 unk_0x0[0x10];
    UnkMapNode_A6_b63<NextOffset>** buckets;
    u32 count;
    u32 modulus;
};


template <u32 MapOffset, u32 NextOffset, void (*Erase)(UnkStrIdMap_A6_b63<NextOffset>*, const CStrId*)>
static inline __attribute__((always_inline)) UnkReflValue RemoveById_A6_b63(char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    asm("" ::: "memory");  // the object is read before the argument
    UnkReflValue* arg = args[0];
    const CStrId* id;
    if (!arg->type)
        id = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        id = static_cast<const CStrId*>(arg->field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(arg);
    UnkStrIdMap_A6_b63<NextOffset>* map = reinterpret_cast<UnkStrIdMap_A6_b63<NextOffset>*>(target + MapOffset);
    bool removed = false;
    if (map->count != 0) {
        UnkKeyData_A6_b63r* key = reinterpret_cast<UnkKeyData_A6_b63r*>(id->data);
        u32 index = (key->field_0x30 ^ key->field_0x34) % map->modulus;
        UnkMapNode_A6_b63<NextOffset>** bucket = map->buckets + index;
        if (bucket) {
            for (UnkMapNode_A6_b63<NextOffset>* node = *bucket; node; node = node->next) {
                if (node->key == key) {
                    Erase(map, id);
                    removed = true;
                    break;
                }
            }
        }
    }
    bool found = removed;
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &found, false);
    return r;
}


void fn_7100f8ede4(UnkStrIdMap_A6_b63<0x38>* map, const CStrId* key);

UnkReflValue fn_7100f9135c(void*, char* obj, UnkReflValue* const* args) {
    return RemoveById_A6_b63<0x30, 0x38, fn_7100f8ede4>(obj, args);
}
