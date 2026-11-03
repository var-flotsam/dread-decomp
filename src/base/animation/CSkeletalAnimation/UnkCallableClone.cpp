// The clone of a callable a std::function wraps in the CSkeletalAnimation unit (a vtable and one
// captured word, allocated with operator new), as in src/game/GUI/UnkFunctionHolderMethods.cpp.
// Names and layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101a19410[];

struct UnkSkelAnimCapture {
    void* owner;
};

struct UnkSkelAnimCallable {
    char* vtable;
    UnkSkelAnimCapture capture;
    explicit UnkSkelAnimCallable(const UnkSkelAnimCapture& c) : vtable(g_unk_7101a19410 + 0x10), capture(c) {}
};

UnkSkelAnimCallable* fn_7100014174(const UnkSkelAnimCallable* self) {
    return new UnkSkelAnimCallable(self->capture);
}
