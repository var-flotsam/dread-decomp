// Graphics start-up: opens the default display and a layer, loads the NVN entry points through
// the bootstrap loader, creates the device and queue, carves the command memory pools, sets up
// the window's two render targets and their sync object, then the performance mode.
#include <nn/oe.h>
#include <nn/vi.h>
#include "game/UnkGraphicsState.h"
#include "base/UnkVector.h"
#include "types.h"

extern "C" void* nvnBootstrapLoader(const char* name);

typedef void* (*GetProcFn)(void* device, const char* name);

// NVN C entry points, stored by the loader (data addresses only; names unknown).
extern GetProcFn g_unk_716e29ac70;
extern void* (*g_unk_716e29ac58)(void* device, void* builder);
extern void (*g_unk_716e29ac40)(void* builder);
extern void (*g_unk_716e29ac48)(void* builder, s32 flags);
extern void (*g_unk_716e29ac78)(void* device, s32 name, s32* value);
extern void (*g_unk_716e29acc0)(void* device, s32 value);
extern void (*g_unk_716e29acb8)(void* device, s32 value);
extern void (*g_unk_716e29acc8)(void* device, const f32* color, s32 format);
extern void (*g_unk_716e29ace0)(void* device, f32 value);
extern void (*g_unk_716e29ad58)(void* builder, void* device);
extern void (*g_unk_716e29ad60)(void* builder);
extern void (*g_unk_716e29ad78)(void* builder, u64 flags);
extern void (*g_unk_716e29ad70)(void* builder, s64 size);
extern void (*g_unk_716e29ad98)(void* builder, s64 size);
extern u64 (*g_unk_716e29ad88)(void* builder);
extern void (*g_unk_716e29ad90)(void* builder, void* memory, u64 size);
extern void (*g_unk_716e29adf0)(void* queue, void* builder);
extern void (*g_unk_716e29b740)(void* builder, void* device);
extern void (*g_unk_716e29b770)(void* builder, void* memory, u64 offset, u64 size);
extern void (*g_unk_716e29b778)(void* builder, void* memory, u64 size);
extern void (*g_unk_716e29b758)(void* builder, void (*callback)());
extern void (*g_unk_716e29bbb0)(void* sync, void* device);
extern void (*g_unk_716e29b4c0)(void* object);
extern void (*g_unk_716e29b570)(void* object);
extern void (*g_unk_716e29b538)(void* object);
extern void (*g_unk_716e29b688)(void* object);
extern void (*g_unk_716e29b588)(void* object);
extern void (*g_unk_716e29b640)(void* object);
extern void (*g_unk_716e29b650)(void* object, s32 value);
extern void (*g_unk_716e29b690)(void* object, u8 value);
extern void (*g_unk_716e29b698)(void* object, u8 value);
extern void (*g_unk_716e29b6a8)(void* object, u8 value);
extern void (*g_unk_716e29b6a0)(void* object, s32 value);
extern void (*g_unk_716e29b6b0)(void* object, s32 a, s32 b);
extern void (*g_unk_716e29b6b8)(void* object, s32 a, s32 b, s32 c, s32 d);
extern void (*g_unk_716e29b648)(void* object, s32 value);
extern void (*g_unk_716e29b658)(void* object, s32 value);
extern void (*g_unk_716e29b590)(void* object, s32 value);
extern void (*g_unk_716e29b030)(void* builder, void* device);
extern void (*g_unk_716e29b038)(void* builder);
extern void (*g_unk_716e29b0c0)(void* builder, void* device);
extern void (*g_unk_716e29b0c8)(void* builder);
extern void (*g_unk_716e29ae38)(void* builder);
extern void (*g_unk_716e29ae30)(void* builder, void* device);
extern void (*g_unk_716e29ae40)(void* builder, void* window);
extern void (*g_unk_716e29bbc8)(void* a, void* b, s32 c, s32 d);
extern void (*g_unk_716e29ae10)(void* object);
extern void (*g_unk_716e29bbd0)(void* object, s64 timeout);
extern void (*g_unk_716e29ae98)(void* window);
extern void (*g_unk_716e29ae48)(void* builder, s32 count, void* textures);
extern void (*g_unk_716e29ae90)(void* window, void* builder);
extern void (*g_unk_716e29aec0)(void* window, s32 interval);
extern void (*g_unk_716e29b938)(void* object, s32 x, s32 y, s32 w, s32 h);
extern void (*g_unk_716e29b950)(void* object, s32 x, s32 y, s32 w, s32 h);
extern void (*g_unk_716e29aea8)(void* window, void* sync, s32* index);

