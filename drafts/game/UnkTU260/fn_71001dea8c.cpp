#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRenderTargetA1;
struct UnkBoundTarget;

extern CStrId g_unk_716e596080 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e5961c0 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596140 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596180 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596128 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596158 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596208 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596210 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596120 __attribute__((visibility("hidden")));
extern u32 g_unk_716e596050[16] __attribute__((visibility("hidden")));
extern u8 g_unk_7101a1e970 __attribute__((visibility("hidden")));

void fn_71001cdd6c(UnkRenderTargetA1* target, CStrId id);
void fn_71001cdd70(UnkRenderTargetA1* target);
void fn_71001cdd74(UnkRenderTargetA1* target, void* block);
void fn_71001cdba4(UnkRenderTargetA1* target, void* block);
void fn_71001cdd18(UnkRenderTargetA1* target);
void fn_71001cde48(UnkRenderTargetA1* target);
void fn_71001cb604(UnkRenderTargetA1* target, u32 a, void* b, void* c, void* d, void* e);
void* fn_710015c450(UnkBoundTarget* target);
void* fn_710015abd0();
void fn_710015b8a0(bool flag);
void fn_710015f770(s32 slot, void* object, bool clear);
void fn_71001ce704(UnkRenderTargetA1* target, u32 value);
void fn_71001ced08(UnkRenderTargetA1* target, u8 value);
void fn_71001d6b94(UnkRenderTargetA1* target);
void fn_71001d0b70(UnkRenderTargetA1* target, void* p, u32 n, s32 a, s32 b);
void fn_71001cde84(UnkRenderTargetA1* target, bool a, bool b, bool c);
void fn_71001d0ae4(UnkRenderTargetA1* target);
void fn_71001d0c84(UnkRenderTargetA1* target, void* p, u32 n, s32 a, s32 b);
void fn_71001d1880(UnkRenderTargetA1* target, void* p, u32 n, u32 a);
void fn_71001d1c50(UnkRenderTargetA1* target, void* p, u32 n);
void fn_71001d09fc(UnkRenderTargetA1* target, u8 value);
void fn_71001d54e0(UnkRenderTargetA1* target, void* a, void* b, void** c, u32 n);
bool fn_71001d48cc(UnkRenderTargetA1* target, void* a, f32 b);
void fn_71001d476c(UnkRenderTargetA1* target, f32 a, f32 b, f32 c, f32 d);
void fn_710015a3e0(u32 value);
void fn_710015a680(u32 value);
void fn_710015a500(u32 a, u32 b);
void fn_7100160ab0(u32 index, s32 mode, u32 group);
void fn_7100160990(s32 a, s32 b, bool c);
void fn_71001d4dbc(UnkRenderTargetA1* target, void* a, void* b);
void fn_71001d09a8(UnkRenderTargetA1* target);
void fn_71001d09b8(UnkRenderTargetA1* target, f32 a, f32 b);
void fn_71001d09d0(UnkRenderTargetA1* target, f32 a);
void fn_71001cc550(UnkRenderTargetA1* target, u32 a, u32 b, u32 c);
void fn_71001d1e78(UnkRenderTargetA1* target, f32 a, f32 b, f32 c, f32 d);
void fn_71001cc570(UnkRenderTargetA1* target);

struct __attribute__((packed)) UnkU64P_b0237 {
    u64 v;
};
u32 fn_7100132f80();
u32 fn_7100132f90();
extern u32 g_unk_716e595ff4;
extern u32 g_unk_716e595ff8;
f32 fn_71001d0ac0(UnkRenderTargetA1* target);
bool fn_71001d206c(UnkRenderTargetA1* target);
bool fn_71001cb51c(UnkRenderTargetA1* target);
void fn_71001d44bc(UnkRenderTargetA1* target, void* a);
void fn_710015a090(s32 a, s32 b, s32 c, s32 d);
struct UnkRenderOwner_b0237;
void fn_71001da83c(UnkRenderOwner_b0237* self, bool a, bool b, u32* values, f32 scale);

struct __attribute__((packed)) UnkRect24_b0237 {
    u64 a, b, c;
};
struct UnkParamB_b0237 {
    u8 pad_0x0[8];
    u32 field_0x8;
    UnkRect24_b0237 rect_0xc;
    u8 field_0x24;
};

