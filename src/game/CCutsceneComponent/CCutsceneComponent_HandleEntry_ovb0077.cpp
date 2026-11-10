// CCutsceneComponent's unit (tools/tu_map.py): the copy assignment of an actor entry with a weak handle after it:
// the entry's own assignment (fn_71007abd58, in CCutsceneComponent_Actors_ovb0077.cpp; kept in another file so
// that its returned pointer is not reused here), then the handle (CopyWeakRef). Names and layouts are
// placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void* fn_71007abd58(void* self, const void* other);

// The entry copied by fn_71007abd58 with a weak handle after it.
struct UnkCutsceneHandleEntry_ovb0077 {
    u8 entry[0x88];
    UnkWeakHandle* handle;  // 0x88
};

UnkCutsceneHandleEntry_ovb0077* fn_71007ad330(UnkCutsceneHandleEntry_ovb0077* self,
                                              const UnkCutsceneHandleEntry_ovb0077* other) {
    fn_71007abd58(self, other);
    if (self != other)
        CopyWeakRef(&self->handle, other->handle);
    return self;
}
