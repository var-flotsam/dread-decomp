// Unit 346 (base::reflection): a reflected function's invoker (fn_71002609ac, set up by
// fn_71002592d0 in src/reflection/function_inits_00.cpp) that sets the bool at +0x78 of the
// object at +8 and answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkFlagged_b60 {
    u8 pad_0x0[0x78];
    bool field_0x78;
};

UnkReflValue fn_71002609ac(void*, char* obj) {
    (*reinterpret_cast<UnkFlagged_b60**>(obj + 8))->field_0x78 = true;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// fn_7100260a40: removes the 0x60-byte record keyed by the argument from the parallel arrays at
// +8 of the object at 8 (records at +8, u64 keys at +0x10, count at +0x18) through the out-of-line
// fn_71002613ac, and answers whether it was there as a reflected bool.
struct UnkRecord60_b60 {
    u8 data[0x60];
};

struct UnkKeyedRecords_b60 {
    UnkRecord60_b60* values;
    u64* keys;
    u32 count;

    UnkRecord60_b60* Find(const u64& id) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == id)
                return &values[i];
        return values + count;
    }
    UnkRecord60_b60* FindByIndex(const u64& id) {
        u32 n = count;
        u32 i = 0;
        for (u64 j = 0; j < n; ++j)
            if (keys[j] == id) {
                i = j;
                return &values[i];
            }
        return &values[n];
    }
};

void* fn_71002613ac(UnkKeyedRecords_b60* records, UnkRecord60_b60* record);

static inline __attribute__((always_inline)) void* ArgPtr_b60(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100260a40(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    const u64* id = static_cast<const u64*>(ArgPtr_b60(a));
    auto* records = reinterpret_cast<UnkKeyedRecords_b60*>(target + 8);
    UnkRecord60_b60* it = records->Find(*id);
    UnkRecord60_b60* end = records->values + records->count;
    if (records->count != 0) {
        UnkRecord60_b60* p = records->FindByIndex(*id);
        if (p != end)
            fn_71002613ac(records, p);
    }
    bool found = it != end;
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &found, false);
    return r;
}