struct UnkSetupSub_b0237 {
    u8 pad_0x0[0x1];
};
struct UnkSetupArg_b0237 {
    u64 a;
    u32 b;
    u32 c;
};
void fn_710014d150(UnkSetupSub_b0237* sub, void* block, u32 flag);
void fn_710014d17c(UnkSetupSub_b0237* sub, UnkSetupArg_b0237* arg);
void fn_710014cb98(UnkSetupSub_b0237* sub, u32 a, void* id, u32 b, u32 c);
void fn_710014c864(UnkSetupSub_b0237* sub, void* p, u32 a);
u64 fn_710014d1a0(UnkSetupSub_b0237* sub);
void fn_710015c420(void* object, u32* w, u32* h);
void fn_710014cfb4(UnkSetupSub_b0237* sub, u32 a, f32* v);
f32 fn_71001d0a70(UnkRenderTargetA1* target);
f32 fn_71001d0a7c(UnkRenderTargetA1* target);
f32 fn_71001d0a34(UnkRenderTargetA1* target);
void fn_710014cff0(UnkSetupSub_b0237* sub, void* a, void* b, u32 c);
u32 fn_7100132fc0();
u32 fn_7100132fd0();


// The list handed to fn_71001cdba4: a count, packed 8-byte entries and a trailing pointer.
struct UnkBindList_b0237 {
    u32 count;
    UnkU64P_b0237 items[16];
    void* tail;
};
static_assert(sizeof(UnkBindList_b0237) == 0x90, "UnkBindList_b0237 layout");

struct UnkFlags_b0237 {
    u64 a, b, c;
    u16 flags;
    UnkFlags_b0237(u8 x, u8 y) {
        b = 0;
        c = 0;
        a = 0;
        flags = static_cast<u16>(x | (y << 8));
    }
};
void fn_710015b650(UnkFlags_b0237* p);

struct UnkRenderOwner_b0237 {
    u8 pad_0x0[0x8];
    u8 block_0x8[0x28];  // 0x8
    void* field_0x30;  // 0x30
    u32 field_0x38;  // 0x38
    u8 pad_0x3c[0x6c];
    void* field_0xa8;  // 0xa8
    u32 field_0xb0;  // 0xb0
    u8 pad_0xb4[0x1c];
    void* field_0xd0;  // 0xd0
    u32 field_0xd8;  // 0xd8
    u8 pad_0xdc[0x4];
    void* field_0xe0;  // 0xe0
    u32 field_0xe8;  // 0xe8
    u8 pad_0xec[0x4];
    UnkRenderTargetA1* target;  // 0xf0
    void* field_0xf8;  // 0xf8
    UnkRect24_b0237 rect_0x100;  // 0x100
    u64 field_0x118;  // 0x118
    u32 field_0x120;  // 0x120
    u8 field_0x124[8];  // 0x124
    u32 field_0x12c;  // 0x12c
    f32 floats_0x130[3];  // 0x130
    u8 pad_0x13c[0x14];
    f32 field_0x150;  // 0x150
    f32 field_0x154;  // 0x154
    f32 field_0x158;  // 0x158
    u8 pad_0x15c[0x74];
    u8 block_0x1d0[8];  // 0x1d0
    u8 pad_0x1d8[0x88];
    UnkBoundTarget* slot_0x260;  // 0x260
    UnkBoundTarget* slot_0x268;  // 0x268
    UnkBoundTarget* slot_0x270;  // 0x270
    UnkBindList_b0237 list_0x278;  // 0x278
    u8 pad_0x308[0x110];
    void* field_0x418;  // 0x418
    u64 field_0x420;  // 0x420
    u32 field_0x428;  // 0x428
    u8 field_0x42c[8];  // 0x42c
    u32 field_0x434;  // 0x434
    f32 field_0x438;  // 0x438
    u8 field_0x43c;  // 0x43c
    u8 pad_0x43d[0x23];
    UnkBoundTarget* slot_0x460;  // 0x460
    UnkBoundTarget* slot_0x468;  // 0x468
    UnkBoundTarget* slots_0x470[3];  // 0x470
    u32 slot_index;  // 0x488
    u32 next_slot_index;  // 0x48c
    u8 pad_0x490[0x8];
    UnkBoundTarget* slot_0x498;  // 0x498
    u8 pad_0x4a0[0x2440];
    UnkSetupSub_b0237 sub_0x28e0;  // 0x28e0
    u8 pad_0x28e1[0x3c67];
    UnkSetupSub_b0237 sub_0x6548;  // 0x6548
    u8 pad_0x6549[0x4907];
    u8 flag_0xae50;  // 0xae50
    u8 flag_0xae51;  // 0xae51
    u8 pad_0xae52[0x2];
    u8 flag_0xae54;  // 0xae54
};

