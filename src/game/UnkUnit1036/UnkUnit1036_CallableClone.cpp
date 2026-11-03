// The clone of a callable a std::function wraps in unit 1036 of tools/tu_map.py (a vtable and one
// captured word, allocated with operator new); a copy of fn_7100122d70
// (src/base/UnkUnit108/UnkUnit108_CallableClone.cpp). Names and layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101a62700[] __attribute__((visibility("hidden")));

struct UnkUnit1036Capture {
    void* owner;
};

struct UnkUnit1036Callable {
    char* vtable;
    UnkUnit1036Capture capture;
    explicit UnkUnit1036Callable(const UnkUnit1036Capture& c) : vtable(g_unk_7101a62700), capture(c) {}
};

UnkUnit1036Callable* fn_71006a12cc(const UnkUnit1036Callable* self) {
    return new UnkUnit1036Callable(self->capture);
}
