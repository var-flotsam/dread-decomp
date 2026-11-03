// tools/tu_map.py unit 1485 (no class name; it holds fn_71009c734c, the child type that
// fn_71009b7ddc adds): calls slot 0x210 of the owner's "ACTIVATABLE" component (fn_7100899610)
// when there is one, then returns what fn_71009c5c84 returns. Names are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::UnkReflObject;

// Declared as src/game/CLiquidSimulationComponent.cpp declares it.
UnkReflObject* fn_7100899610(void* owner);

struct UnkNavChildOwner_b45a2 {
    u8 pad_0x0[0x30];
    void* owner;
};

struct UnkNavChildArg_b45a2 {
    void* pad_0x0;
    UnkNavChildOwner_b45a2* actor;
};

// A result returned in memory (x8); its layout is unknown here.
struct UnkNavChildResult_b45a2 {
    u64 field_0x0;
    ~UnkNavChildResult_b45a2();
};

UnkNavChildResult_b45a2 fn_71009c5c84(UnkNavChildArg_b45a2* arg);

UnkNavChildResult_b45a2 fn_71009c768c(UnkNavChildArg_b45a2* arg) {
    if (UnkReflObject* c = fn_7100899610(arg->actor->owner))
        reinterpret_cast<void (*)(UnkReflObject*)>((*reinterpret_cast<void***>(c))[0x210 / 8])(c);
    return fn_71009c5c84(arg);
}
