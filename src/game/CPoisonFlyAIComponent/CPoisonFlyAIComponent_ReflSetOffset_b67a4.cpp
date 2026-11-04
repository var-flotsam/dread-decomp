// CPoisonFlyAIComponent's unit (tools/tu_map.py, the name from reflection): a reflected function's
// invoker. Hands its two float arguments to the registry at +0x80 of the manager g_unk_71726af468
// (created on first use, as CPoisonFlyRequestAttackOffset_Update_b65a4.cpp does) through
// fn_710063c7a4; returns an empty value. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_710063c7a4(u8* registry, f32 a, f32 b);

static inline __attribute__((always_inline)) void* ArgPtr_b67a4(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100639afc(void*, char*, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    f32 x = *static_cast<const f32*>(ArgPtr_b67a4(&a[0]));
    f32 y = *static_cast<const f32*>(ArgPtr_b67a4(&a[1]));
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_710063c7a4(manager->pad_0x0 + 0x80, x, y);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
