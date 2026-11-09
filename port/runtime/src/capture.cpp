// Entry-state snapshots for the semantic-lift oracle (tools/pcode_emu.py load_snapshot, format
// "port-core"). A function the port has no code for, and whose lift the oracle could not judge on
// made-up memory (the original faulted on every input), can be judged on the state a real run gives
// it: tools/port_link.py --capture ADDR,... links a thunk for each such trap that saves the argument
// registers and calls dread_port_capture before the trap. With $DREAD_SNAPSHOTS set to a directory,
// the first entry of each writes there:
//   fn_<addr>.core  the game's writable memory as it was: the image's writable segments (.data,
//                   .bss), the memory heap's handed-out part and the calling thread's stack, one
//                   region after another, every all-zero page left as a hole (a sparse file);
//   fn_<addr>.snap  the snapshot magic, then one JSON line: the AArch64 argument registers (x0-x5 from
//                   rdi..r9, v0-v7 from xmm0-xmm7: the functions captured take no stack arguments and
//                   return no result through x8, which port_link checks), and each region's address
//                   range and offset in the core file.
// The image is mapped at the console's address, so nothing is translated. Read-only pages (code,
// .rodata, the vtables) are left out: the oracle reads those from the original image. The files hold
// game memory: they stay in build/, never in the repository. Linux only (the Docker runs).

#include "dread_port.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>

