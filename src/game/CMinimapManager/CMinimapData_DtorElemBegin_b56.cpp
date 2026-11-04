// CMinimapData's unit (tools/tu_map.py unit 2563): begin and end (base::refl_Begin, base::refl_End)
// of an array of 0x138-byte elements destroyed by fn_7100e4e310, whose iterator assignment
// src/clones/clones_05.cpp instantiates. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

void fn_7100e4e310(char*);

struct UnkElem138_b56 {
    u64 unk_0x0[0x138 / 8];
    ~UnkElem138_b56() { fn_7100e4e310(reinterpret_cast<char*>(this)); }
};

void fn_7100e62f54(const base::UnkVector<UnkElem138_b56>* v, base::UnkCollectionIter<UnkElem138_b56>* out) {
    base::refl_Begin(v, out);
}

void fn_7100e63034(const base::UnkVector<UnkElem138_b56>* v, base::UnkCollectionIter<UnkElem138_b56>* out) {
    base::refl_End(v, out);
}
