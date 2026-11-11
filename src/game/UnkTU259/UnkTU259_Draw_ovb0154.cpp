// Unit 259 of tools/tu_map.py (0x71001c6960-0x71001d948c, the graphics system's unit): functions of
// the large engine object (the graphics system) and of the draw-object table: its draw-pass setters
// and a few resets, the decal resource manager (constructor and the two destructors, four arrays of slots), and the
// per-frame draw-object passes. Names and layouts are placeholders.
#include <new>

#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkTU177.h"
#include "game/UnkTU179.h"
#include "game/UnkTU180.h"
#include "game/UnkTU182.h"
#include "game/UnkTU230.h"
#include "game/UnkTU259.h"
#include "types.h"

struct UnkStruct_71001cdd74;
struct UnkHandle;
struct UnkEntry_71001cdba4;
struct UnkRenderTargetA1;
struct UnkMaskedEntry_b67a1;

extern char g_unk_7101a1e738[];
extern f32 g_unk_7101a1e720[] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1e610[];
extern u32 g_unk_716e554910;
extern u32 g_unk_7101d58e48;
extern u32 g_unk_716e554938;
extern void* g_unk_716e589b58 __attribute__((visibility("hidden")));
extern void* g_unk_716e589b60 __attribute__((visibility("hidden")));

char* fn_71001a07fc(char* p0);
void fn_71001d796c(void* sub, u32 a, u32 b, void* c);
void fn_7100159530();
void fn_7100159810(s32 value);
void fn_7100160910(u32 index, s32 mode, u32 group);
void fn_7100160cf0(u32 index, u32 value, u32 slot);

struct UnkGfxD_b0154;
struct UnkFilterEntry_b0154;
struct UnkOwner_b0154e;
struct UnkEntry_b0154e;
struct UnkGfxJ_b0154;
struct UnkGfxO_b0154;
struct UnkGfxN_b0154;
struct UnkVec4_b0154r;

extern UnkVec4_b0154r g_unk_7101647c5c __attribute__((visibility("hidden")));
extern UnkVec4_b0154r g_unk_7101647c2c __attribute__((visibility("hidden")));
extern UnkHandle* g_unk_716e595ee8;
extern UnkHandle* g_unk_716e595ef0;
extern UnkHandle* g_unk_716e595ed0;
extern UnkHandle* g_unk_716e595ed8;
extern UnkHandle* g_unk_716e595ee0;
extern "C" float powf(float, float);

void fn_71001ce86c(UnkGfxJ_b0154* self, s32 a);
void fn_71001cfe44(UnkGfxJ_b0154* self);
void fn_71001cf9ec(UnkGfxJ_b0154* self);
void fn_71001cf790(UnkGfxO_b0154* self, s32 a);
void fn_71001ce514(UnkGfxO_b0154* self);
u32 fn_7100132f80();
u32 fn_7100132f90();
void fn_710014e6fc(void* p);
void fn_710014dc5c(void* p);
void fn_710014ddbc(void* p);
void fn_710014f074(void* p);
void fn_71001ce148(UnkGfxN_b0154* self);


struct UnkVec4_b0154r {
    f32 v[4];
};

struct UnkGfxA_b0154 {
    u8 pad_0x0[0x317ac40];
    f32 field_0x317ac40;
};

void fn_71001d09d0(UnkGfxA_b0154* self, f32 v) {
    self->field_0x317ac40 = !(v <= 0.1f) ? (!(v >= 10.0f) ? v : 10.0f) : 0.1f;
}


struct UnkPoolOwner_b0154 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* block);
};

struct UnkSlot_b0154 {
    void* ptr;
    u32 count;
    UnkSlot_b0154() : ptr(nullptr), count(0) {}
    ~UnkSlot_b0154() {
        count = 0;
        ptr = nullptr;
    }
};

struct UnkSlotGroup_b0154 {
    UnkSlot_b0154 slot[4];
};

struct UnkPoolSub_b0154 {
    UnkPoolOwner_b0154* owner_a;  // 0x0
    u8 pad_0x8[0x8];
    UnkBlockFreeList list;  // 0x10
    void* buffer_b;         // 0x38
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    bool field_0x4c;
    u8 pad_0x4d[3];
    u32 field_0x50;
    u8 pad_0x54[4];
    UnkPoolOwner_b0154* owner_b;  // 0x58
    u8 pad_0x60[0x8];
    u32 field_0x68;
    bool field_0x6c;
    u8 pad_0x6d[3];
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    u8 pad_0x80[4];
    u32 field_0x84;
    u32 field_0x88;
};

