// scratch
#include "base/color/CColor4F.h"
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;

extern void* g_unk_7101a6d9f0[];  // the element's vtable
void fn_710074580c(void* sub);

void fn_71007457cc(void* sub);
void fn_7100745714();

struct UnkElementRender_ovb0215 {
    f32 pos[3];
    CColor4F color;  // 0xc
    u8 pad_0x1c[4];
    u8 field_0x20;
    u8 field_0x21;
    u8 field_0x22;
    u8 field_0x23;
    u8 field_0x24;
};

struct UnkElementParent_ovb0215 {
    u8 pad_0x0[0x24];
    u8 field_0x24;
    u8 pad_0x25[3];
    f32 scale;  // 0x28
    u8 pad_0x2c[0x10];
    u8 field_0x3c;
};

struct UnkUnit2560Element {
    void* vtable;                      // 0x0
    u64 field_0x8;
    UnkElementParent_ovb0215* parent;  // 0x10
    UnkElementRender_ovb0215* render;  // 0x18
    s32 field_0x20;
    u8 field_0x24;
    u8 field_0x25;
    u8 pad_0x26[2];
    s32 field_0x28;
    f32 pos[3];  // 0x2c
    f32 vel[3];  // 0x38
    u8 pad_0x44[8];
    u8 field_0x4c;
    u8 pad_0x4d[3];
    void* field_0x50;
    void* field_0x58;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    u8 pad_0x74[4];
    u8 sub[0x34];  // 0x78
    f32 offset[3];  // 0xac
    f32 field_0xb8;
    u8 field_0xbc;
    u8 pad_0xbd[3];
    f32 field_0xc0;
    u8 pad_0xc4[4];
    void* owner;            // 0xc8
    void (*callback)();     // 0xd0
    u32 field_0xd8;
    u32 field_0xdc;
    u32 field_0xe0;
    u8 pad_0xe4[4];
    void* field_0xe8;
    void* field_0xf0;
    void* field_0xf8;
    void* field_0x100;
    CColor4F color_0x108;
    CColor4F color_0x118;
    f32 field_0x128;
    f32 field_0x12c;
    CColor4F color_0x130;
    CColor4F color_0x140;
    u8 field_0x150;
    u8 pad_0x151[7];
    f32 field_0x158;
    f32 field_0x15c;
    f32 field_0x160;
    f32 field_0x164;
    u64 field_0x168;
    u16 field_0x170;
    u8 field_0x172[2];
    u8 field_0x174[14];
    u8 pad_0x182[2];
    f32 field_0x184;
    u8 field_0x188;
};
static_assert(__builtin_offsetof(UnkUnit2560Element, field_0x184) == 0x184, "layout");
static_assert(__builtin_offsetof(UnkUnit2560Element, color_0x140) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkUnit2560Element, field_0xc0) == 0xc0, "layout");
static_assert(__builtin_offsetof(UnkUnit2560Element, owner) == 0xc8, "layout");

void fn_7100744d90(UnkUnit2560Element* self) {
    self->field_0x20 = -1;
    self->pos[0] = 0.0f;
    self->pos[1] = 0.0f;
    self->pos[2] = 0.0f;
    self->field_0x4c = 0;
    self->field_0x60 = 0.0f;
    self->field_0x68 = 1.0f;
    self->field_0x6c = 0.0f;
    self->field_0x70 = 0.0f;
    self->field_0x24 = 0;
    self->field_0x50 = nullptr;
    self->field_0x58 = nullptr;
    self->field_0x8 = 0;
    self->parent = nullptr;
    fn_710074580c(self->sub);
    self->field_0xd8 = 0;
    fn_7100030728(&self->color_0x118, &self->color_0x108);
    self->field_0x25 = 1;
    self->field_0x28 = 0;
    self->field_0x184 = 1.0f;
    self->field_0x188 = 1;
    if (UnkElementRender_ovb0215* render = self->render) {
        // a char store: the original reloads the render pointer after it
        reinterpret_cast<char*>(render)[0x22] = 0;
        self->render->field_0x23 = 0;
    }
}

