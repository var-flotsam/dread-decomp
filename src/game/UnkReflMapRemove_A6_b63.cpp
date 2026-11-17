// Copy of fn_7101004edc (src/game/UnkReflMapRemove_b41.cpp) in unit 2604 (no name hints): look the string-id
// argument up in a map of the object at 8 and erase it with the map's own erase (fn_7100f8ede4).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkStrIdToIdMap.h"
#include "game/UnkTU2604.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkKeyData_A6_b63r {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

template <u32 MapOffset, UnkStrIdToIdNode* (*Erase)(UnkStrIdToIdMap*, const CStrId*)>
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
    UnkStrIdToIdMap* map = reinterpret_cast<UnkStrIdToIdMap*>(target + MapOffset);
    bool removed = false;
    if (map->bucket_count != 0) {
        UnkKeyData_A6_b63r* key = reinterpret_cast<UnkKeyData_A6_b63r*>(id->data);
        u32 index = (key->field_0x30 ^ key->field_0x34) % map->divisor;
        UnkStrIdToIdNode** bucket = map->buckets + index;
        if (bucket) {
            for (UnkStrIdToIdNode* node = *bucket; node; node = node->chain) {
                if (node->key.data == id->data) {
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

UnkReflValue fn_7100f9135c(void*, char* obj, UnkReflValue* const* args) {
    return RemoveById_A6_b63<0x30, fn_7100f8ede4>(obj, args);
}
