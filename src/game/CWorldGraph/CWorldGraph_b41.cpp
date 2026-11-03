// CWorldGraph unit (tools/tu_map.py, the name from reflection): a reflected function of the object at 8
// that passes a u32 argument and two nullable arguments to fn_7100acc2f0 and returns the node it finds
// by reference, typed as SWorldGraphNode (the class object g_unk_7172755e70, named where
// fn_7100adcf10 registers it). Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkReflType;
extern UnkReflType g_unk_7172755e70;
void* fn_7100acc2f0(char* self, u32 value, void* arg1, void* arg2);

static inline __attribute__((always_inline)) void* ArgPtr_b41a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100accfc8(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    u32 value = *base::reflection::refl_ArgValue<u32>(&a[0]);
    void* p1 = ArgPtr_b41a3(&a[1]);
    void* p2 = ArgPtr_b41a3(&a[2]);
    void* node = fn_7100acc2f0(target, value, p1, p2);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, reinterpret_cast<const CType*>(&g_unk_7172755e70), node, true);
    return r;
}
