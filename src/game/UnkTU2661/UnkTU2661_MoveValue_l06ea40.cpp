// Unit 2661 (CCutscene::CTunableCutscene): move helper of the iterator value over a reflected array of 0x30-byte polymorphic elements,
// as base::refl_IterValueMove. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "game/UnkTU2661.h"
#include "types.h"

// 35 bytes copied as one block.
struct __attribute__((packed)) UnkElemData2661 {
    u8 bytes[35];
};

struct UnkElem2661 {
    char* vtable;
    UnkElemData2661 data;  // 0x8

    UnkElem2661(UnkElem2661&& other) : vtable((char*)g_unk_7101cac988 + 0x10), data(other.data) {}
};

void fn_710106ea40(void* dst, const void* src) {
    base::refl_IterValueMove<UnkElem2661>(dst, src);
}
