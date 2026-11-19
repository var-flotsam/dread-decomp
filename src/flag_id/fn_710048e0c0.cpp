// Deleting-destructor shape: destroy, then tail-call operator delete.
#include "types.h"

struct UnkChozoWarriorXAI_l48de58;
void fn_710048de58(UnkChozoWarriorXAI_l48de58* self);

void fn_710048e0c0(void* self) {
    fn_710048de58(static_cast<UnkChozoWarriorXAI_l48de58*>(self));
    operator delete(self);
}
