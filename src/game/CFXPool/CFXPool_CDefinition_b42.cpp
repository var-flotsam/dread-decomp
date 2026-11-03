// CFXPool::CDefinition's unit (tools/tu_map.py, unit 2654): the moving "set element at key" of a
// reflected map of {file-path id, word} records (slot 0x148): the record is found or added out of line
// (fn_710104ff64), its id (from the file-path pool, g_unk_7101d47200) released as a destructor would (decrement and test in two steps, the empty
// id of a reloaded pool left) and replaced by the other's (left empty), then the word is copied.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

char* fn_710104ff64(void*, u32);  // as src/clones/ declares it

struct UnkFXPoolRecord_b42a3 {
    UnkStringData* id;
    u64 value;
};

void fn_710104f7a4(void* self, const u32* key, UnkFXPoolRecord_b42a3* other) {
    auto* e = reinterpret_cast<UnkFXPoolRecord_b42a3*>(fn_710104ff64(self, *key));
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (e->id && e->id != &pool->empty) {
            --e->id->ref_count;
            if (e->id->ref_count == 0) {
                fn_71000803a4(pool, e->id);
                pool = g_unk_7101d47200;
            }
        }
        e->id = &pool->empty;
    }
    e->id = other->id;
    other->id = &g_unk_7101d47200->empty;
    e->value = other->value;
}