void fn_7100744c80(UnkUnit2560Element* self) {
    self->field_0x20 = -1;
    self->field_0x4c = 0;
    self->parent = nullptr;
    self->render = nullptr;
    self->field_0x24 = 0;
    self->field_0x25 = 1;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6d9f0) + 0x10;
    self->field_0x8 = 0;
    self->pos[0] = 0.0f;
    self->pos[1] = 0.0f;
    self->pos[2] = 0.0f;
    self->vel[0] = 0.0f;
    self->vel[1] = 0.0f;
    self->vel[2] = 0.0f;
    self->field_0x60 = 0.0f;
    self->field_0x64 = 1.0f;
    self->field_0x68 = 1.0f;
    self->field_0x50 = nullptr;
    self->field_0x58 = nullptr;
    self->field_0x6c = 1.0f;
    self->field_0x70 = 1.0f;
    fn_71007457cc(self->sub);
    self->field_0xd8 = 0;
    self->field_0xdc = 0;
    self->field_0xe0 = 0;
    self->field_0x150 = 0;
    self->field_0x168 = 0;
    self->field_0x170 = 0;
    self->field_0xe8 = nullptr;
    self->field_0xf0 = nullptr;
    self->field_0xf8 = nullptr;
    self->field_0x100 = nullptr;
    self->color_0x108 = CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    self->color_0x118 = CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    __builtin_memset(&self->field_0x128, 0, 0x28);
    self->field_0x158 = 0.0f;
    self->field_0x15c = 0.0f;
    self->field_0x160 = 0.0f;
    self->field_0x164 = 0.0f;
    __builtin_memset(self->field_0x174, 0, sizeof(self->field_0x174));
    self->field_0x184 = 1.0f;
    self->field_0x188 = 1;
    self->owner = self;
    self->callback = fn_7100745714;
}

void fn_7100745830(void* sub, f32 dt, void* anim, f32 time);
UnkVec3f fn_7100745f80(UnkElementParent_ovb0215* parent);

void fn_71007451d0(UnkUnit2560Element* self, f32 dt) {
    fn_7100745830(self->sub, dt, self->field_0x58, self->field_0x60);
    f32 x, y, a, b;
    if (self->field_0xd8 == 1) {
        x = self->pos[0] + self->offset[0];
        y = self->pos[1] + self->offset[1];
        a = self->pos[2];
        b = self->offset[2];
    } else if (self->parent) {
        UnkVec3f base = fn_7100745f80(self->parent);
        f32 vx = self->pos[0] + self->offset[0];
        f32 vy = self->pos[1] + self->offset[1];
        f32 vz = self->pos[2] + self->offset[2];
        f32 scale = self->parent->scale;
        x = base.x + vx * scale;
        y = vy * scale + base.y;
        a = vz * scale;
        b = base.z;
    } else {
        x = self->pos[0] + self->offset[0];
        y = self->pos[1] + self->offset[1];
        a = self->pos[2];
        b = self->offset[2];
    }
    self->render->pos[0] = x;
    self->render->pos[1] = y;
    self->render->pos[2] = a + b;
}

UnkVec3f fn_71007452d0(UnkUnit2560Element* self) {
    if (self->field_0xd8 == 1)
        return UnkVec3f(self->pos[0] + self->offset[0], self->pos[1] + self->offset[1],
                        self->pos[2] + self->offset[2]);
    if (self->parent) {
        UnkVec3f base = fn_7100745f80(self->parent);
        f32 vx = self->pos[0] + self->offset[0];
        f32 vy = self->pos[1] + self->offset[1];
        f32 vz = self->pos[2] + self->offset[2];
        f32 scale = self->parent->scale;
        return UnkVec3f(base.x + vx * scale, vy * scale + base.y, vz * scale + base.z);
    }
    return UnkVec3f(self->pos[0] + self->offset[0], self->pos[1] + self->offset[1], self->pos[2] + self->offset[2]);
}

struct UnkElementAnim_ovb0215 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    u32 base;   // 0xc
    u32 field_0x10;
    bool loop;  // 0x14
    u8 pad_0x15[3];
    s32 width;   // 0x18
    s32 height;  // 0x1c
    f32 rate;    // 0x20
};
static_assert(__builtin_offsetof(UnkElementAnim_ovb0215, rate) == 0x20, "layout");

typedef bool (*UnkElementDone_ovb0215)(void* object, UnkUnit2560Element* element);

