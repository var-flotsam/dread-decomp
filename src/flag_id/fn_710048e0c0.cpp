// Deleting-destructor shape: destroy, then tail-call operator delete.
#include "types.h"

void fn_710048de58(void* self);

void fn_710048e0c0(void* self) {
    fn_710048de58(self);
    operator delete(self);
}