struct UnkDecalResourceMgr {
    void* vtable;  // 0x0
    bool field_0x8;
    u8 pad_0x9[7];
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    UnkPoolSub_b0154 sub;  // 0x38
    UnkSlotGroup_b0154 slots_a[64];  // 0xc8
    UnkSlotGroup_b0154 slots_b[64];  // 0x10c8
    UnkSlotGroup_b0154 slots_c[64];  // 0x20c8
    UnkSlotGroup_b0154 slots_d[64];  // 0x30c8
};


static inline __attribute__((always_inline)) void DestroyGroups(UnkSlotGroup_b0154 (&groups)[64]) {
    for (UnkSlotGroup_b0154* p = groups + 64; p != groups;) {
        --p;
        p->~UnkSlotGroup_b0154();
    }
}

static inline __attribute__((always_inline)) void Destroy(UnkDecalResourceMgr* self) {
    self->vtable = g_unk_7101a1e738 + 0x10;
    fn_71001a07fc((char*)self);
    DestroyGroups(self->slots_d);
    DestroyGroups(self->slots_c);
    DestroyGroups(self->slots_b);
    DestroyGroups(self->slots_a);
    void* buffer = self->sub.list.buffer;
    if (buffer) {
        fn_71000837f4(&self->sub.list);
        UnkPoolOwner_b0154* owner = self->sub.owner_a;
        self->sub.field_0x40 = 0;
        owner->vf_0x48(buffer);
        self->sub.owner_a = nullptr;
    }
    if (self->sub.buffer_b) {
        UnkPoolOwner_b0154* owner = self->sub.owner_b;
        self->sub.field_0x40 = 0;
        owner->vf_0x48(self->sub.buffer_b);
        self->sub.field_0x44 = 0;
    }
}

void fn_71001d76d4(UnkDecalResourceMgr* self) {
    Destroy(self);
}

void fn_71001d7820(UnkDecalResourceMgr* self) {
    Destroy(self);
    __builtin_operator_delete(self);
}


void fn_71001d7548(UnkDecalResourceMgr* self) {
    self->field_0x8 = false;
    self->vtable = g_unk_7101a1e738 + 0x10;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    self->field_0x30 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->sub.owner_a = (UnkPoolOwner_b0154*)fn_7100083990();
    fn_710008377c(&self->sub.list);
    self->sub.field_0x48 = 10;
    self->sub.buffer_b = nullptr;
    self->sub.field_0x40 = 0;
    self->sub.field_0x44 = 0;
    self->sub.field_0x4c = true;
    self->sub.field_0x50 = 15;
    self->sub.owner_b = (UnkPoolOwner_b0154*)fn_7100083990();
    fn_71001d796c(&self->sub, 0x6834, 0x35, nullptr);
    self->sub.field_0x68 = 0;
    self->sub.field_0x70 = 0.0f;
    self->sub.field_0x74 = 2.0f;
    self->sub.field_0x78 = 1.0f;
    self->sub.field_0x7c = 1.0f;
    self->sub.field_0x6c = false;
    self->sub.field_0x84 = 0;
    self->sub.field_0x88 = 0;
    for (UnkSlotGroup_b0154& g : self->slots_a)
        new (&g) UnkSlotGroup_b0154;
    for (UnkSlotGroup_b0154& g : self->slots_b)
        new (&g) UnkSlotGroup_b0154;
    for (UnkSlotGroup_b0154& g : self->slots_c)
        new (&g) UnkSlotGroup_b0154;
    for (UnkSlotGroup_b0154& g : self->slots_d)
        new (&g) UnkSlotGroup_b0154;
}

struct UnkFilterEntry_b0154 {
    u32 kinds;  // 0x0
    u32 flags;  // 0x4
};

struct UnkGfxD_b0154 {
    u8 pad_0x0[0xf8];
    u32 field_0xf8;
    u8 pad_0xfc[0x120 - 0xfc];
    u32 field_0x120;
    u8 pad_0x124[0xa0128 - 0x124];
    u32 field_0xa0128;
    u8 pad_0xa012c[0xb0130 - 0xa012c];
    u32 field_0xb0130;
    u8 pad_0xb0134[0xc0138 - 0xb0134];
    u32 field_0xc0138;
    u8 pad_0xc013c[0xd0140 - 0xc013c];
    u32 field_0xd0140;
    u8 pad_0xd0144[0xe0148 - 0xd0144];
    u32 field_0xe0148;
    u8 pad_0xe014c[0xf0150 - 0xe014c];
    u32 field_0xf0150;
    u8 pad_0xf0154[0x100158 - 0xf0154];
    u32 field_0x100158;
    u8 pad_0x10015c[0x110160 - 0x10015c];
    u32 field_0x110160;
    u8 pad_0x110164[0x1948168 - 0x110164];
    u32 field_0x1948168;
    u8 pad_0x194816c[0x3178170 - 0x194816c];
    u32 field_0x3178170;
    u8 pad_0x3178174[0x317a978 - 0x3178174];
    u32 field_0x317a978;
};


