// A path query (tools/tu_map.py unit 1130, no class name), next to UnkNavPathQuery_b50.cpp's
// fn_7100707c80: whether the path is finished. False without the object at +0x10 or when its
// vf_0x80 says no; true when the current node index (+0x37fc8) is past the path list (+0x12ac8, its
// vf_0x28) or the node (0x120-byte entries at +0x12ad4) lacks either link. Names and layouts are
// placeholders.
#include "types.h"

struct UnkPathList_b67a4d {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual u32 vf_5();
};

struct UnkPathNode_b67a4d {
    u8 pad_0x0[0x1a];
    s16 field_0x1a;
    u8 pad_0x1c[0x1a];
    s16 field_0x36;
    u8 pad_0x38[0xe8];
};

struct UnkPathOwner_b67a4d {
    void** vtable;
    bool IsActive() { return reinterpret_cast<bool (*)(UnkPathOwner_b67a4d*)>(vtable[0x80 / 8])(this); }
};

bool fn_7100707d28(char* self) {
    UnkPathOwner_b67a4d* owner = *(UnkPathOwner_b67a4d**)(self + 0x10);
    if (!owner || !owner->IsActive())
        return false;
    u32* index = (u32*)(self + 0x37fc8);
    if (*index >= ((UnkPathList_b67a4d*)(self + 0x12ac8))->vf_5())
        return true;
    u32 i = *index;
    UnkPathNode_b67a4d* nodes = (UnkPathNode_b67a4d*)(self + 0x12ad4);
    if (nodes[i].field_0x1a == -1)
        return true;
    return nodes[i].field_0x36 == -1;
}
