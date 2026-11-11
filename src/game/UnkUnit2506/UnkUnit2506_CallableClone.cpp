// Like fn_7100014174 (src/base/animation/CSkeletalAnimation/UnkCallableClone.cpp): the clone of a
// callable a std::function wraps (a vtable and two captured words, allocated with operator new).
// Names and layouts are placeholders.
#include <new>

#include "game/UnkTU2506.h"
#include "types.h"

struct UnkUnit2506Capture {
    void* owner;
    void* arg;
};

struct UnkUnit2506Callable {
    char* vtable;
    UnkUnit2506Capture capture;
    explicit UnkUnit2506Callable(const UnkUnit2506Capture& c) : vtable(g_unk_7101aa87e8 + 0x10), capture(c) {}
};

UnkUnit2506Callable* fn_7100d4b868(const UnkUnit2506Callable* self) {
    return new UnkUnit2506Callable(self->capture);
}
