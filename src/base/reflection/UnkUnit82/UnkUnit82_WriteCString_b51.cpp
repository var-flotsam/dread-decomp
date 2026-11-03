// Unit 82 (base::reflection): a writer's handler for a reflected value that, when the value's
// type is a pointer to fn_7100008500's type (a C string, by the look of it), passes the
// pointed-to string to the writer's slot 0x168 and reports it handled (0). Names are
// placeholders inferred from the code.

#include "base/reflection/CPointerType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CPointerType;
using base::reflection::CType;
using base::reflection::UnkReflObject;

const base::reflection::CType* fn_7100008500();

struct UnkTypedValue_71000a33b4 {
    const CType* type;
    const char** ptr;
};

struct UnkWriter_71000a33b4;
typedef void (*UnkWriterFn_71000a33b4)(UnkWriter_71000a33b4*, const char*);
struct UnkWriter_71000a33b4 {
    UnkWriterFn_71000a33b4* vtable;
};

u32 fn_71000a33b4(UnkWriter_71000a33b4* self, const UnkTypedValue_71000a33b4* value) {
    const CType* type = value->type;
    const CPointerType* pt = fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_710009bedc())
                                 ? static_cast<const CPointerType*>(type)
                                 : nullptr;
    if (pt->m_pointee != fn_7100008500())
        return 1;
    const char* str = *value->ptr;
    UnkWriterFn_71000a33b4 fn = self->vtable[0x168 / 8];
    fn(self, str);
    return 0;
}
