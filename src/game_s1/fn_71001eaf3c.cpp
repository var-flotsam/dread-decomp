// Places one entry of a large pool (entries between +0x30d48 and +0x588048 of the owner) with a
// transform matrix: copies the matrix to the entry (+8), passes its translation and its largest
// scale component to the entry's sphere at +0x48 (fn_7100216978, fn_7100216d30), then latches
// the owner's flag at +0x5e9b87 (set, or set from whether the entry's +0xd8 is zero). Returns
// false for a null or out-of-pool entry.
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"

struct UnkPoolEntry {
    u8 pad_0x0[0x8];
    UnkMatrix44 matrix;
    UnkSphere sphere_0x48;
    u8 pad_0x98[0x40];
    u32 field_0xd8;
};

void fn_7100216d30(UnkSphere* sphere, f32 scale);

bool fn_71001eaf3c(char* self, UnkPoolEntry* entry, UnkXform* x) {
    if (!entry)
        return false;
    char* p = reinterpret_cast<char*>(entry);
    if (self + 0x30d48 > p || self + 0x588048 <= p)
        return false;
    UnkCopyMatrix(&entry->matrix, *reinterpret_cast<const UnkMatrix44*>(x->m));
    {
        UnkVec3f t = x->GetTranslation();
        fn_7100216978(&entry->sphere_0x48, &t);
    }
    {
        UnkVec3f s = fn_7100076cec(x);
        f32 m = s.x < s.y ? s.y : s.x;
        m = m < s.z ? s.z : m;
        fn_7100216d30(&entry->sphere_0x48, m);
    }
    bool& flag = *reinterpret_cast<bool*>(self + 0x5e9b87);
    flag = flag ? true : entry->field_0xd8 == 0;
    return true;
}
