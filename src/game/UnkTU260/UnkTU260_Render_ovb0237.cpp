// Unit 260 (render passes, no reliable name): passes that hand a string id to the render target at +0xf0 and run a
// fixed run of its methods, plus one property query. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU177.h"
#include "game/UnkTU179.h"
#include "game/UnkTU182.h"
#include "game/UnkTU259.h"
#include "types.h"
#include "game/UnkTU151.h"

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

void* fn_710015abd0();
void fn_710015b8a0(bool flag);
void fn_71001d6b94(UnkRenderTargetA1* target);
void fn_71001cde84(UnkRenderTargetA1* target, bool a, bool b, bool c);
void fn_71001d0ae4(UnkRenderTargetA1* target);
void fn_71001d0c84(UnkRenderTargetA1* target, void* p, u32 n, s32 a, s32 b);
void fn_71001d1880(UnkRenderTargetA1* target, void* p, u32 n, u32 a);
void fn_71001d1c50(UnkRenderTargetA1* target, void* p, u32 n);
void fn_71001d09fc(UnkRenderTargetA1* target, u8 value);
void fn_71001d54e0(UnkRenderTargetA1* target, void* a, void* b, void** c, u32 n);
bool fn_71001d48cc(UnkRenderTargetA1* target, void* a, f32 b);
void fn_71001d476c(UnkRenderTargetA1* target, f32 a, f32 b, f32 c, f32 d);
void fn_71001d4dbc(UnkRenderTargetA1* target, void* a, void* b);
void fn_71001d09a8(UnkRenderTargetA1* target);
void fn_71001d09b8(UnkRenderTargetA1* target, f32 a, f32 b);
void fn_71001d09d0(UnkRenderTargetA1* target, f32 a);

struct __attribute__((packed)) UnkU64P_b0237 {
    u64 v;
};
extern u32 g_unk_716e595ff4;
extern u32 g_unk_716e595ff8;
f32 fn_71001d0ac0(UnkRenderTargetA1* target);
bool fn_71001d206c(UnkRenderTargetA1* target);
bool fn_71001cb51c(UnkRenderTargetA1* target);
void fn_71001d44bc(UnkRenderTargetA1* target, void* a);
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

void fn_710015c420(void* object, u32* w, u32* h);
void fn_710014cfb4(UnkSetupSub_b0237* sub, u32 a, f32* v);
f32 fn_71001d0a70(UnkRenderTargetA1* target);
f32 fn_71001d0a7c(UnkRenderTargetA1* target);
f32 fn_71001d0a34(UnkRenderTargetA1* target);
void fn_710014cff0(UnkSetupSub_b0237* sub, void* a, void* b, u32 c);


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
bool fn_710015b650(UnkFlags_b0237* p);

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

// Draws with the slot the next index names: the current index is swapped for it around fn_71001dba58,
// then the setup's values are kept at +0x420.
void fn_71001db2b8(UnkRenderOwner_b0237* self) {
    fn_71001cdd6c(self->target, g_unk_716e596120);
    u32 index = self->next_slot_index;
    u32 saved = self->slot_index;
    self->slot_index = index;
    fn_71001dba58(self, fn_710015c450(self->slots_0x470[index]), self->field_0x418);
    self->field_0x420 = self->field_0x118;
    u32 w = self->field_0x120;
    u8* dst = self->field_0x42c;
    self->slot_index = saved;
    self->field_0x428 = w;
    __builtin_memcpy(dst, reinterpret_cast<u8*>(&self->field_0x120) + 4, 8);
    self->field_0x434 = self->field_0x12c;
    fn_71001cdd70(self->target);
    self->field_0x43c = 0;
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







extern base::math::CVector4D g_unk_7101a1e98c __attribute__((visibility("hidden")));
extern base::math::CVector4D g_unk_7101a1e99c __attribute__((visibility("hidden")));
extern u8 g_unk_716e596034 __attribute__((visibility("hidden")));

int fn_71001e065c(UnkRenderOwner_b0237* self, u64 id, void* out) {
    switch (id) {
    case 0x38bb3d82ecfaffaaull:
        if (out)
            *static_cast<f32*>(out) = g_unk_716e596034 ? 1.0f : 0.0f;
        return 1;
    case 0xb37573f01782316aull:
        if (out)
            fn_710007b518(static_cast<base::math::CVector4D*>(out), &g_unk_7101a1e99c);
        return 4;
    case 0x95a2e304fd05cda8ull:
        if (out)
            fn_710007b518(static_cast<base::math::CVector4D*>(out), &g_unk_7101a1e98c);
        return 4;
    }
    return 0;
}
