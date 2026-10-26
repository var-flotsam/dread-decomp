// Five identical callbacks: when the id at 0x2238 of the game manager g_unk_7101d47310 is the
// empty id, and the manager singleton's state is not 6, they call fn_7100fdb9e0 on
// the manager. The state is the override at 0x38c, or, while that is unset (INT_MAX), the one
// at +0x10 of the object at 0x2f8 (none skips the test).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkState {
    u8 unk_0x0[0x10];
    s32 state;
};

struct UnkMgr_0xf18 {
    u8 unk_0x0[0x2f8];
    UnkState* field_0x2f8;
    u8 unk_0x300[0x8c];
    s32 field_0x38c;
    u8 unk_0x390[0xb88];
};

extern UnkMgr_0xf18* g_unk_71726bc560;

void fn_7100fd3804(UnkMgr_0xf18*);
void fn_7100fdb9e0(UnkMgr_0xf18*, u32, u32, f32, f32);

static inline UnkMgr_0xf18* GetMgr() {
    if (!g_unk_71726bc560) {
        UnkMgr_0xf18* m = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(m);
        g_unk_71726bc560 = m;
    }
    return g_unk_71726bc560;
}

static inline void Callback() {
    UnkMgr_0xf18* mgr = GetMgr();
    if (g_unk_7101d47310->field_0x2238.data != &g_unk_7101d47108->empty)
        return;
    s32 state = mgr->field_0x38c;
    if (state == 0x7fffffff) {
        UnkState* s = mgr->field_0x2f8;
        if (!s) {
            fn_7100fdb9e0(mgr, 1, 0, -1.0f, 0.0f);
            return;
        }
        state = s->state;
    }
    if (state == 6)
        return;
    fn_7100fdb9e0(mgr, 1, 0, -1.0f, 0.0f);
}

void fn_7100fce86c() {
    Callback();
}

void fn_7100fcf07c() {
    Callback();
}

void fn_7100fcf31c() {
    Callback();
}

void fn_7100fcf74c() {
    Callback();
}

void fn_7100fcfa5c() {
    Callback();
}
