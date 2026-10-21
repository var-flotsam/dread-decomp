// The default allocator: the one UnkVector and other engine containers are created with.
#include "base/UnkVector.h"

// Set at startup; not reached through the GOT, so it is defined in this module.
extern base::UnkAllocator* g_unk_7101d58ed0 __attribute__((visibility("hidden")));

base::UnkAllocator* fn_7100083990() {
    return g_unk_7101d58ed0;
}
