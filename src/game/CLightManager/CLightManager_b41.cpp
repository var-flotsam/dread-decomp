// CLightManager unit (tools/tu_map.py, the name from reflection): a reflected function that removes
// the entry named by the string-id argument from the manager's map at +0x148 (buckets at +0x158,
// count at +0x160, modulus at +0x164): when found, its value at +8 is released (fn_7100019fc0) and
// the map erases the key (fn_7100e0b52c). Returns whether it was found. Names and layouts are
// placeholders.
#include "base/UnkCache.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkKeyData_b41a3 {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkLightNode_b41a3 {
    UnkKeyData_b41a3* key;
    u8 value[0xa0];
    UnkLightNode_b41a3* next;
};

struct UnkLightMap_b41a3 {
    u8 unk_0x0[0x10];
    UnkLightNode_b41a3** buckets;
    u32 count;
    u32 modulus;
};

void fn_7100e0b52c(UnkLightMap_b41a3* map, const CStrId* key);

UnkReflValue fn_7100e0dbf4(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    const CStrId* id;
    if (!arg->type)
        id = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        id = static_cast<const CStrId*>(arg->field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(arg);
    UnkLightMap_b41a3* map = reinterpret_cast<UnkLightMap_b41a3*>(target + 0x148);
    bool removed = false;
    if (map->count != 0) {
        UnkKeyData_b41a3* key = reinterpret_cast<UnkKeyData_b41a3*>(id->data);
        u32 index = (key->field_0x30 ^ key->field_0x34) % map->modulus;
        UnkLightNode_b41a3** bucket = map->buckets + index;
        if (bucket) {
            for (UnkLightNode_b41a3* node = *bucket; node; node = node->next) {
                if (node->key == key) {
                    fn_7100019fc0(node->value, false);
                    fn_7100e0b52c(map, id);
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
