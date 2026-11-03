// The clone of a callable a std::function wraps in the CFixedDynamicSmartLinkRule unit (a vtable
// and two captured words, allocated with operator new); the vtable symbol is its address point.
// Names and layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101a20e20[] __attribute__((visibility("hidden")));

struct UnkSmartLinkRuleCapture {
    void* a;
    void* b;
};

struct UnkSmartLinkRuleCallable {
    char* vtable;
    UnkSmartLinkRuleCapture capture;
    explicit UnkSmartLinkRuleCallable(const UnkSmartLinkRuleCapture& c) : vtable(g_unk_7101a20e20), capture(c) {}
};

UnkSmartLinkRuleCallable* fn_7100273eb0(const UnkSmartLinkRuleCallable* self) {
    return new UnkSmartLinkRuleCallable(self->capture);
}
