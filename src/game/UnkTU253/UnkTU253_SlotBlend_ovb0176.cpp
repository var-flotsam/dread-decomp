// Unit 253 (the object of game/UnkSlots.h; see src/game/UnkSlotBlend.cpp): more ways to start a timed
// blend of a slot's colour. Names and layouts are placeholders.
#include <new>

#include "base/UnkInitRecord_0x5c.h"
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTU23.h"
#include "game/UnkTU308.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;

struct UnkSlotBlend_ovb0176 {
    CColor4F from;
    CColor4F to;
    f32 time;  // left, counting down to 0
    f32 duration;
    u32 id;
    u32 kind;
    u32 mode;
};
static_assert(__builtin_offsetof(UnkSlotBlend_ovb0176, id) == 0x28, "layout");

static inline UnkSlotBlend_ovb0176* BlendAt(UnkSlots* self, u64 i) {
    return reinterpret_cast<UnkSlotBlend_ovb0176*>(self->entries[i]);
}

bool fn_7100199d88(void* table, const u64* key, u32* id);

static inline bool StartBlend(UnkSlots* self, u32 id, u32 kind, const CColor4F* from, const CColor4F* to, u32 mode,
                              f32 duration) {
    u32 count = self->count;
    u64 i = 0;
    bool running = false;
    s32 index = count;
    for (; i < count; ++i) {
        UnkSlotBlend_ovb0176* blend = BlendAt(self, i);
        if (blend->id == id && blend->kind == kind) {
            bool same = blend->mode == mode;
            if (same)
                index = i;
            running |= same;
        }
    }
    bool ok = (count < 9) | running;
    // Keeps the combined flag a live value, returned after the calls.
    asm("" : "+r"(ok));
    if (ok) {
        if (!running)
            self->count = count + 1;
        UnkSlotBlend_ovb0176* blend = BlendAt(self, index);
        fn_7100030728(&blend->from, from);
        fn_7100030728(&blend->to, to);
        blend->time = duration;
        blend->duration = duration;
        blend->id = id;
        blend->kind = kind;
        blend->mode = mode;
    }
    return ok;
}

bool fn_71001c2c70(UnkSlots* self, f32 duration, u32 id, u32 kind, const CColor4F* from, const CColor4F* to,
                   u32 mode) {
    return StartBlend(self, id, kind, from, to, mode, duration);
}

struct UnkSlotsInfo_ovb0176 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
};

// A polymorphic object the owner drives each frame.
struct UnkDriver_ovb0176 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(f32 dt);
    virtual void vf_0x28(void* arg);
};

struct UnkSlotsOwner_ovb0176 {
    u8 pad_0x0[0x70];
    bool field_0x70;
    u8 pad_0x71[7];
    UnkSlotsInfo_ovb0176* info;  // 0x78
    u8 pad_0x80[0x60];
    u64 field_0xe0;
    UnkDriver_ovb0176* driver;  // 0xe8
    u8 field_0xf0[0x260];
    u32 count;  // 0x350
    u8 pad_0x354[5];
    bool field_0x359;
    u8 pad_0x35a[4];
    bool field_0x35e;
    bool field_0x35f;
};
static_assert(__builtin_offsetof(UnkSlotsOwner_ovb0176, count) == 0x350, "layout");
static_assert(__builtin_offsetof(UnkSlotsOwner_ovb0176, field_0x35f) == 0x35f, "layout");

void fn_7100199e24(UnkSlotsInfo_ovb0176* info);
void fn_7100199eac(UnkSlotsInfo_ovb0176* info);
void fn_7100199370(UnkSlotsInfo_ovb0176* info, f32 dt);
void fn_71001c113c(UnkSlotsOwner_ovb0176* self, f32 dt);
void fn_71001c12a8(UnkSlotsOwner_ovb0176* self);
u32 fn_7100219e44(UnkRenderManager* manager, void* owner);

void fn_71001c0968(UnkSlotsOwner_ovb0176* self, bool enable) {
    bool value = enable;
    if (self->field_0x35e != value) {
        if (enable) {
            fn_7100199e24(self->info);
        } else {
            fn_7100219e44(g_unk_7101d47300, self);
            fn_7100199eac(self->info);
        }
        self->field_0x35e = value;
    }
}

