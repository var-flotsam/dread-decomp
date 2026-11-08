// arm_member_check: include/UnkArmMemberFn.h's DREAD_CALL_MEMBER on member function pointers
// written in the ARM C++ ABI's encoding, as the game stores them. Built for the host (x86-64);
// exits non-zero on the first failed check.
//   arm_member_check

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "UnkArmMemberFn.h"

namespace {

int g_failed = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++g_failed;
    }
}

struct Target;
typedef void (Target::*Method)(bool, float);

void* g_self = nullptr;
int g_which = 0;
float g_value = 0;

void First(void* self, bool flag, float v) {
    g_self = self;
    g_which = flag ? 1 : -1;
    g_value = v;
}

void Second(void* self, bool flag, float v) {
    g_self = self;
    g_which = flag ? 2 : -2;
    g_value = v;
}

// {ptr, adj} as the ARM C++ ABI writes it: adj = 2 * this-offset + (virtual ? 1 : 0).
Method Arm(std::uint64_t ptr, std::int64_t offset, bool is_virtual) {
    std::uint64_t words[2] = {ptr, static_cast<std::uint64_t>(offset * 2 + (is_virtual ? 1 : 0))};
    Method m;
    std::memcpy(&m, words, sizeof(m));
    return m;
}

}  // namespace

int main() {
    // A non-virtual function: ptr is its address, `this` unmoved.
    alignas(16) char object[0x40] = {};
    Target* obj = reinterpret_cast<Target*>(object);
    DREAD_CALL_MEMBER(obj, Arm(reinterpret_cast<std::uint64_t>(&First), 0, false))(true, 1.5f);
    Check(g_which == 1 && g_self == object && g_value == 1.5f, "a non-virtual function, this unmoved");

    // A virtual one: `this` moved by 0x10, ptr the byte offset 0x98 of the slot in that vtable.
    void* vtable[0x20] = {};
    vtable[0x98 / 8] = reinterpret_cast<void*>(&Second);
    void* vptr = vtable;
    std::memcpy(object + 0x10, &vptr, sizeof(vptr));
    DREAD_CALL_MEMBER(obj, Arm(0x98, 0x10, true))(false, -2.0f);
    Check(g_which == -2 && g_self == object + 0x10 && g_value == -2.0f,
          "a virtual function: slot 0x98 of the adjusted object's vtable, called on it");

    if (g_failed) {
        std::fprintf(stderr, "arm_member_check: %d failed\n", g_failed);
        return 1;
    }
    std::printf("arm_member_check: all passed\n");
    return 0;
}