void fn_71001d0b70(UnkGfxD_b0154* self, UnkFilterEntry_b0154** entries, u32 count, u32 kinds, u32 flags) {
    self->field_0xa0128 = 0;
    self->field_0xb0130 = 0;
    self->field_0xc0138 = 0;
    self->field_0xd0140 = 0;
    self->field_0x1948168 = 0;
    self->field_0x3178170 = 0;
    self->field_0x317a978 = 0;
    self->field_0xe0148 = 0;
    self->field_0xf0150 = 0;
    self->field_0x100158 = 0;
    self->field_0x110160 = 0;
    self->field_0xf8 = 0;
    self->field_0x120 = 0;
    for (u64 n = count; n != 0; n--, entries++) {
        UnkFilterEntry_b0154* entry = *entries;
        if ((entry->kinds & kinds) && (entry->flags & flags))
            fn_71001d0d10(self, reinterpret_cast<UnkMaskedEntry_b67a1*>(entry));
    }
}

struct UnkPair_b0154e {
    u32 first;
    u32 second;
};

struct UnkEntry_b0154e {
    u32 count;
    UnkPair_b0154e items[16];
    u64 field_0x88;
};
static_assert(sizeof(UnkEntry_b0154e) == 0x90, "entry size");

struct UnkViewport_b0154e {
    s32 x, y, width, height;
    f32 min_depth, max_depth;
};

struct UnkScissor_b0154e {
    s32 x, y, width, height;
};

struct UnkOwner_b0154e {
    u8 pad_0x0[0x317a9e0];
    base::UnkVector<UnkEntry_b0154e> entries;  // 0x317a9e0
    u8 pad_0x317aXXX[0x317ae20 - 0x317a9e0 - sizeof(base::UnkVector<UnkEntry_b0154e>)];
    u64 field_0x317ae20;
    u8 pad_0x317ae28[0x317af68 - 0x317ae28];
    void* field_0x317af68;
};


void fn_71001d4f84(UnkOwner_b0154e* self, UnkEntry_b0154e* entry, UnkViewport_b0154e* viewport,
                   UnkScissor_b0154e* scissor, void** objects, u32 object_count) {
    if (self->field_0x317ae20)
        fn_710015e1f0(self->field_0x317af68, 0, self->field_0x317ae20, 4, g_unk_7101a1e720);
    fn_71001cdba4(reinterpret_cast<UnkRenderTargetA1*>(self), entry);
    fn_7100159490(viewport->x, viewport->y, viewport->width, viewport->height, viewport->min_depth,
                  viewport->max_depth);
    for (u64 i = 0; i < object_count; i++) {
        void* object = objects[i];
        fn_710015f770(i, object, true);
        fn_71001606f0(i, object != nullptr, true);
    }
    if (scissor->width && scissor->height)
        fn_710015a090(scissor->x, scissor->y, scissor->width, scissor->height);
    if (fn_7100159c80())
        fn_7100159f90(0, -1);
    fn_7100159530();
    base::UnkVector<UnkEntry_b0154e>& v = self->entries;
    u32 n = v.count - 1;
    typedef u32 __attribute__((may_alias)) UnkAliasCount;
    *reinterpret_cast<UnkAliasCount*>(&v.data[(s32)n].count) = 0;
    v.count = n;
    if (n != 0) {
        fn_710015c2d0(reinterpret_cast<UnkEntry_71001cdba4*>(v.data + n - 1));
        return;
    }
    fn_710015c2d0(fn_7100159aa0());
}

struct UnkUniformSet_b0154f {
    u64 location;        // 0x0
    u8 pad_0x8[0x28];
    u64 mode_a;          // 0x30
    u64 mode_b;          // 0x38
    u64 mode_c;          // 0x40
    u8 pad_0x48[0x88];
    void* state;         // 0xd0
};

