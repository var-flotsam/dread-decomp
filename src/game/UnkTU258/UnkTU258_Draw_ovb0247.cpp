// Engine unit 258 (graphics, next to the graphics system behind g_unk_710b372a98; see
// src/engine/UnkUnit258/): draw-pass handlers that take the graphics system and a queue entry whose
// command (at +0x8) names the draw state or item list: the item and mask test with the bound-matrix
// bookkeeping (fn_71001c5a74, fn_71001c5b74, which also sets the viewport-style uniforms), the two
// passes that bind a program state and its uniforms (fn_71001c5114, fn_71001c52d4) and the item
// list submission (fn_71001c4894). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector4D.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkTU174.h"
#include "game/UnkTU177.h"
#include "game/UnkTU180.h"
#include "game/UnkTU182.h"
#include "game/UnkTU194.h"
#include "game/UnkTU230.h"
#include "game/UnkTU259.h"
#include "types.h"

extern UnkMatrix44 g_unk_7101a1a680;

struct UnkRenderItem_b0247 {
    u8 pad_0x0[0x18];
    u32 field_0x18;
    u8 pad_0x1c[0x2c];
    u32 field_0x48;
    u8 pad_0x4c[0x34];
    u8 field_0x80;
    u8 pad_0x81[0xcb];
    f32 field_0x14c[4];
    u8 pad_0x15c[0x10];
    f32 field_0x16c[4];
};

struct UnkRenderCmd_b0247 {
    void* field_0x0;
    UnkRenderItem_b0247* item;  // 0x8
};

struct UnkDrawState_b0247 {
    u8 pad_0x0[0x20];
    void* field_0x20[8];  // 0x20: indexed by field_0x98
    u8 pad_0x60[0x38];
    u32 field_0x98;
    u8 pad_0x9c[4];
    void* field_0xa0;
    void* field_0xa8;
    u32 field_0xb0;
    u32 field_0xb4;
    u32 field_0xb8;
    u8 pad_0xbc[4];
    void* field_0xc0;
    u8 pad_0xc8[8];
    u32 field_0xd0;
    u32 field_0xd4;
    u32 field_0xd8;
    u8 pad_0xdc[4];
    u32 field_0xe0[4];
    u8 pad_0xf0[0xc];
    u32 field_0xfc;
    u8 pad_0x100[0x30];
    u32 field_0x130;
    u8 pad_0x134[2];
    u8 field_0x136;
};

struct UnkDrawState2_b0247 {
    u8 pad_0x0[0xa0];
    void* field_0xa0[2];  // 0xa0: indexed by field_0xb0
    u32 field_0xb0;
    u8 pad_0xb4[4];
    void* field_0xb8;
    void* field_0xc0;
    u8 pad_0xc8[8];
    u32 field_0xd0;
    u32 field_0xd4;
    u32 field_0xd8;
    u8 pad_0xdc[4];
    f32 field_0xe0[4];
    u8 pad_0xf0[0x40];
    u32 field_0x130;
    u8 pad_0x134[2];
    u8 field_0x136;
};

struct UnkRenderEntry_b0247 {
    u8 pad_0x0[0x8];
    UnkRenderCmd_b0247* cmd;  // 0x8
};

struct UnkGfx258_b0247 {
    u8 pad_0x0[0x317a998];
    u32 field_0x317a998;
    u32 field_0x317a99c;
    u8 pad_0x317a9a0[0x317aa08 - 0x317a9a0];
    s32 field_0x317aa08[4];
    u8 pad_0x317aa18[0x317ab28 - 0x317aa18];
    const UnkMatrix44* field_0x317ab28;
    u8 pad_0x317ab30[0x317acc0 - 0x317ab30];
    UnkRenderItem_b0247* field_0x317acc0;
    u8 pad_0x317acc8[0x317add0 - 0x317acc8];
    u32 field_0x317add0;
    u8 pad_0x317add4[0x317ae18 - 0x317add4];
    u64 field_0x317ae18;
    u8 pad_0x317ae20[0x317af68 - 0x317ae20];
    void* field_0x317af68;
    void* field_0x317af70;
    u8 pad_0x317af78[0x3183650 - 0x317af78];
    s32 field_0x3183650;
    u8 pad_0x3183654[0x318366b - 0x3183654];
    u8 field_0x318366b;
};

void fn_71001a7bfc(UnkRenderItem_b0247* item, u32 mode);
bool fn_7100196e80(UnkRenderCmd_b0247* cmd);
void fn_71001d010c(UnkGfx258_b0247* self, UnkRenderItem_b0247* item, bool flag);
void fn_71001d03f8(UnkGfx258_b0247* self, void* arg, bool a, bool b);

