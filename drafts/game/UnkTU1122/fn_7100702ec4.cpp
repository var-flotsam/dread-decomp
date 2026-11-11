// Unit 1122 of tools/tu_map.py (0x7100702e38-0x7100703058, no reflected class): the kind-6 handler of
// the unit 1123 family (src/game/UnkUnit1123/UnkUnit1123_b38.cpp): its constructor, whether the
// owner's AI is within two units of the end of the path it follows, and the step that sends the
// AI its effect. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::math::CVector2D;

struct UnkUnit1124Object;
void fn_7100703814(UnkUnit1124Object* self, u32 id);
u32 fn_71010cc3a4(void* ai);
struct UnkEffectSink;
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);
bool fn_71010cab0c(char* ai);
bool fn_710035e53c(char* p0);

extern void* g_unk_7101a6bc28[];  // the kind-6 handler's vtable
extern void* g_unk_7101a25fb0[];  // the kind-6 parameter block's vtable
extern void* g_unk_7101a24db8[];  // the effect block's vtable

template <class T>
struct UnkOpt_ovb0177 {
    T value;
    bool has;
};

struct UnkVec3_ovb0177 {
    u32 x;
    u32 y;
    u32 z;
};

typedef u64 u64_a4_ovb0177 __attribute__((aligned(4)));

struct UnkWords4_ovb0177 {
    u64_a4_ovb0177 lo;
    u64_a4_ovb0177 hi;
};

struct UnkParams6_ovb0177 {
    void** vtable;
    UnkOpt_ovb0177<UnkVec3_ovb0177> field_0x8;
    UnkOpt_ovb0177<u32> field_0x18;
    UnkOpt_ovb0177<UnkWords4_ovb0177> field_0x20;
    u32 kind;  // 0x34
    UnkOpt_ovb0177<u8> tail;  // 0x38
};

struct UnkHandler6_ovb0177 {
    void** vtable;
    u8 pad_0x8[0xa8];
    UnkParams6_ovb0177 params;  // 0xb0
};

void fn_7100702e38(UnkHandler6_ovb0177* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    UnkParams6_ovb0177* params = &self->params;
    params->field_0x18.has = false;
    params->field_0x20.has = false;
    self->vtable = g_unk_7101a6bc28 + 2;
    params->field_0x20.value.lo = 0;
    params->field_0x20.value.hi = 0;
    params->vtable = g_unk_7101a25fb0 + 2;
    __builtin_memset(&params->field_0x8, 0, 13);
    params->tail.has = false;
    params->kind = 6;
}

struct UnkPathPoint_ovb0177 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPathObject_ovb0177 {
    u8 pad_0x0[0x68];
    UnkPathPoint_ovb0177* points;
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 loop;  // 0x94
};

struct UnkTargetInfo_ovb0177 {
    u8 pad_0x0[0x8c];
    u32 x;  // 0x8c
    u32 y;  // 0x90
};

struct UnkHandlerOwner_ovb0177 {
    u8 pad_0x0[0x218];
    u8* ai;  // 0x218
    UnkAnimationComponent* anim;  // 0x220
    u8 pad_0x228[0x18];
    UnkPathObject_ovb0177* path;  // 0x240
};

struct UnkHandlerState_ovb0177 {
    u8 pad_0x0[0x30];
    UnkHandlerOwner_ovb0177* owner;  // 0x30
    u8 pad_0x38[0x20];
    void* callback_arg;  // 0x58
    void (*callback)(void*, UnkHandlerOwner_ovb0177*);  // 0x60
    u8 pad_0x68[0x48];
    UnkParams6_ovb0177 params;  // 0xb0
};
static_assert(__builtin_offsetof(UnkHandlerState_ovb0177, params) == 0xb0, "UnkHandlerState_ovb0177 layout");

