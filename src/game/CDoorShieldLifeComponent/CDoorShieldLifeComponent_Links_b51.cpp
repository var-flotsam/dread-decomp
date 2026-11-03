// CDoorShieldLifeComponent (unit 1377): dropping the navigation links the component added.
#include "base/UnkVector.h"
#include "game/UnkNavMeshManager.h"

struct UnkNode;
void fn_71002b5698(UnkNode* self);
void fn_710027e18c(UnkNavMesh* navmesh, UnkNode* link);

// The links follow their count directly (4-byte aligned).
typedef UnkNode* __attribute__((aligned(4))) UnkNodePtr;

struct UnkDoorShieldLinks {
    u8 pad_0x0[0x730];
    u32 field_0x730;
    u8 pad_0x734[0x1c];
    u32 count;          // 0x750
    UnkNodePtr links[1];  // 0x754
};

static inline UnkNavMeshManager* UnkNavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

// Removes every link from the current navigation mesh and destroys it.
bool fn_71008dcf50(UnkDoorShieldLinks* self) {
    self->field_0x730 = 0;
    if (UnkNavMesh* navmesh = UnkNavManager()->navmesh) {
        // The walk runs over the byte size of the array.
        u64 n = (u64)self->count * sizeof(UnkNodePtr);
        asm("" : "+r"(n));
        if (n != 0) {
            UnkNodePtr* it = self->links;
            do {
                UnkNode* link = *it;
                fn_710027e18c(navmesh, link);
                if (link) {
                    fn_71002b5698(link);
                    fn_7100083c00(link);
                }
                ++it;
                n -= 8;
            } while (n != 0);
        }
        self->count = 0;
    }
    return false;
}
