// CSmartObjectComponent unit: sets an object's second name id (a default one when the given id is
// empty), then builds its second "left" and "right" ids from it. Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkStrIdSlot.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The second name an object with an empty one gets.
__attribute__((visibility("hidden"))) extern CStrId g_unk_7172742db0;

struct UnkSmartObjectNames {
    u8 pad_0x0[0x68];
    CStrId name;          // 0x68
    CStrId other_name;    // 0x70
    CStrId left;          // 0x78
    CStrId other_left;    // 0x80
    CStrId right;         // 0x88
    CStrId other_right;   // 0x90
};

// The id's string, counting the read.
inline const CRntString& IdString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_7100a03790(UnkSmartObjectNames* self, const CStrId* name) {
    if (name->data == &g_unk_7101d47108->empty)
        self->other_name = g_unk_7172742db0;
    else
        self->other_name = *name;

    UnkStrIdSlot slot;
    UnkBufferString<0x80, g_unk_7101a1b750> left("left");
    UnkBufferString<0x80, g_unk_7101a1b750> right("right");
    CRntString_Append(left.Str(), &IdString(self->other_name));
    CRntString_Append(right.Str(), &IdString(self->other_name));
    fn_7100000850(&slot.id, *left.Str(), true);
    self->other_left.ReleaseAndTake(slot.id);
    slot.id.~CStrId();
    fn_7100000850(&slot.id, *right.Str(), true);
    self->other_right.ReleaseAndTake(slot.id);
    slot.id.~CStrId();
}
