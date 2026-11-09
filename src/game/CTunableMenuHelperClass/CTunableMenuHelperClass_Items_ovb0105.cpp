// CTunableMenuHelperClass's unit (tools/tu_map.py): the tunable menu's items (0x58 bytes each), which
// hold their child items in an array (grown by fn_7100aded3c, destroyed by fn_7100ade944), an entry's
// two ids, and the helper's list collected with an empty path. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_7101043904(void*);
struct UnkCameraCtrl_ovb0049;
void fn_7101043934(UnkCameraCtrl_ovb0049*);
extern char g_unk_7101a975f0[];  // the item's vtable

// An item: the base object (fn_7101043904 / fn_7101043934), three ids, the child items and two flags.
// The ids sit in a union so that the destructor below, which goes through the vtable, leaves them alone.
struct UnkTunableMenuItem_b0105 {
    void** vtable;
    u64 field_0x8;
    union {
        CStrId field_0x10;
    };
    union {
        CStrId field_0x18;
    };
    union {
        CStrId field_0x20;
    };
    union {
        base::UnkVector<UnkTunableMenuItem_b0105> children;  // 0x28
    };
    bool field_0x50;
    bool field_0x51;

    UnkTunableMenuItem_b0105(UnkTunableMenuItem_b0105&& o);
    // Destroys the item through its vtable (slot 1).
    ~UnkTunableMenuItem_b0105() {
        reinterpret_cast<void (*)(UnkTunableMenuItem_b0105*)>(vtable[1])(this);
    }
};
static_assert(sizeof(UnkTunableMenuItem_b0105) == 0x58, "UnkTunableMenuItem_b0105 layout");

static inline void MoveId(CStrId& dst, CStrId& src) {
    dst.data = src.data;
    src.data = &g_unk_7101d47108->empty;
}

inline UnkTunableMenuItem_b0105::UnkTunableMenuItem_b0105(UnkTunableMenuItem_b0105&& o) {
    fn_7101043904(this);
    vtable = reinterpret_cast<void**>(g_unk_7101a975f0) + 2;
    MoveId(field_0x10, o.field_0x10);
    MoveId(field_0x18, o.field_0x18);
    MoveId(field_0x20, o.field_0x20);
    children.data = o.children.data;
    children.count = o.children.count;
    children.capacity = o.children.capacity;
    children.step = o.children.step;
    children.field_0x14 = o.children.field_0x14;
    children.flags = o.children.flags;
    children.allocator = o.children.allocator;
    o.children.data = nullptr;
    o.children.count = 0;
    o.children.capacity = 0;
    field_0x50 = o.field_0x50;
    field_0x51 = o.field_0x51;
}

// Grows the child array to at least n items.
__attribute__((noinline)) void fn_7100aded3c(base::UnkVector<UnkTunableMenuItem_b0105>* self, u32 n) {
    if (self->capacity < n) {
        auto* p = static_cast<UnkTunableMenuItem_b0105*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkTunableMenuItem_b0105), 8, self->flags));
        if (self->data) {
            UnkTunableMenuItem_b0105* src = self->data;
            UnkTunableMenuItem_b0105* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkTunableMenuItem_b0105(static_cast<UnkTunableMenuItem_b0105&&>(*src));
                src->~UnkTunableMenuItem_b0105();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}

// The item's destructor body: frees the children, releases the ids, then the base destructor.
void fn_7100ade944(UnkTunableMenuItem_b0105* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a975f0) + 2;
    self->children.Free();
    self->field_0x20.Reset();
    self->field_0x18.Reset();
    self->field_0x10.Reset();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}

struct UnkTunableMenuEntry_b0105 {
    u8 pad_0x0[0x18];
    CStrId field_0x18;
    CStrId field_0x20;
    u8 pad_0x28[0x28];
    bool dirty;  // 0x50
};

// Marks the entry changed and sets its two ids.
void fn_7100addddc(UnkTunableMenuEntry_b0105* self, const CStrId& a, const CStrId& b) {
    self->dirty = true;
    self->field_0x18 = a;
    self->field_0x20 = b;
}

// What fn_7100ade544 returns (0x28 bytes): an array header like base::UnkVector's with its own
// field order. Names are placeholders.
struct UnkTunableMenuList_b0105 {
    void* data = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u64 field_0x10 = 0;
    u32 field_0x18 = 10;
    u32 field_0x1c = 2;
    base::UnkAllocator* allocator = fn_7100083990();
};

// A path element: an id released in two steps (CStrId::Reset).
struct UnkTunableMenuPathId_b0105 {
    union {
        CStrId id;
    };
    ~UnkTunableMenuPathId_b0105() { id.Reset(); }
};

struct UnkTunableMenuHelper_b0105;
void fn_7100addec4(UnkTunableMenuHelper_b0105* self, UnkTunableMenuList_b0105* out,
                   base::UnkVector<UnkTunableMenuPathId_b0105>* path);

// Collects the helper's list with an empty path.
UnkTunableMenuList_b0105 fn_7100ade544(UnkTunableMenuHelper_b0105* self) {
    UnkTunableMenuList_b0105 out;
    base::UnkVector<UnkTunableMenuPathId_b0105> path(0xf);
    fn_7100addec4(self, &out, &path);
    path.Free();
    return out;
}