struct UnkHandle {
    u8 pad_0x0[0x8];
    void* field_0x8;
};

// Up to 16 handles (stored 4-byte aligned) and an owner.
typedef UnkHandle* UnkHandlePtr __attribute__((aligned(4)));
struct UnkHandleSet {
    u32 count;
    UnkHandlePtr items[16];
    UnkHandle* owner;

    UnkHandleSet() {}
    UnkHandleSet(UnkHandle* const& item, UnkHandle* owner_) : count(1), owner(owner_) { items[0] = item; }
    // Keeps the non-null handles of a list.
    UnkHandleSet(UnkHandle* const* list, u32 n, UnkHandle* owner_) : count(0), owner(owner_) {
        for (u32 i = 0; i < n; i++) {
            if (list[i])
                items[count++] = list[i];
        }
    }
    UnkHandleSet& operator=(const UnkHandleSet& other) {
        __builtin_memcpy(items, other.items, (u32)(other.count * sizeof(UnkHandlePtr)));
        count = other.count;
        owner = other.owner;
        return *this;
    }
};
static_assert(sizeof(UnkHandleSet) == 0x90, "");

struct UnkPool {
    u8 pad_0x0[0x120];
};

struct UnkRect {
    s32 x, y, w, h;
};

extern void* g_unk_716e29bf10[];
extern void* g_unk_716e29bcc8[];
extern void* g_unk_716e29c188;
extern s32 g_unk_716e29c1d0;
extern u64 g_unk_716e29c218;
extern UnkHandle* g_unk_716e29c170[2];
extern void* g_unk_716e29bcc0;
extern UnkPool g_unk_716e29bcd0[];
extern u8 g_unk_716e29c1d8[];
extern u8 g_unk_716e29c680[];
extern u8 g_unk_716e29c6a8[];
extern u8 g_unk_716e29c6b0[];
extern u8 g_unk_716e29c6cc[];
extern u8 g_unk_716e29c690;
extern u8 g_unk_7101a1d7e5;
extern u8 g_unk_716e29c691;
extern s32 g_unk_7101a1d7ec;
extern s32 g_unk_7101a1d7f0;
extern s32 g_unk_7101a1d7f4;
extern s32 g_unk_7101a1d7f8;
extern s32 g_unk_7101a1d7fc;
extern s32 g_unk_7101a1d808;
extern u8 g_unk_716e29c190[];
extern u8 g_unk_716e29bca0[];
extern void* g_unk_716e29c180;
extern UnkHandleSet g_unk_716e29bf30[2];
extern UnkHandleSet g_unk_716e29c050[2];
extern s32 g_unk_716e29c24c;
extern bool g_unk_7101a1d7bc;
extern u32 g_unk_716e29c238;
extern u32 g_unk_716e29c23c;
extern u32 g_unk_716e29c650;
extern bool g_unk_716e29c248;
extern UnkRect g_unk_716e29c250[];
extern bool g_unk_716e29c654;

