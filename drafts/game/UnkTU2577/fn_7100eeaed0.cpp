// o0314: MATCHES when fn_7100eea4c8 takes `const UnkMask& mask` (its temporary then gets lifetime markers and shares
// the zero vector's stack slot), but src/game/CSubAreaManager.cpp declares it with a by-value UnkMask.
// Also define fn_7100eeaed0 with a CSubAreaManager* parameter (its existing declaration) and cast inside.
#include "UnkLeftoverReg.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct CSubAreaManager;

// A sub-area (a placeholder for the fields used here): two ids.
struct UnkSubArea {
    CStrId field_0x0;
    u8 pad_0x8[0x48];
    CStrId field_0x50;
};
static inline const CStrId& FirstId(const UnkSubArea* area) { return area ? area->field_0x0 : g_unk_7101d470e0; }
static inline const CStrId& SecondId(const UnkSubArea* area) { return area ? area->field_0x50 : g_unk_7101d470e0; }

// A bit mask passed by value (copied into a temporary).
struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkMaskOf {
    static const UnkMask& None() {
        static UnkMask s;
        return s;
    }
    static const UnkMask& All() {
        static UnkMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkSubAreaMaskTag;

struct UnkScenarioSub {
    u8 pad_0x0[0x58];
    bool field_0x58;
};
struct UnkScenario {
    u8 pad_0x0[0x820];
    UnkScenarioSub* field_0x820;
};

// Helpers that take an empty value first (their callers leave x0 as it is).
struct UnkSubAreaTag : UnkLeftoverReg {};
void* fn_7100eea4c8(UnkSubAreaTag, UnkSubArea* area, bool a, bool b, const UnkMask& mask);
void* fn_7100eea618(UnkSubAreaTag, UnkSubArea* area, bool flag);
void fn_7100fac120(UnkSubArea* area, UnkMask mask, bool a, bool b, void* c, bool d);
void fn_7100eeaa48(CSubAreaManager* self, const CStrId& prev_a, const CStrId& prev_b, const CStrId& cur_a,
                   const CStrId& cur_b, bool flag);
void fn_7100ee6ae4(CSubAreaManager* self);
void fn_7100ee4d34(CSubAreaManager* self, f32 dt);

struct UnkVec;
void fn_710105383c(void* effects);
bool fn_7101053a0c(void* effects);
void fn_7101053850(void* effects, UnkVec* position, u32 flag, f32 duration, f32 scale);
struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);

struct UnkSubareaRenderOwner;
struct UnkSubareaRenderSetup;
struct UnkSubareaRenderOut {
    void* data;
    u32 count;
};
void fn_7100facb08(UnkSubareaRenderOwner* self, s32 kind, UnkSubareaRenderSetup* setup, bool flag,
                   UnkSubareaRenderOut* out);

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u8 pad_0x10c[4];
    u32 field_0x110;
    u8 pad_0x114[0x2dc];
    UnkSubArea* field_0x3f0;
    UnkSubArea* field_0x3f8;
    u8 pad_0x400[0x538];
    UnkSubareaRenderOut field_0x938;
    u8 pad_0x948[0x18];
    UnkSubareaRenderOut field_0x960;
    u8 pad_0x970[0x18];
    f32 field_0x988;
    u8 pad_0x98c[0x154];
    bool field_0xae0;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x938) == 0x938, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x960) == 0x960, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x988) == 0x988, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0xae0) == 0xae0, "layout");

void fn_7100eeaed0(UnkSubAreaMgr_b190* self) {
    self->field_0x110 = 1;
    UnkGameManager* gm = g_unk_7101d47310;
    reinterpret_cast<UnkScenario*>(gm->field_0x608)->field_0x820->field_0x58 = false;
    if (self->field_0x108 == 3) {
        if (self->field_0xae0) {
            void* effects = gm->field_0x1040;
            fn_710105383c(effects);
            if (!fn_7101053a0c(effects)) {
                UnkVec3f zero(0.0f, 0.0f, 0.0f);
                fn_7101053850(effects, reinterpret_cast<UnkVec*>(&zero), 0, 0.2f, 1.0f);
            }
        }
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(fn_7100defcec(gm)), false);
        fn_7100ee6ae4(reinterpret_cast<CSubAreaManager*>(self));
    } else {
        fn_7100eea4c8(UnkSubAreaTag(), self->field_0x3f0, false, false, UnkMask(0x1c0));
        fn_7100eea618(UnkSubAreaTag(), self->field_0x3f0, false);
        fn_7100fac120(self->field_0x3f0, UnkMaskOf<UnkSubAreaMaskTag>::All(), true, false, self->field_0x3f8, false);
    }
    self->field_0x960.count = 0;
    self->field_0x938.count = 0;
    if (self->field_0x3f0) {
        fn_7100facb08(reinterpret_cast<UnkSubareaRenderOwner*>(self->field_0x3f0), 1,
                      reinterpret_cast<UnkSubareaRenderSetup*>(self->field_0x3f8), false, &self->field_0x960);
        fn_7100facb08(reinterpret_cast<UnkSubareaRenderOwner*>(self->field_0x3f0), 1,
                      reinterpret_cast<UnkSubareaRenderSetup*>(self->field_0x3f8), true, &self->field_0x938);
    }
    self->field_0x988 = 0.0f;
    fn_7100ee4d34(reinterpret_cast<CSubAreaManager*>(self), 0.0f);
    if (self->field_0x108 != 3)
        fn_7100eeaa48(reinterpret_cast<CSubAreaManager*>(self), FirstId(self->field_0x3f8), SecondId(self->field_0x3f8),
                      FirstId(self->field_0x3f0), SecondId(self->field_0x3f0), false);
}