bool fn_7100703058(UnkHandlerState_ovb0177* self) {
    u8 atEnd = self->params.tail.has ? self->params.tail.value : 0;
    u8* ai = self->owner->ai;
    u32 kind = fn_71010cc3a4(ai);
    UnkPathObject_ovb0177* path = self->owner->path;
    u32 n = path->count;
    u32 open = (u32)(path->loop == 0) & (u32)(n != 0);
    u32 legs = n - open;
    if (legs == 0)
        return true;
    UnkTargetInfo_ovb0177* info = kind == 1 ? reinterpret_cast<UnkTargetInfo_ovb0177*>(ai + 0x8e8) : nullptr;
    u32 xw = info->x;
    u32 yw = info->y;
    // Keeps the target's coordinates in integer registers across the segment call.
    asm("" : "+r"(xw), "+r"(yw));
    f32 sx, sy;
    if (atEnd) {
        const UnkPathPoint_ovb0177* points = path->points;
        const UnkPathPoint_ovb0177* end = open ? &points[legs] : points;
        UnkSegment2D seg(points[legs - 1].pos, end->pos);
        sx = seg.end.x;
        sy = seg.end.y;
    } else {
        const UnkPathPoint_ovb0177* a = path->points;
        const UnkPathPoint_ovb0177* b = n != 1 ? a + 1 : a;
        UnkSegment2D seg(a->pos, b->pos);
        sx = seg.start.x;
        sy = seg.start.y;
    }
    f32 px, py;
    __builtin_memcpy(&px, &xw, 4);
    __builtin_memcpy(&py, &yw, 4);
    f32 dx = px - sx;
    f32 dy = py - sy;
    if (dx * dx + dy * dy <= 4.0f)
        return true;
    return false;
}

// The block handed to the AI (fn_71010c7e6c), vtable g_unk_7101a24db8.
struct UnkEffectParams_ovb0177 {
    void** vtable;
    f32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u32 field_0x28[3];
    u8 field_0x34;
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 field_0x3c;
    u8 field_0x3d;
    u8 pad_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 pad_0x51;
    u8 field_0x52;
};

void fn_71007038c0(UnkHandlerState_ovb0177* self, f32 v);

void fn_7100702ec4(UnkHandlerState_ovb0177* self, f32 v) {
    u8* ai = self->owner->ai;
    if (!ai)
        return;
    if (fn_71010cc3a4(ai) != 1)
        return;
    if (fn_71010cab0c(reinterpret_cast<char*>(ai)))
        return;
    UnkAnimationComponent* anim = self->owner->anim;
    u32 state = fn_71010cc3a4(ai);
    if (anim && state == 4 && !fn_710117b348(anim, fn_7100c0f988()))
        return;
    u32 a = *reinterpret_cast<u32*>(ai + 0x9dc);
    u32 b = *reinterpret_cast<u32*>(ai + 0x9e0);
    u8 target = ai[0x971];
    u8 atEnd = self->params.tail.has ? self->params.tail.value : 0;
    f32 speed = self->params.field_0x18.has ? *reinterpret_cast<f32*>(&self->params.field_0x18.value) : 0.0f;
    UnkEffectParams_ovb0177 p;
    p.field_0x14 = 0;
    __builtin_memset(p.field_0x18, 0, 13);
    __builtin_memset(p.field_0x28, 0, 13);
    p.vtable = g_unk_7101a24db8 + 2;
    p.field_0x38 = 1;
    p.field_0xc = 1;
    p.field_0x3f = 0;
    p.field_0x41 = 0;
    p.field_0x52 = 0;
    __builtin_memset(p.field_0x44, 0, 13);
    p.field_0x8 = speed;
    p.field_0x3c = 1;
    p.field_0x3d = 1;
    u32 z;
    if (atEnd == target) {
        p.field_0x28[0] = a;
        p.field_0x28[1] = b;
        z = 0;
    } else {
        p.field_0x28[0] = a ^ 0x80000000;
        p.field_0x28[1] = b ^ 0x80000000;
        z = 0x80000000;
    }
    p.field_0x28[2] = z;
    p.field_0x34 = 1;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self->owner->ai), &p, 1);
    UnkPathObject_ovb0177* path = self->owner->path;
    if (path) {
        if (!fn_710035e53c(reinterpret_cast<char*>(path) + 0x68) && fn_7100703058(self)) {
            UnkHandlerOwner_ovb0177* owner = self->owner;
            if (self->callback)
                self->callback(self->callback_arg, owner);
        }
    }
    fn_71007038c0(self, v);
}
