// CAnimatedCameraCtrl's unit (tools/tu_map.py unit 2692): the constructor. After the camera base
// (fn_7100763ed4) it builds the empty ids, the two script strings (assigned ""), the end callback,
// the timer at +0x5a8 and the state, then resets the blend values. Names and layouts are placeholders.
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkStruct_7100763ed4;
extern void* g_unk_7101cb1550[];

// An id made the pool's empty string (creating the pool first when there is none).
struct UnkEmptyId_l0e81e8 {
    base::global::UnkStringData* data;
    UnkEmptyId_l0e81e8() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

// The timer at +0x5a8, built by fn_710002f4e4.
struct UnkTimer_l0e81e8 {
    u8 data[0xb0];
    UnkTimer_l0e81e8() { fn_710002f4e4(this); }
};

// A callback run once at the end: fn(arg).
struct UnkCallback_l0e81e8 {
    void* arg = nullptr;
    void (*fn)(void* arg) = nullptr;
};

using UnkScriptString_l0e81e8 = UnkBufferString<0x80, g_unk_7101a1b750>;

struct UnkAnimatedCameraBase_l0e81e8 {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x9c - 0x2c];
    u32 field_0x9c;
    u8 pad_0xa0[0x2a0 - 0xa0];

    UnkAnimatedCameraBase_l0e81e8() {
        fn_7100763ed4(reinterpret_cast<UnkStruct_7100763ed4*>(this));
        vtable = g_unk_7101cb1550 + 2;
    }
};

struct UnkAnimatedCameraCtrl_l0e81e8 : UnkAnimatedCameraBase_l0e81e8 {
    bool field_0x2a0 = false;
    void* field_0x2a8 = nullptr;
    base::global::CFilePathStrId field_0x2b0;
    UnkEmptyId_l0e81e8 field_0x2b8;
    bool field_0x2c0 = false;
    f32 field_0x2c4 = 0.0f;
    f32 field_0x2c8 = 0.0f;
    f32 field_0x2cc;
    bool field_0x2d0;
    f32 field_0x2d4 = 0.0f;
    f32 field_0x2d8 = 0.0f;
    f32 field_0x2dc;
    UnkScriptString_l0e81e8 field_0x2e0;
    u64 field_0x3a8 = 0;
    u64 field_0x3b0 = 0;
    UnkScriptString_l0e81e8 script;  // 0x3b8
    UnkCallback_l0e81e8 on_end;  // 0x480
    u8 pad_0x490[4];
    u32 field_0x494 = 0;
    bool field_0x498 = true;
    f32 field_0x49c;
    f32 field_0x4a0 = -1.0f;
    bool field_0x4a4 = false;
    u64 field_0x4a8 = 0;
    u32 field_0x4b0 = 0;
    void* field_0x4b8 = nullptr;
    u64 field_0x4c0 = 0;
    u64 field_0x4c8 = 0;
    u64 field_0x4d0 = 0;
    u8 pad_0x4d8[4];
    s32 state;  // 0x4dc
    u64 field_0x4e0 = 0;
    u64 field_0x4e8 = 0;
    u64 field_0x4f0 = 0;
    u64 field_0x4f8 = 0;
    u32 field_0x500 = 0;
    u8 pad_0x504[0xc];
    UnkEmptyId_l0e81e8 field_0x510;
    bool field_0x518 = false;
    f32 field_0x51c = 0.0f;
    f32 field_0x520 = 0.0f;
    f32 field_0x524 = 0.0f;
    f32 field_0x528 = 0.0f;
    f32 field_0x52c = 0.0f;
    f32 field_0x530 = 0.0f;
    f32 field_0x534 = 0.0f;
    f32 field_0x538 = 0.0f;
    f32 field_0x53c = 0.0f;
    u8 pad_0x540[0xc];
    bool field_0x54c = false;
    u64 field_0x550 = 0;
    u64 field_0x558 = 0;
    u64 field_0x560 = 0;
    f32 field_0x568 = 0.0f;
    f32 field_0x56c = 0.0f;
    u32 field_0x570 = 0;
    u8 pad_0x574[0xc];
    bool field_0x580 = false;
    f32 field_0x584 = 0.0f;
    f32 field_0x588 = 0.0f;
    f32 field_0x58c = 0.0f;
    f32 field_0x590 = 0.0f;
    f32 field_0x594 = 0.0f;
    bool field_0x598 = false;
    bool field_0x599 = false;
    bool field_0x59a = false;
    UnkEmptyId_l0e81e8 field_0x5a0;
    UnkTimer_l0e81e8 field_0x5a8;
    bool field_0x658;

    UnkAnimatedCameraCtrl_l0e81e8() asm("fn_71010e81e8");

    void ResetBlend() {
        field_0x49c = -1.0f;
        field_0x2cc = -1.0f;
        field_0x2d0 = false;
        field_0x2dc = -1.0f;
        field_0x4a8 = 0;
        field_0x4b0 = 0;
        field_0x568 = 0.0f;
        field_0x4d0 = 0;
        field_0x4c0 = 0;
        field_0x4c8 = 0;
        field_0x56c = 1.0f;
    }
};
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e81e8, field_0x3b0) == 0x3b0, "layout");
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e81e8, state) == 0x4dc, "layout");
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e81e8, field_0x510) == 0x510, "layout");
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e81e8, field_0x594) == 0x594, "layout");
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e81e8, field_0x5a0) == 0x5a0, "layout");
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e81e8, field_0x658) == 0x658, "layout");

UnkAnimatedCameraCtrl_l0e81e8::UnkAnimatedCameraCtrl_l0e81e8() {
    field_0x28 = 5;
    field_0x658 = false;
    state = 0x7fff;
    field_0x9c = 1;
    fn_710007e77c(field_0x2e0.Str(), g_unk_710162e2ed);
    fn_710007e77c(script.Str(), g_unk_710162e2ed);
    ResetBlend();
}
