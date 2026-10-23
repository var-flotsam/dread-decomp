// Destruction of arrays of values keyed by string ids (base::UnkKeyedValueArray, as the generated
// fn_7100aeff00): the keys are released, the values destroyed, then both freed. Values destroyed
// by fn_7100020164 in an object with its own vtable (unk_7101a19590), the array at 8:
// fn_710001fa30; polymorphic values destroyed through vtable slot 0, the array at 0 (0x60-byte
// values fn_710025fc54, 0xb8-byte values fn_71000ddd84).
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"

extern char unk_7101a19590[];
void fn_7100020164(char*);

struct UnkKeyedRecords_0x48;
template <>
struct UnkCloneDtorTraits<UnkKeyedRecords_0x48> {
    static void destroy(char* p) { fn_7100020164(p); }
};

__attribute__((flatten)) void fn_710001fa30(char* self) {
    *reinterpret_cast<char**>(self) = unk_7101a19590 + 0x10;
    base::refl_KeyedValueArrayDtor<UnkKeyedRecords_0x48, UnkCloneDtorElem<UnkKeyedRecords_0x48, 0x48>, 0x8>(self);
}

struct UnkKeyedObjects;

__attribute__((flatten)) void fn_710025fc54(char* self) {
    base::refl_KeyedValueArrayDtor<UnkKeyedObjects, UnkClonePolyElem<UnkKeyedObjects, UnkClonePolyA, 0x60>, 0>(self);
}

__attribute__((flatten)) void fn_71000ddd84(char* self) {
    base::refl_KeyedValueArrayDtor<UnkKeyedObjects, UnkClonePolyElem<UnkKeyedObjects, UnkClonePolyA, 0xb8>, 0>(self);
}
