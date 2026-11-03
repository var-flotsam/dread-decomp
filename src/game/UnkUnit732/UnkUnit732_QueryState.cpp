#include "types.h"

struct UnkUnit732Item {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual u32 vf_11();
};

struct UnkUnit732Target {
    u8 unk_0x0[0x60];
    UnkUnit732Item* item;
    u8 unk_0x68[0x4c];
    u32 unk_bits_0xb4 : 15;
    u32 flag_0xb5_7 : 1;
    u32 unk_bits_0xb6 : 16;
};

u32 fn_7100497874(void* a, void* b, void* c, UnkUnit732Target* target) {
    if (!(target->flag_0xb5_7))
        return 3;
    if (UnkUnit732Item* item = target->item) {
        if (!(item->vf_11() & 0x80))
            return 4;
    }
    return 2;
}
