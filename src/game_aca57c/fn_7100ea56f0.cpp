// Sets a record's number and builds its two ids from a base string: the string with the number
// appended, and then with "_border" appended as well.
#include "UnkBufferString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a1b5a0[];
using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

// A string id made from a string (fn_7100000850) and moved into place, so never destroyed.
struct UnkTempStrId {
    UnkStringData* data;
};
void fn_7100000850(UnkTempStrId* self, const void* str, bool flag);

// A string id taking over another (CStrId's move-assignment).
struct UnkAssignedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) void operator=(UnkTempStrId&& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
        }
        data = other.data;
        other.data = &pool->empty;
    }
};

extern u8 g_unk_71729cdad8[] __attribute__((visibility("hidden")));
void fn_710007eb6c(void* self, const void* other);
void fn_710007f634(void* self, u32 value);       // appends a number
void fn_710007f564(void* self, const char* str);  // appends a string

struct UnkNumberedIds {
    u32 field_0x0;
    u32 field_0x4;
    UnkAssignedStrId field_0x8;
    UnkAssignedStrId field_0x10;
};

struct UnkTempStrIdT : UnkTempStrId {
    __attribute__((always_inline)) UnkTempStrIdT(const UnkString20& str) { fn_7100000850(this, &str, true); }
};

// NON_MATCHING(minor): the original keeps the moved-from temporary's empty-string stores (it
// outlives the string here), which ours drops as dead.
void fn_7100ea56f0(UnkNumberedIds* self, u32 number) {
    self->field_0x4 = number;
    UnkString20 str;
    fn_710007eb6c(&str, g_unk_71729cdad8);
    fn_710007f634(&str, self->field_0x4);
    self->field_0x8 = UnkTempStrIdT(str);
    fn_710007f564(&str, "_border");
    self->field_0x10 = UnkTempStrIdT(str);
}
