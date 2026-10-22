// The static initialiser of the system fonts' unit: the header, the default allocator kept in a
// global, the two font texture paths with an empty handle each (destroyed by fn_7100154538), and
// two identity matrices.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71001545d0 {
    HeaderIds ids;
    UnkFilePathStrIdBase font;
    UnkFilePathStrIdBase font_small;
};
struct UnkMatrix44f {
    f32 m[4][4];

    void SetIdentity() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = i == j ? 1.0f : 0.0f;
    }
    // Zeroes everything, then sets the diagonal from the last element.
    void ClearToIdentity() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = 0.0f;
        for (int i = 3; i >= 0; --i)
            m[i][i] = 1.0f;
    }
};
extern UnkStatics_71001545d0 g_unk_716e29ac18 __attribute__((visibility("hidden")));
extern void* g_unk_716e29ab80;
extern void* g_unk_716e29ab88;
extern void* g_unk_716e29ab90;
extern UnkMatrix44f g_unk_716e29ab98;
extern UnkMatrix44f g_unk_716e29abd8;
void* fn_7100083990();
void fn_7100154538(void* handle);

// NON_MATCHING(minor): the first matrix's stores are merged differently (the original pairs every
// element from the second on, then stores the first).
void fn_71001545d0() {
    InitHeaderIds(&g_unk_716e29ac18.ids);
    g_unk_716e29ab80 = fn_7100083990();
    InitPathId(&g_unk_716e29ac18.font, "system/engine/fonts/sys_font.bctex");
    g_unk_716e29ab88 = nullptr;
    __cxa_atexit(fn_7100154538, &g_unk_716e29ab88, g_unk_7101a19000);
    InitPathId(&g_unk_716e29ac18.font_small, "system/engine/fonts/sys_font_small.bctex");
    g_unk_716e29ab90 = nullptr;
    __cxa_atexit(fn_7100154538, &g_unk_716e29ab90, g_unk_7101a19000);
    g_unk_716e29ab98.SetIdentity();
    g_unk_716e29abd8.ClearToIdentity();
}
