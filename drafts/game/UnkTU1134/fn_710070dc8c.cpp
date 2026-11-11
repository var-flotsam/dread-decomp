#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayer_b42;
struct UnkAnimTiming;
struct UnkTimeline;
struct UnkMotionTracks;

struct UnkRefObj_b257a {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    void Release() {
        if (--ref_count == 0)
            vf_0x10();
    }
};

struct UnkOwner_b257a {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
};

struct UnkArgs_b257a {
    UnkRefObj_b257a* obj;
    s32 value;
    ~UnkArgs_b257a() {
        if (obj)
            obj->Release();
    }
};

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_b257a() {
    static UnkFlags_71007142e8 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_b257a() {
    static UnkFlags_71007142e8 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_b257a().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkTrackDef_b257a {
    u8 pad_0x0[0x14];
    s32 field_0x14;
    u8 pad_0x18[0x1c];
    u32 field_0x34;
    CStrId field_0x38;
};

struct UnkTrackEntry_b257a {
    u8 pad_0x0[0x10];
    UnkTrackDef_b257a* def;  // 0x10
};

struct UnkTrackPlayer_b257a : UnkRefObj_b257a {
    u8 pad_0xc[4];
    UnkTrackDef_b257a* current;  // 0x10
    u8 pad_0x18[0x98];
    UnkMotionTracks* field_0xb0;
};

struct UnkTrackEvent_b257a {
    UnkTrackEntry_b257a* entry;
    u32 flags;  // 0x8
    u8 flag;  // 0xc
};

struct UnkTrackListener_b257a;

struct UnkTrackCall_b257a {
    void* action;
    UnkTrackEntry_b257a* entry;
    bool flag;
};

struct UnkTrackListener_b257a {
    virtual void vf_0x0(UnkTrackCall_b257a* call);
    virtual void vf_0x8(UnkTrackCall_b257a* call);
};

struct UnkTU1134_b257a {
    void** vtable;
    u64 field_0x8;
    UnkTrackListener_b257a* field_0x10;
    UnkOwner_b257a* field_0x18;
    u64 field_0x20;
    UnkTrackPlayer_b257a* field_0x28;
    CStrId field_0x30;
    u8 field_0x38[0x70];
    f32 field_0xa8;
    u8 pad_0xac[4];
    u8 field_0xb0[0xb0];
    f32 field_0x160;
    f32 field_0x164;
    bool field_0x168;
    u8 pad_0x169[3];
    u32 field_0x16c;
    u32 field_0x170;
    u16 field_0x174;
};

bool fn_7100c95ed8(UnkPlayer_b42*);
s32 fn_7100c95f28(UnkPlayer_b42*);
f32 fn_7100c95f34(UnkPlayer_b42*);
f32 fn_7100b8aff0(f32, const UnkAnimTiming*);
void fn_7100b8b3bc(UnkTimeline*, void*, UnkMotionTracks*, const u32*, f32, f32);
void fn_71007142e8(UnkStruct_71007142e8*);
void fn_71007143c8(UnkStruct_71007142e8*, UnkFlags_71007142e8);
void fn_7100714808(void* part, UnkStruct_71007142e8* motion, bool flag);
void fn_7100714eb4(void* part, const u32* flags);
void fn_7100714dac(void* part);
void fn_7100c976e4(UnkRefObj_b257a* self, UnkArgs_b257a* args);
void fn_710002fa90(void* list);
void fn_710002f4e4(void*);

void fn_710070d5dc(UnkTU1134_b257a* self) {
    if (self->field_0x18)
        self->field_0x18->vf_0x48();
    if (self->field_0x28) {
        UnkArgs_b257a args = {nullptr, 1};
        fn_7100c976e4(self->field_0x28, &args);
    }
    if (self->field_0x28) {
        UnkRefObj_b257a* o = self->field_0x28;
        o->Release();
        self->field_0x28 = nullptr;
    }
    self->field_0x30 = g_unk_7101d470e0;
    fn_710002fa90(self->field_0xb0);
    self->field_0x168 = false;
    self->field_0x160 = 0.0f;
    self->field_0x164 = 1.0f;
}

extern void* g_unk_7101a6c048[];

struct UnkMaskZero8_b257a {
    u32 value;
    UnkMaskZero8_b257a() : value(0) {}
};

inline const UnkMaskZero8_b257a& UnkMaskZero8() {
    static UnkMaskZero8_b257a s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkMask1_b257a() {
    static u32 s_mask asm("g_unk_7101d57cb8") = u8(~UnkMaskZero8().value);
    return s_mask;
}

struct UnkMaskZero32_b257a {
    u32 value;
    UnkMaskZero32_b257a() : value(0) {}
};

inline const UnkMaskZero32_b257a& UnkMaskZero32_b257() {
    static UnkMaskZero32_b257a s_value asm("g_unk_7101d57ce8");
    return s_value;
}

inline u32 UnkMask2_b257a() {
    static u32 s_mask asm("g_unk_7101d57cd8") = ~UnkMaskZero32_b257().value & 0x3f;
    return s_mask;
}

__attribute__((flatten)) void fn_710070d188(UnkTU1134_b257a* self) {
    self->vtable = &g_unk_7101a6c048[2];
    self->field_0x8 = 0;
    self->field_0x10 = nullptr;
    self->field_0x18 = nullptr;
    self->field_0x20 = 0;
    self->field_0x28 = nullptr;
    self->field_0x30.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x30.data = &g_unk_7101d47108->empty;
    fn_7100714dac(self->field_0x38);
    fn_710002f4e4(self->field_0xb0);
    self->field_0x160 = 0.0f;
    self->field_0x164 = 1.0f;
    self->field_0x16c = UnkMask1_b257a();
    self->field_0x170 = UnkMask2_b257a();
    self->field_0x174 = 1;
}

void fn_710070dc8c(UnkTU1134_b257a* self, u32* event, bool reset) {
    if (!fn_7100c95ed8(reinterpret_cast<UnkPlayer_b42*>(self->field_0x28))) {
        UnkTrackDef_b257a* def = self->field_0x28->current;
        u32 flags = *event;
        u32 def_flags = def->field_0x34;
        if (((flags & 2) && (def_flags & 1)) || ((flags & 4) && (def_flags & 2))) {
            UnkStruct_71007142e8 motion;
            fn_71007142e8(&motion);
            void* part = self->field_0x38;
            f32 time = fn_7100b8aff0(self->field_0xa8, reinterpret_cast<UnkAnimTiming*>(def));
            f32 frames = fn_7100c95f28(reinterpret_cast<UnkPlayer_b42*>(self->field_0x28));
            fn_7100b8b3bc(reinterpret_cast<UnkTimeline*>(def), &motion,
                          self->field_0x28->field_0xb0, event, time, frames);
            if (reset)
                fn_71007143c8(reinterpret_cast<UnkStruct_71007142e8*>(part), UnkFlagsAll_b257a());
            fn_7100714808(part, &motion, true);
        }
    }
    self->field_0xa8 = fn_7100c95f34(reinterpret_cast<UnkPlayer_b42*>(self->field_0x28));
}

void fn_710070da74(UnkTU1134_b257a* self, UnkTrackEvent_b257a* ev) {
    u32 no_flag = ev->flag == 0;
    UnkTrackDef_b257a* def = ev->entry->def;
    u32 def_flags = def->field_0x34;
    // keeps the flag test separate from the second bit test
    asm("" : "+r"(no_flag));
    if (__builtin_expect((def_flags & no_flag) != 0, 1) || (def_flags & 2)) {
        u32 flags = ev->flags;
        if (flags & 1) {
            UnkFlags_71007142e8 all = UnkFlagsAll_b257a();
            fn_7100714eb4(self->field_0x38, &all.bits);
            self->field_0xa8 = fn_7100c95f34(reinterpret_cast<UnkPlayer_b42*>(ev->entry));
        } else if (def->field_0x14 == 1 || (def_flags & 2) == 0) {
            self->field_0xa8 = fn_7100c95f34(reinterpret_cast<UnkPlayer_b42*>(ev->entry));
        } else if (!(flags & 2)) {
            u32 value = 4;
            fn_710070dc8c(self, &value, true);
        }
    }
    self->field_0x30 = ev->entry->def->field_0x38;
    if (self->field_0x28) {
        UnkRefObj_b257a* o = self->field_0x28;
        o->Release();
        self->field_0x28 = nullptr;
    }
    if (UnkTrackListener_b257a* listener = self->field_0x10) {
        UnkTrackCall_b257a call;
        call.action = self;
        call.entry = ev->entry;
        call.flag = ev->flag;
        listener->vf_0x8(&call);
    }
}
