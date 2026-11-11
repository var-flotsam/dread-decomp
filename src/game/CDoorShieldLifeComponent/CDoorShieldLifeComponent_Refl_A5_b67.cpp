// CDoorShieldLifeComponent's unit (tools/tu_map.py): two reflected functions of the door. fn_71008dc7b0 passes three
// bool arguments to the door's open/close (fn_71008d498c, as fn_71008dc890 in CDoorLifeComponent_ReflInvoker_A5_b66.cpp)
// and returns its bool result; fn_71008dca58 locks the door (fn_71008d586c with true, 1 and an empty string id, as
// fn_7100bd37dc in CCharClassSetDoorLockedEvent_Execute.cpp) and returns the int 1. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU1376.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkDoorLife_b44a2;
struct UnkDoorLock;
bool fn_71008d586c(UnkDoorLock* door, bool locked, u32 mode, CStrId* name);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b67(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008dc7b0(void*, char* obj, UnkReflValue* const* args) {
    UnkDoorLife_b44a2* target = *reinterpret_cast<UnkDoorLife_b44a2**>(obj + 8);
    UnkReflValue* values = args[0];
    bool a = *static_cast<u8*>(ArgPtr_A5_b67(&values[0]));
    bool b = *static_cast<u8*>(ArgPtr_A5_b67(&values[1]));
    u8 c = *static_cast<u8*>(ArgPtr_A5_b67(&values[2]));
    bool result = reinterpret_cast<bool (*)(UnkDoorLife_b44a2*, u32, u32, u32)>(&fn_71008d498c)(target, a, b, c);
    const CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &result, false);
    return r;
}

UnkReflValue fn_71008dca58(void*, char* obj, UnkReflValue* const*) {
    UnkDoorLock* door = *reinterpret_cast<UnkDoorLock**>(obj + 8);
    {
        CStrId name(g_unk_7101d470e0);
        fn_71008d586c(door, true, 1, &name);
    }
    s32 one = 1;
    const CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}