struct UnkGfxF_b0154 {
    u8 pad_0x0[0x317ae98];
    UnkUniformSet_b0154f uniforms;  // 0x317ae98
    u8 pad_after[0x31836fc - 0x317ae98 - sizeof(UnkUniformSet_b0154f)];
    u32 mode;                       // 0x31836fc
};


void fn_71001d2eec(UnkGfxF_b0154* self, u32 mode, bool force) {
    if (!force && self->mode == mode)
        return;
    self->mode = mode;
    UnkUniformSet_b0154f* u = &self->uniforms;
    if (u->location)
        fn_710015e1f0(u->state, 0, u->location, 1, g_unk_7101a1e610 + mode);
    if (u->mode_a) {
        u32 m = self->mode;
        bool is_0 = m == 0;
        bool is_1 = m == 1;
        bool is_2 = m == 2;
        fn_710015e020(u->state, 0, u->mode_a, is_0);
        fn_710015e020(u->state, 0, u->mode_b, is_1);
        fn_710015e020(u->state, 0, u->mode_c, is_2);
    }
}

struct UnkDrawSub_b0154 {
    u8 pad_0x0[0xa9];
    u8 field_0xa9;
};

struct UnkDrawObject_b0154 {
    u8 pad_0x0[0x20];
    UnkDrawSub_b0154* sub;  // 0x20
    u8 pad_0x28[0x18];
    void (*callback)(UnkDrawObject_b0154* self);  // 0x40
};

struct UnkGfxJ_b0154 {
    u8 pad_0x0[0xb0130];
    u32 field_0xb0130;
    u8 pad_0xb0134[0xc0140 - 0xb0134];
    UnkDrawObject_b0154* objects[0x2000];  // 0xc0140
    u32 object_count;                      // 0xd0140
    u8 pad_0xd0144[0x1948168 - 0xd0144];
    u32 field_0x1948168;
    u8 pad_0x194816c[0x317a998 - 0x194816c];
    u32 field_0x317a998;
    u32 mode;  // 0x317a99c
    u8 pad_0x317a9a0[0x317acb8 - 0x317a9a0];
    u64 field_0x317acb8;
    u64 field_0x317acc0;
    u64 field_0x317acc8;
};


void fn_71001ce704(UnkGfxJ_b0154* self, u32 value) {
    if (g_unk_716e554910 == g_unk_7101d58e48)
        return;
    if (self->field_0xb0130 == 0 && self->field_0x1948168 == 0)
        return;
    self->field_0x317a998 = value;
    self->field_0x317acc0 = 0;
    g_unk_716e554938 = 0;
    fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 0, 0, 0);
    fn_71001ce86c(self, 1);
    self->field_0x317acb8 = 0;
    self->field_0x317acc0 = 0;
    u32 mode = self->mode;
    if (__builtin_expect(mode <= 10 && ((1u << mode) & 0x4b8), 1))
        fn_71001cfe44(self);
    else
        fn_71001cf9ec(self);
    self->field_0x317acb8 = 0;
    self->field_0x317acc0 = 0;
    UnkDrawObject_b0154** p = self->objects;
    for (u32 n = self->object_count; n != 0; n--) {
        UnkDrawObject_b0154* object = *p++;
        if (self->mode == 2 && object->sub && !object->sub->field_0xa9)
            continue;
        object->callback(object);
    }
}

struct UnkGfxN_b0154 {
    u8 pad_0x0[0x317a998];
    u32 mode;  // 0x317a998
};



static inline __attribute__((always_inline)) void BindSlot(u32 slot, void* object) {
    if (object) {
        fn_71001606f0(slot, 1, 1);
        fn_710015f770(slot, object, 1);
        fn_7100160810(slot, 0, 1);
        fn_7100160890(slot, 0, 1);
        fn_7100160910(slot, 0, 1);
        fn_7100160990(slot, 2, 1);
        fn_7100160ab0(slot, 2, 1);
        fn_7100160d50(slot, 2, 1);
        fn_7100160cf0(slot, 0, 1);
    } else {
        fn_71001606f0(slot, 0, 1);
    }
}

void fn_71001ce148(UnkGfxN_b0154* self) {
    u32 mode = self->mode;
    if (mode == 1 || mode == 0x10)
        return;
    BindSlot(8, g_unk_716e589b58);
    BindSlot(10, g_unk_716e589b60);
}

struct UnkDrawThing_b0154o {
    u8 pad_0x0[0x1c];
    u32 kind;  // 0x1c
    u8 pad_0x20[0x10];
    u8 flag;   // 0x30
};