void fn_7100153790();
u32 fn_7100153d90();
u32 fn_7100153d00();
nn::vi::Display** fn_7100153cb0();
nn::vi::Layer** fn_7100153cc0();
void* fn_7100153cd0();
void* fn_7100153e00();
void* fn_7100153e10();
void* fn_7100153d10(u32 index);
void* fn_7100153d30(u32 index);
void* fn_7100153db0(u32 index);
void* fn_7100153d50();
void* fn_7100153dd0();
void* fn_7100153df0();
void fn_7100154770(void* device, GetProcFn get_proc);
void fn_7100153350();
u64 fn_710015383c(u64 size, u64 align);
void fn_710015385c(UnkPool* pool, u64 size, s32 flags, void* device);
void fn_7100153968(UnkPool* pool, u64 size, u64 align);
void fn_71001587e0();
void* fn_71000839a0(u64 size, u32 flags);
void fn_71001b3dc0();
void fn_71000668f8(void* object);
void fn_7100161520();
void fn_7100153f40();
void fn_710015f100();
void fn_710015c520();
void fn_7100154560();
void fn_710015c140();
void fn_710015bde0();
s32 fn_7100132f80();
s32 fn_7100132f90();
u32 fn_7100132fc0();
u32 fn_7100132fd0();
void fn_710015c1e0(UnkHandle** out, s32 width, s32 height, s32 count, const s32& format);
void* fn_71001611e0(void* object);
void fn_7100132a30(s32 value);

// The NVN entry points are called through inline wrappers, so the arguments are evaluated
// before the pointer is loaded.
template <class R, class... A, class... B>
__attribute__((always_inline)) inline R Call(R (*&fn)(A...), B... args) {
    return fn(args...);
}

static inline GetProcFn GetProc() {
    static GetProcFn s_get_proc = reinterpret_cast<GetProcFn>(nvnBootstrapLoader("nvnDeviceGetProcAddress"));
    return s_get_proc;
}

static inline void SetPresentInterval(s32 interval) {
    if (g_unk_716e29c24c == interval)
        return;
    g_unk_716e29c24c = interval;
    Call(g_unk_716e29aec0, g_unk_716e29c188, interval);
    g_unk_7101a1d7bc = g_unk_716e29c24c > 0;
}

