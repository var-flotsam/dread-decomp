// Unit 208 of tools/tu_map.py (0x710016b51c-0x710016e03c): particle emitter objects. Each keeps the
// owner (+0x8) whose scene allocates per-particle arrays on first use. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101a1db18[];
extern char g_unk_7101a1d968[];
extern char g_unk_7101a1da28[];
extern char g_unk_7101a1d9c8[];
extern char g_unk_7101a1d998[];

struct UnkScene_ovb0167 {
    u8 pad_0x0[0x38c];
    u32 count;  // 0x38c
};

// Twelve-byte per-particle entry, zeroed.
struct UnkEntry_ovb0167 {
    u32 words[3] = {};
};

struct UnkOwner_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x10];
    void* entries;  // 0x20
};

static inline void EnsureEntries(UnkOwner_ovb0167* owner) {
    if (!owner->entries) {
        u32 n = owner->scene->count;
        UnkEntry_ovb0167* p = static_cast<UnkEntry_ovb0167*>(fn_7100083ba0((u64)n * sizeof(UnkEntry_ovb0167), 0x1a));
        if (n) {
            UnkEntry_ovb0167* e = p;
            do {
                new (e) UnkEntry_ovb0167;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
}

struct UnkEmitter_ovb0167 {
    char* vtable;
    UnkOwner_ovb0167* owner;  // 0x8
    void* field_0x10;
    void* field_0x18;
};

void fn_710016bdec(UnkEmitter_ovb0167* self, UnkOwner_ovb0167* owner) {
    self->owner = owner;
    self->vtable = g_unk_7101a1db18 + 0x10;
    EnsureEntries(owner);
    self->vtable = g_unk_7101a1d968 + 0x10;
    u8* scene = reinterpret_cast<u8*>(self->owner->scene);
    self->field_0x10 = scene + 0x108;
    self->field_0x18 = scene + 0x1d8;
}

void fn_710016c9a8(UnkEmitter_ovb0167* self, UnkOwner_ovb0167* owner, void* arg) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    EnsureEntries(owner);
    self->field_0x10 = arg;
    self->vtable = g_unk_7101a1da28 + 0x10;
}

struct UnkEmitter2_ovb0167 {
    char* vtable;
    UnkOwner_ovb0167* owner;  // 0x8
    void* field_0x10;
    UnkOwner_ovb0167* field_0x18;
    UnkOwner_ovb0167* field_0x20;
};

void fn_710016c038(UnkEmitter2_ovb0167* self, UnkOwner_ovb0167* owner, void* arg) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    EnsureEntries(owner);
    self->vtable = g_unk_7101a1d998 + 0x10;
    self->field_0x20 = nullptr;
    self->field_0x10 = arg;
    __builtin_memset(&self->field_0x18, 0, 8);
    u8* scene = reinterpret_cast<u8*>(self->owner->scene);
    self->field_0x18 = reinterpret_cast<UnkOwner_ovb0167*>(scene + 0x90);
    EnsureEntries(self->field_0x18);
    u8* scene2 = *reinterpret_cast<u8**>(reinterpret_cast<u8*>(self->owner) + 8);
    self->field_0x20 = reinterpret_cast<UnkOwner_ovb0167*>(scene2 + 0x108);
    EnsureEntries(self->field_0x20);
}

struct UnkEmitter3_ovb0167 {
    char* vtable;
    UnkOwner_ovb0167* owner;  // 0x8
    void* field_0x10;
    void* field_0x18;
    f32 field_0x20;
    bool field_0x24;
};

void fn_710016c3e8(UnkEmitter3_ovb0167* self, UnkOwner_ovb0167* owner, bool flag, f32 value) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    EnsureEntries(owner);
    self->vtable = g_unk_7101a1d9c8 + 0x10;
    self->field_0x20 = value;
    self->field_0x24 = flag;
    u8* scene = reinterpret_cast<u8*>(self->owner->scene);
    self->field_0x10 = scene + 0x68;
    self->field_0x18 = scene + 8;
}

extern char g_unk_7101a1db48[];
extern char g_unk_7101a1da58[];

struct UnkEntry8_ovb0167 {
    u32 words[2] = {};
};

struct UnkOwner8_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x8];
    void* entries;  // 0x18
};

struct UnkGridEmitter_ovb0167 {
    char* vtable;
    UnkOwner8_ovb0167* owner;  // 0x8
    void* field_0x10;
    u32 cell_count;  // 0x18
    u32 columns;     // 0x1c
    f32 origin_x;    // 0x20
    f32 origin_y;    // 0x24
    void* field_0x28;
};

