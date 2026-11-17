// A unit with no known name (tools/tu_map.py unit 210): a GPU particle emitter's destructor. Hands its two
// buffers back to the free list, frees the index array, destroys the owned children of its two lists and
// then its buffer members in reverse order. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkSlot_b0235.h"
#include "game/UnkTU210.h"
#include "game/UnkTU221.h"
#include "types.h"

// The buffer members' vtables (each + 0x10).
extern char g_unk_7101a1dbd0[];  // the emitter's own
extern char g_unk_7101a1dbf0[];
extern char g_unk_7101a1dc10[];
extern char g_unk_7101a1dc30[];
extern char g_unk_7101a1dc50[];

struct UnkBuffer20_l16f29c {
    void* vtable;
    u8 pad_0x8[0x10];
    void* data;  // 0x18
};

struct UnkBuffer28_l16f29c {
    void* vtable;
    u8 pad_0x8[0x18];
    void* data;  // 0x20
};

struct UnkChild_l16f29c {
    virtual ~UnkChild_l16f29c();
};

struct UnkChildList_l16f29c {
    UnkChild_l16f29c** items;
    u32 capacity;  // 0x8
    u32 count;     // 0xc
};

struct UnkEmitter_l16f29c {
    void* vtable;
    UnkBuffer20_l16f29c field_0x8;
    UnkBuffer20_l16f29c field_0x28;
    UnkBuffer20_l16f29c field_0x48;
    UnkBuffer28_l16f29c field_0x68;
    UnkBuffer28_l16f29c field_0x90;
    UnkBuffer28_l16f29c field_0xb8;
    UnkBuffer28_l16f29c field_0xe0;
    UnkBuffer28_l16f29c field_0x108;
    UnkBuffer28_l16f29c field_0x130;
    UnkBuffer20_l16f29c field_0x158;
    UnkBuffer20_l16f29c field_0x178;
    UnkBuffer20_l16f29c field_0x198;
    UnkBuffer20_l16f29c field_0x1b8;
    UnkBuffer28_l16f29c field_0x1d8;
    UnkChildList_l16f29c field_0x200;
    UnkChildList_l16f29c children;  // 0x210
    u8 field_0x220[0x398 - 0x220];
    void* indices;      // 0x398
    void* field_0x3a0;
    void* field_0x3a8;
};
static_assert(__builtin_offsetof(UnkEmitter_l16f29c, field_0x200) == 0x200, "UnkEmitter_l16f29c layout");
static_assert(__builtin_offsetof(UnkEmitter_l16f29c, field_0x3a8) == 0x3a8, "UnkEmitter_l16f29c layout");

template <char* Vtable, class T>
inline void DestroyBuffer(T& buffer) {
    buffer.vtable = Vtable + 0x10;
    if (buffer.data) {
        fn_7100083c30(buffer.data);
        buffer.data = nullptr;
    }
}

inline void DestroyChildren(UnkChildList_l16f29c& list) {
    if (list.items) {
        for (u64 i = 0; i < list.count; ++i) {
            if (list.items[i]) {
                delete list.items[i];
                list.items[i] = nullptr;
            }
        }
        if (list.items)
            fn_7100083c30(list.items);
        list.items = nullptr;
    }
    list.capacity = 0;
    list.count = 0;
}

inline void Release(void*& slot) {
    if (slot) {
        u32 n = g_unk_716e352430;
        g_unk_716e34e430[n] = static_cast<UnkSlot_b0235*>(slot);
        g_unk_716e352430 = n + 1;
    }
    slot = nullptr;
}

void fn_710016f29c(UnkEmitter_l16f29c* self) {
    self->vtable = g_unk_7101a1dbd0 + 0x10;
    Release(self->field_0x3a0);
    Release(self->field_0x3a8);
    if (self->indices) {
        fn_7100083990()->vf_0x48(self->indices);
        self->indices = nullptr;
    }
    DestroyChildren(self->field_0x200);
    DestroyChildren(self->children);
    fn_710019adec(self->field_0x220);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0x1d8);
    DestroyBuffer<g_unk_7101a1dc50>(self->field_0x1b8);
    DestroyBuffer<g_unk_7101a1dc10>(self->field_0x198);
    DestroyBuffer<g_unk_7101a1dc50>(self->field_0x178);
    DestroyBuffer<g_unk_7101a1dc10>(self->field_0x158);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0x130);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0x108);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0xe0);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0xb8);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0x90);
    DestroyBuffer<g_unk_7101a1dc30>(self->field_0x68);
    DestroyBuffer<g_unk_7101a1dc10>(self->field_0x48);
    DestroyBuffer<g_unk_7101a1dc10>(self->field_0x28);
    DestroyBuffer<g_unk_7101a1dbf0>(self->field_0x8);
}
