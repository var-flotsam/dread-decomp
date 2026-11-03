// Unit 1049 of tools/tu_map.py (no class name). fn_71006b5fec forwards to vtable slot 45 of the
// object at +0x2a0 of the owner's actor (+0x8), with the unit's id (g_unk_7172706938) twice, the
// empty id, the entry at +0x628 of the id table (fn_7100031894) and constant options. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdTable_b50_1049 {
    UnkIdTable_b50_1049();  // fn_7100031894
    ~UnkIdTable_b50_1049();
    u8 pad_0x0[0x628];
    u8 field_0x628[8];
};

inline UnkIdTable_b50_1049& IdTable_b50_1049() {
    static UnkIdTable_b50_1049 s_table;
    return s_table;
}

extern char g_unk_7172706938[] __attribute__((visibility("hidden")));

struct UnkTarget_b50_1049 {
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
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual void vf_38();
    virtual void vf_39();
    virtual void vf_40();
    virtual void vf_41();
    virtual void vf_42();
    virtual void vf_43();
    virtual void vf_44();
    virtual void* vf_45(const void* a, const void* b, const void* c, const void* d, u32 e, u32 f, u32 g,
                        u64 h, s32 i, u64 j, s32 k, u64 l, s32 m, bool n, bool o, bool p);
};

struct UnkActor_b50_1049 {
    u8 pad_0x0[0x2a0];
    UnkTarget_b50_1049* field_0x2a0;
};

struct UnkOwner_b50_1049 {
    void* vtable;
    UnkActor_b50_1049* actor;
};

void* fn_71006b5fec(UnkOwner_b50_1049* self) {
    if (UnkActor_b50_1049* actor = self->actor) {
        if (UnkTarget_b50_1049* target = actor->field_0x2a0)
            return target->vf_45(g_unk_7172706938, g_unk_7172706938, &g_unk_7101d470e0,
                                 IdTable_b50_1049().field_0x628, 0x100, 0, 1, 0, -1, 0, -1, 0, -1, false,
                                 false, true);
    }
    return nullptr;
}
