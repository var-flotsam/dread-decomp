// Unit 2666 (Lua bindings): plays the vibration file named by the first argument, the second
// argument setting its flag at +0x41. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::input::UnkRumbleManager;

struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

struct UnkRumbleWords_l07f820 {
    u32 x, y, z;
};

// What a rumble is played from (UnkRumbleParams), with its defaults.
struct UnkRumbleParams_l07f820 {
    CFilePathStrId path;
    u64 field_0x8;
    s32 field_0x10;
    UnkRumbleWords_l07f820 field_0x14;
    u64 field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkRumbleParams_l07f820()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff),
          field_0x14(reinterpret_cast<const UnkRumbleWords_l07f820&>(g_unk_710164779c)), field_0x20(0),
          field_0x28(1.0f), field_0x2c(1.0f), field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
    ~UnkRumbleParams_l07f820();  // fn_7100af1b94
};

// The rumble manager, created on first use.
static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

s32 fn_710107f820(lua_State* L) {
    const char* name;
    bool flag = false;
    if (fn_71010c0270(L, "sb", &name, &flag)) {
        static UnkRumbleParams_l07f820 s_params;
        s_params.path = CFilePathStrId(name, true);
        s_params.field_0x10 = 0;
        s_params.field_0x41 = flag;
        fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&s_params));
    }
    return 0;
}
