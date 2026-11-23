// CSunnapAIComponent unit: writes a float under a key into an in-memory file, at the offset already
// recorded for the key or, for a new key, at the end of what was written (recorded with type 'f').
// Names and layouts are placeholders.
#include <algorithm>

#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntFile;

// Where a keyed value sits in the file, and its type character.
struct UnkKeyedFileValue {
    u64 offset;
    u64 key;
    u32 type;

    UnkKeyedFileValue(u64 offset, u64 key, u32 type) : offset(offset), key(key), type(type) {}
    ~UnkKeyedFileValue() {}
};

struct UnkKeyedFileWriter {
    base::global::CRntVector<UnkKeyedFileValue> values;
    CRntFile file;  // 0x28

    UnkKeyedFileValue* Find(u64 key) {
        return std::find_if(values.data, values.data + values.count,
                            [key](const UnkKeyedFileValue& value) { return value.key == key; });
    }
};
static_assert(__builtin_offsetof(UnkKeyedFileWriter, file) == 0x28, "UnkKeyedFileWriter placeholder layout");

void fn_71006cc484(UnkKeyedFileWriter* self, const u64* key, f32 value) {
    UnkKeyedFileValue* entry = self->Find(*key);
    u64 end = fn_710007c1a4(&self->file);
    u64 offset;
    if (entry == self->values.data + self->values.count) {
        offset = end;
        self->values.EmplaceBack(UnkKeyedFileValue(end, *key, 'f'));
    } else {
        offset = entry->offset;
    }
    CRntFile_Seek(&self->file, offset, 0);
    CRntFile_WriteElements(&self->file, &value, sizeof(value), 1);
}
