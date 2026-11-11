// CCharClassForceUpdateEnvironmentEvent's unit (tools/tu_map.py): when the sub-area manager
// (g_unk_7101d47360, 0xae8 bytes, created on first use) has a current entry at +0x3f0, forces the
// environment manager (g_unk_7101d47368, 0x4c8 bytes, created on first use) to that entry's id
// (+0x98) through fn_7100d8e32c. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "types.h"

using base::global::CStrId;

// Signatures as src/game/CSubAreaManager.cpp declares them.
struct UnkEnvironmentManager;
extern void* g_unk_7101d47360;
extern void* g_unk_7101d47368;
void fn_7100ee1f34(void*);
void fn_7100d8e32c(UnkEnvironmentManager* self, const CStrId* id, bool a, bool b);

struct UnkSubAreaEntry_ovb0092 {
    u8 pad_0x0[0x98];
    CStrId id;  // 0x98
};

struct UnkSubAreaManager_ovb0092 {
    u8 pad_0x0[0x3f0];
    UnkSubAreaEntry_ovb0092* current;  // 0x3f0
};

__attribute__((always_inline)) static inline UnkSubAreaManager_ovb0092* GetSubAreaManager() {
    if (!g_unk_7101d47360) {
        void* m = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(m);
        g_unk_7101d47360 = m;
    }
    return static_cast<UnkSubAreaManager_ovb0092*>(g_unk_7101d47360);
}

__attribute__((always_inline)) static inline UnkEnvironmentManager* GetEnvironmentManager() {
    if (!g_unk_7101d47368) {
        void* m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    return static_cast<UnkEnvironmentManager*>(g_unk_7101d47368);
}

void fn_7100cbbf2c() {
    if (!GetSubAreaManager()->current)
        return;
    fn_7100d8e32c(GetEnvironmentManager(), &GetSubAreaManager()->current->id, true, false);
}
