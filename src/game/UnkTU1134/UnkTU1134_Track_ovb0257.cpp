// Unit 1134 of tools/tu_map.py (no name hint; the track actions of game/UnkAnimationComponent.h). fn_710070d5dc
// ends a track action: the owner (+0x18) is told, the player at +0x28 is asked to stop and released, the
// name is reset to the global empty id and the list at +0xb0 is cleared. fn_710070d188 is its
// constructor: empty links and name, the member at +0x38 and the list at +0xb0 initialised, the start time
// cleared and the default masks (guarded statics) at +0x16c and +0x170. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CActionInstance.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU39.h"
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

s32 fn_7100c95f28(UnkPlayer_b42*);
f32 fn_7100c95f34(UnkPlayer_b42*);
void fn_71007142e8(UnkStruct_71007142e8*);
void fn_7100714808(void* part, UnkStruct_71007142e8* motion, s32);
void fn_7100714dac(void* part);
void fn_7100c976e4(UnkRefObj_b257a* self, UnkArgs_b257a* args);
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
