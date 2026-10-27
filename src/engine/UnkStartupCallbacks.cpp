// From unit 2 of tools/tu_map.py (0x7100001570-0x710000185c; the string hash fn_7100001570 is in
// src/base/global/): the two callbacks of the start-up settings record that src/engine/HostArguments.cpp
// builds, taken there PC-relatively. Names are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

// The manager at g_unk_7101d47318 as far as it is called here (src/game/CSubAreaManager.cpp calls
// the same slot).
struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;
struct UnkStartupDropTargetView {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(u32 a, u32 b);
};

void fn_71010c2080();  // tail-calls fn_7100132920, which sets a flag
void fn_71002191a4(UnkRenderManager* self, u32 value);

__attribute__((visibility("hidden"))) void fn_7100001714() {
    fn_71010c2080();
}

__attribute__((visibility("hidden"))) void fn_7100001718() {
    fn_71002191a4(g_unk_7101d47300, 4);
    reinterpret_cast<UnkStartupDropTargetView*>(g_unk_7101d47318)->vf_0x38(0, 0);
}
