// Projects a position to screen coordinates through the view selected by (a, b) and reports
// whether it lands inside the scaled screen area.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkStruct_7100219588_Xform {
    f32 v[6];
    UnkStruct_7100219588_Xform() {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
        v[3] = 0.0f;
        v[4] = 0.0f;
        v[5] = 1.0f;
    }
};

struct UnkStruct_7100219588_View {
    u8 pad_0x0[0x108];
    void* field_0x108;
};

void fn_71001fd7b4(void* views, UnkStruct_7100219588_Xform* xform, u32 a, u32 b);
UnkStruct_7100219588_View* fn_71001fce38(void* views, u32 a, u32 b);
void fn_7100150d30(void* camera, UnkStruct_7100219588_Xform* xform, const void* pos, s32* x, s32* y, bool flag);
u32 fn_7100132f80();
u32 fn_7100132f90();
f32 fn_71001d08f4(void* obj);

bool fn_7100219588(UnkRenderManager* self, const void* pos, u32 a, u32 b) {
    s32 x;
    s32 y;
    if (self->field_0x268) {
        UnkStruct_7100219588_Xform xform;
        fn_71001fd7b4(self->field_0x220, &xform, a, b);
        if (UnkStruct_7100219588_View* view = fn_71001fce38(self->field_0x220, a, b))
            fn_7100150d30(view->field_0x108, &xform, pos, &x, &y, true);
    }
    UnkRenderManager* g = g_unk_7101d47300;
    u32 width = fn_7100132f80();
    f32 sx = fn_71001d08f4(g->field_0x258);
    g = g_unk_7101d47300;
    u32 height = fn_7100132f90();
    f32 sy = fn_71001d08f4(g->field_0x258);
    s32 max_y = (u32)(sy * (f32)height);
    s32 max_x = (u32)(sx * (f32)width);
    return x >= 0 && x <= max_x && y >= 0 && y <= max_y;
}
