// CBreakableTileGroupComponent: a reflected function's invoker (fn_710075d5cc) taking two ints, a pair of
// ints and a string id; the result of fn_7100758b14 is answered by reference. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

struct UnkEnumType;
extern UnkEnumType g_unk_71727115d0;

struct UnkIntPair_l75d5cc {
    u32 first;
    u32 second;
};

struct UnkRawId_l75d5cc {
    UnkStringData* data;
};

void* fn_7100758b14(void* self, u32 a, u32 b, const UnkIntPair_l75d5cc* __attribute__((noescape)) pair, const CStrId* __attribute__((noescape)) name);

UnkReflValue fn_710075d5cc(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    void* self = *reinterpret_cast<void**>(obj + 8);
    u32 first = *base::reflection::refl_ArgValue<u32>(&a[0]);
    u32 second = *base::reflection::refl_ArgValue<u32>(&a[1]);
    const UnkIntPair_l75d5cc* src = base::reflection::refl_ArgValue<UnkIntPair_l75d5cc>(&a[2]);
    UnkIntPair_l75d5cc pair;
    pair.first = src->first;
    pair.second = src->second;
    // A copy of the string id, released by hand: its data pointer stays in a register across the call.
    UnkRawId_l75d5cc name = *base::reflection::refl_ArgValue<UnkRawId_l75d5cc>(&a[3]);
    UnkStringData* data = name.data;
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
    void* result = fn_7100758b14(self, first, second, &pair, reinterpret_cast<const CStrId*>(&name));
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, reinterpret_cast<const base::reflection::CType*>(&g_unk_71727115d0), result, true);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (data && data != &pool->empty) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        name.data = &pool->empty;
    }
    return r;
}