// NON_MATCHING(minor): in the render-target block the two format arguments are stored before the
// size calls instead of after them, and the count of each filtered handle set is stored after its
// null check instead of inside it, which shifts register allocation there. Same behaviour.
bool fn_71001587f0() {
    fn_7100153790();
    u32 pool_count = fn_7100153d90();
    if (pool_count)
        __builtin_memset(g_unk_716e29bf10, 0, pool_count * sizeof(void*));
    u32 queue_count = fn_7100153d00();
    if (queue_count)
        __builtin_memset(g_unk_716e29bcc8, 0, queue_count * sizeof(void*));
    g_unk_716e29c188 = nullptr;
    g_unk_716e29c1d0 = -1;
    g_unk_716e29c218 = 0;
    g_unk_716e29c170[0] = nullptr;
    g_unk_716e29c170[1] = nullptr;

    nn::vi::Initialize();
    nn::vi::OpenDefaultDisplay(fn_7100153cb0());
    nn::vi::Layer** layer = fn_7100153cc0();
    nn::vi::CreateLayer(layer, *fn_7100153cb0());
    nn::vi::GetNativeWindow(&g_unk_716e29bcc0, *fn_7100153cc0());

    g_unk_716e29ac58 = reinterpret_cast<void* (*)(void*, void*)>(GetProc()(nullptr, "nvnDeviceInitialize"));
    g_unk_716e29ac70 = reinterpret_cast<GetProcFn>(GetProc()(nullptr, "nvnDeviceGetProcAddress"));
    g_unk_716e29ac40 = reinterpret_cast<void (*)(void*)>(Call(g_unk_716e29ac70, nullptr, "nvnDeviceBuilderSetDefaults"));
    g_unk_716e29ac48 = reinterpret_cast<void (*)(void*, s32)>(Call(g_unk_716e29ac70, nullptr, "nvnDeviceBuilderSetFlags"));
    fn_7100154770(nullptr, g_unk_716e29ac70);

    s32 major, minor;
    Call(g_unk_716e29ac78, nullptr, 0, &major);
    Call(g_unk_716e29ac78, nullptr, 1, &minor);

    Call(g_unk_716e29ac40, fn_7100153e00());
    Call(g_unk_716e29ac48, fn_7100153e00(), 0);
    {
        void* device = fn_7100153cd0();
        void* builder = fn_7100153e00();
        Call(g_unk_716e29ac58, device, builder);
    }
    fn_7100154770(fn_7100153cd0(), g_unk_716e29ac70);
    fn_7100153350();
    Call(g_unk_716e29acc0, fn_7100153cd0(), 0);
    Call(g_unk_716e29acb8, fn_7100153cd0(), 1);
    f32 clear_color[4] = {};
    Call(g_unk_716e29acc8, fn_7100153cd0(), clear_color, 0xa);
    Call(g_unk_716e29acc8, fn_7100153cd0(), clear_color, 0x3f);
    Call(g_unk_716e29acc8, fn_7100153cd0(), clear_color, 0x25);
    Call(g_unk_716e29ace0, fn_7100153cd0(), 1.0f);

    {
        void* builder = fn_7100153e10();
        void* device = fn_7100153cd0();
        Call(g_unk_716e29ad58, builder, device);
    }
    Call(g_unk_716e29ad60, fn_7100153e10());
    Call(g_unk_716e29ad78, fn_7100153e10(), 0);
    s32 command_size = 0;
    Call(g_unk_716e29ac78, fn_7100153cd0(), 0x51, &command_size);
    Call(g_unk_716e29ad70, fn_7100153e10(), command_size);
    Call(g_unk_716e29ad98, fn_7100153e10(), command_size);
    u64 queue_memory_size = Call(g_unk_716e29ad88, fn_7100153e10());
    g_unk_716e29bcc8[0] = fn_7100083990()->vf_0x40(queue_memory_size, 0x1000, 0x3a);
    Call(g_unk_716e29ad90, fn_7100153e10(), g_unk_716e29bcc8[0], queue_memory_size);
    {
        void* queue = fn_7100153d10(0);
        void* builder = fn_7100153e10();
        Call(g_unk_716e29adf0, queue, builder);
    }

    s32 control_alignment = 0;
    s32 command_alignment = 0;
    Call(g_unk_716e29ac78, fn_7100153cd0(), 0x13, &control_alignment);
    Call(g_unk_716e29ac78, fn_7100153cd0(), 0x14, &command_alignment);
    for (u32 i = 0; i < pool_count; i++) {
        UnkPool* pool = &g_unk_716e29bcd0[i];
        u64 size = fn_710015383c(0xf00000, control_alignment);
        fn_710015385c(pool, size, 0x22, fn_7100153cd0());
        {
            void* builder = fn_7100153db0(i);
            void* device = fn_7100153cd0();
            Call(g_unk_716e29b740, builder, device);
        }
        g_unk_716e29bf10[i] = fn_7100083990()->vf_0x40(0x200000, command_alignment, 8);
        fn_7100153968(pool, 0xf00000, control_alignment);
        Call(g_unk_716e29b770, fn_7100153db0(i), pool->pad_0x0 + 8, 0, 0xf00000);
        Call(g_unk_716e29b778, fn_7100153db0(i), g_unk_716e29bf10[i], 0x200000);
        Call(g_unk_716e29b758, fn_7100153db0(i), fn_71001587e0);
    }

    {
        void* sync = fn_7100153d30(0);
        void* device = fn_7100153cd0();
        Call(g_unk_716e29bbb0, sync, device);
    }
    Call(g_unk_716e29bbb0, g_unk_716e29c1d8, fn_7100153cd0());
    Call(g_unk_716e29b4c0, g_unk_716e29c678);
    Call(g_unk_716e29b570, g_unk_716e29c680);
    Call(g_unk_716e29b538, g_unk_716e29c68c);
    Call(g_unk_716e29b688, g_unk_716e29c6a8);
    Call(g_unk_716e29b588, g_unk_716e29c6b0);
    Call(g_unk_716e29b640, g_unk_716e29c6cc);
    Call(g_unk_716e29b650, g_unk_716e29c6cc, 1);
    Call(g_unk_716e29b690, g_unk_716e29c6a8, g_unk_716e29c690);
    Call(g_unk_716e29b698, g_unk_716e29c6a8, g_unk_7101a1d7e5);
    Call(g_unk_716e29b6a8, g_unk_716e29c6a8, g_unk_716e29c691);
    Call(g_unk_716e29b6a0, g_unk_716e29c6a8, g_unk_7101a1d7ec);
    Call(g_unk_716e29b6b0, g_unk_716e29c6a8, 1, g_unk_7101a1d7f0);
    Call(g_unk_716e29b6b8, g_unk_716e29c6a8, 1, g_unk_7101a1d7f4, g_unk_7101a1d7f8, g_unk_7101a1d7fc);
    Call(g_unk_716e29b648, g_unk_716e29c6cc, g_unk_7101a1d808);
    Call(g_unk_716e29b658, g_unk_716e29c6cc, 2);
    Call(g_unk_716e29b590, g_unk_716e29c6b0, 0);

    {
        void* builder = fn_7100153dd0();
        void* device = fn_7100153cd0();
        Call(g_unk_716e29b030, builder, device);
    }
    Call(g_unk_716e29b038, fn_7100153dd0());
    {
        void* builder = fn_7100153df0();
        void* device = fn_7100153cd0();
        Call(g_unk_716e29b0c0, builder, device);
    }
    Call(g_unk_716e29b0c8, fn_7100153df0());
    Call(g_unk_716e29ae38, g_unk_716e29c190);
    Call(g_unk_716e29ae30, g_unk_716e29c190, fn_7100153cd0());
    Call(g_unk_716e29ae40, g_unk_716e29c190, g_unk_716e29bcc0);
    {
        void* queue = fn_7100153d10(0);
        void* sync = fn_7100153d30(0);
        Call(g_unk_716e29bbc8, queue, sync, 0, 0);
    }
    Call(g_unk_716e29ae10, fn_7100153d10(0));
    Call(g_unk_716e29bbd0, fn_7100153d30(0), -1);

    if (g_unk_716e29c188)
        Call(g_unk_716e29ae98, g_unk_716e29c188);
    else
        g_unk_716e29c188 = fn_71000839a0(0x180, 8);

    fn_71001b3dc0();
    fn_71000668f8(g_unk_716e29bca0);
    fn_7100161520();
    fn_7100153f40();
    fn_710015f100();
    fn_710015c520();
    fn_7100154560();
    fn_710015c140();
    fn_710015bde0();

    UnkHandle* depth;
    s32 depth_format = 2;
    fn_710015c1e0(&depth, fn_7100132f80(), fn_7100132f90(), 0xe, depth_format);
    g_unk_716e29c180 = fn_71001611e0(depth->field_0x8);
    UnkHandle* colors[2];
    for (s32 i = 0; i < 2; i++) {
        s32 color_format = 0x22;
        fn_710015c1e0(&colors[i], fn_7100132f80(), fn_7100132f90(), 3, color_format);
        g_unk_716e29c170[i] = static_cast<UnkHandle*>(fn_71001611e0(colors[i]->field_0x8));
        g_unk_716e29bf30[i] = UnkHandleSet(colors[i], depth);
        g_unk_716e29c050[i] = UnkHandleSet(&colors[i], 1, depth);
    }
    Call(g_unk_716e29ae48, g_unk_716e29c190, 2, g_unk_716e29c170);
    Call(g_unk_716e29ae90, g_unk_716e29c188, g_unk_716e29c190);
    SetPresentInterval(1);

    g_unk_716e29c238 = fn_7100132fc0() / 4;
    g_unk_716e29c23c = fn_7100132fd0() / 4;
    s32 width = fn_7100132f80();
    s32 height = fn_7100132f90();
    u32 index = g_unk_716e29c650++;
    if (g_unk_716e29c248) {
        UnkRect* rect = &g_unk_716e29c250[index];
        rect->x = 0;
        rect->y = 0;
        rect->w = width;
        rect->h = height;
        Call(g_unk_716e29b938, fn_7100153d50(), rect->x, rect->y, rect->w, rect->h);
        Call(g_unk_716e29b950, fn_7100153d50(), rect->x, rect->y, rect->w, rect->h);
        g_unk_716e29c654 = false;
    } else {
        g_unk_716e29c654 = true;
    }

    nn::oe::Initialize();
    fn_7100132a30(1);
    nn::oe::SetPerformanceConfiguration(nn::oe::PerformanceMode(1), 0x10001);
    Call(g_unk_716e29aea8, g_unk_716e29c188, g_unk_716e29c1d8, &g_unk_716e29c1d0);
    return true;
}
