// Unit 259 (the graphics system's unit): runs one draw pass of the graphics system, either the full
// per-object pass or the simple entry list. Names and layouts are placeholders.
#include "game/UnkTU259.h"
#include "game/UnkTU58.h"
#include "types.h"

struct UnkGfxN_b0154;
struct UnkGfxJ_b0154;
struct UnkGfxO_b0154;

extern u32 g_unk_716e554910;

struct UnkGfxPass_l1cde84;
void fn_71001ce2f0(UnkGfxPass_l1cde84* self, s32 a);

struct UnkPassEntry_l1cde84 {
    u8 pad_0x0[0x10];
    u32 pass_mask;  // 0x10
    u8 pad_0x14[0x40 - 0x14];
    void (*callback)(UnkPassEntry_l1cde84* self);  // 0x40
    u8 pad_0x48[0x8];
};

struct UnkPassSub_l1cde84 {
    u8 pad_0x0[0x30];
    u8 field_0x30;
    u8 pad_0x31[0xa9 - 0x31];
    u8 field_0xa9;
    u8 pad_0xaa[0xb0 - 0xaa];
    u8 field_0xb0;
};

struct UnkPassObject_l1cde84 {
    u8 pad_0x0[0x20];
    UnkPassSub_l1cde84* sub;  // 0x20
    u8 pad_0x28[0x18];
    void (*callback)(UnkPassObject_l1cde84* self);  // 0x40
};

struct UnkGfxPass_l1cde84 {
    u8 pad_0x0[0x128];
    UnkPassEntry_l1cde84 entries[0x2000];  // 0x128
    u32 entry_count;                       // 0xa0128
    u8 pad_0xa012c[0xb0130 - 0xa012c];
    u32 field_0xb0130;
    u8 pad_0xb0134[0xc0138 - 0xb0134];
    u32 field_0xc0138;
    u8 pad_0xc013c[0x4];
    UnkPassObject_l1cde84* objects[0x2000];  // 0xc0140
    u32 object_count;                        // 0xd0140
    u8 pad_0xd0144[0x1948168 - 0xd0144];
    u32 field_0x1948168;
    u8 pad_0x194816c[0x3178170 - 0x194816c];
    u32 field_0x3178170;
    u8 pad_0x3178174[0x317a978 - 0x3178174];
    u32 field_0x317a978;
    u32 field_0x317a97c;
    u32 pass_kind;  // 0x317a980
    u8 pad_0x317a984[0x317a998 - 0x317a984];
    u32 pass;  // 0x317a998
    u32 mode;  // 0x317a99c
    u8 pad_0x317a9a0[0x317acb8 - 0x317a9a0];
    u64 field_0x317acb8;
    u64 field_0x317acc0;
    u8 pad_0x317acc8[0x3183672 - 0x317acc8];
    u8 field_0x3183672;
};

static_assert(__builtin_offsetof(UnkGfxPass_l1cde84, entry_count) == 0xa0128, "");
static_assert(__builtin_offsetof(UnkGfxPass_l1cde84, object_count) == 0xd0140, "");

void fn_71001cde84(UnkGfxPass_l1cde84* self, u32 pass, bool full) {
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
        UnkPassObject_l1cde84** p = self->objects;
        for (u32 n = self->object_count; n != 0; n--) {
            UnkPassObject_l1cde84* object = *p++;
            if (self->mode == 2) {
                UnkPassSub_l1cde84* sub = object->sub;
                if (!sub || !sub->field_0xa9 || sub->field_0x30 || !sub->field_0xb0)
                    continue;
            }
            object->callback(object);
        }
        if (self->field_0x317a978 && (self->field_0x317a97c & self->pass))
            fn_71001ce514(reinterpret_cast<UnkGfxO_b0154*>(self));
    } else {
        fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 1, 0, 0);
        UnkPassEntry_l1cde84* entry = self->entries;
        for (u32 n = self->entry_count; n != 0; n--) {
            UnkPassEntry_l1cde84* current = entry++;
            if (current->pass_mask & self->pass)
                current->callback(current);
        }
    }
    if (self->mode == 2)
        self->field_0x3183672 = 1;
}
