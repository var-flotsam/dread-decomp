// animtree::CAnimTreeDef's unit (tools/tu_map.py): builds a link value (a word and two string ids, kind
// at +0x18) from another holder: kind 1 holds the value inline, kind 2 holds pointers to the word and to
// the pair of ids; anything else leaves the result empty (kind 0). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkLinkIds_A2_b72 {
    CStrId first;
    CStrId second;
};

struct UnkLinkValue_A2_b72 {
    u32 word;
    UnkLinkIds_A2_b72 ids;  // 0x8
    u32 kind;               // 0x18
};

struct UnkLinkRef_A2_b72 {
    const u32* word;
    const UnkLinkIds_A2_b72* ids;
};

struct UnkLinkSource_A2_b72 {
    union {
        UnkLinkValue_A2_b72 value;
        UnkLinkRef_A2_b72 ref;
    };
};

void fn_71002d9c64(UnkLinkValue_A2_b72* self, const UnkLinkSource_A2_b72* src) {
    self->kind = 0;
    u32 kind = src->value.kind;
    if (kind == 1) {
        self->kind = kind;
        self->word = src->value.word;
        new (&self->ids.first) CStrId(src->value.ids.first);
        new (&self->ids.second) CStrId(src->value.ids.second);
    } else if (kind == 2) {
        self->kind = 1;
        const UnkLinkIds_A2_b72* ids = src->ref.ids;
        self->word = *src->ref.word;
        new (&self->ids.first) CStrId(ids->first);
        new (&self->ids.second) CStrId(ids->second);
    }
}
