// CCharClassVulkranAIComponent's unit (tools/tu_map.py unit 1083): the destructor of a class with vtable
// 0x7101a687e8: a string id at +0x6d8, an array of 0x48-byte records at +0x690, four records at +0x648,
// +0x600, +0x5b8 and +0x570, then the base's destructor fn_7100416e80. A record is a plain array and a
// string id. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "types.h"

extern void* g_unk_7101a687e8[];
struct UnkStruct_7100416e80;

struct UnkVulkranValues_A4_b70 : base::UnkVector<u32> {
    ~UnkVulkranValues_A4_b70() { Free(); }
};

struct UnkVulkranRecord_A4_b70 {
    UnkVulkranValues_A4_b70 values;
    base::UnkStrIdMember id;
    u8 field_0x30[0x18];
};
static_assert(sizeof(UnkVulkranRecord_A4_b70) == 0x48, "UnkVulkranRecord_A4_b70 layout");

struct UnkCharClassVulkranDtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkCharClassVulkranDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a687e8); }
    static void base(char* self) { fn_7100416e80(reinterpret_cast<UnkStruct_7100416e80*>(self)); }
};

struct UnkVulkranRecords_A4_b70 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkVulkranRecord_A4_b70>*>(self + 0x690)->Free();
    }
};

template <u32 Offset>
using UnkVulkranRecordAt_A4_b70 = base::UnkMemberAt<UnkVulkranRecord_A4_b70, Offset>;

__attribute__((flatten)) void fn_71006da89c(char* self) {
    base::refl_Dtor<UnkCharClassVulkranDtor_A4_b70, base::UnkStrIdAt<0x6d8>, UnkVulkranRecords_A4_b70,
                    UnkVulkranRecordAt_A4_b70<0x648>, UnkVulkranRecordAt_A4_b70<0x600>,
                    UnkVulkranRecordAt_A4_b70<0x5b8>, UnkVulkranRecordAt_A4_b70<0x570>>(self);
}
