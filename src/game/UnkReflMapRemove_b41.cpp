// Reflected functions of one shape in units 2605, 2608 and 2640 (tools/tu_map.py, no name hints):
// look the string-id argument up in a map of the object at 8 (at +0 or +8: buckets at +0x10, count
// at +0x18, modulus at +0x1c; the nodes chained at a per-map offset) and, when found, erase it with
// the map's own erase. Return whether it was found. The same as the music manager's
// (src/game/sound/CMusicManager/CMusicManager_b41.cpp) without the notification; the object pointer
// is read before the argument. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkKeyData_b41a3r {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

// A map node: the key, the value, then the next node at NextOffset.
template <u32 NextOffset>
struct UnkMapNode_b41a3 {
    UnkKeyData_b41a3r* key;
    u8 value[NextOffset - 8];
    UnkMapNode_b41a3* next;
};

template <u32 NextOffset>
struct UnkStrIdMap_b41a3 {
    u8 unk_0x0[0x10];
    UnkMapNode_b41a3<NextOffset>** buckets;
    u32 count;
    u32 modulus;
};

void* fn_7100f94d0c(UnkStrIdMap_b41a3<0x90>* map, const CStrId* key);
void* fn_7100f796b8(UnkStrIdMap_b41a3<0x3c0>* map, const CStrId* key);
void* fn_7100fedf08(UnkStrIdMap_b41a3<0x48>* map, const CStrId* key);
void* fn_7100feefa0(UnkStrIdMap_b41a3<0x68>* map, const CStrId* key);

template <u32 MapOffset, u32 NextOffset, void* (*Erase)(UnkStrIdMap_b41a3<NextOffset>*, const CStrId*)>
static inline __attribute__((always_inline)) UnkReflValue RemoveById_b41a3(char* obj, UnkReflValue* const* args) {
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
    UnkStrIdMap_b41a3<NextOffset>* map = reinterpret_cast<UnkStrIdMap_b41a3<NextOffset>*>(target + MapOffset);
    bool removed = false;
    if (map->count != 0) {
        UnkKeyData_b41a3r* key = reinterpret_cast<UnkKeyData_b41a3r*>(id->data);
        u32 index = (key->field_0x30 ^ key->field_0x34) % map->modulus;
        UnkMapNode_b41a3<NextOffset>** bucket = map->buckets + index;
        if (bucket) {
            for (UnkMapNode_b41a3<NextOffset>* node = *bucket; node; node = node->next) {
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

UnkReflValue fn_7100f96530(void*, char* obj, UnkReflValue* const* args) {
    return RemoveById_b41a3<0, 0x90, fn_7100f94d0c>(obj, args);
}

UnkReflValue fn_7100f9c9b4(void*, char* obj, UnkReflValue* const* args) {
    return RemoveById_b41a3<0, 0x3c0, fn_7100f796b8>(obj, args);
}

UnkReflValue fn_7101003bf4(void*, char* obj, UnkReflValue* const* args) {
    return RemoveById_b41a3<8, 0x48, fn_7100fedf08>(obj, args);
}

UnkReflValue fn_7101004edc(void*, char* obj, UnkReflValue* const* args) {
    return RemoveById_b41a3<8, 0x68, fn_7100feefa0>(obj, args);
}
