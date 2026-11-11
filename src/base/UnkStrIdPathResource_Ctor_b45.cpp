// Unit 32 of tools/tu_map.py: builds an id and path pair with its resource (base/UnkStrIdPathPair.h)
// from a source object's path text: the source's use count (+0x44) goes up, the pair starts as the
// empty ids (g_unk_7101d470e0, g_unk_7101d470e8) with no resource, and is then assigned the ids the
// text gives (fn_710001f074), which resolves the resource (fn_71000267f8). Names and layouts are
// placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CShotManager.cpp declares the builder.
struct UnkPathIds;
struct UnkInlineString;

// The ids the builder fills in place (the members start out unset), destroyed in reverse order.
struct UnkBuiltPathIds_b45a1 {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkBuiltPathIds_b45a1(const UnkInlineString* source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), source);
    }
    ~UnkBuiltPathIds_b45a1() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

struct UnkPathSource_b45a1 {
    u8 pad_0x0[0x10];
    u8 text[0x34];  // 0x10
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
    UnkBuiltPathIds_b45a1 ids(reinterpret_cast<const UnkInlineString*>(source->ptr->text));
    new (&self->ids.id) CStrId(g_unk_7101d470e0);
    new (&self->ids.path) base::global::CFilePathStrId(g_unk_7101d470e8);
    self->resource = nullptr;
    fn_71000267f8(self, reinterpret_cast<const UnkStrIdPathPair&>(ids));
}
