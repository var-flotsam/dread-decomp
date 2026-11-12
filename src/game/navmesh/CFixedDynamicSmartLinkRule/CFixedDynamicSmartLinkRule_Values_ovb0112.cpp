// navmesh::CFixedDynamicSmartLinkRule's unit (tools/tu_map.py unit 354): the link kind a name gives
// (fn_710026f548); the rule's application to a cell (fn_710026f690: unless the filter at +0x18 masks the
// cell's kind (fn_71002b009c) to something other than +0x74, fn_710026f790 runs one way, and the other
// way too when +0x81 is set); the rule's reading from a file (fn_710026ebe8: the link kind by name, the
// two cell kinds by name (fn_710026d2e4), flags and the string id at +0x88, then fn_710026ee04); and the
// 0x50-byte values of UnkNavRuleValues_A1_b75.h: the move construction of a pair held in place or by
// reference (fn_7100272cc4, the shape of fn_7100272bbc.cpp), a value's copy constructor with the array's
// copy inlined (fn_7100272598), and inserting at an index by copy (fn_71002737b0) or move
// (fn_7100273940): appending at the end, else moving the tail up one and constructing the value in the
// hole. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "UnkNavRuleValues_A1_b75.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU367.h"
#include "game/UnkTU56.h"
#include "types.h"

u32 fn_710026d2e4(const char*);

s32 fn_710026f548(const char* name) {
    if (strcmp(name, "None") == 0)
        return 0;
    if (strcmp(name, "Solid") == 0)
        return 1;
    if (strcmp(name, "Portal") == 0)
        return 2;
    if (strcmp(name, "Hole") == 0)
        return 3;
    if (strcmp(name, "Stitched") == 0)
        return 4;
    if (strcmp(name, "Forbidden") == 0)
        return 5;
    if (strcmp(name, "Any") == 0)
        return 6;
    return 7;
}

struct UnkSmartLinkRuleFilter_ovb0112 {
    u8 pad_0x0[0x64];
    u32 mask;  // 0x64
};

struct UnkSmartLinkRule_ovb0112 {
    u8 pad_0x0[0x18];
    UnkSmartLinkRuleFilter_ovb0112* filter;  // 0x18
    u8 pad_0x20[0x54];
    u32 value;  // 0x74
    u8 pad_0x78[0x9];
    bool both_ways;  // 0x81
};

void fn_710026f790(UnkSmartLinkRule_ovb0112* self, void* target, UnkNavMesh* mesh, u32 a, u16 cell, u32 b,
                   bool reverse, void* c, void* d, u32 e, u32 f);

void fn_710026f690(UnkSmartLinkRule_ovb0112* self, void* target, UnkNavMesh* mesh, u32 a, u16 cell, u32 b,
                   void* c, void* d, u32 e, u32 f) {
    if (UnkSmartLinkRuleFilter_ovb0112* filter = self->filter) {
        u32 mask = filter->mask;
        if (mask != 0) {
            u32 kind = fn_71002b009c(fn_710027a328(mesh, cell)) & mask;
            if (self->value != kind)
                return;
        }
    }
    fn_710026f790(self, target, mesh, a, cell, b, false, c, d, e, f);
    if (self->both_ways)
        fn_710026f790(self, target, mesh, a, cell, b, true, c, d, e, f);
}


struct UnkSmartLinkRuleData_ovb0112 {
    u8 pad_0x0[0x70];
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u8 field_0x80;
    u8 field_0x81;
    u8 pad_0x82[0x6];
    base::global::UnkStrId field_0x88;
    u8 field_0x90;
    u8 field_0x91;
    u8 field_0x92;
    u32 field_0x94;
    s32 link_kind;  // 0x98
};

void fn_710026ee04(UnkSmartLinkRuleData_ovb0112* self, base::global::CRntFile* file, void* arg);

static inline s32 LinkKind(const char* name) {
    if (strcmp(name, "None") == 0)
        return 0;
    if (strcmp(name, "EdgeToEdge") == 0)
        return 1;
    if (strcmp(name, "EdgeToPoly") == 0)
        return 2;
    if (strcmp(name, "PolyToEdge") == 0)
        return 3;
    return 4;
}

void fn_710026ebe8(UnkSmartLinkRuleData_ovb0112* self, base::global::CRntFile* file) {
    const char* kind;
    const char* from;
    const char* to;
    const char* name;
    fn_710007cee4(file, &kind);
    fn_710007c714(file, &self->field_0x70, 4, 1);
    fn_710007cee4(file, &from);
    fn_710007c714(file, &self->field_0x78, 4, 1);
    fn_710007cee4(file, &to);
    fn_710007c714(file, &self->field_0x80, 1, 1);
    fn_710007c714(file, &self->field_0x81, 1, 1);
    fn_710007cee4(file, &name);
    fn_710007c714(file, &self->field_0x90, 1, 1);
    fn_710007c714(file, &self->field_0x91, 1, 1);
    fn_710007c714(file, &self->field_0x92, 1, 1);
    fn_710007c714(file, &self->field_0x94, 4, 1);
    {
        base::global::UnkStrId id(name, true);
        self->field_0x88 = static_cast<base::global::UnkStrId&&>(id);
    }
    self->field_0x74 = fn_710026d2e4(from);
    self->field_0x7c = fn_710026d2e4(to);
    if (*kind)
        self->link_kind = LinkKind(kind);
    fn_710026ee04(self, file, nullptr);
}


