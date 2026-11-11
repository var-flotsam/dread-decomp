// Unit 252 (0x71001bdeac-0x71001c071c, no reflected class; after src/engine/UnkUnit252/): a model's colour read from
// its shared state (fn_71001bff28, the counterpart of fn_71001bfea4's write); a three-part upload into a slot's buffer
// (fn_71001c02e8, as fn_71001c01fc but with a third list); the creation of a slot's pass object in the scene registry's
// pool (fn_71001bef34, a pooled creator as src/base/UnkUnit204/UnkPooledCreate_b54.cpp's) and the colour copied to
// all slot objects (fn_71001bf068). Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkTU182.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkColor_ovb0092 {
    f32 r, g, b, a;
};

struct UnkLockonModel_ovb0092 {
    u8 pad_0x0[0xe8];
    char* state;  // 0xe8: colours at +0x14c
};

static inline f32 NonNegative(f32 v) {
    return !(v <= 0.0f) ? v : 0.0f;
}

// Unit 252: reads colour `index` of the shared state, through the vector assignment, into `color`.
void fn_71001bff28(UnkLockonModel_ovb0092* model, u32 index, UnkColor_ovb0092* color) {
    UnkVector4D out;
    const u32* src = reinterpret_cast<const u32*>(reinterpret_cast<CVector4D*>(model->state + 0x14c) + index);
    CVector4D v;
    u32* dst = reinterpret_cast<u32*>(&v);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    fn_710007b518(&out, &v);
    v.x = NonNegative(out.x);
    v.y = NonNegative(out.y);
    v.z = NonNegative(out.z);
    v.w = NonNegative(out.w);
    fn_7100030728(reinterpret_cast<CColor4F*>(color), reinterpret_cast<CColor4F*>(&v));
}

struct UnkGfxBuffer252;

struct UnkGfxBufferHolder252 {
    u8 pad_0x0[0x8];
    UnkGfxBuffer252* buffer;
};

struct UnkGfxSlot252 {
    UnkGfxBufferHolder252* holder;
    u8 pad_0x8[0x10];
};

struct UnkGfxSlots252 {
    u8 pad_0x0[0x148];
    UnkGfxSlot252 slots[5];
};

struct UnkGfxData252 {
    const void* data;
    u32 count;
};

bool fn_71001c02e8(UnkGfxSlots252* self, u32 index, const UnkGfxData252* vertices, const UnkGfxData252* records,
                   const UnkGfxData252* extra) {
    if (index > 4)
        return false;
    UnkGfxBufferHolder252* holder = self->slots[index].holder;
    u32 vertex_size = vertices->count * 12;
    u32 record_size = records->count * 16;
    u32 extra_size = extra->count * 8;
    UnkGfxBuffer252* buffer = holder->buffer;
    if (vertex_size + record_size + extra_size > fn_71001622e0(buffer))
        return false;
    fn_7100161d70(buffer);
    fn_7100161e60(buffer, 0, 0, 0, fn_71001622f0(3, 3), vertices->data, vertex_size, 0);
    fn_7100161e60(buffer, 5, 0, vertex_size, fn_71001622f0(4, 3), records->data, record_size, 0);
    fn_7100161e60(buffer, 2, 0, vertex_size + record_size, fn_71001622f0(2, 3), extra->data, extra_size, 0);
    fn_7100161dc0(buffer);
    fn_7100161d60(buffer);
    return false;
}

#include <new>

#include "base/UnkVector.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkPass213Activate;
void fn_710019545c(void* self);
void fn_7100195548(UnkPass213Activate* self, u32 width, u32 height);
extern void* g_unk_716e34c2e0;

struct UnkPoolNode_b54a1 {
    u64 key;
    u64 value;
    UnkPoolNode_b54a1* next;
    UnkPoolNode_b54a1* prev;
    UnkPoolNode_b54a1* chain;

    explicit UnkPoolNode_b54a1(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkPoolNode_b54a1*& Next() { return next; }
    UnkPoolNode_b54a1*& Prev() { return prev; }
};

struct UnkPool252 {
    u64 field_0x0;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;                      // 0x10
    u32 count;                      // 0x14
    UnkPoolNode_b54a1** buckets;    // 0x18
    u32 bucket_count;               // 0x20
    u32 bucket_capacity;            // 0x24
    u8 pad_0x28[0x18];
    UnkPoolNode_b54a1* tail;  // 0x40
    UnkPoolNode_b54a1* head;  // 0x48

    void Insert(u64 key) {
        UnkPoolNode_b54a1** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkPoolNode_b54a1* old = *bucket;
        auto* node = static_cast<UnkPoolNode_b54a1*>(allocator->vf_0x40(sizeof(UnkPoolNode_b54a1), 8, flags));
        if (node)
            new (node) UnkPoolNode_b54a1(key);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

struct UnkPassSlot252 {
    u8 pad_0x0[0x18];
};

struct UnkPassOwner252 {
    u8 pad_0x0[0x140];
    struct {
        UnkPass213Activate* pass;
        u8 pad_0x8[0x10];
    } passes[5];
};

void fn_71001bef34(UnkPassOwner252* self, u32 index, u32 width, u32 height) {
    UnkPool252* pool = static_cast<UnkPool252*>(g_unk_716e34c2e0);
    void* obj = fn_71000839a0(0x40, 0x3c);
    fn_710019545c(obj);
    if (obj)
        pool->Insert(reinterpret_cast<u64>(obj));
    self->passes[index].pass = static_cast<UnkPass213Activate*>(obj);
    fn_7100195548(static_cast<UnkPass213Activate*>(obj), width, height);
}

// Copies the object's colour (the four words at +0x128) into the colour (+0x114) of each of its five slot objects
// and of the models of its array (+0x1b8) when the dirty flag at +0x138 is set.
struct UnkTarget252c {
    u8 pad_0x0[0x114];
    typedef u32 __attribute__((may_alias)) AliasU32;
    AliasU32 color[4];  // 0x114
};

struct UnkSlot252c {
    void* pass;                // 0x0
    UnkTarget252c* target;     // 0x8
    u8 pad_0x10[0x8];
};

struct UnkModelArray252c {
    u8 pad_0x0[0x28];
    u32 count;  // 0x28
    u8 pad_0x2c[0x14];
    UnkTarget252c** items;  // 0x40
};

struct UnkObject252c {
    u8 pad_0x0[0x128];
    u32 color[4];  // 0x128
    bool dirty;    // 0x138
    u8 pad_0x139[0x7];
    UnkSlot252c slots[5];  // 0x140
    UnkModelArray252c* models;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkObject252c, models) == 0x1b8, "UnkObject252c layout");

static inline void SetColor(UnkTarget252c* t, u32 a, u32 b, u32 c, u32 d) {
    t->color[0] = a;
    t->color[1] = b;
    t->color[2] = c;
    t->color[3] = d;
}

void fn_71001bf068(UnkObject252c* self) {
    if (!self->dirty)
        return;
    u32 d = self->color[3];
    u32 c = self->color[2];
    u32 b = self->color[1];
    u32 a = self->color[0];
    for (int i = 0; i < 5; ++i) {
        if (self->slots[i].target)
            SetColor(self->slots[i].target, a, b, c, d);
    }
    if (self->models && self->models->count) {
        u64 count = self->models->count;
        SetColor(self->models->items[0], a, b, c, d);
        for (u64 i = 1; i < count; ++i)
            SetColor(self->models->items[i], a, b, c, d);
    }
}
