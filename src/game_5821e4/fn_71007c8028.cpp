// SDropProbabilities (reflection: size 0x90): its copy constructor (fn_71007c8028) and move
// constructor (fn_71007c80d0), both implicit. The string id is copied or moved; the move leaves
// the source empty.
#include "CAttack.h"
#include "base/global/CStrId.h"

#include <new>
#include <utility>

struct UnkStrIdField_71007c8028 {
    base::global::UnkStringData* data;

    UnkStrIdField_71007c8028(const UnkStrIdField_71007c8028& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkStrIdField_71007c8028(UnkStrIdField_71007c8028&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct SDropProbabilities {
    u64 field_0x0[11];
    u32 field_0x58;
    UnkStrIdField_71007c8028 field_0x60;
    UnkAttackWords<6> field_0x68;
    u64 field_0x80;
    u16 field_0x88;
    u8 field_0x8a;
};

void fn_71007c8028(SDropProbabilities* self, const SDropProbabilities* other) {
    new (self) SDropProbabilities(*other);
}

void fn_71007c80d0(SDropProbabilities* self, SDropProbabilities* other) {
    new (self) SDropProbabilities(std::move(*other));
}
