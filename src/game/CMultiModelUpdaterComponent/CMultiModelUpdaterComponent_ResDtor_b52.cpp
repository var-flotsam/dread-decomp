// CMultiModelUpdaterComponent unit (unit 1426 of tools/tu_map.py): a destructor of a resource
// reference (base/UnkStrIdPathPair.h's UnkStrIdPathResource), fn_7100938dec: the counted resource is
// dropped, then the file-path id and the string id are released. The same as the end of
// fn_7100026f70 (src/base/UnkUnit32/UnkUnit32_Release.cpp). Names are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "types.h"

void fn_7100938dec(UnkStrIdPathResource* self) {
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
    self->ids.path.~UnkStrIdMemberOf();
    self->ids.id.~UnkStrIdMemberOf();
}
