// A second copy of fn_7100e2f4bc (CTunableDroppableBillboard_Copy_b43.cpp) in CLogicDroppedItemManager::CTunableDroppableBillboard's unit:
// the copy constructor of another billboard description of the same layout.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkBillboardRes_b44a3 {
    UnkCountedResource* ptr;

    UnkBillboardRes_b44a3(const UnkBillboardRes_b44a3& other) : ptr(nullptr) {
        if (UnkCountedResource* p = other.ptr) {
            ++p->ref_count;
            if (UnkCountedResource* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = p;
        }
    }
};

struct UnkBillboardEntry_b44a3 {
    CStrId name;
    CFilePathStrId path;
    UnkBillboardRes_b44a3 res;
};

// Three words copied one by one (a user-written copy, as of a vector).
struct UnkBillboardTriple_b44a3 {
    u32 x;
    u32 y;
    u32 z;

    UnkBillboardTriple_b44a3(const UnkBillboardTriple_b44a3& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkBillboard_b44a3 {
    UnkBillboardTriple_b44a3 field_0x0;
    f32 field_0xc[7];
    bool field_0x28;
    UnkBillboardEntry_b44a3 entries[3];  // 0x30
};
static_assert(sizeof(UnkBillboard_b44a3) == 0x78, "UnkBillboard_b44a3 placeholder layout");

__attribute__((flatten)) void fn_7100e2fba4(UnkBillboard_b44a3* self, const UnkBillboard_b44a3* other) {
    new (self) UnkBillboard_b44a3(*other);
}