namespace {

constexpr u64 kPage = 4096;
constexpr u64 kStackBytes = 1 << 20;  // the part of the thread's stack above sp that is kept

struct Region {
    u64 start, end;
};

// Fixed storage only: the global operator new hands out the game's heap (src/heap.cpp), which a
// capture must not change while it copies it.
constexpr int kMaxRegions = 256;
Region g_regions[kMaxRegions];
int g_region_count = 0;
char g_json[1 << 16];
size_t g_json_len = 0;

void Append(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int n = std::vsnprintf(g_json + g_json_len, sizeof(g_json) - g_json_len, format, args);
    va_end(args);
    if (n > 0)
        g_json_len = g_json_len + static_cast<size_t>(n) < sizeof(g_json) ? g_json_len + n : sizeof(g_json) - 1;
}

void AddRegion(u64 start, u64 end) {
    if (g_region_count < kMaxRegions)
        g_regions[g_region_count++] = {start, end};
}

#if defined(__linux__)
// Adds the writable mappings of /proc/self/maps that overlap [lo, hi), clipped to it.
void AddWritableMappings(u64 lo, u64 hi) {
    FILE* f = std::fopen("/proc/self/maps", "r");
    if (!f)
        return;
    char line[512];
    while (std::fgets(line, sizeof(line), f)) {
        unsigned long long a, b;
        char perms[8];
        if (std::sscanf(line, "%llx-%llx %7s", &a, &b, perms) != 3 || perms[0] != 'r' || perms[1] != 'w')
            continue;
        u64 s = a > lo ? a : lo, e = b < hi ? b : hi;
        if (s < e)
            AddRegion(s, e);
    }
    std::fclose(f);
}

bool Zero(const unsigned char* p) {
    for (u64 i = 0; i < kPage; i += 8) {
        u64 w;
        std::memcpy(&w, p + i, 8);
        if (w)
            return false;
    }
    return true;
}

void Capture(u64 address, const u64* regs, const char* dir) {
    const u64 sp = regs[22];
    g_region_count = 0;
    if (dread_port_image_end)
        AddWritableMappings(0x7100000000ull, dread_port_image_end);
    const char* heap = nullptr;
    u64 heap_used = 0;
    dread_port_game_heap(&heap, &heap_used);
    if (heap && heap_used)
        AddRegion(reinterpret_cast<u64>(heap), reinterpret_cast<u64>(heap) + heap_used);
    AddWritableMappings(sp & ~(kPage - 1), sp + kStackBytes);

    char core_name[64], core_path[1024], snap_path[1024];
    std::snprintf(core_name, sizeof(core_name), "fn_%llx.core", static_cast<unsigned long long>(address));
    std::snprintf(core_path, sizeof(core_path), "%s/%s", dir, core_name);
    std::snprintf(snap_path, sizeof(snap_path), "%s/fn_%llx.snap", dir, static_cast<unsigned long long>(address));

    // Standard I/O only (the port's link has no POSIX file calls): a seek past the end leaves a hole.
    FILE* core = std::fopen(core_path, "wb");
    if (!core) {
        std::fprintf(stderr, "dread: capture: cannot write %s\n", core_path);
        return;
    }
    g_json_len = 0;
    Append("{\"format\": \"port-core\", \"core\": \"%s\", \"x\": [", core_name);
    for (int i = 0; i < 31; ++i)
        Append("%s\"%llx\"", i ? ", " : "", static_cast<unsigned long long>(i < 6 ? regs[i] : 0));
    Append("], \"q\": [");
    for (int i = 0; i < 32; ++i) {
        // q<n> as one 128-bit value: the high word first.
        unsigned long long lo = i < 8 ? regs[6 + 2 * i] : 0, hi = i < 8 ? regs[7 + 2 * i] : 0;
        Append("%s\"%llx%016llx\"", i ? ", " : "", hi, lo);
    }
    Append("], \"sp\": \"%llx\", \"stack\": \"\", \"pages\": 0, \"regions\": [", static_cast<unsigned long long>(sp));
    u64 offset = 0, written = 0;
    for (int k = 0; k < g_region_count; ++k) {
        const Region& r = g_regions[k];
        Append("%s[\"%llx\", \"%llx\", \"%llx\"]", k ? ", " : "", static_cast<unsigned long long>(r.start),
               static_cast<unsigned long long>(r.end), static_cast<unsigned long long>(offset));
        for (u64 a = r.start; a < r.end; a += kPage) {
            const auto* p = reinterpret_cast<const unsigned char*>(a);
            if (Zero(p))
                continue;
            if (std::fseek(core, static_cast<long>(offset + (a - r.start)), SEEK_SET) != 0 ||
                std::fwrite(p, 1, kPage, core) != kPage) {
                std::fprintf(stderr, "dread: capture: write to %s failed\n", core_path);
                std::fclose(core);
                return;
            }
            ++written;
        }
        offset += r.end - r.start;
    }
    Append("]}\n");
    // The trailing holes too: the file ends at the last region's end.
    static const unsigned char zero = 0;
    if (offset && (std::fseek(core, static_cast<long>(offset - 1), SEEK_SET) != 0 || std::fwrite(&zero, 1, 1, core) != 1))
        std::fprintf(stderr, "dread: capture: cannot size %s\n", core_path);
    std::fclose(core);
    FILE* s = std::fopen(snap_path, "wb");
    if (!s) {
        std::fprintf(stderr, "dread: capture: cannot write %s\n", snap_path);
        return;
    }
    std::fputs("DSNAP1\n", s);
    std::fwrite(g_json, 1, g_json_len, s);
    std::fclose(s);
    std::fprintf(stderr, "dread: capture: fn_%llx's entry state -> %s (%d regions, %llu MB, %llu MB of it data)\n",
                 static_cast<unsigned long long>(address), snap_path, g_region_count,
                 static_cast<unsigned long long>(offset >> 20), static_cast<unsigned long long>((written * kPage) >> 20));
}
#endif

std::mutex g_lock;
u64 g_done[64];
int g_done_count = 0;

}  // namespace

extern "C" void dread_port_capture(u64 address, const u64* regs) {
#if defined(__linux__)
    const char* dir = std::getenv("DREAD_SNAPSHOTS");
    if (!dir || !*dir)
        return;
    std::lock_guard<std::mutex> hold(g_lock);
    for (int i = 0; i < g_done_count; ++i)
        if (g_done[i] == address)
            return;
    if (g_done_count == 64)
        return;
    g_done[g_done_count++] = address;
    Capture(address, regs, dir);
#else
    (void)address;
    (void)regs;
#endif
}