void fn_7100744fb0(UnkUnit2560Element* self, f32 dt) {
    UnkElementAnim_ovb0215* anim = reinterpret_cast<UnkElementAnim_ovb0215*>(self->field_0x58);
    if (!anim)
        return;
    f32 w = (f32)anim->width;
    f32 h = (f32)anim->height;
    f32 sx = self->field_0x6c;
    f32 sy = self->field_0x70;
    bool unit = sx + 0.001f >= 1.0f && sx + -0.001f <= 1.0f && sy + 0.001f >= 1.0f && sy + -0.001f <= 1.0f;
    f32 pw = unit ? w : sx * w;
    f32 ph = unit ? h : sy * h;
    f32 s = self->field_0xb8;
    bool unit2 = (s + -0.001f <= 1.0f) & (s + 0.001f >= 1.0f);
    f32 qw = unit2 ? pw : pw * s;
    f32 qh = unit2 ? ph : ph * s;
    self->render->field_0x22 = (u8)(s32)qw;
    self->render->field_0x23 = (u8)(s32)qh;
    anim = reinterpret_cast<UnkElementAnim_ovb0215*>(self->field_0x58);
    f32 time = self->field_0x60 + anim->rate * dt * self->field_0x64;
    self->field_0x60 = time;
    f32 duration = (f32)anim->count / 30.0f;
    if (anim->loop) {
        if (time > duration) {
            do {
                time -= duration;
            } while (time > duration);
            self->field_0x60 = time;
        }
    } else if (time > duration) {
        self->field_0x60 = duration;
        UnkElementDone_ovb0215 done = reinterpret_cast<UnkElementDone_ovb0215>(self->field_0xf0);
        if (!done || !done(self->field_0xe8, self)) {
            void** next = reinterpret_cast<void**>(self->field_0x50);
            if (next) {
                void** current = reinterpret_cast<void**>(self->field_0x58);
                if (!current || *current != *next) {
                    self->field_0x58 = next;
                    self->field_0x60 = 0.0f;
                }
            }
        }
    }
    anim = reinterpret_cast<UnkElementAnim_ovb0215*>(self->field_0x58);
    u32 index = (u32)(self->field_0x60 * 30.0f);
    self->render->field_0x20 = (index < anim->count ? index : anim->count - 1) + anim->base;
    self->render->field_0x21 = anim->field_0x10;
    while (true) {
        u8 column = self->render->field_0x20;
        if (self->field_0xdc > column)
            break;
        self->render->field_0x20 = column - self->field_0xdc;
        self->render->field_0x21 = self->render->field_0x21 + 1;
        if (!(self->field_0xe0 > self->render->field_0x21))
            self->render->field_0x21 = 0;
    }
}

extern "C" int rand();

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);
UnkMatrix44 fn_7100078e54(f32 x, f32 y, f32 z);

void fn_710074547c(UnkUnit2560Element* self, const UnkVec3f& position, f32 size, f32 low, f32 high, s32 index) {
    if (self->field_0x28 < index) {
        UnkMatrix44_7100078c9c translation = fn_7100078cd8(position);
        UnkMatrix44& move = *reinterpret_cast<UnkMatrix44*>(&translation);
        f32 angle = (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * 0.7853982f + -0.3926991f;
        UnkMatrix44 turn = fn_7100078e54(0.0f, 0.0f, angle);
        UnkCopyMatrix(&move, fn_71000127c8(turn, move));
        self->field_0x158 = size;
        self->field_0x15c = size;
        self->field_0x28 = index;
        self->vel[0] = move.m[3];
        self->vel[1] = move.m[7];
        self->vel[2] = move.m[11];
        f32 lifetime = (high - low) * (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) + low;
        self->field_0x160 = lifetime;
        self->field_0x164 = lifetime / (size * -0.66f);
    }
}

struct UnkElementName_ovb0215 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkElementNamed_ovb0215 {
    UnkElementName_ovb0215* name;
};

void fn_7100744fb0(UnkUnit2560Element* self, f32 dt);
void fn_71007451d0(UnkUnit2560Element* self, f32 dt);

void fn_7100744e4c(UnkUnit2560Element* self, f32 dt) {
    bool special = false;
    if (self->field_0x58)
        special = reinterpret_cast<UnkElementNamed_ovb0215*>(self->field_0x58)->name->hash == 0x7531238cb72381d8ULL;
    bool run = false;
    if (self->field_0x25) {
        run = special;
        if (self->field_0x24) {
            if (UnkElementParent_ovb0215* parent = self->parent)
                run = special | (parent->field_0x24 != 0);
            else
                run = true;
        }
    }
    if (!run) {
        // a char store: the original reloads the render pointer after it
        reinterpret_cast<char*>(self->render)[0x22] = 0;
        self->render->field_0x23 = 0;
        return;
    }
    fn_7100744fb0(self, dt);
    if (self->field_0x12c > 0.0f) {
        f32 remaining = self->field_0x12c - dt;
        self->field_0x12c = remaining;
        if (remaining <= 0.0f) {
            self->field_0x12c = 0.0f;
            const CColor4F* target = &self->color_0x130;
            fn_7100030728(&self->color_0x118, target);
            if (self->field_0x150)
                self->field_0x24 = 0;
        } else {
            f32 duration = self->field_0x128;
            CColor4F c = fn_710003088c((duration - remaining) / duration, self->color_0x140, self->color_0x130);
            fn_7100030728(&self->color_0x118, &c);
        }
    }
    const CColor4F* current = &self->color_0x118;
    fn_7100030728(&self->render->color, current);
    self->render->color.a = self->field_0xc0 * self->render->color.a;
    fn_71007451d0(self, dt);
    u8 a = self->field_0xbc;
    u8 b;
    if (self->parent) {
        b = self->parent->field_0x3c + a;
        a = self->field_0x4c;
    } else {
        b = self->field_0x4c;
    }
    self->render->field_0x24 = b + a;
}
