// CShakernautAIComponent's unit: runs the base's routine fn_71003695fc, then drops the refcounted pointer
// of every 0x20-byte record in the two arrays at +0x14818 and +0x14840. Names are placeholders.
#include "base/UnkDestructors.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

struct UnkNode;

namespace {

struct UnkShakRecord_A4_b71 {
    u8 unk_0x0[0x18];
    base::UnkRefCounted* ref;
};

struct UnkShakArray_A4_b71 {
    UnkShakRecord_A4_b71* data;
    u32 count;
};

__attribute__((always_inline)) inline void ClearRefs(UnkShakArray_A4_b71* a) {
    for (UnkShakRecord_A4_b71 *it = a->data, *end = a->data + a->count; it != end; ++it) {
        if (base::UnkRefCounted* p = it->ref) {
            if (--p->ref_count == 0)
                delete p;
            it->ref = nullptr;
        }
    }
}

}  // namespace

void fn_71006ae1fc(char* self) {
    fn_71003695fc(reinterpret_cast<UnkNode*>(self));
    ClearRefs(reinterpret_cast<UnkShakArray_A4_b71*>(self + 0x14818));
    ClearRefs(reinterpret_cast<UnkShakArray_A4_b71*>(self + 0x14840));
}
