#include "types.h"

struct UnkScourgeQuery;
bool fn_710069ae3c(UnkScourgeQuery* query, u32 kind, u32* outIndex, f32* outValue);

struct CScourgeAIComponent {
    u8 unk_0x0[0x146c8];
    u8 query[0x8];  // 0x146c8
};

// Whether the query at 0x146c8 finds anything of kind 0.
bool fn_71006a0884(CScourgeAIComponent* self) {
    u32 index = 0;
    f32 value = -1.0f;
    return fn_710069ae3c(reinterpret_cast<UnkScourgeQuery*>(self->query), 0, &index, &value);
}
