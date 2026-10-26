// A translation unit's static initialiser: the two header string ids every unit copies
// ("binaries", "sources", after a u32 6), the end-screen statistic ids and the "EndScreen"
// string, each registered for destruction.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrIdBase;

struct UnkStatics_71729dc370 {
    u32 field_0x0;
    UnkStrIdBase ids[12];
    CRntString name;
};
extern UnkStatics_71729dc370 g_unk_71729dc370 __attribute__((visibility("hidden")));

static inline void InitId(UnkStrIdBase* id, const char* str) {
    new (id) UnkStrIdBase(str, true);
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

void fn_7100f17b70() {
    UnkStatics_71729dc370* s = &g_unk_71729dc370;
    s->field_0x0 = 6;
    InitId(&s->ids[0], "binaries");
    InitId(&s->ids[1], "sources");
    InitId(&s->ids[2], "BB_LIFE");
    InitId(&s->ids[3], "BB_MISSILE");
    InitId(&s->ids[4], "BB_POWERBOMB");
    InitId(&s->ids[5], "PS_DEATHS");
    InitId(&s->ids[6], "PS_DAMAGE");
    InitId(&s->ids[7], "PS_BOSSKILL");
    InitId(&s->ids[8], "PS_SAVES");
    InitId(&s->ids[9], "PS_SHOTKILL");
    InitId(&s->ids[10], "");
    InitId(&s->ids[11], "REWARDS_BK");
    new (&s->name) CRntString("EndScreen");
    __cxa_atexit(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), &s->name, g_unk_7101a19000);
}
