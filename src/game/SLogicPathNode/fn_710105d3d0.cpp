// SLogicPathNode unit: a reflected setter that erases the element of a point array (0x20-byte
// polymorphic elements) at the index a value holder gives, and answers the 0x18-byte status {0, 0, 2, 1}.
// Names and layouts are placeholders.
#include <new>

#include "types.h"
#include "game/SLogicPathNode.h"


struct UnkValueType_l05d3d0 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
};

struct UnkValue_l05d3d0 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType_l05d3d0* type;  // 0x8
    u8 pad_0x10[0x4];
    u32 mode;  // 0x14
};

struct UnkPathPoint_l05d3d0 {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    void* field_0x10;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    UnkPathPoint_l05d3d0(UnkPathPoint_l05d3d0&& other) {
        vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x18 = other.field_0x18;
    }
    ~UnkPathPoint_l05d3d0() { reinterpret_cast<void (*)(UnkPathPoint_l05d3d0*)>(vtable[1])(this); }
};

struct UnkPathPointList {
    u8 pad_0x0[0x8];
    void* data;  // 0x8
    u32 count;           // 0x10
};

struct UnkListOwner_l05d3d0 {
    u8 pad_0x0[0x8];
    UnkPathPointList* list;  // 0x8
};

struct UnkStatus {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkStatus fn_710105d3d0(void* context, UnkListOwner_l05d3d0* owner, UnkValue_l05d3d0* const* value) {
    UnkValue_l05d3d0* v = *value;
    UnkPathPointList* list = owner->list;
    const u8* bytes = (v->mode == 1 || v->type->size >= 9) ? v->data : v->inline_data;
    u32 index = *reinterpret_cast<const u32*>(bytes);
    if (index < list->count) {
        static_cast<UnkPathPoint_l05d3d0*>(list->data)[index].~UnkPathPoint_l05d3d0();
        for (u64 i = index; i < list->count - 1; ++i) {
            new (&static_cast<UnkPathPoint_l05d3d0*>(list->data)[i]) UnkPathPoint_l05d3d0(static_cast<UnkPathPoint_l05d3d0&&>(static_cast<UnkPathPoint_l05d3d0*>(list->data)[i + 1]));
            static_cast<UnkPathPoint_l05d3d0*>(list->data)[i + 1].~UnkPathPoint_l05d3d0();
        }
        --list->count;
    }
    UnkStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}
