// Projects a position to screen coordinates through the view selected by (a, b) and reports
// whether it lands inside the scaled screen area.
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

struct UnkStruct_7100219588_Global {
    u8 pad_0x0[0x258];
    void* field_0x258;
};
extern UnkStruct_7100219588_Global* unk_7101cba0d0;  // reached through the GOT

void fn_71001fd7b4(void* views, UnkStruct_7100219588_Xform* xform, u32 a, u32 b);
UnkStruct_7100219588_View* fn_71001fce38(void* views, u32 a, u32 b);
void fn_7100150d30(void* camera, UnkStruct_7100219588_Xform* xform, const void* pos, s32* x, s32* y, bool flag);
u32 fn_7100132f80();
u32 fn_7100132f90();
f32 fn_71001d08f4(void* obj);

struct UnkStruct_7100219588 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x40];
    bool field_0x268;
};

bool fn_7100219588(UnkStruct_7100219588* self, const void* pos, u32 a, u32 b) {
    s32 x;
    s32 y;
    if (self->field_0x268) {
        UnkStruct_7100219588_Xform xform;
        fn_71001fd7b4(self->field_0x220, &xform, a, b);
        if (UnkStruct_7100219588_View* view = fn_71001fce38(self->field_0x220, a, b))
            fn_7100150d30(view->field_0x108, &xform, pos, &x, &y, true);
    }
    UnkStruct_7100219588_Global* g = unk_7101cba0d0;
    u32 width = fn_7100132f80();
    f32 sx = fn_71001d08f4(g->field_0x258);
    g = unk_7101cba0d0;
    u32 height = fn_7100132f90();
    f32 sy = fn_71001d08f4(g->field_0x258);
    s32 max_y = (u32)(sy * (f32)height);
    s32 max_x = (u32)(sx * (f32)width);
    return x >= 0 && x <= max_x && y >= 0 && y <= max_y;
}
