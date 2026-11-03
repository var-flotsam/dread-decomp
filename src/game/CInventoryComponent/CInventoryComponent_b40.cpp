// CInventoryComponent unit (tools/tu_map.py, the name from reflection): a reflected getter of the
// amount the inventory's map holds under the id given as argument (through a string-id handle,
// fn_7100ae986c), 0 when there is none, as a reflected float. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

struct UnkKeyData_b40a3 {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkAmountNode_b40a3 {
    UnkKeyData_b40a3* key;
    f32* value;
    u8 unk_0x10[0x10];
    UnkAmountNode_b40a3* next;
};

struct UnkInventory_b40a3 {
    u8 unk_0x0[0x50];
    UnkAmountNode_b40a3** buckets;
    u32 count;
    u32 modulus;
};

UnkReflValue fn_7101149564(void*, char* obj, UnkReflValue* const* args) {
    auto* self = *reinterpret_cast<UnkInventory_b40a3**>(obj + 8);
    UnkReflValue* arg = args[0];
    const CStrId* id;
    if (!arg->type)
        id = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        id = static_cast<const CStrId*>(arg->field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(arg);
    f32 amount;
    {
        alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
        UnkStrIdHandle& h = *reinterpret_cast<UnkStrIdHandle*>(storage);
        fn_7100ae986c(&h, *id);
        amount = 0;
        if (self->count != 0) {
            UnkKeyData_b40a3* key = reinterpret_cast<UnkKeyData_b40a3*>(h.id.data);
            u32 index = (key->field_0x30 ^ key->field_0x34) % self->modulus;
            UnkAmountNode_b40a3** bucket = self->buckets + index;
            if (bucket) {
                for (UnkAmountNode_b40a3* node = *bucket; node; node = node->next) {
                    if (node->key == key) {
                        if (node->value)
                            amount = *node->value;
                        break;
                    }
                }
            }
        }
        h.id.~CStrId();
    }
    f32 v = amount;
    const base::reflection::CType* type = fn_710000abbc();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
