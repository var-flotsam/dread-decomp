// Unit 32 of tools/tu_map.py: builds an id and path pair with its resource (base/UnkStrIdPathPair.h)
// from a source object's path text: the source's use count (+0x44) goes up, the pair starts as the
// empty ids (g_unk_7101d470e0, g_unk_7101d470e8) with no resource, and is then assigned the ids the
// text gives (fn_710001f074), which resolves the resource (fn_71000267f8). Names and layouts are
// placeholders.
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "game/UnkTU32.h"
#include "types.h"

struct UnkPathSource_b45a1 {
    u8 pad_0x0[0x10];
    base::global::CRntString text;  // 0x10
    u8 pad_0x40[0x4];
    // A union member (no type-based alias info): the original reloads the source pointer after
    // the increment.
    union {
        u32 use_count;  // 0x44
    };
};

struct UnkPathSourceRef_b45a1 {
    UnkPathSource_b45a1* ptr;
};

void fn_7100027464(UnkStrIdPathResource* self, const UnkPathSourceRef_b45a1* source) {
    ++source->ptr->use_count;
    UnkPathIdsFromText ids(source->ptr->text);
    new (self) UnkStrIdPathResource();
    fn_71000267f8(self, ids.ids);
}
