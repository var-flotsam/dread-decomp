// media::CMoviePlayer's unit (tools/tu_map.py unit 323): drops one reference of the shared object a
// handle points to (an atomic count at +0x8) and, when the count reads zero afterwards, calls its
// vtable slot 2. Names are placeholders.
#include "types.h"

struct UnkShared323 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    s32 ref_count;  // 0x8
};

void fn_710021e338(UnkShared323** handle) {
    UnkShared323* obj = *handle;
    if (obj == nullptr) {
        return;
    }
    __atomic_fetch_sub(&obj->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&obj->ref_count, __ATOMIC_SEQ_CST) == 0) {
        obj->vf_2();
    }
}
