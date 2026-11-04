// A path query (tools/tu_map.py unit 1130, no class name), as UnkNavPathQuery_b50.cpp's fn_7100707c80:
// when the path at +0x12ac0 is active and the current node (index at +0x37fc8 into 0x120-byte entries at
// +0x12ad4) has both links set, copies the node's point (+0x24) out and answers true. Names and layouts
// are placeholders.
#include "types.h"

struct UnkPathList_b67a4 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual u32 vf_5();
};

struct UnkPathPoint_b67a4 {
    f32 x, y, z;
};

struct UnkPathNode_b67a4 {
    u8 pad_0x0[0x1a];
    s16 field_0x1a;
    u8 pad_0x1c[0x8];
    UnkPathPoint_b67a4 point;  // 0x24
    u8 pad_0x30[0x6];
    s16 field_0x36;
    u8 pad_0x38[0xe8];
};

bool fn_7100707df4(char* self, UnkPathPoint_b67a4* out) {
    char* path = self + 0x12ac0;
    if (*path) {
        u32* index = (u32*)(self + 0x37fc8);
        if (*index < ((UnkPathList_b67a4*)(path + 8))->vf_5()) {
            u32 i = *index;
            UnkPathNode_b67a4* nodes = (UnkPathNode_b67a4*)(self + 0x12ad4);
            if (nodes[i].field_0x1a != -1 && nodes[i].field_0x36 != -1) {
                // Copied as a word pair (the original stores the low 8 bytes first).
                const char* p = reinterpret_cast<const char*>(&nodes[i].point);
                u64 xy;
                u32 z;
                __builtin_memcpy(&xy, p, 8);
                __builtin_memcpy(&z, p + 8, 4);
                __builtin_memcpy(out, &xy, 8);
                __builtin_memcpy(reinterpret_cast<char*>(out) + 8, &z, 4);
                return true;
            }
        }
    }
    return false;
}
