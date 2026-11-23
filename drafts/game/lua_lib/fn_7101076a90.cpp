// lua_lib's unit (2665): writes a three-float vector property into a property file by name. The
// first write of a name appends an entry (its offset, the name's key and the type 'v') and the name
// itself; a later one overwrites the value in place. Names and layouts are placeholders.
#include <algorithm>
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"
#include "../../../src/game_s7/UnkIter.h"

using base::global::CStrId;

// Where a property's value sits in the file, by the key of its name.
struct UnkLuaPropertyEntry {
    u64 offset;
    u64 key;
    u32 type;

    UnkLuaPropertyEntry(u64 offset, u64 key, u32 type) : offset(offset), key(key), type(type) {}
    UnkLuaPropertyEntry(const UnkLuaPropertyEntry& other)
        : offset(other.offset), key(other.key), type(other.type) {}
};

struct UnkLuaPropertyFile {
    base::global::CRntVector<UnkLuaPropertyEntry> entries;  // 0x0
    base::global::CRntFile file;                            // 0x28
    base::UnkStrIdVec names;                                // 0x60
};

void fn_7101076a90(UnkLuaPropertyFile* self, const CStrId* name, const f32* value) {
    UnkIter<UnkLuaPropertyEntry> first{self->entries.data};
    UnkIter<UnkLuaPropertyEntry> it =
        std::find_if(first, first + self->entries.count,
                     [name](const UnkLuaPropertyEntry& e) { return name->data->Key() == e.key; });
    u64 end_of_file = fn_710007c1a4(&self->file);
    u64 offset;
    UnkIter<UnkLuaPropertyEntry> last{self->entries.data};
    if (it != last + self->entries.count) {
        offset = (*it).offset;
    } else {
        offset = end_of_file;
        self->entries.EmplaceBack(UnkLuaPropertyEntry(end_of_file, name->data->Key(), 'v'));
    }
    fn_710007c00c(&self->file, offset, 0);
    fn_710007c450(&self->file, value, 0xc, 1);

    UnkIter<CStrId> names_first{self->names.data};
    UnkIter<CStrId> names_last = names_first + self->names.count;
    if (std::find(names_first, names_last, *name) == names_last)
        self->names.PushBack(*name);
}
