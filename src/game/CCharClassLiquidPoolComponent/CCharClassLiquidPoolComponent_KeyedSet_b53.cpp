// CCharClassLiquidPoolComponent unit (tools/tu_map.py): a keyed array's set (refl_KeyedSet, as
// fn_7100251a10) with 8-byte keys and 16-byte values of four words copied one by one. Names are
// placeholders.
#include "base/UnkCloneCalls.h"
#include "types.h"

struct UnkFourWords_b53 {
    u32 x, y, z, w;

    UnkFourWords_b53& operator=(const UnkFourWords_b53& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }
};

struct UnkClone_fn_71009047b4;

void fn_71009047b4(base::UnkKeyedArray<UnkClone_fn_71009047b4, u64, UnkFourWords_b53>* a, const u64* key,
                   const UnkFourWords_b53* value) {
    UnkFourWords_b53* p = a->Find(*key);
    if (p != a->End())
        *p = *value;
}