static_assert(__builtin_offsetof(UnkRenderOwner_b0237, sub_0x28e0) == 0x28e0, "UnkRenderOwner_b0237 layout");
static_assert(__builtin_offsetof(UnkRenderOwner_b0237, flag_0xae51) == 0xae51, "UnkRenderOwner_b0237 layout");

struct UnkTargetData_b0237 {
    u8 pad_0x0[0x100158];
    u32 field_0x100158;
    u8 pad_0x10015c[0x30834fc];
    u32 field_0x3183658;
};

__attribute__((noinline)) void fn_71001dba58(UnkRenderOwner_b0237* self, void* id, void* p) {
    fn_71001cdd6c(self->target, g_unk_716e596210);
    UnkSetupSub_b0237* sub = &self->sub_0x28e0;
    fn_710014d150(sub, &self->rect_0x100, 1);
    u64 a = self->field_0x118;
    u32 b = self->field_0x120;
    u32 c = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&self->field_0x118) + 0xc);
    UnkSetupArg_b0237 arg;
    arg.a = a;
    arg.b = b;
    arg.c = c;
    fn_710014d17c(sub, &arg);
    fn_710014cb98(sub, 0, &id, 0, 0);
    fn_710014c864(sub, p, 0);
    fn_710014d1a0(sub);
    fn_71001cdd70(self->target);
}

void fn_71001de554(UnkRenderOwner_b0237* self) {
    fn_71001cdd6c(self->target, g_unk_716e596080);
    fn_71001cdba4(self->target, self->block_0x1d0);
    fn_71001cdd74(self->target, &self->field_0x118);
    fn_71001cb604(self->target, 1, nullptr, nullptr, nullptr, nullptr);
    fn_71001ce704(self->target, 1);
    fn_71001cde48(self->target);
    fn_71001cdd18(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001cdd70(self->target);
}

void fn_71001dea8c(UnkRenderOwner_b0237* self) {
    if (reinterpret_cast<UnkTargetData_b0237*>(self->target)->field_0x100158 == 0)
        return;
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    UnkFlags_b0237 flags(0, 0);
    fn_710015b650(&flags);
    fn_71001cdd74(self->target, &self->field_0x118);
    fn_71001cdd6c(self->target, g_unk_716e5961c0);
    UnkBindList_b0237 list;
    list.items[0].v = reinterpret_cast<u64>(self->slot_0x468);
    list.count = 1;
    list.tail = fn_710015abd0();
    fn_71001cdba4(self->target, &list);
    fn_71001ced08(self->target, self->flag_0xae51);
    fn_710015f770(9, fn_710015c450(self->slot_0x498), true);
    fn_71001d6b94(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);
    fn_71001cde48(self->target);
    fn_710015b8a0(true);
}

void fn_71001da4e4(UnkRenderOwner_b0237* self) {
    if (self->field_0xb0 == 0)
        return;
    fn_71001cdd6c(self->target, g_unk_716e596140);
    fn_71001cb604(self->target, 6, nullptr, nullptr, nullptr, nullptr);
    fn_71001d0b70(self->target, self->field_0xa8, self->field_0xb0, -1, -1);
    fn_71001cdd74(self->target, &self->field_0x118);
    fn_71001cde84(self->target, true, true, true);
    fn_71001cde48(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001d0ae4(self->target);
    fn_71001d0c84(self->target, self->field_0x30, self->field_0x38, -1, -1);
    fn_71001d1880(self->target, self->field_0xd0, self->field_0xd8, 0);
    fn_71001d1c50(self->target, self->field_0xe0, self->field_0xe8);
    fn_71001cdd70(self->target);
}

void fn_71001dbe3c(UnkRenderOwner_b0237* self, u64 id) {
    fn_71001cdd6c(self->target, g_unk_716e596208);
    void* slot = self->slots_0x470[self->slot_index];
    UnkSetupSub_b0237* sub = &self->sub_0x6548;
    fn_710014d150(sub, &self->rect_0x100, 1);
    u64 a = self->field_0x118;
    u32 b = self->field_0x120;
    u32 c = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&self->field_0x118) + 0xc);
    UnkSetupArg_b0237 arg;
    arg.a = a;
    arg.b = b;
    arg.c = c;
    fn_710014d17c(sub, &arg);
    fn_710014c864(sub, slot, 0);
    struct {
        u32 a, b;
        f32 c, d;
    } rect;
    rect.a = g_unk_716e596050[0];
    rect.b = g_unk_716e596050[1];
    rect.c = 1.0f / static_cast<f32>(fn_7100132fc0());
    rect.d = 1.0f / static_cast<f32>(fn_7100132fd0());
    fn_710014cff0(sub, reinterpret_cast<u8*>(g_unk_716e596050) + 0x20, &rect, 1);
    fn_710014cb98(sub, 0, &id, 0, 0);
    fn_710014d1a0(sub);
    fn_71001cdd70(self->target);
}