struct UnkDrawHolder_b0154o {
    u8 pad_0x0[0x8];
    UnkDrawThing_b0154o* thing;  // 0x8
};

struct UnkDrawObject_b0154o {
    u32 kind;  // 0x0
    u8 pad_0x4[0x4];
    UnkDrawHolder_b0154o* holder;  // 0x8
    u8 pad_0x10[0x10];
    UnkDrawThing_b0154o* thing;  // 0x20
    u8 pad_0x28[0x18];
    void (*callback)(UnkDrawObject_b0154o* self);  // 0x40
};

struct UnkGfxO_b0154 {
    u8 pad_0x0[0xc0138];
    u32 field_0xc0138;
    u8 pad_0xc013c[0x4];
    UnkDrawObject_b0154o* objects[0x2000];  // 0xc0140
    u32 object_count;                       // 0xd0140
    u8 pad_0xd0144[0x3178170 - 0xd0144];
    u32 field_0x3178170;
    u8 pad_0x3178174[0x317a978 - 0x3178174];
    u32 field_0x317a978;
    u8 pad_0x317a97c[0x317a998 - 0x317a97c];
    u32 field_0x317a998;
    u8 pad_0x317a99c[0x317acc0 - 0x317a99c];
    u64 field_0x317acc0;
};


void fn_71001cf5e4(UnkGfxO_b0154* self, s32 mode) {
    if (g_unk_716e554910 == g_unk_7101d58e48)
        return;
    if (self->field_0xc0138 == 0 && self->field_0x3178170 == 0)
        return;
    self->field_0x317a998 = 0xe;
    fn_71001ce148(reinterpret_cast<UnkGfxN_b0154*>(self));
    self->field_0x317acc0 = 0;
    g_unk_716e554938 = 0;
    fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 0, 0, 0);
    if (mode != 2)
        fn_71001cf790(self, 1);
    UnkDrawObject_b0154o** p = self->objects;
    for (u32 n = self->object_count; n != 0; n--) {
        UnkDrawObject_b0154o* object = *p++;
        if (mode > 0) {
            UnkDrawThing_b0154o* thing = object->kind == 6 ? object->holder->thing : object->thing;
            if (mode == 1 && thing) {
                if ((thing->kind - 0x37) <= 0x5f && thing->flag)
                    continue;
            } else if (mode == 2 && thing) {
                if (!((thing->kind - 0x37) <= 0x5f && thing->flag))
                    continue;
            }
        }
        object->callback(object);
    }
    if (mode != 2 && self->field_0x317a978)
        fn_71001ce514(self);
}

struct UnkViewport_b0154s {
    s32 x, y, width, height;
    f32 min_depth, max_depth;
};

struct UnkGfxS_b0154 {
    u8 pad_0x0[0x5c];
    f32 field_0x5c;
    u8 pad_0x60[0x317af68 - 0x60];
    void* state;    // 0x317af68
    void* program;  // 0x317af70
};



void fn_71001d4be4(UnkGfxS_b0154* self, UnkViewport_b0154s* viewport, f32 r, f32 g, f32 b) {
    void** slots = &self->state;
    u64 location;
    f32 values[4];
    UnkVec4_b0154r color;
    fn_71001aa0b4(slots[1], 1, 0x5b, &location);
    values[0] = 0.0f;
    values[1] = 1.0f / self->field_0x5c;
    values[2] = values[3] = 0.0f;
    if (location)
        fn_710015e1f0(slots[0], 1, location, 4, values);
    if (fn_71001aa0b4(slots[1], 1, 0xd2, &location)) {
        color = g_unk_7101647c5c;
        fn_710015e1f0(slots[0], 1, location, 4, color.v);
    }
    if (fn_71001aa0b4(slots[1], 1, 0x5a, &location)) {
        color.v[0] = powf(r, 2.2f);
        color.v[1] = powf(g, 2.2f);
        color.v[2] = powf(b, 2.2f);
        color.v[3] = 1.0f;
        fn_710015e1f0(slots[0], 1, location, 4, color.v);
    }
    fn_7100159490(viewport->x, viewport->y, viewport->width, viewport->height, viewport->min_depth,
                  viewport->max_depth);
    fn_710015f770(0, nullptr, true);
    fn_71001606f0(0, 0, true);
    if (fn_7100159c80()) {
        fn_7100159800(0);
        fn_7100159f90(0, -1);
        fn_7100159810(0);
    }
    fn_7100159530();
    if (location) {
        values[0] = 1.0f;
        fn_710015e1f0(slots[0], 1, location, 4, values);
    }
}

