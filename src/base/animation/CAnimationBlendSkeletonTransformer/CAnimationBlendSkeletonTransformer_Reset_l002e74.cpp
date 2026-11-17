// base::animation::CAnimationBlendSkeletonTransformer's unit (tools/tu_map.py unit 10):
// fn_7100002e74 empties a pose buffer's six growable arrays and resets its other members.
// Names and layouts are placeholders.
#include <string.h>

#include "types.h"

void* fn_7100002be0(u64 size);
void fn_7100002cd0(void* p);

template <typename T>
struct UnkGrowArray_l002e74 {
    s32 count;
    s32 capacity;
    T* data;

    void Resize(s32 n) {
        if (capacity < n) {
            s32 grown = capacity + capacity / 2;
            if (grown < n)
                grown = n;
            if (capacity < grown) {
                T* fresh = static_cast<T*>(fn_7100002be0(grown * sizeof(T)));
                if (data)
                    memcpy(fresh, data, count * sizeof(T));
                fn_7100002cd0(data);
                data = fresh;
                capacity = grown;
            }
        }
        count = n;
    }
};

struct UnkElem30_l002e74 {
    u8 bytes[0x30];
};

struct UnkElem14_l002e74 {
    u8 bytes[0x14];
};

struct UnkElem10_l002e74 {
    u8 bytes[0x10];
};

struct UnkPoseBuffer_l002e74 {
    UnkGrowArray_l002e74<UnkElem30_l002e74> array_0x0;
    UnkGrowArray_l002e74<u16> array_0x10;
    UnkGrowArray_l002e74<UnkElem14_l002e74> array_0x20;
    u32 pad_0x30[2];
    s32 field_0x38;
    u32 pad_0x3c;
    void* field_0x40;
    void* field_0x48;
    UnkGrowArray_l002e74<UnkElem10_l002e74> array_0x50;
    UnkGrowArray_l002e74<u64> array_0x60;
    UnkGrowArray_l002e74<u64> array_0x70;
    s32 field_0x80;
    s32 field_0x84;
};

void fn_7100002e74(UnkPoseBuffer_l002e74* self) {
    self->array_0x0.Resize(0);
    self->array_0x10.Resize(0);
    self->array_0x20.Resize(0);
    self->field_0x38 = 0;
    self->field_0x40 = nullptr;
    self->field_0x48 = nullptr;
    self->array_0x50.Resize(0);
    self->array_0x60.Resize(0);
    self->array_0x70.Resize(0);
    self->field_0x80 = 0;
    self->field_0x84 = 1;
}
