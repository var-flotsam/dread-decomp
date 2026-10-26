// Destructors of a pair of a string id (default pool) at 0 and a file-path id at 8, which release
// both ids and reset them to the empty string: fn_710001cbd8, fn_710001cfb4 and fn_710008d8e8.

#include "base/UnkStrIdPathPair.h"
#include "types.h"

void fn_710001cbd8(UnkStrIdPathPair* self) { self->~UnkStrIdPathPair(); }

void fn_710001cfb4(UnkStrIdPathPair* self) { self->~UnkStrIdPathPair(); }

void fn_710008d8e8(UnkStrIdPathPair* self) { self->~UnkStrIdPathPair(); }
