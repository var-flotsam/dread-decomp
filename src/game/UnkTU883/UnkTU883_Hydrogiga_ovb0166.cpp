// CHydrogigaAIComponent's unit (tools/tu_map.py, no reliable name of its own): the destructor of a keyed
// table entry and the table's reallocation (the table is two parallel arrays: 0x58-byte entries holding
// six string ids, and the string ids that name them; fn_71005c4630 is base::UnkCloneKeyedArray::Reserve's
// twin, base/UnkCloneHelpers.h); moving an entry to the front of the other object's list (+0x28) once the
// component's flag (+0x149b9) is set (as src/game/CQuetzoaXAIComponent/CQuetzoaXAIComponent_List_ovb0134.cpp);
// the number of its two parts whose value at +0x4bc has reached the one at +0x4b0; and applying a colour to
// every slot of an actor's shape through the material table (fn_7100926874,
// src/game/CMaterialFXComponent/fn_7100926874.cpp). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// ---- the keyed table ----

struct UnkHydrogigaEntry_b0166 {
    u32 field_0x0;
    CStrId field_0x8;
    u8 unk_0x10[0x20];
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
};
static_assert(sizeof(UnkHydrogigaEntry_b0166) == 0x58, "layout");

struct UnkHydrogigaTable_b0166 {
    UnkHydrogigaEntry_b0166* values;
    base::UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

__attribute__((noinline)) void fn_71005bdf1c(UnkHydrogigaEntry_b0166* self) {
    self->field_0x50.Reset();
    self->field_0x48.Reset();
    self->field_0x40.Reset();
    self->field_0x38.Reset();
    self->field_0x30.Reset();
    self->field_0x8.Reset();
}

// ---- the component ----

struct UnkHydrogigaListTag_b0166;
using UnkHydrogigaList_b0166 = base::UnkSList<UnkHydrogigaListTag_b0166>;

struct UnkHydrogigaOther_b0166 {
    u8 pad_0x0[0x28];
    UnkHydrogigaList_b0166 list;  // 0x28
};

// What a part's object at +0x10 holds: an id and the two values compared by fn_71005c0f2c.
struct UnkHydrogigaObject_b0166 {
    u8 pad_0x0[0x18];
    u64 id;  // 0x18
    u8 pad_0x20[0x490];
    f32 field_0x4b0;
    u8 pad_0x4b4[8];
    f32 field_0x4bc;
};

struct UnkHydrogigaPart_b0166 {
    u8 pad_0x0[0x10];
    UnkHydrogigaObject_b0166* object;
};

struct UnkHydrogigaAI_b0166 {
    u8 pad_0x0[0x149a8];
    UnkHydrogigaPart_b0166* parts[2];  // 0x149a8
    u8 pad_0x149b8[1];
    bool pending;  // 0x149b9
    u8 pad_0x149ba[0x4e00 - 0x49ba];
    u8 entry[0x8];  // 0x14e00
};
static_assert(__builtin_offsetof(UnkHydrogigaAI_b0166, entry) == 0x14e00, "layout");

// Declared with the Quetzoa component's types in src/game/CQuetzoaXAIComponent/CQuetzoaXAIComponent_List_ovb0134.cpp.
struct UnkQuetzoaXAI_ovb0134;
struct UnkQuetzoaOther_ovb0134;
void fn_710110bb4c(UnkQuetzoaXAI_ovb0134* self, UnkQuetzoaOther_ovb0134* other, void* arg, bool flag);

static inline s32 IndexOf(UnkHydrogigaList_b0166* list, u64 v) {
    u32 i = 0;
    for (UnkHydrogigaList_b0166::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkHydrogigaList_b0166::Node* Find(UnkHydrogigaList_b0166* list, u64 v) {
    for (UnkHydrogigaList_b0166::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkHydrogigaList_b0166* list, UnkHydrogigaList_b0166::Node* pos) {
    UnkHydrogigaList_b0166::Node* prev = nullptr;
    for (UnkHydrogigaList_b0166::Node* it = list->head; it;) {
        UnkHydrogigaList_b0166::Node* next = it->next;
        if (it == pos) {
            if (prev) {
                prev->next = next;
                if (list->tail == pos)
                    list->tail = prev;
            } else {
                list->head = next;
            }
            if (--list->count == 0) {
                list->head = nullptr;
                list->tail = nullptr;
            }
            list->allocator->vf_0x48(pos);
            return;
        }
        prev = it;
        it = next;
    }
}

void fn_71005c0d10(UnkHydrogigaAI_b0166* self, UnkHydrogigaOther_b0166* other, void* arg, bool flag) {
    fn_710110bb4c(reinterpret_cast<UnkQuetzoaXAI_ovb0134*>(self), reinterpret_cast<UnkQuetzoaOther_ovb0134*>(other), arg, flag);
    if (!self->pending)
        return;
    u64 v = reinterpret_cast<u64>(self->entry);
    if (IndexOf(&other->list, v) < 0)
        return;
    if (UnkHydrogigaList_b0166::Node* n = Find(&other->list, v))
        Erase(&other->list, n);
    other->list.PushFront(v);
    self->pending = false;
}

s32 fn_71005c0f2c(UnkHydrogigaAI_b0166* self) {
    UnkHydrogigaObject_b0166* a = self->parts[0]->object;
    UnkHydrogigaObject_b0166* b = self->parts[1]->object;
    s32 n = a->field_0x4bc >= a->field_0x4b0;
    return b->field_0x4bc >= b->field_0x4b0 ? n + 1 : n;
}

// ---- the colour of a shape's slots ----

struct UnkTable;
struct UnkKey;
struct UnkVec4;
struct UnkPolygon2D;
void fn_7100119b88(UnkPolygon2D* self);
char* fn_71005c1008(u64 id);
void fn_7100926874(UnkTable* self, const UnkKey& key, u32 id, const UnkVec4& a, const UnkVec4& b, u32 kind, u32 value,
                   f32 weight);

// The key as fn_7100926874.cpp's, with its id released in two steps.
struct UnkMaterialKey_b0166 {
    u64 field_0x0;
    union {
        CStrId field_0x8;
    };
    u32 field_0x10;
    u32 field_0x14;
    UnkMaterialKey_b0166(u32 index) : field_0x0(0), field_0x8(g_unk_7101d470e0), field_0x10(index), field_0x14(1) {}
    ~UnkMaterialKey_b0166() { field_0x8.Reset(); }
};

struct UnkShapeList_b0166 {
    u8 pad_0x0[0x2c];
    u32 count;
};

struct UnkShapeHolder_b0166 {
    u8 pad_0x0[0x78];
    UnkShapeList_b0166* list;
};

struct UnkShape_b0166 {
    u8 pad_0x0[0x138];
    UnkShapeHolder_b0166* holder;
};

struct UnkActorSub_b0166 {
    u8 pad_0x0[0x50];
    UnkShape_b0166* shape;
};

struct UnkActorWith_b0166 {
    u8 pad_0x0[0x228];
    UnkActorSub_b0166* field_0x228;
    u8 pad_0x230[0x58];
    UnkTable* field_0x288;
};

struct UnkFoundObject_b0166 {
    u8 pad_0x0[0x2d];
    bool field_0x2d;
    u8 pad_0x2e[0xe];
    f32 field_0x3c;
};

struct UnkFound_b0166 {
    u8 pad_0x0[0x48];
    UnkFoundObject_b0166* object;
};

void fn_71005c18dc(UnkHydrogigaAI_b0166* self, UnkActorWith_b0166* actor) {
    UnkActorSub_b0166* sub = actor->field_0x228;
    UnkTable* table = actor->field_0x288;
    if (!sub || !table)
        return;
    UnkShape_b0166* shape = sub->shape;
    if (!shape)
        return;
    auto* found = reinterpret_cast<UnkFound_b0166*>(fn_71005c1008(self->parts[0]->object->id));
    UnkFoundObject_b0166* object = found->object;
    if (!object)
        return;
    if (object->field_0x2d)
        fn_7100119b88(reinterpret_cast<UnkPolygon2D*>(object));
    f32 red = object->field_0x3c;
    base::color::CColor4F color(red <= 0.0f ? 0.0f : red, 0.0f, 0.0f, 0.0f);
    u32 count = shape->holder->list->count;
    for (u32 i = 0; i != count; ++i) {
        UnkMaterialKey_b0166 key(i);
        base::math::UnkVector4D a = fn_7100030878(&color);
        base::math::UnkVector4D b = fn_7100030878(&color);
        fn_7100926874(table, reinterpret_cast<const UnkKey&>(key), 2, reinterpret_cast<const UnkVec4&>(a),
                      reinterpret_cast<const UnkVec4&>(b), 1, 0, 0.0f);
    }
}

// ---- the table's reallocation ----

void fn_71005c4630(UnkHydrogigaTable_b0166* self, u32 n) {
    if (self->capacity >= n)
        return;
    auto* v = static_cast<UnkHydrogigaEntry_b0166*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkHydrogigaEntry_b0166), 8, self->flags));
    auto* k = static_cast<base::UnkStrIdMember*>(self->allocator->vf_0x40((u64)n * 8, 8, self->flags));
    if (self->values) {
        UnkHydrogigaEntry_b0166* src = self->values;
        UnkHydrogigaEntry_b0166* dst = v;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            union Slot {
                base::global::UnkStringData* p;
            };
            dst->field_0x0 = src->field_0x0;
            dst->field_0x8.data = src->field_0x8.data;
            reinterpret_cast<Slot*>(&src->field_0x8.data)->p = &g_unk_7101d47108->empty;
            __builtin_memcpy(dst->unk_0x10, src->unk_0x10, sizeof(dst->unk_0x10));
            dst->field_0x30.data = src->field_0x30.data;
            reinterpret_cast<Slot*>(&src->field_0x30.data)->p = &g_unk_7101d47108->empty;
            dst->field_0x38.data = src->field_0x38.data;
            reinterpret_cast<Slot*>(&src->field_0x38.data)->p = &g_unk_7101d47108->empty;
            dst->field_0x40.data = src->field_0x40.data;
            reinterpret_cast<Slot*>(&src->field_0x40.data)->p = &g_unk_7101d47108->empty;
            dst->field_0x48.data = src->field_0x48.data;
            reinterpret_cast<Slot*>(&src->field_0x48.data)->p = &g_unk_7101d47108->empty;
            dst->field_0x50.data = src->field_0x50.data;
            reinterpret_cast<Slot*>(&src->field_0x50.data)->p = &g_unk_7101d47108->empty;
            fn_71005bdf1c(src);
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        base::UnkStrIdMember* src = self->keys;
        base::UnkStrIdMember* dst = k;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            union Slot {
                base::global::UnkStringData* p;
            };
            dst->data = src->data;
            reinterpret_cast<Slot*>(&src->data)->p = &g_unk_7101d47108->empty;
            src->~UnkStrIdMemberOf();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = v;
    self->keys = k;
    self->capacity = n;
}