void fn_71001c5a74(UnkGfx258_b0247* self, UnkRenderEntry_b0247* entry) {
    UnkRenderCmd_b0247* cmd = entry->cmd;
    bool ok = fn_7100196e80(cmd);
    UnkRenderItem_b0247* item = cmd->item;
    if (!item)
        return;
    if (!(item->field_0x18 & self->field_0x317a998))
        return;
    if (self->field_0x317a99c == 2) {
        if (item->field_0x80)
            return;
        fn_710015ab00(item->field_0x48);
    }
    if (!ok)
        return;
    UnkRenderItem_b0247* current = cmd->item;
    self->field_0x317ab28 = &g_unk_7101a1a680;
    if (self->field_0x317acc0 == current) {
        g_unk_716e554938++;
    } else if (self->field_0x318366b) {
        fn_71001d010c(self, current, false);
    }
    fn_71001d03f8(self, cmd->field_0x0, true, true);
}


void fn_71001c5b74(UnkGfx258_b0247* self, UnkRenderEntry_b0247* entry) {
    UnkRenderCmd_b0247* cmd = entry->cmd;
    bool ok = fn_7100196e80(cmd);
    UnkRenderItem_b0247* item = cmd->item;
    if (!item)
        return;
    if (!(item->field_0x18 & self->field_0x317a998))
        return;
    if (self->field_0x317a99c == 2) {
        if (item->field_0x80)
            return;
        fn_710015ab00(item->field_0x48);
    }
    if (!ok)
        return;
    item = cmd->item;
    self->field_0x317ab28 = &g_unk_7101a1a680;
    self->field_0x317acc0 = nullptr;
    if (self->field_0x318366b)
        fn_71001d010c(self, item, true);
    fn_71001d03f8(self, cmd->field_0x0, true, false);
    if (self->field_0x318366b) {
        fn_710015a3e0(1);
        fn_710015a680(0);
        fn_710015a500(6, 7);
        base::math::CVector4D tmp;
        base::math::CVector4D a;
        a.x = item->field_0x16c[0];
        a.y = item->field_0x16c[1];
        a.z = item->field_0x16c[2];
        a.w = item->field_0x16c[3];
        u64 location;
        if (fn_71001aa0b4(self->field_0x317af70, 1, fn_7100167cc0(0), &location)) {
            tmp.x = a.x;
            tmp.y = a.y;
            tmp.z = a.z;
            tmp.w = a.w;
            fn_710015e1f0(self->field_0x317af68, 1, location, 4, &tmp.x);
        }
        if (fn_7100159c80()) {
            self->field_0x317add0++;
            fn_7100159f90(0, -1);
        }
        tmp.x = item->field_0x14c[0];
        tmp.y = item->field_0x14c[1];
        tmp.z = item->field_0x14c[2];
        tmp.w = item->field_0x14c[3];
        fn_710007b518(&a, &tmp);
        if (fn_71001aa0b4(self->field_0x317af70, 1, fn_7100167cc0(0), &location)) {
            tmp.x = a.x;
            tmp.y = a.y;
            tmp.z = a.z;
            tmp.w = a.w;
            fn_710015e1f0(self->field_0x317af68, 1, location, 4, &tmp.x);
        }
        fn_71001a7bfc(item, 2);
    }
    if (fn_7100159c80()) {
        self->field_0x317add0++;
        fn_7100159f90(0, -1);
    }
}

struct UnkCachedMatrix_7100196674;  // src/game_s1/fn_7100196674.cpp
struct UnkSize_7100196674;
UnkMatrix44* fn_7100196674(UnkCachedMatrix_7100196674* self, const UnkSize_7100196674* size);
void fn_710015e4b0(void* state, u32 stage, u64 location, u32 count, u32 a, const UnkMatrix44* values, u32 b);
extern const base::math::CVector4D g_unk_7101647b1c __attribute__((visibility("hidden")));

void fn_71001c5114(UnkGfx258_b0247* self, UnkRenderEntry_b0247* entry) {
    UnkDrawState_b0247* cmd = reinterpret_cast<UnkDrawState_b0247*>(entry->cmd);
    if (self->field_0x3183650 != (s32)cmd->field_0xfc) {
        u32 a = cmd->field_0xb0;
        u32 b = cmd->field_0xb4;
        u32 c = cmd->field_0xb8;
        fn_710015a680(a);
        fn_710015a500(b, c);
        s32 mode = cmd->field_0x136 ? 2 : 1;
        fn_7100160ab0(0, mode, 1);
        fn_7100160990(0, mode, 1);
        self->field_0x3183650 = cmd->field_0xfc;
    }
    fn_710015f770(0, cmd->field_0xa8, true);
    const UnkMatrix44* matrix = fn_7100196674(reinterpret_cast<UnkCachedMatrix_7100196674*>(cmd),
                                              reinterpret_cast<const UnkSize_7100196674*>(self->field_0x317aa08));
    fn_710015e4b0(self->field_0x317af68, 0, self->field_0x317ae18, 4, 1, matrix, 1);
    u64 location;
    if (fn_71001aa0b4(self->field_0x317af70, 1, 0x59, &location)) {
        base::math::CVector4D v = g_unk_7101647b1c;
        fn_710015e1f0(self->field_0x317af68, 1, location, 4, &v.x);
    }
    if (fn_71001aa0b4(self->field_0x317af70, 1, 0x5a, &location)) {
        base::math::CVector4D v = {};
        fn_710015e1f0(self->field_0x317af68, 1, location, 4, &v.x);
    }
    fn_7100161800(cmd->field_0x20[cmd->field_0x98]);
    fn_7100154420(cmd->field_0xa0);
    u32 count = cmd->field_0x130;
    if (fn_7100159c80()) {
        u32 n = count * 6;
        self->field_0x317add0++;
        fn_7100159f90(0, n);
    }
}

