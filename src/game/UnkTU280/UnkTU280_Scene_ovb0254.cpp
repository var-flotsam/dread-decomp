// Unit 280 (tools/tu_map.py, 0x71001fb9ac-0x71001fe698): the scene manager (src/engine/scene/CSceneManager.cpp) and
// its entry arrays: show the item with id 3 (fn_71001fbde0), destroy everything (fn_71001fc440), the first
// element of an item set handed on (fn_71001fd07c, fn_71001fd1b4), and the entry array's copy assignment and
// PushBack (fn_71001fdacc, fn_71001fe390). Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkSceneManager;
extern UnkSceneManager* g_unk_716e598008 __attribute__((visibility("hidden")));

struct UnkItem {
    s32 id;
};

struct UnkSceneItem {
    s32 id;
};

struct UnkPtrArray_ovb0254 {
    UnkItem** data;
    u32 count;
};

struct UnkSceneMgr_ovb0254 {
    UnkPtrArray_ovb0254 items;
    u8 unk_0x10[0x18];
    u8 unk_0x28[0x18];
    void (*field_0x40)();
};

struct UnkItemTarget_ovb0254 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
};

struct UnkItemOwner_ovb0254 {
    u8 unk_0x0[8];
    UnkItemTarget_ovb0254* field_0x8;
};

bool fn_7100150e70();
void fn_7100150c50(f32 a, f32 b, f32 c, f32 d, f32 e);
void* fn_7100202574(UnkItem* item);
void fn_710020014c(UnkItem* item, bool value);
void fn_7100200138(UnkItem* item, bool value);
void fn_7100200160(UnkSceneItem* item, f32 dt);
void fn_71002003ec(UnkItem* item);

__attribute__((always_inline)) static inline UnkItem* Find_ovb0254(const UnkPtrArray_ovb0254* items, s32 id) {
    for (UnkItem **it = items->data, **end = it + items->count; it != end; ++it) {
        if ((*it)->id == id)
            return *it;
    }
    return nullptr;
}

bool fn_71001fbde0() {
    if (!g_unk_716e598008)
        return false;
    if (!fn_7100150e70())
        return false;
    UnkSceneMgr_ovb0254* mgr = reinterpret_cast<UnkSceneMgr_ovb0254*>(g_unk_716e598008);
    if (mgr->field_0x40)
        mgr->field_0x40();
    fn_7100150c50(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    mgr = reinterpret_cast<UnkSceneMgr_ovb0254*>(g_unk_716e598008);
    UnkItem* item = Find_ovb0254(&mgr->items, 3);
    if (!item)
        return false;
    UnkItemOwner_ovb0254* owner = static_cast<UnkItemOwner_ovb0254*>(fn_7100202574(item));
    if (!owner)
        return false;
    owner->field_0x8->vf_0x80();
    fn_710020014c(item, true);
    fn_7100200138(item, true);
    fn_7100200160(reinterpret_cast<UnkSceneItem*>(item), 1.0f / 30.0f);
    fn_71002003ec(item);
    fn_710020014c(item, false);
    fn_7100200138(item, false);
    return true;
}

struct UnkClone_fn_71001fc54c;
using UnkSceneSlot = UnkCloneElem<UnkClone_fn_71001fc54c, 8>;
extern template UnkSceneSlot& base::UnkVector<UnkSceneSlot>::GrowAt(u32);


struct UnkSceneDrawer_ovb0254 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkSceneLayerEntry_ovb0254 {
    u8 unk_0x0[0x28];
};

struct UnkSceneManager_ovb0254 {
    base::UnkVector<UnkSceneSlot> items;
    UnkSceneDrawer_ovb0254* field_0x28;
    u8 unk_0x30[0x18];
    base::UnkVector<UnkSceneLayerEntry_ovb0254> layers[100];  // 0x48
};
static_assert(sizeof(base::UnkVector<UnkSceneLayerEntry_ovb0254>) == 0x28, "layer size");

void fn_71001ffe08(UnkSceneItem* item);

// Destroys every item and the drawer and frees the arrays.
void fn_71001fc440(UnkSceneManager_ovb0254* self) {
    u32 n = self->items.count;
    for (u32 i = 0; i != n; ++i) {
        if (*reinterpret_cast<UnkSceneItem**>(self->items.GrowAt(i).data)) {
            UnkSceneItem* item = *reinterpret_cast<UnkSceneItem**>(self->items.GrowAt(i).data);
            if (item) {
                fn_71001ffe08(item);
                fn_7100083c00(item);
            }
            *reinterpret_cast<UnkSceneItem**>(self->items.GrowAt(i).data) = nullptr;
        }
    }
    self->items.count = 0;
    if (self->field_0x28) {
        self->field_0x28->vf_0x8();
        self->field_0x28 = nullptr;
    }
    for (u32 i = 100; i-- > 0;)
        self->layers[i].Free();
    self->items.Free();
}

void fn_7100202d00(UnkItem* item, void* arg, bool value);
void fn_7100202d14(UnkItem* item, void* arg1, void* arg2);

// A set of item pointers whose first element is created on demand.
struct UnkItemSet_ovb0254 {
    base::UnkVector<UnkItem*> items;
};

__attribute__((flatten)) void fn_71001fd07c(UnkItemSet_ovb0254* self, void* arg, bool value) {
    fn_7100202d00(self->items.GrowAt(0), arg, value);
}

__attribute__((flatten)) void fn_71001fd1b4(UnkItemSet_ovb0254* self, void* arg1, void* arg2) {
    fn_7100202d14(self->items.GrowAt(0), arg1, arg2);
}

struct UnkSceneEntry;

// An entry (0x28 bytes) copied field by field.
typedef u64 __attribute__((may_alias)) UnkAliasU64_ovb0254;
typedef u32 __attribute__((may_alias)) UnkAliasU32_ovb0254;
struct UnkEntry_ovb0254 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    bool field_0x24;

    UnkEntry_ovb0254() : field_0x0(0), field_0x8(-1), field_0xc(0), field_0x10(0), field_0x14(0), field_0x18(0), field_0x1c(0), field_0x20(0x3f800000), field_0x24(false) {}

    __attribute__((always_inline)) void Copy(const UnkEntry_ovb0254& o) {
        char* d = reinterpret_cast<char*>(this);
        u64 v0 = o.field_0x0;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x8) = o.field_0x8;
        *reinterpret_cast<UnkAliasU64_ovb0254*>(d + 0x0) = v0;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0xc) = o.field_0xc;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x10) = o.field_0x10;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x14) = o.field_0x14;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x18) = o.field_0x18;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x1c) = o.field_0x1c;
        *reinterpret_cast<UnkAliasU32_ovb0254*>(d + 0x20) = o.field_0x20;
        *reinterpret_cast<u8*>(d + 0x24) = o.field_0x24;
    }
    UnkEntry_ovb0254(const UnkEntry_ovb0254& o) { Copy(o); }
    UnkEntry_ovb0254(UnkEntry_ovb0254&& o) { Copy(o); }
};
static_assert(sizeof(UnkEntry_ovb0254) == 0x28, "entry layout");

