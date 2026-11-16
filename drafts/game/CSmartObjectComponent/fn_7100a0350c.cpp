// CSmartObjectComponent unit: sets an object's name id (a default one when the given id is
// empty), then builds its "left" and "right" ids from it. Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern void* g_unk_7101a1b750[];
// The name an object with an empty one gets.
__attribute__((visibility("hidden"))) extern CStrId g_unk_7172742da8;

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; it is left
// without a destructor, since its string is always handed on.
union UnkStrIdSlot {
    CStrId id;
    UnkStrIdSlot() {}
    ~UnkStrIdSlot() {}
};

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

void fn_7100a0350c(UnkSmartObjectNames* self, const CStrId* name) {
    if (name->data == &g_unk_7101d47108->empty)
        self->name = g_unk_7172742da8;
    else
        self->name = *name;

    UnkStrIdSlot slot;
    UnkBufferString<0x80, g_unk_7101a1b750> left("left");
    UnkBufferString<0x80, g_unk_7101a1b750> right("right");
    fn_710007f3b4(left.Str(), &IdString(self->name));
    fn_710007f3b4(right.Str(), &IdString(self->name));
    fn_7100000850(&slot.id, *left.Str(), true);
    self->left.ReleaseAndTake(slot.id);
    fn_7100000850(&slot.id, *right.Str(), true);
    self->right.ReleaseAndTake(slot.id);
}
