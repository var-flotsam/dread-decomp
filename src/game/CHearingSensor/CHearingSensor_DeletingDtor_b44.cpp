// CHearingSensor's unit (tools/tu_map.py): a deleting destructor that releases the counted reference at
// 0x10 (count at +0x30, deleted through its vtable). Shaped like fn_710063cac4 in src/clones/, with its
// own vtable and member layout. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

extern char g_unk_7101caa308[];  // the object's vtable (also declared in CHearingSensor_Dtor.cpp)

struct UnkHearingRefCounted_b44a3 {
    virtual ~UnkHearingRefCounted_b44a3();
    u8 field_0x8[0x28];
    s32 ref_count;  // 0x30
};

struct UnkHearingRefPtr_b44a3 {
    UnkHearingRefCounted_b44a3* ptr;

    ~UnkHearingRefPtr_b44a3() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkHearingSensorObj_b44a3;
template <>
struct base::UnkDtorTraits<UnkHearingSensorObj_b44a3> {
    static char* vtable() { return g_unk_7101caa308; }
};

__attribute__((flatten)) void fn_7100f03850(char* self) {
    base::refl_DeletingDtorNoBase<UnkHearingSensorObj_b44a3,
                                  base::UnkMemberAt<UnkHearingRefPtr_b44a3, 0x10>>(self);
}