void fn_71001c1030(UnkSlotsOwner_ovb0176* self, bool force, f32 dt) {
    bool scaled = self->field_0x35f;
    const u8* flags = reinterpret_cast<const u8*>(g_unk_710b372a98) + 0x3183669;
    if (scaled)
        dt = *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g_unk_710b372a98) + 0x38) * dt;
    f32 step = flags[0x13] ? 0.0f : dt;
    if ((self->field_0x35e && self->info->field_0x30 != 0) || force || self->field_0x70 || flags[0]) {
        if (!self->field_0x359) {
            if (UnkDriver_ovb0176* driver = self->driver) {
                if (self->field_0xe0)
                    driver->vf_0x20(step);
                self->driver->vf_0x28(self->field_0xf0);
            }
            fn_71001c113c(self, step);
            fn_7100199370(self->info, step);
            fn_71001c12a8(self);
            self->field_0x359 = true;
            self->field_0x35f = true;
        }
    } else if (self->count) {
        fn_71001c113c(self, step);
    }
}

struct UnkBlendInit_ovb0176 {
    CColor4F from;
    CColor4F to;
    f32 time;
    f32 duration;
    u32 id;
    u32 kind;
    u32 mode;

    UnkBlendInit_ovb0176() : from(1.0f, 1.0f, 1.0f, 1.0f), to(1.0f, 1.0f, 1.0f, 1.0f), time(0.0f), duration(0.0f), id(0) {}
};
static_assert(sizeof(UnkBlendInit_ovb0176) == 0x34, "layout");

struct UnkSlotsInit_ovb0176 {
    u64 field_0x0;
    u32 field_0x8;
    u8 pad_0xc[4];
    UnkSlotsInit_ovb0176* self_ref;  // 0x10
    u32 field_0x18;
    u8 pad_0x1c[4];
    u8 sphere[0x50];  // 0x20
    bool field_0x70;
    u8 pad_0x71[7];
    void* info;  // 0x78
    u8 record[0x60];  // 0x80
    u64 field_0xe0;
    u64 field_0xe8;
    u8 object[0x38];  // 0xf0
    f32 field_0x128;
    f32 field_0x12c;
    f32 field_0x130;
    UnkBlendInit_ovb0176 blends[9];  // 0x134
    UnkBlendInit_ovb0176* entries[9];  // 0x308
    u64 count;  // 0x350
    u8 flags[8];  // 0x358
    u16 field_0x360;
    u8 field_0x362;
};
static_assert(__builtin_offsetof(UnkSlotsInit_ovb0176, blends) == 0x134, "layout");
static_assert(__builtin_offsetof(UnkSlotsInit_ovb0176, entries) == 0x308, "layout");
static_assert(__builtin_offsetof(UnkSlotsInit_ovb0176, count) == 0x350, "layout");

void fn_71001c071c(UnkSlotsInit_ovb0176* self) {
    self->field_0x0 = 0;
    self->field_0x8 = ~0u;
    self->field_0x18 = 0;
    self->self_ref = nullptr;
    fn_710021685c(self->sphere);
    self->field_0x70 = false;
    self->info = nullptr;
    fn_7100025624(self->record);
    self->field_0xe0 = 0;
    self->field_0xe8 = 0;
    fn_7100014d94(self->object, 0);
    self->field_0x128 = 0.0f;
    self->field_0x12c = 0.0f;
    self->field_0x130 = 0.0f;
    for (auto& blend : self->blends)
        new (&blend) UnkBlendInit_ovb0176;
    self->count = 0;
    self->flags[0] = 1;
    self->flags[1] = 0;
    self->flags[2] = 1;
    self->flags[3] = 0;
    self->flags[4] = 1;
    self->flags[5] = 1;
    self->flags[6] = 0;
    self->flags[7] = 1;
    self->field_0x360 = 1;
    self->field_0x362 = 0;
    self->field_0x0 = 0x100000002ull;
    self->self_ref = self;
    for (int i = 0; i < 9; ++i)
        self->entries[i] = &self->blends[i];
}

static inline bool SetAndRemove(UnkSlots* self, u32 id, u32 kind, const CColor4F* color, u32 mode) {
    if (g_unk_710b372a98->field_0x318367c)
        return false;
    if (!fn_71001996e0(self->info, id, kind, fn_7100030878(color), mode))
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        UnkSlotEntry* e = self->entries[i];
        if (e->field_0x28 == id && e->field_0x2c == kind) {
            --self->count;
            self->entries[i] = self->entries[self->count];
            self->entries[self->count] = e;
        }
    }
    return true;
}

bool fn_71001c2a2c(UnkSlots* self, u32 kind, const CColor4F* color, u32 mode) {
    u32 n = self->info->count;
    bool ok = true;
    for (u32 id = 0; id != n; ++id) {
        if (ok)
            ok = SetAndRemove(self, id, kind, color, mode);
    }
    return ok;
}
