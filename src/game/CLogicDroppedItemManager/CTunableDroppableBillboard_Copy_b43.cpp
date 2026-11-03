// CLogicDroppedItemManager::CTunableDroppableBillboard's unit (tools/tu_map.py): the copy constructor
// of a billboard description: three words and a block of plain values, then three entries of a name, a
// file path and a counted resource reference (taken from the other's when it has one). Names and
// layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkBillboardRes_b43a3 {
    UnkCountedResource* ptr;

    UnkBillboardRes_b43a3(const UnkBillboardRes_b43a3& other) : ptr(nullptr) {
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

struct UnkBillboardEntry_b43a3 {
    CStrId name;
    CFilePathStrId path;
    UnkBillboardRes_b43a3 res;
};

// Three words copied one by one (a user-written copy, as of a vector).
struct UnkBillboardTriple_b43a3 {
    u32 x;
    u32 y;
    u32 z;

    UnkBillboardTriple_b43a3(const UnkBillboardTriple_b43a3& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkBillboard_b43a3 {
    UnkBillboardTriple_b43a3 field_0x0;
    f32 field_0xc[7];
    bool field_0x28;
    UnkBillboardEntry_b43a3 entries[3];  // 0x30
};
static_assert(sizeof(UnkBillboard_b43a3) == 0x78, "UnkBillboard_b43a3 placeholder layout");

__attribute__((flatten)) void fn_7100e2f4bc(UnkBillboard_b43a3* self, const UnkBillboard_b43a3* other) {
    new (self) UnkBillboard_b43a3(*other);
}
