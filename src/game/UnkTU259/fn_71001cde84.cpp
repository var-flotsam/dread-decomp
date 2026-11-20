// Unit 259 (the graphics system's unit): runs one draw pass of the graphics system, either the full
// per-object pass or the simple entry list. Names and layouts are placeholders.
#include "game/UnkRenderTargetA1.h"
#include "game/UnkTU259.h"
#include "game/UnkTU58.h"
#include "types.h"


void fn_71001ce2f0(UnkRenderTargetA1* self, s32 a);

// `unused` is passed by every caller and never read.
void fn_71001cde84(UnkRenderTargetA1* self, u32 pass, bool full, bool unused) {
    if (g_unk_716e554910 == g_unk_7101d58e48)
        return;
    if (self->entry_count == 0 && self->field_0x1948168 == 0 && self->field_0x3178170 == 0 &&
        self->field_0x317a978 == 0)
        return;
    if (pass == 1) {
        if (self->field_0xb0130 == 0 && self->field_0x1948168 == 0)
            return;
    } else if (pass == 0xe) {
        if (self->field_0xc0138 == 0 && self->field_0x3178170 == 0)
            return;
    }
    self->pass = pass;
    self->field_0x317acc0 = 0;
    g_unk_716e554938 = 0;
    fn_71001ce148(reinterpret_cast<UnkGfxN_b0154*>(self));
    if (full) {
        fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 0, 0, 0);
        self->pass = pass;
        u32 kind;
        switch (pass) {
        case 2:
        case 4:
        case 8:
        case 14:
            kind = 2;
            break;
        case 1:
        case 16:
            kind = self->mode == 2 ? 1 : 3;
            break;
        default:
            kind = 0;
            break;
        }
        self->pass_kind = kind;
        fn_71001ce2f0(self, 1);
        self->field_0x317acb8 = 0;
        self->field_0x317acc0 = 0;
        switch (self->mode) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 10:
            fn_71001cfe44(reinterpret_cast<UnkGfxJ_b0154*>(self));
            break;
        default:
            fn_71001cf9ec(reinterpret_cast<UnkGfxJ_b0154*>(self));
            break;
        }
        self->field_0x317acb8 = 0;
        self->field_0x317acc0 = 0;
        UnkPassObjectA1** p = self->objects;
        for (u32 n = self->object_count; n != 0; n--) {
            UnkPassObjectA1* object = *p++;
            if (self->mode == 2) {
                UnkPassSubA1* sub = object->sub;
                if (!sub || !sub->field_0xa9 || sub->field_0x30 || !sub->field_0xb0)
                    continue;
            }
            object->callback(object);
        }
        if (self->field_0x317a978 && (self->field_0x317a97c & self->pass))
            fn_71001ce514(reinterpret_cast<UnkGfxO_b0154*>(self));
    } else {
        fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 1, 0, 0);
        UnkPassEntryA1* entry = self->entries;
        for (u32 n = self->entry_count; n != 0; n--) {
            UnkPassEntryA1* current = entry++;
            if (current->pass_mask & self->pass)
                current->callback(current);
        }
    }
    if (self->mode == 2)
        self->field_0x3183672 = 1;
}
