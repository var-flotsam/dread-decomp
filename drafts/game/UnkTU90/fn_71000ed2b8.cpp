// Unit 90 of tools/tu_map.py (no class hint): the version writer's start of the root value: pushes an
// entry for its type (owned as the previous entry is), writes the name "Root", then, for a pointer
// type, the type's name as a CName. Names and layouts are placeholders.
#include "base/global/CName.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "types.h"

using base::global::CName;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValueRef;

struct UnkWriterEntry_b0202 {
    u64 key;     // 0x0: the type
    u64 owner;   // 0x8
    bool flag;   // 0x10: a value was written with a length prefix at `extra`
    u64 extra;   // 0x18: the file position of the prefix
};
static_assert(sizeof(UnkWriterEntry_b0202) == 0x20, "entry");

struct UnkWriter_b0202 {
    u32 count;
    u8 entries_0x4[0xc80];
    u8 pad_0xc84[4];
    base::global::CRntFile* file;  // 0xc88
};
static_assert(__builtin_offsetof(UnkWriter_b0202, file) == 0xc88, "writer");

static inline UnkWriterEntry_b0202* Entries(UnkWriter_b0202* self) {
    return reinterpret_cast<UnkWriterEntry_b0202*>(self->entries_0x4);
}

void fn_71000ed480(UnkWriter_b0202* self, const UnkReflValueRef* ref);
bool CType_ObjectIsKindOf(const UnkReflObject* obj, const CType* type);

s32 fn_71000ed2b8(UnkWriter_b0202* self, const CType* type) {
    u32 count = self->count;
    u64 owner = count ? Entries(self)[count - 1].owner : 0;
    Entries(self)[count] = UnkWriterEntry_b0202{reinterpret_cast<u64>(type), owner, false, 0};
    self->count = count + 1;

    CName root("Root");
    CRntFile_Write(self->file, &root, sizeof(root));

    CStrId name = g_unk_7101d470e0;
    if (CType_ObjectIsKindOf(reinterpret_cast<const UnkReflObject*>(type), reinterpret_cast<const CType*>(fn_710009bedc())))
        name = type->sName;
    if (name.data != &g_unk_7101d47108->empty) {
        CName hashed(base::global::UnkNameHash(), name.data->Key());
        UnkReflValueRef value = {fn_710000fd7c(), &hashed};
        fn_71000ed480(self, &value);
    }
    return 1;
}
