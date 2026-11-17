// UnkTU2772 (nn::atk): detaches a memory pool from the audio renderer, waits until it is detached (state 3),
// rendering or sleeping meanwhile, then releases it. Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkAtkTask;
void* fn_7101228d6c();
u32 fn_710123a3f0(UnkAtkTask* task);
void fn_7101238f90(s64 nanoseconds);
void fn_710123a410(UnkAtkTask* pool);                          // nn::audio::RequestDetachMemoryPool
u32 fn_710123a420(void* renderer, void* config);               // nn::audio::ExecuteAudioRendererRendering
void fn_710123a430(void* event);                               // nn::os::WaitSystemEvent
void fn_710123a440(void* config, UnkAtkTask* pool);           // nn::audio::ReleaseMemoryPool

// The task manager of fn_7101214040, with the members this function reads.
struct UnkAtkRendererOwner_2140f0 {
    u8 pad_0x0[0x8];
    void* renderer;  // 0x8
    void* config;  // 0x10
    u8 config_0x18[0x8];  // 0x18
    u8 pad_0x20[0x70];
    u8 event[0x8];  // 0x90
    u8 pad_0x98[0x8bd];
    bool field_0x955;
    u8 pad_0x956[0x110];
    bool field_0xa66;
};
static_assert(__builtin_offsetof(UnkAtkRendererOwner_2140f0, field_0xa66) == 0xa66, "");

static inline UnkAtkLock* AtkPoolLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_71001039f8()) + 0x960);
}
static inline UnkAtkLock* AtkRenderLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_7101228d6c()) + 0x348);
}
static inline u32 PoolState(UnkAtkTask* pool) {
    fn_71012392d0(AtkPoolLock());
    u32 state = fn_710123a3f0(pool);
    fn_71012392e0(AtkPoolLock());
    return state;
}
static inline void AbortOnFailure(u32 code) {
    alignas(8) u32 result = code;  // an 8-byte aligned slot, as in the original
    if (result != 0)
        fn_71012390d0("", "", "", 0, &result, "");
}

void fn_71012140f0(UnkAtkRendererOwner_2140f0* self, UnkAtkTask* pool, u32 flags) {
    fn_71012392d0(AtkPoolLock());
    fn_710123a410(pool);
    fn_71012392e0(AtkPoolLock());

    UnkAtkTaskManager* manager = reinterpret_cast<UnkAtkTaskManager*>(self);
    if ((flags & 1) && self->field_0x955 && !self->field_0xa66) {
        while (PoolState(pool) != 3)
            fn_7101238f90(5000000);
    } else if (self->field_0xa66) {
        fn_71012392d0(AtkRenderLock());
        fn_71012392d0(AtkPoolLock());
        while (PoolState(pool) != 3) {
            AbortOnFailure(fn_7101214040(manager));
            AbortOnFailure(fn_710123a420(self->renderer, self->config));
            fn_710123a430(self->event);
        }
        fn_71012392e0(AtkPoolLock());
        fn_71012392e0(AtkRenderLock());
    } else {
        while (PoolState(pool) != 3) {
            AbortOnFailure(fn_7101214040(manager));
            fn_710123a430(self->event);
        }
    }

    fn_71012392d0(AtkPoolLock());
    fn_710123a440(self->config_0x18, pool);
    if (self->field_0xa66) {
        fn_71012392d0(AtkRenderLock());
        AbortOnFailure(fn_710123a420(self->renderer, self->config));
        fn_71012392e0(AtkRenderLock());
    }
    fn_71012392e0(AtkPoolLock());
}