using EntryVec_ovb0254 = base::UnkVector<UnkEntry_ovb0254>;

void fn_71001fe390(base::UnkVector<UnkSceneEntry>* self_, const UnkSceneEntry* entry_) {
    EntryVec_ovb0254* self = reinterpret_cast<EntryVec_ovb0254*>(self_);
    const UnkEntry_ovb0254* entry = reinterpret_cast<const UnkEntry_ovb0254*>(entry_);
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkEntry_ovb0254* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkEntry_ovb0254*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_ovb0254), 8, self->flags));
        if (self->data) {
            UnkEntry_ovb0254* src = self->data;
            UnkEntry_ovb0254* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                asm("" : "+r"(dst));
                new (dst) UnkEntry_ovb0254(static_cast<UnkEntry_ovb0254&&>(*src));
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    UnkEntry_ovb0254* slot = &p[n];
    // keeps the element address computed before the copy's loads
    asm("" : "+r"(slot));
    new (slot) UnkEntry_ovb0254(*entry);
    ++self->count;
}

// Copy-assigns an entry array: reserve the source's capacity, then append its entries.
base::UnkVector<UnkSceneEntry>* fn_71001fdacc(base::UnkVector<UnkSceneEntry>* self_,
                                              const base::UnkVector<UnkSceneEntry>* other_) {
    EntryVec_ovb0254* self = reinterpret_cast<EntryVec_ovb0254*>(self_);
    const EntryVec_ovb0254* other = reinterpret_cast<const EntryVec_ovb0254*>(other_);
    if (self != other) {
        self->count = 0;
        u32 cap = other->capacity;
        if (self->capacity < cap) {
            UnkEntry_ovb0254* p = static_cast<UnkEntry_ovb0254*>(
                self->allocator->vf_0x40((u64)cap * sizeof(UnkEntry_ovb0254), 8, self->flags));
            if (self->data) {
                UnkEntry_ovb0254* src = self->data;
                UnkEntry_ovb0254* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    asm("" : "+r"(dst));
                    new (dst) UnkEntry_ovb0254(static_cast<UnkEntry_ovb0254&&>(*src));
                }
                self->allocator->vf_0x48(self->data);
            }
            self->data = p;
            self->capacity = cap;
        }
        for (const UnkEntry_ovb0254 *it = other->data, *end = it + other->count; it != end; ++it)
            fn_71001fe390(self_, reinterpret_cast<const UnkSceneEntry*>(it));
    }
    return self_;
}