void fn_710016cea4(UnkGridEmitter_ovb0167* self, UnkOwner8_ovb0167* owner, void* arg, u32 cell_count, u32 columns,
                   u32 rows, void* cells) {
    self->vtable = g_unk_7101a1db48 + 0x10;
    self->owner = owner;
    if (!owner->entries) {
        u32 n = owner->scene->count;
        UnkEntry8_ovb0167* p = static_cast<UnkEntry8_ovb0167*>(fn_7100083ba0((u64)n * sizeof(UnkEntry8_ovb0167), 0x1a));
        if (n) {
            UnkEntry8_ovb0167* e = p;
            do {
                new (e) UnkEntry8_ovb0167;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
    self->vtable = g_unk_7101a1da58 + 0x10;
    self->field_0x10 = arg;
    self->cell_count = cell_count;
    self->columns = columns;
    self->field_0x28 = cells;
    self->origin_x = (f32)(columns - 1) * 0.5f;
    self->origin_y = (f32)(rows - 1) * 0.5f;
}

extern char g_unk_7101a1db78[];
extern char g_unk_7101a1da88[];

// A short list of floats the emitter reads: element count, a mode (2 once set), the second and the
// first value and the pointer to the second value.
struct UnkFloatList_ovb0167 {
    u32 count = 0;
    u32 mode = 0x7fffffff;
    f32 second = 0.0f;
    u32 first = 0;
    const f32* rest = nullptr;
};

static inline void SetList(UnkFloatList_ovb0167* list, u32 n, const f32* values) {
    list->count = n;
    list->mode = 2;
    list->second = n ? values[1] : 0.0f;
    list->first = *reinterpret_cast<const u32*>(values++);
    list->rest = n ? values : nullptr;
}

struct UnkOwner4_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x8];
    f32* entries;  // 0x18
};

struct UnkCurveEmitter_ovb0167 {
    char* vtable;
    UnkOwner4_ovb0167* owner;  // 0x8
    void* field_0x10;
    f32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20 = 0;
    UnkFloatList_ovb0167 list;  // 0x28
};

void fn_710016d154(UnkCurveEmitter_ovb0167* self, UnkOwner4_ovb0167* owner, void* arg, u32 count, const f32* values,
                   f32 scale) {
    self->vtable = g_unk_7101a1db78 + 0x10;
    self->owner = owner;
    if (!owner->entries)
        owner->entries = static_cast<f32*>(fn_7100083ba0((u64)owner->scene->count * sizeof(f32), 0x1a));
    self->field_0x10 = arg;
    self->field_0x18 = scale;
    self->vtable = g_unk_7101a1da88 + 0x10;
    new (&self->field_0x20) u32(0);
    new (&self->list) UnkFloatList_ovb0167;
    SetList(&self->list, count, values);
}

extern char g_unk_7101a1dab8[];

struct UnkOwner8b_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x8];
    UnkEntry8_ovb0167* entries;  // 0x18
};

struct UnkCurveEmitter2_ovb0167 {
    char* vtable;
    UnkOwner8b_ovb0167* owner;  // 0x8
    void* field_0x10;
    f32 field_0x18;
    u64 field_0x20 = 0;
    UnkFloatList_ovb0167 list_a;  // 0x28
    UnkFloatList_ovb0167 list_b;  // 0x40
};

void fn_710016d59c(UnkCurveEmitter2_ovb0167* self, UnkOwner8b_ovb0167* owner, void* arg, u32 count_a,
                   const f32* values_a, u32 count_b, const f32* values_b, f32 scale) {
    self->owner = owner;
    self->vtable = g_unk_7101a1db48 + 0x10;
    if (!owner->entries) {
        u32 n = owner->scene->count;
        UnkEntry8_ovb0167* p = static_cast<UnkEntry8_ovb0167*>(fn_7100083ba0((u64)n * sizeof(UnkEntry8_ovb0167), 0x1a));
        if (n) {
            UnkEntry8_ovb0167* e = p;
            do {
                new (e) UnkEntry8_ovb0167;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
    self->field_0x10 = arg;
    self->vtable = g_unk_7101a1dab8 + 0x10;
    new (&self->field_0x20) u64(0);
    new (&self->list_a) UnkFloatList_ovb0167;
    new (&self->list_b) UnkFloatList_ovb0167;
    self->field_0x18 = scale;
    SetList(&self->list_a, count_a, values_a);
    SetList(&self->list_b, count_b, values_b);
}

extern char g_unk_7101a1dae8[];

struct UnkCurveEmitter3_ovb0167 {
    char* vtable;
    UnkOwner_ovb0167* owner;  // 0x8
    void* field_0x10;
    f32 field_0x18;
    u64 field_0x20 = 0;
    u32 field_0x28 = 0;
    UnkFloatList_ovb0167 list_a;  // 0x30
    UnkFloatList_ovb0167 list_b;  // 0x48
    UnkFloatList_ovb0167 list_c;  // 0x60
};

void fn_710016d854(UnkCurveEmitter3_ovb0167* self, UnkOwner_ovb0167* owner, void* arg, u32 count_a,
                   const f32* values_a, u32 count_b, const f32* values_b, u32 count_c, const f32* values_c,
                   f32 scale) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    EnsureEntries(owner);
    self->field_0x10 = arg;
    new (&self->field_0x20) u64(0);
    new (&self->field_0x28) u32(0);
    new (&self->list_a) UnkFloatList_ovb0167;
    new (&self->list_b) UnkFloatList_ovb0167;
    new (&self->list_c) UnkFloatList_ovb0167;
    self->vtable = g_unk_7101a1dae8 + 0x10;
    self->field_0x18 = scale;
    SetList(&self->list_a, count_a, values_a);
    SetList(&self->list_b, count_b, values_b);
    SetList(&self->list_c, count_c, values_c);
}
