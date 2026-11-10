// Unit 322 (no reliable name): a player's media object creation and the widget it adds for a video. Names and layouts are placeholders.
#include <new>
#include "base/UnkVector.h"
#include "types.h"
#include "base/UnkPairArrays.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "engine/UnkGraphicsSystem.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkRenderManager.h"

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

static inline void RefAdd(UnkRefObj* o) { __atomic_fetch_add(&o->ref_count, 1, __ATOMIC_SEQ_CST); }
static inline void RefRelease(UnkRefObj* o) {
    __atomic_fetch_sub(&o->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&o->ref_count, __ATOMIC_SEQ_CST) == 0)
        o->vf_0x10();
}

struct UnkRef {
    UnkRefObj* ptr;
    UnkRef() : ptr(nullptr) {}
    UnkRef(const UnkRef& o) : ptr(o.ptr) {
        if (ptr)
            RefAdd(ptr);
    }
    ~UnkRef() {
        if (ptr)
            RefRelease(ptr);
    }
    UnkRef& operator=(const UnkRef& o) {
        UnkRefObj* p = o.ptr;
        if (p)
            RefAdd(p);
        if (ptr)
            RefRelease(ptr);
        ptr = p;
        return *this;
    }
};

struct UnkPlayer;
struct UnkPathKey {
    u8 data[0x10];
};

extern void* g_unk_7101a20470[];
extern void* g_unk_7101a20440[];

void fn_710021e6e8(void* obj);
void fn_71002206fc(void* obj);
void fn_710021d394();
void fn_710021ff8c(void* obj, void* pair);
void fn_710021ecb4(void* obj, const UnkPathKey& path, u32 a, bool b);
void fn_710021d238(base::UnkVector<UnkRef>* vec, u32 n);

struct UnkCab8Obj_b0246 : UnkRefObj {
    u8 pad_0xc[0xacc];
    u64 field_0xad8;
    u8 field_0xae0[0x40];
};

struct UnkCab8Player_b0246 {
    u8 pad_0x0[0x18];
    base::UnkVector<UnkRef> list;  // 0x18
};

struct UnkCab8Pair_b0246 {
    UnkCab8Player_b0246* player;
    void (*callback)();
};

UnkRef fn_710021cab8(UnkPlayer* player, const UnkPathKey& path, u32 a, u32 b) {
    UnkCab8Player_b0246* self = reinterpret_cast<UnkCab8Player_b0246*>(player);
    void* mem = operator new(0xb20);
    __builtin_memset(mem, 0, 0xb20);
    fn_710021e6e8(mem);
    *reinterpret_cast<void**>(mem) = &g_unk_7101a20470[2];
    UnkCab8Obj_b0246* obj = reinterpret_cast<UnkCab8Obj_b0246*>(mem);
    obj->field_0xad8 = 0;
    fn_71002206fc(obj->field_0xae0);
    *reinterpret_cast<void**>(mem) = &g_unk_7101a20440[2];
    {
        base::UnkVector<UnkRef>& list = self->list;
        UnkRef local;
        local.ptr = obj;
        RefAdd(obj);
        u32 n = list.count;
        u32 cap = list.capacity;
        u32 new_cap = cap;
        if (cap <= n && list.step != 0)
            new_cap = (n / list.step + 1) * list.step;
        if ((s32)n >= (s32)new_cap)
            for (;;) {
            }
        fn_710021d238(&list, new_cap);
        UnkRef* slot = &list.data[list.count];
        slot->ptr = nullptr;
        *slot = local;
        ++list.count;
    }
    UnkCab8Pair_b0246 pair = {self, fn_710021d394};
    fn_710021ff8c(obj, &pair);
    fn_710021ecb4(obj, path, a, b & 1);
    UnkRef result;
    UnkRef tmp;
    tmp.ptr = obj;
    result = tmp;
    tmp.ptr = nullptr;
    return result;
}

using base::global::CStrId;

struct UnkWidget;
struct UnkRender;
struct UnkStruct_710020a5a0;
struct UnkMinimapVec3;
struct UnkBlendLayer_710019c284;