void fn_71001debe8(UnkRenderOwner_b0237* self) {
    fn_71001cdd6c(self->target, g_unk_716e596180);
    fn_71001cb604(self->target, 0xb, nullptr, nullptr, nullptr, nullptr);
    self->list_0x278.tail = fn_710015abd0();
    UnkBoundTarget* slot = self->slots_0x470[self->slot_index];
    if (self->list_0x278.count == 0) {
        self->list_0x278.items[0].v = 0;
        self->list_0x278.count = 1;
    }
    self->list_0x278.items[0].v = reinterpret_cast<u64>(slot);
    fn_71001cdba4(self->target, &self->list_0x278);
    void* bound[4];
    bound[0] = fn_710015c450(self->slot_0x260);
    bound[1] = fn_710015c450(self->slot_0x268);
    bound[2] = fn_710015c450(self->slot_0x270);
    bound[3] = fn_710015c450(self->slot_0x498);
    fn_71001d09fc(self->target, g_unk_7101a1e970);
    fn_71001d54e0(self->target, &self->field_0x118, self->block_0x8, bound, 4);
    fn_71001cdd18(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001cdd70(self->target);
}

void fn_71001de3a8(UnkRenderOwner_b0237* self) {
    fn_71001cdd6c(self->target, g_unk_716e596128);
    UnkFlags_b0237 flags(0, 1);
    fn_710015b650(&flags);
    if (fn_71001d48cc(self->target, nullptr, 1.0f - self->field_0x438)) {
        f32 a = static_cast<f32>(self->field_0x120);
        f32 b = static_cast<f32>(self->field_0x428);
        f32 c = static_cast<f32>(*reinterpret_cast<u32*>(self->field_0x124));
        f32 d = static_cast<f32>(*reinterpret_cast<u32*>(self->field_0x42c));
        fn_71001d476c(self->target, (a - b) / b, (c - d) / d, a / b, c / d);
        UnkBindList_b0237 list;
        list.items[0].v = reinterpret_cast<u64>(self->slots_0x470[self->slot_index]);
        list.count = 1;
        list.tail = fn_710015abd0();
        fn_71001cdba4(self->target, &list);
        fn_710015a3e0(1);
        fn_710015a680(0);
        fn_710015a500(6, 7);
        fn_7100160ab0(0, 0, 1);
        fn_7100160990(0, 0, true);
        UnkRenderTargetA1* target = self->target;
        fn_71001d4dbc(target, fn_710015c450(reinterpret_cast<UnkBoundTarget*>(self->field_0x418)),
                      &self->rect_0x100);
        fn_71001cdd18(self->target);
    }
    fn_710015b8a0(true);
    fn_71001cdd70(self->target);
}

void fn_71001da650(UnkRenderOwner_b0237* self, void* a, void* b, f32* values) {
    fn_71001cdd6c(self->target, g_unk_716e596158);
    UnkBindList_b0237 list;
    list.items[0].v = reinterpret_cast<u64>(self->slot_0x260);
    list.items[1].v = reinterpret_cast<u64>(self->slot_0x268);
    list.items[2].v = reinterpret_cast<u64>(self->slot_0x270);
    list.items[3].v = reinterpret_cast<u64>(self->slot_0x498);
    list.items[4].v = reinterpret_cast<u64>(self->slot_0x460);
    list.items[5].v = reinterpret_cast<u64>(self->slots_0x470[0]);
    list.items[6].v = reinterpret_cast<u64>(self->slots_0x470[1]);
    list.items[7].v = reinterpret_cast<u64>(self->slots_0x470[2]);
    list.count = 8;
    list.tail = nullptr;
    fn_71001cdba4(self->target, &list);
    fn_71001cdd74(self->target, &self->rect_0x100);
    bool use_own = self->flag_0xae51 == 0;
    f32 x = *(use_own ? &values[0] : &self->floats_0x130[0]);
    f32 y = *(use_own ? &values[1] : &self->floats_0x130[1]);
    f32 z = *(use_own ? &values[2] : &self->floats_0x130[2]);
    fn_71001d09a8(self->target);
    fn_71001d09b8(self->target, self->field_0x150, self->field_0x154);
    fn_71001ced08(self->target, self->flag_0xae51);
    fn_71001d09d0(self->target, self->field_0x158);
    fn_71001cc550(self->target, 1, 0, 0);
    fn_71001d1e78(self->target, x, y, z, 0.0f);
    fn_71001cc570(self->target);
    fn_71001cde48(self->target);
    fn_71001cdd18(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001cdd70(self->target);
}

void fn_71001ded64(UnkRenderOwner_b0237* self, void* bound, UnkParamB_b0237* param, UnkRect24_b0237* rect,
                   u32 flag_a, u32 flag_b, u32* values, f32 scale) {
    self->slot_index = 0;
    if (self->flag_0xae50 == 0 && fn_71001d206c(self->target))
        return;
    *reinterpret_cast<UnkRect24_b0237*>(&self->field_0x118) = param ? param->rect_0xc : *rect;
    self->rect_0x100 = *rect;
    u8* block = reinterpret_cast<u8*>(&self->field_0x118);
    g_unk_716e595ff4 = fn_7100132f80();
    g_unk_716e595ff8 = fn_7100132f90();
    f32 width = static_cast<f32>(g_unk_716e595ff4);
    g_unk_716e595ff4 = static_cast<u32>(fn_71001d0ac0(self->target) * width);
    f32 height = static_cast<f32>(g_unk_716e595ff8);
    g_unk_716e595ff8 = static_cast<u32>(fn_71001d0ac0(self->target) * height);
    f32* size = reinterpret_cast<f32*>(g_unk_716e596050);
    size[0] = static_cast<f32>(g_unk_716e595ff4);
    size[1] = static_cast<f32>(g_unk_716e595ff8);
    fn_71001d0ac0(self->target);
    fn_71001d0ac0(self->target);
    u32 w = g_unk_716e595ff4;
    u32 h = g_unk_716e595ff8;
    self->field_0x118 = 0;
    self->field_0x120 = w;
    *reinterpret_cast<u32*>(block + 0xc) = h;
    reinterpret_cast<UnkU64P_b0237*>(self->field_0x124 + 4)->v = 0x3f80000000000000ull;
    fn_710015a090(0, 0, w, h);
    UnkTargetData_b0237* data = reinterpret_cast<UnkTargetData_b0237*>(self->target);
    u32 saved = data->field_0x3183658;
    u8 flag = 0;
    if (param) {
        data->field_0x3183658 = param->field_0x8;
        flag = param->field_0x24;
    }
    UnkRenderTargetA1* target = self->target;
    self->flag_0xae54 = flag;
    if (fn_71001cb51c(target)) {
        self->field_0xf8 = bound;
        fn_71001d44bc(self->target, bound);
        u32 copy[3];
        copy[0] = values[0];
        copy[1] = values[1];
        copy[2] = values[2];
        fn_71001da83c(self, flag_a & 1, flag_b & 1, copy, scale);
    }
    reinterpret_cast<UnkTargetData_b0237*>(self->target)->field_0x3183658 = saved;
}

struct UnkSlotPair_b0237 {
    void* first;
    u8 pad_0x8[8];
    void* second;
    u8 pad_0x18[8];
};

struct UnkRenderOwner2_b0237 {
    u8 pad_0x0[0xf0];
    UnkRenderTargetA1* target;  // 0xf0
    u8 pad_0xf8[0x2c8];
    UnkSlotPair_b0237 pairs_0x3c0[1];
    u8 pad_0x3e0[0xce0];
    UnkSetupSub_b0237 sub_0x10c0;
    u8 pad_0x10c1[0xc0f];
    UnkSetupSub_b0237 sub_0x1cd0;
    u8 pad_0x1cd1[0x5487];
    UnkSetupSub_b0237 sub_0x7158;
};
static_assert(__builtin_offsetof(UnkRenderOwner2_b0237, sub_0x7158) == 0x7158, "UnkRenderOwner2_b0237 layout");
static_assert(__builtin_offsetof(UnkRenderOwner2_b0237, sub_0x10c0) == 0x10c0, "UnkRenderOwner2_b0237 layout");
static_assert(__builtin_offsetof(UnkRenderOwner2_b0237, sub_0x1cd0) == 0x1cd0, "UnkRenderOwner2_b0237 layout");

struct UnkLocalDc_b0237 {
    u64 a;
    u32 w;
    u32 h;
    u32 f40;
    f32 one;
    u64 id;
};

void fn_71001dc3bc(UnkRenderOwner2_b0237* self, u64 id, u32 index) {
    void* first = self->pairs_0x3c0[index].first;
    void* second = self->pairs_0x3c0[index].second;
    UnkLocalDc_b0237 local;
    local.a = 0;
    local.w = 0;
    local.h = 0;
    local.f40 = 0;
    local.one = 1.0f;
    local.id = id;
    fn_710015c420(first, &local.w, &local.h);
    UnkSetupSub_b0237* sub1 = &self->sub_0x10c0;
    fn_710014d150(sub1, &local, 1);
    fn_710014c864(sub1, first, 0);
    UnkSetupSub_b0237* sub3 = &self->sub_0x7158;
    fn_710014d150(sub3, &local, 1);
    fn_710014c864(sub3, second, 0);
    UnkSetupArg_b0237 arg;
    arg.a = local.a;
    arg.b = local.w;
    arg.c = local.h;
    fn_710014d17c(sub1, &arg);
    UnkSetupSub_b0237* sub2 = &self->sub_0x1cd0;
    fn_710014d17c(sub2, &arg);
    f32 half = fn_71001d0a70(self->target) * 0.5f;
    f32 zero4[4] = {0, 0, 0, 0};
    fn_710014cfb4(sub1, 1, zero4);
    fn_710014cfb4(sub2, 1, zero4);
    f32 six = fn_71001d0a7c(self->target) * 6.0f;
    f32 v[4];
    f32 z = fn_71001d0a34(self->target) * 0.8f;
    v[0] = half * six / static_cast<f32>(local.w);
    v[1] = 0;
    v[2] = z;
    v[3] = 0;
    fn_710014cfb4(sub1, 0, v);
    f32 zeroF = 0.0f;
    u32 count = index ? 3 : 1;
    do {
        fn_710014cfb4(sub1, 0, v);
        fn_710014cb98(sub1, 0, &local.id, 1, 1);
        local.id = fn_710014d1a0(sub1);
        v[0] = v[0] * 0.8f;
        v[1] = v[1] * zeroF;
        v[2] = v[2] * 0.6f;
        fn_710014cfb4(sub3, 0, v);
        fn_710014cb98(sub3, 0, &local.id, 1, 1);
        local.id = fn_710014d1a0(sub3);
        v[0] = v[0] * 0.8f;
        v[1] = v[1] * zeroF;
        v[2] = v[2] * 0.6f;
    } while (--count);
}
