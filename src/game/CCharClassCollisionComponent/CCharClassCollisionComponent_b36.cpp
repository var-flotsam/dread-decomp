// Functions in the CCharClassCollisionComponent unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;

struct UnkCollisionClassDef {
    u8 pad_0x0[0x58];
    CRntString field_0x58;
    bool field_0x88;
    bool field_0x89;
};

bool fn_71011aced4(UnkCollisionClassDef* self, CRntFile* file);  // the base read

// The read: after the base, a string into +0x58 and two flags into +0x88 and +0x89.
bool fn_710079a840(UnkCollisionClassDef* self, CRntFile* file) {
    bool result = fn_71011aced4(self, file);
    if (result) {
        const char* str;
        fn_710007cee4(file, &str);
        fn_710007e77c(&self->field_0x58, str);
        fn_710007c714(file, &self->field_0x88, 1, 1);
        fn_710007c714(file, &self->field_0x89, 1, 1);
    }
    return result;
}
