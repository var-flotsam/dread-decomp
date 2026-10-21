// The string-id constructor (UnkStrIdBase(const char*, bool)), out of line. Game code calls
// it for every CStrId and UnkStrId it builds from a C string.
#include <new>

#include "base/global/CStrId.h"

using base::global::UnkStrIdBase;
using base::global::UnkStrIdInline;

void fn_71000003d4(UnkStrIdBase* self, const char* str, bool flag) {
    new (self) UnkStrIdBase(UnkStrIdInline(), str, flag);
}