void fn_710021d3d4(void* a, void* b, UnkWidget** out, const CStrId* id);
void fn_71001c3504(UnkRender* render, void* p);
void* fn_71001d086c(UnkGraphicsSystem* self, const CStrId* key);
void fn_71001606c0(void* obj, u32* a, u32* b);
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
void fn_710020a4c4(void* p, const UnkMinimapVec3& pos);
void fn_710019c284(UnkBlendLayer_710019c284* self);
void fn_710019c564(void* target, f32 a, f32 b, f32 c, f32 d);
void fn_710020ad90(void* self, s32 value);
void fn_710021d58c(base::UnkPairArrays* self, u32 n);

struct UnkCd80Render_b0246 {
    u8 pad_0x0[0x78];
    u8* field_0x78;
    u8 pad_0x80[8];
    union {
        u32 field_0x88;
    };
};

struct UnkCd80Widget_b0246 {
    u8 pad_0x0[0x18];
    u8 field_0x18[0x160];
    UnkCd80Render_b0246* render;  // 0x178
};

struct UnkCd80Res_b0246 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

struct UnkCd80Ref_b0246 {
    UnkCd80Res_b0246* ptr;
};

struct UnkCd80Player_b0246 {
    u8 pad_0x0[0x40];
    base::UnkPairArrays widgets;  // 0x40
};

struct UnkCd80Zero_b0246 {
    u64 a;
    u32 b;

    UnkCd80Zero_b0246() : a(0), b(0) {}
};


static inline __attribute__((always_inline)) UnkCd80Widget_b0246* CreateWidget_b0246(UnkRenderManager* manager) {
    UnkWidget* created = nullptr;
    fn_710021d3d4(manager->field_0x218, nullptr, &created, &g_unk_7101d470e0);
    fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(created), 2);
    return reinterpret_cast<UnkCd80Widget_b0246*>(created);
}

static inline __attribute__((always_inline)) void SetScale_b0246(void* node, f32 y) {
    base::math::CVector2D scale;
    scale.x = 1.0f;
    scale.y = y;
    fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(node), &scale);
}

static inline __attribute__((always_inline)) void SetOrigin_b0246(void* node) {
    UnkCd80Zero_b0246 zero;
    fn_710020a4c4(node, *reinterpret_cast<const UnkMinimapVec3*>(&zero));
}

void fn_710021cd80(UnkCd80Player_b0246* self, UnkCd80Ref_b0246* ref) {
    UnkCd80Widget_b0246* widget = CreateWidget_b0246(g_unk_7101d47300);
    void* node = widget->field_0x18;
    fn_710020ac34(reinterpret_cast<UnkToggle*>(node), true);
    fn_71001c3504(reinterpret_cast<UnkRender*>(widget->render), &ref->ptr->name);
    void* entry = fn_71001d086c(reinterpret_cast<UnkGraphicsSystem*>(g_unk_710b372a98), &ref->ptr->name);
    u32 height, width;
    fn_71001606c0(entry, &height, &width);
    SetScale_b0246(node, (f32)width / (f32)height);
    SetOrigin_b0246(node);
    u8* layer = widget->render->field_0x78;
    layer[0x181] = 0;
    fn_710019c284(reinterpret_cast<UnkBlendLayer_710019c284*>(layer));
    widget->render->field_0x88 = 0;
    fn_710019c564(widget->render->field_0x78, 1.0f, 1.0f, 1.0f, 1.0f);
    fn_710020ad90(node, -30000);
    UnkCd80Res_b0246* res = ref->ptr;
    if (self->widgets.count == self->widgets.capacity)
        fn_710021d58c(&self->widgets, self->widgets.step + self->widgets.count);
    u64* values = self->widgets.values;
    base::global::UnkStringData* name = res->name.data;
    __builtin_memcpy(&values[self->widgets.count], &name, 8);
    if (&g_unk_7101d47108->empty != name)
        ++name->ref_count;
    u64* keys = self->widgets.keys;
    __builtin_memcpy(&keys[self->widgets.count], &widget, 8);
    self->widgets.count++;
}
