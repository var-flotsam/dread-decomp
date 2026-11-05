// The destructors of CAcidBlobsLaunchPattern (fn_71005d6aa4) and CBouncingCreaturesLaunchPattern
// (fn_71005dfdb0), whose constructors are in CAcidBlobsLaunchPattern.cpp: the vtable, the array of
// owned-object lists after the string id freed (at +0x110 and +0xf8), the string id at +0xf0
// released, then the shared base's destructor fn_71005dbe04. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The string id at +0xf0: its pointer is a union member (no type-based alias info), so the release
// reloads it after the count decrement, as the original does.
struct UnkPatternStrId_b56a3 {
    union {
        UnkStringData* data;
    };
    __attribute__((always_inline)) void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

void fn_71005dbe04(void* self);

extern void* g_unk_7101a53428[];
extern void* g_unk_7101a53618[];

struct UnkLaunchPatternBase_b56a3 {
    void** vtable;
    u8 unk_0x8[0xe8];
    UnkPatternStrId_b56a3 field_0xf0;
};

template <u32 Pad>
struct UnkLaunchPattern_b56a3 : UnkLaunchPatternBase_b56a3 {
    u8 unk_0xf8[Pad];
    base::UnkVector<UnkOwnedList> lists;
};

template <u32 Pad>
static inline __attribute__((always_inline)) void Destroy_b56a3(UnkLaunchPattern_b56a3<Pad>* self, void** vtable) {
    self->vtable = vtable;
    self->lists.Free();
    self->field_0xf0.Release();
    fn_71005dbe04(self);
}

void fn_71005d6aa4(UnkLaunchPattern_b56a3<0x18>* self) { Destroy_b56a3(self, g_unk_7101a53428 + 2); }
void fn_71005dfdb0(UnkLaunchPattern_b56a3<0>* self) { Destroy_b56a3(self, g_unk_7101a53618 + 2); }