UnkMatrix44* fn_7100196248(UnkDrawState2_b0247* self, const void* size);

void fn_71001c52d4(UnkGfx258_b0247* self, UnkRenderEntry_b0247* entry) {
    UnkDrawState2_b0247* cmd = reinterpret_cast<UnkDrawState2_b0247*>(entry->cmd);
    if (self->field_0x3183650 != (s32)cmd->field_0x130) {
        u32 a = cmd->field_0xd0;
        u32 b = cmd->field_0xd4;
        u32 c = cmd->field_0xd8;
        fn_710015a680(a);
        fn_710015a500(b, c);
        s32 mode = cmd->field_0x136 ? 2 : 1;
        fn_7100160ab0(0, mode, 1);
        fn_7100160990(0, mode, 1);
        self->field_0x3183650 = cmd->field_0x130;
    }
    fn_710015f770(0, cmd->field_0xc0, true);
    if (self->field_0x317ae18) {
        const UnkMatrix44* matrix = fn_7100196248(cmd, self->field_0x317aa08);
        fn_710015e4b0(self->field_0x317af68, 0, self->field_0x317ae18, 4, 1, matrix, 1);
    }
    u64 location;
    if (fn_71001aa0b4(self->field_0x317af70, 1, 0x59, &location)) {
        base::math::CVector4D v;
        v.x = cmd->field_0xe0[0];
        v.y = cmd->field_0xe0[1];
        v.z = cmd->field_0xe0[2];
        v.w = cmd->field_0xe0[3];
        fn_710015e1f0(self->field_0x317af68, 1, location, 4, &v.x);
    }
    if (fn_71001aa0b4(self->field_0x317af70, 1, 0x5a, &location)) {
        base::math::CVector4D v = {};
        fn_710015e1f0(self->field_0x317af68, 1, location, 4, &v.x);
    }
    fn_7100161800(cmd->field_0xa0[cmd->field_0xb0]);
    fn_7100154420(cmd->field_0xb8);
    fn_7100160ab0(0, 2, 1);
    fn_7100160990(0, 2, 1);
    fn_7100160d50(0, 2, 1);
    if (fn_7100159c80()) {
        self->field_0x317add0++;
        fn_7100159f90(0, -1);
    }
}

struct UnkNamedEntries_710016b194;
void* fn_710016b0ec(UnkNamedEntries_710016b194* self, u32 index);
void fn_710016b194(UnkNamedEntries_710016b194* self, u32 index);
void fn_71001d0638(UnkGlobal_710b372a98* gfx, void* item, void* a, u32 b);

struct UnkSubmitGroup_b0247 {
    u8 pad_0x0[0x34];
    u32 count;  // 0x34
    u8* elements;  // 0x38
};

struct UnkSubmitList_b0247 {
    u8 pad_0x0[0x8];
    UnkRenderItem_b0247* item;  // 0x8
    u8 pad_0x10[0x8];
    u32 count;  // 0x18
};

void fn_71001c4894(UnkGfx258_b0247* self, UnkRenderEntry_b0247* entry) {
    UnkSubmitList_b0247* list = reinterpret_cast<UnkSubmitList_b0247*>(entry->cmd);
    u32 n = list->count;
    if (n == 0)
        return;
    UnkRenderItem_b0247* item = list->item;
    if (self->field_0x318366b) {
        if (self->field_0x317acc0 == item)
            g_unk_716e554938++;
        else
            fn_71001d010c(self, item, false);
    }
    for (u32 i = 0; i != n; i++) {
        auto* group = static_cast<UnkSubmitGroup_b0247*>(
            fn_710016b0ec(reinterpret_cast<UnkNamedEntries_710016b194*>(list), i));
        if (!group)
            continue;
        self->field_0x317ab28 = reinterpret_cast<const UnkMatrix44*>(reinterpret_cast<u8*>(group) + 0x88);
        u64 size = (u64)group->count * 0x20;
        for (u64 off = 0; off != size; off += 0x20) {
            u8* element = group->elements + off;
            if (element)
                fn_71001d0638(reinterpret_cast<UnkGlobal_710b372a98*>(self), element, item, 0);
        }
        fn_710016b194(reinterpret_cast<UnkNamedEntries_710016b194*>(list), i);
    }
}
