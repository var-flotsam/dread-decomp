// CHydrogigaAIComponent unit: fn_71005c3d38 handles one message id (the global id g_unk_71726eb3d0): it clears a
// flag of the lazily created manager g_unk_7101d47360 and reports the message handled. Names are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU881.h"
#include "types.h"

struct UnkManager;
extern UnkManager* g_unk_7101d47360;
void fn_7100ee1f34(void*);

struct UnkHydrogigaMsg_b62a3 {
    u8 pad_0x0[0x10];
    u64 id;
};

bool fn_71005c3d38(void* self, UnkHydrogigaMsg_b62a3* msg) {
    if (__builtin_expect(msg->id == *reinterpret_cast<u64*>(g_unk_71726eb3d0), 0)) {
        void* manager = reinterpret_cast<void*&>(g_unk_7101d47360);
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        reinterpret_cast<void*&>(g_unk_7101d47360) = manager;
    }
    static_cast<u8*>(manager)[0xae0] = 0;
    return true;
    }
    return false;
}
