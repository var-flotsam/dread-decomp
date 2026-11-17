// Destructor of a plain object with a member at 0x20 (destroyed by fn_7100216934), a string id
// at 0x80 and a counted object at 0xc8 (count at +0x54, freed by fn_710001737c), destroyed in
// reverse order. fn_7100269170 is a relative. Also one with a counted object at 0 and a file path
// id at 0x18 (fn_7100071928).

#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkMember_71001bd92c {
    char storage[0x60];

    __attribute__((always_inline)) ~UnkMember_71001bd92c() { fn_7100216934(storage); }
};

struct UnkCountedRef_71001bd92c {
    UnkCountedResource* counted;

    __attribute__((always_inline)) ~UnkCountedRef_71001bd92c() {
        if (UnkCountedResource* held = counted; held && --held->ref_count == 0)
            fn_710001737c(held);
    }
};

struct UnkObject_71001bd92c {
    u8 pad_0x0[0x20];
    UnkMember_71001bd92c member;
    base::UnkStrIdMember id;
    u8 pad_0x88[0x40];
    UnkCountedRef_71001bd92c counted;
};

void fn_71001bd92c(UnkObject_71001bd92c* self) { self->~UnkObject_71001bd92c(); }

// A counted object at 0 and a file path id at 0x18 (fn_7100071928).
struct UnkObject_7100071928 {
    UnkCountedRef_71001bd92c counted;
    u8 pad_0x8[0x10];
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
};

void fn_7100071928(UnkObject_7100071928* self) { self->~UnkObject_7100071928(); }
