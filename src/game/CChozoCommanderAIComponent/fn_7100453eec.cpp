// Frees the record's list of pooled nodes and clears its counters, then starts one of two effects for the
// state at +0x1140 (1 or 2): a parameter block is built on the stack and handed to the owner's object at
// +0x218, the object at +0x228 is called six times with a name each, and the manager's current index
// selects a float that is passed to fn_7100dcdce4 together with its integer part.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNode {
    u8 pad_0x0[0x18];
    UnkNode* field_0x18;
};

struct UnkEffectSink;
struct UnkFlagTarget;

struct UnkOwnerObject {
    u8 pad_0x0[0x218];
    UnkEffectSink* field_0x218;
    u8 pad_0x220[0x8];
    UnkFlagTarget* field_0x228;
};

struct UnkRecord {
    u8 pad_0x0[0x30];
    UnkOwnerObject* field_0x30;
    u8 pad_0x38[0x1108];
    u32 field_0x1140;
    u8 pad_0x1144[0x2c];
    u32 field_0x1170;
    u8 pad_0x1174[4];
    UnkNode* field_0x1178;
    u64 field_0x1180;
    u8 pad_0x1188[0x8];
    base::UnkAllocator* field_0x1190;
    u8 pad_0x1198[0x14b78];
    u32 field_0x15d10;
};

struct UnkManager {
    u8 pad_0x0[0xb8];
    u8 field_0xb8;
    u8 pad_0xb9[2];
    u8 field_0xbb;
    u8 pad_0xbc[4];
    s32 field_0xc0;
};

struct UnkTuning {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
};

// The two parameter blocks differ in their vtable and in how far their fields go.
struct UnkParamsSmall {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkParamsLarge {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 pad_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 pad_0x51;
    u8 field_0x52;
};

extern void* g_unk_7101a2e208[];
extern void* g_unk_7101a24db8[];
extern UnkManager* g_unk_71726bc570;
extern UnkTuning* g_unk_7101a37310[];
extern char g_unk_71727bd078[];
extern char g_unk_71016496c0[];
extern char g_unk_71016496c8[];
extern char g_unk_71016496d0[];
extern char g_unk_71016496d8[];
extern char g_unk_71016496e0[];
extern char g_unk_71016496e8[];

void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);
void fn_71010cc214(UnkEffectSink* sink, u32 mode);
void fn_7101172780(UnkFlagTarget* target, const char* name);
void fn_7101172764(UnkFlagTarget* target, const char* name);
void fn_7100dcdce4(UnkManager* manager, s32 value, f32 scale);

// NON_MATCHING(minor): the original tests the state against 2 first and lays the state-1 path out ahead of the
// state-2 one; here the compiler turns the two tests into a switch in the other order. Same behaviour.
void fn_7100453eec(UnkRecord* self) {
    if (!self->field_0x30)
        return;
    for (UnkNode* node = self->field_0x1178; node;) {
        UnkNode* next = node->field_0x18;
        self->field_0x1190->vf_0x48(node);
        node = next;
    }
    self->field_0x1170 = 0;
    self->field_0x1178 = nullptr;
    self->field_0x1180 = 0;
    self->field_0x15d10 = 0;
    UnkEffectSink* sink;
    UnkManager* manager;
    if (self->field_0x1140 == 2) {
        UnkParamsLarge params;
        params.field_0xc = 0;
        params.field_0x14 = 0;
        __builtin_memset(params.field_0x28, 0, 13);
        __builtin_memset(params.field_0x18, 0, 13);
        params.vtable = g_unk_7101a24db8 + 2;
        params.field_0x3d = 0;
        params.field_0x3f = 0;
        params.field_0x41 = 0;
        params.field_0x52 = 0;
        __builtin_memset(params.field_0x44, 0, 13);
        params.field_0x38 = 1;
        sink = self->field_0x30->field_0x218;
        fn_71010c7e6c(sink, &params, 1);
        UnkFlagTarget* target = self->field_0x30->field_0x228;
        if (target) {
            fn_7101172780(target, g_unk_71016496c0);
            fn_7101172764(self->field_0x30->field_0x228, g_unk_71016496c8);
            fn_7101172780(self->field_0x30->field_0x228, g_unk_71016496d0);
            fn_7101172764(self->field_0x30->field_0x228, g_unk_71016496d8);
            fn_7101172780(self->field_0x30->field_0x228, g_unk_71016496e0);
            fn_7101172764(self->field_0x30->field_0x228, g_unk_71016496e8);
        }
        fn_71010cc214(sink, 2);
        manager = g_unk_71726bc570;
        if (!manager->field_0xb8 || !manager->field_0xbb)
            return;
        f32 value = (u32)manager->field_0xc0 <= 2 ? g_unk_7101a37310[manager->field_0xc0]->field_0x18
                                                  : reinterpret_cast<UnkTuning*>(g_unk_71727bd078 + 0x98)->field_0x18;
        fn_7100dcdce4(manager, (s32)value, value);
    } else if (self->field_0x1140 == 1) {
        UnkParamsSmall params;
        params.field_0xc = 0;
        params.field_0x14 = 0;
        __builtin_memset(params.field_0x28, 0, 13);
        __builtin_memset(params.field_0x18, 0, 13);
        params.vtable = g_unk_7101a2e208 + 2;
        params.field_0x38 = 3;
        params.field_0x3d = 0;
        sink = self->field_0x30->field_0x218;
        fn_71010c7e6c(sink, &params, 1);
        fn_71010cc214(sink, 5);
        UnkFlagTarget* target = self->field_0x30->field_0x228;
        if (target) {
            fn_7101172780(target, g_unk_71016496c0);
            fn_7101172764(self->field_0x30->field_0x228, g_unk_71016496c8);
            fn_7101172780(self->field_0x30->field_0x228, g_unk_71016496d0);
            fn_7101172764(self->field_0x30->field_0x228, g_unk_71016496d8);
            fn_7101172764(self->field_0x30->field_0x228, g_unk_71016496e0);
            fn_7101172780(self->field_0x30->field_0x228, g_unk_71016496e8);
        }
        manager = g_unk_71726bc570;
        if (!manager->field_0xb8 || !manager->field_0xbb)
            return;
        f32 value = (u32)manager->field_0xc0 <= 2 ? g_unk_7101a37310[manager->field_0xc0]->field_0x14
                                                  : reinterpret_cast<UnkTuning*>(g_unk_71727bd078 + 0x98)->field_0x14;
        fn_7100dcdce4(manager, (s32)value, value);
    }
}
