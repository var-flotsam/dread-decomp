// The clone of a callable a std::function wraps in the CNavMeshGenerator unit (a vtable, a
// captured pointer and two 16-byte captures, allocated with operator new); the vtable symbol is
// its address point. Names and layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101a21080[] __attribute__((visibility("hidden")));

struct UnkNavGenCapture {
    struct Pair {
        u64 a, b;
    };
    void* owner;
    Pair first;
    Pair second;
};

struct UnkNavGenCallable {
    char* vtable;
    UnkNavGenCapture capture;
    explicit UnkNavGenCallable(const UnkNavGenCapture& c) : vtable(g_unk_7101a21080), capture(c) {}
};

UnkNavGenCallable* fn_71002a7d54(const UnkNavGenCallable* self) {
    return new UnkNavGenCallable(self->capture);
}
