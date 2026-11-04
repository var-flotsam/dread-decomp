// Unit 2640 (tools/tu_map.py; no reflected class): Reset of two-state variants whose state-1 object
// holds an array (base::UnkVector) of 0x18-byte records led by a file path id, destroyed inline (the
// ids released, the array freed). The array sits at +0x20 (state at +0x68) or +0x10 (state at
// +0x58). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkIdRecord18_b55 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    u8 unk_0x8[0x10];
};

template <int Tag, u32 Pre>
struct UnkIdRecordsObject_b55 {
    u8 unk_0x0[Pre];
    base::UnkVector<UnkIdRecord18_b55> items;
    u8 unk_tail[0x20];
};

template <int Tag, u32 Pre>
struct UnkRecordsVariant_b55 {
    union {
        UnkIdRecordsObject_b55<Tag, Pre> object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.items.Free();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
};

void fn_7100ff13c8(UnkRecordsVariant_b55<0, 0x20>* self) { self->Reset(); }
void fn_7100ff1930(UnkRecordsVariant_b55<1, 0x20>* self) { self->Reset(); }
void fn_7100ffc914(UnkRecordsVariant_b55<2, 0x10>* self) { self->Reset(); }
void fn_7100ffcea0(UnkRecordsVariant_b55<3, 0x10>* self) { self->Reset(); }
