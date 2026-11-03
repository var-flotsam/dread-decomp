// A path query (tools/tu_map.py unit 1130, no class name). fn_7100707c80: whether the current
// path node (index at +0x37fc8 into 0x120-byte entries at +0x12ad4) has both links set, when the
// path at +0x12ac0 is active. Names and layouts are placeholders.
#include "types.h"

struct UnkPathList_b50 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual u32 vf_5();
};

struct UnkPathNode_b50 {
    u8 pad_0x0[0x1a];
    s16 field_0x1a;
    u8 pad_0x1c[0x1a];
    s16 field_0x36;
    u8 pad_0x38[0xe8];
};

bool fn_7100707c80(char* self) {
    char* path = self + 0x12ac0;
    if (*path) {
        u32* index = (u32*)(self + 0x37fc8);
        if (*index < ((UnkPathList_b50*)(path + 8))->vf_5()) {
            u32 i = *index;
            UnkPathNode_b50* nodes = (UnkPathNode_b50*)(self + 0x12ad4);
            if (nodes[i].field_0x1a != -1)
                return nodes[i].field_0x36 != -1;
        }
    }
    return false;
}
