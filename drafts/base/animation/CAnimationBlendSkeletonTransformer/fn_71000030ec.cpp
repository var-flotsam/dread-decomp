// base::animation::CAnimationBlendSkeletonTransformer's unit (tools/tu_map.py unit 10): frees a pose
// buffer's storage: its six growable arrays, then each entry's two arrays and the entry array itself,
// resetting the other members on the way. Names and layouts are placeholders.
#include "game/UnkTU9.h"
#include "types.h"

template <typename T>
struct UnkGrowArray_l0030ec {
    s32 count;
    s32 capacity;
    T* data;

    void Free() {
        if (data) {
            count = 0;
            capacity = 0;
            fn_7100002cd0(data);
            data = nullptr;
        }
    }
};

struct UnkElem30_l0030ec {
    u8 bytes[0x30];
};

struct UnkElem14_l0030ec {
    u8 bytes[0x14];
};

struct UnkElem10_l0030ec {
    u8 bytes[0x10];
};

struct UnkPoseEntry_l0030ec {
    UnkGrowArray_l0030ec<u64> array_0x0;
    UnkGrowArray_l0030ec<u64> array_0x10;
};

struct UnkPoseBuffer_l0030ec {
    UnkGrowArray_l0030ec<UnkElem30_l0030ec> array_0x0;
    UnkGrowArray_l0030ec<u16> array_0x10;
    UnkGrowArray_l0030ec<UnkElem14_l0030ec> array_0x20;
    u32 pad_0x30[2];
    s32 field_0x38;
    u32 pad_0x3c;
    void* field_0x40;
    void* field_0x48;
    UnkGrowArray_l0030ec<UnkElem10_l0030ec> array_0x50;
    UnkGrowArray_l0030ec<u64> array_0x60;
    UnkGrowArray_l0030ec<u64> array_0x70;
    s32 field_0x80;
    s32 field_0x84;
    UnkGrowArray_l0030ec<UnkPoseEntry_l0030ec> entries;  // 0x88
};

void fn_71000030ec(UnkPoseBuffer_l0030ec* self) {
    self->array_0x0.Free();
    self->array_0x10.Free();
    self->array_0x20.Free();
    self->field_0x38 = 0;
    self->field_0x40 = nullptr;
    self->field_0x48 = nullptr;
    self->array_0x50.Free();
    self->array_0x60.Free();
    self->array_0x70.Free();
    self->field_0x80 = 0;
    self->field_0x84 = 1;
    for (s32 i = 0; i < self->entries.count; i++) {
        // The first entry is emptied without freeing its arrays (as the original does).
        UnkPoseEntry_l0030ec& entry = self->entries.data[i];
        if (i == 0)
            entry = UnkPoseEntry_l0030ec();
        self->entries.data[i].array_0x0.Free();
        self->entries.data[i].array_0x10.Free();
    }
    self->entries.Free();
}