struct UnkViewport_b0154r {
    s32 x, y, width, height;
    f32 min_depth, max_depth;
};


struct UnkGfxR_b0154 {
    u8 pad_0x0[0x5c];
    f32 field_0x5c;
    u8 pad_0x60[0x317af68 - 0x60];
    void* state;    // 0x317af68
    void* program;  // 0x317af70
};



void fn_71001d4dbc(UnkGfxR_b0154* self, void* object, UnkViewport_b0154r* viewport) {
    void** slots = &self->state;
    u64 location;
    f32 values[4];
    UnkVec4_b0154r color;
    if (fn_71001aa0b4(slots[1], 1, 0x5a, &location)) {
        values[0] = values[1] = values[2] = values[3] = 0.0f;
        fn_710015e1f0(slots[0], 1, location, 4, values);
    }
    fn_71001aa0b4(slots[1], 1, 0x5b, &location);
    values[0] = 0.0f;
    values[1] = 1.0f / self->field_0x5c;
    values[2] = values[3] = 0.0f;
    if (location)
        fn_710015e1f0(slots[0], 1, location, 4, values);
    if (fn_71001aa0b4(slots[1], 1, 0xd2, &location)) {
        color = g_unk_7101647c5c;
        fn_710015e1f0(slots[0], 1, location, 4, color.v);
    }
    if (object == nullptr) {
        if (fn_71001aa0b4(slots[1], 1, 0x5a, &location)) {
            color = g_unk_7101647c2c;
            fn_710015e1f0(slots[0], 1, location, 4, color.v);
        }
    }
    fn_7100159490(viewport->x, viewport->y, viewport->width, viewport->height, viewport->min_depth,
                  viewport->max_depth);
    fn_710015f770(0, object, true);
    fn_71001606f0(0, object != nullptr, true);
    if (fn_7100159c80()) {
        fn_7100159800(0);
        fn_7100159f90(0, -1);
        fn_7100159810(0);
    }
    fn_7100159530();
    if (location) {
        values[0] = 1.0f;
        fn_710015e1f0(slots[0], 1, location, 4, values);
    }
}

struct UnkTargetSet_b0154t {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    UnkGlobal_710b372a98* field_0x8;  // 0x8
    u8 pad_0x10[0xc10 - 0x0];
};

struct __attribute__((packed)) UnkTargetInfo_b0154t {
    u32 flag;
    UnkHandle* handle;
    u8 pad_0xc[0x7c];
    u64 field_0x88;
};

extern UnkTargetInfo_b0154t g_unk_716e595ef8;



void fn_71001d8330(UnkTargetSet_b0154t* self) {
    self->field_0x8 = g_unk_710b372a98;
    self->vf_0x50();
    alignas(8) s32 format_0;
    alignas(8) s32 format_1;
    alignas(8) s32 format_2;
    alignas(8) s32 format_3;
    alignas(8) s32 format_4;
    s32 width = fn_7100132f80();
    s32 height = fn_7100132f90();
    if (!g_unk_716e595ee8) {
        format_0 = 0x12;
        fn_710015c1e0(&g_unk_716e595ee8, width, height, 0xb, format_0);
        g_unk_716e595ef8.handle = g_unk_716e595ee8;
        g_unk_716e595ef8.flag = 1;
        g_unk_716e595ef8.field_0x88 = 0;
    }
    if (!g_unk_716e595ef0) {
        format_1 = 0x12;
        fn_710015c1e0(&g_unk_716e595ef0, width, height, 0xb, format_1);
    }
    if (!g_unk_716e595ed0) {
        format_2 = 0x12;
        fn_710015c1e0(&g_unk_716e595ed0, 0x320, 0x1c2, 0xb, format_2);
    }
    if (!g_unk_716e595ed8) {
        format_3 = 0x12;
        fn_710015c1e0(&g_unk_716e595ed8, 0x320, 0x1c2, 0xb, format_3);
    }
    if (!g_unk_716e595ee0) {
        format_4 = 0x12;
        fn_710015c1e0(&g_unk_716e595ee0, 0x320, 0x1c2, 0xb, format_4);
    }
    fn_710014e6fc((u8*)self + 0x10);
    fn_710014dc5c((u8*)self + 0xc20);
    fn_710014ddbc((u8*)self + 0x1830);
    fn_710014f074((u8*)self + 0x2440);
}