struct UnkSmartLinkEntry_ovb0112 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    void* data;  // 0x10
    u32 count;   // 0x18
    u32 capacity;
    u32 step;    // 0x20
    bool field_0x24;
    u32 flags;   // 0x28
    void* allocator;  // 0x30
    u8 field_0x38[0x15];

    UnkSmartLinkEntry_ovb0112(UnkSmartLinkEntry_ovb0112&& o)
        : field_0x0(o.field_0x0), field_0x4(o.field_0x4), field_0x8(o.field_0x8),
          field_0xc(o.field_0xc), data(o.data), count(o.count), capacity(o.capacity),
          step(o.step), field_0x24(o.field_0x24), flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
        __builtin_memcpy(field_0x38, o.field_0x38, sizeof(field_0x38));
    }
};

struct UnkSmartLinkPair_ovb0112 {
    union {
        struct {
            u32 x;
            UnkSmartLinkEntry_ovb0112 y;
        } values;
        struct {
            u32* x;
            UnkSmartLinkEntry_ovb0112* y;
        } refs;
    };
    s32 state;  // 0x58
};

void fn_7100272cc4(UnkSmartLinkPair_ovb0112* self, UnkSmartLinkPair_ovb0112* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = 1;
        new (&self->values.x) u32(other->values.x);
        new (&self->values.y) UnkSmartLinkEntry_ovb0112(static_cast<UnkSmartLinkEntry_ovb0112&&>(other->values.y));
        break;
    case 2: {
        self->state = 1;
        u32* x = other->refs.x;
        UnkSmartLinkEntry_ovb0112* y = other->refs.y;
        new (&self->values.x) u32(*x);
        new (&self->values.y) UnkSmartLinkEntry_ovb0112(static_cast<UnkSmartLinkEntry_ovb0112&&>(*y));
        break;
    }
    }
}

void fn_7100272598(UnkNavRuleValue_A1_b75* self, const UnkNavRuleValue_A1_b75* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x4 = other->field_0x4;
    self->field_0x8 = other->field_0x8;
    self->field_0xc = other->field_0xc;
    UnkNavRuleValues_A1_b75* children = &self->children;
    const UnkNavRuleValues_A1_b75* from = &other->children;
    children->data = nullptr;
    children->count = 0;
    children->capacity = 0;
    children->step = from->step;
    children->field_0x14 = from->field_0x14;
    children->flags = from->flags;
    children->allocator = from->allocator;
    fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(children), from->capacity);
    const UnkNavRuleValue_A1_b75* it = from->data;
    const UnkNavRuleValue_A1_b75* end = it + from->count;
    for (; it != end; ++it)
        children->PushBackCopy(*it);
    new (&self->tail) navmesh::UnkNavRuleTail_A1_b75(other->tail);
}



void fn_71002737b0(UnkNavRuleValues_A1_b75* self, const UnkNavRuleValue_A1_b75* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        u32 capacity = self->capacity;
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)capacity)
            for (;;) {
            }
        fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), capacity);
        new (&self->data[self->count]) UnkNavRuleValue_A1_b75(*value);
    } else {
        u32 capacity = self->capacity;
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkSmartLinkEntry_ovb0112* from = &reinterpret_cast<UnkSmartLinkEntry_ovb0112*>(self->data)[i];
            new (from + 1) UnkSmartLinkEntry_ovb0112(static_cast<UnkSmartLinkEntry_ovb0112&&>(*from));
            fn_710026ead0(&self->data[i]);
            if (i == 0)
                break;
        }
        fn_7100272598(&self->data[index], value);
    }
    ++self->count;
}

struct UnkSmartLinkEntry;
void fn_7100273ad4(UnkSmartLinkEntryArray* self, UnkSmartLinkEntry* value);

void fn_7100273940(UnkNavRuleValues_A1_b75* self, UnkNavRuleValue_A1_b75* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7100273ad4(reinterpret_cast<UnkSmartLinkEntryArray*>(self), reinterpret_cast<UnkSmartLinkEntry*>(value));
        return;
    }
    u32 capacity = self->capacity;
    if (capacity <= n && self->step != 0)
        capacity = (n / self->step + 1) * self->step;
    fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), capacity);
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkSmartLinkEntry_ovb0112* from = &reinterpret_cast<UnkSmartLinkEntry_ovb0112*>(self->data)[i];
        new (from + 1) UnkSmartLinkEntry_ovb0112(static_cast<UnkSmartLinkEntry_ovb0112&&>(*from));
        fn_710026ead0(&self->data[i]);
        if (i == 0)
            break;
    }
    new (&reinterpret_cast<UnkSmartLinkEntry_ovb0112*>(self->data)[index])
        UnkSmartLinkEntry_ovb0112(static_cast<UnkSmartLinkEntry_ovb0112&&>(*reinterpret_cast<UnkSmartLinkEntry_ovb0112*>(value)));
    ++self->count;
}
