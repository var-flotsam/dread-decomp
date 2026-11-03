// The clone of a callable a std::function wraps in unit 108 of tools/tu_map.py (a vtable and one
// captured word, allocated with operator new); the vtable symbol is its address point. Names and
// layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101a1bdc0[] __attribute__((visibility("hidden")));

struct UnkUnit108Capture {
    void* owner;
};

struct UnkUnit108Callable {
    char* vtable;
    UnkUnit108Capture capture;
    explicit UnkUnit108Callable(const UnkUnit108Capture& c) : vtable(g_unk_7101a1bdc0), capture(c) {}
};

UnkUnit108Callable* fn_7100122d70(const UnkUnit108Callable* self) {
    return new UnkUnit108Callable(self->capture);
}
