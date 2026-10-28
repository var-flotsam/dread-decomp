// A crash report for the port: on a fault (SIGSEGV, SIGBUS, SIGILL, SIGFPE) print the faulting
// address, the instruction and the return addresses, then die with the signal as before. On
// SIGABRT (a trap's abort, or the C library's: "munmap_chunk(): invalid pointer") only the return
// addresses, which say who called. The traps (missing.cpp) name the next function the boot needs;
// a fault names nothing, so this says where it happened. Symbolise the addresses with the executable:
//   llvm-symbolizer --obj=build/port/linux/link/dread -f -C 0x... 0x...
// Installed before the data loader and the game's static initialisers (constructor priority 100;
// image.cpp maps the image at 101). Linux and macOS only; elsewhere it does nothing.

#include "dread_port.h"

#if defined(__linux__) || defined(__APPLE__)

#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

namespace {

constexpr u64 kImageBase = 0x7100000000;
constexpr u64 kImageEnd = 0x7172a19000;  // the end of main's .bss (image.cpp)
constexpr int kSignals[] = {SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGABRT};

alignas(16) char g_alt_stack[64 * 1024];  // a stack overflow still gets its report

const char* Where(u64 address) {
    if (address >= kImageBase && address < kImageEnd)
        return " (in the game image)";
    if (address < 0x10000)
        return " (a null pointer, or near one)";
    return "";
}

u64 InstructionOf(void* context) {
    if (!context)
        return 0;
#if defined(__linux__) && defined(__x86_64__)
    return static_cast<u64>(static_cast<ucontext_t*>(context)->uc_mcontext.gregs[REG_RIP]);
#elif defined(__linux__) && defined(__aarch64__)
    return static_cast<u64>(static_cast<ucontext_t*>(context)->uc_mcontext.pc);
#elif defined(__APPLE__) && defined(__x86_64__)
    return static_cast<u64>(static_cast<ucontext_t*>(context)->uc_mcontext->__ss.__rip);
#elif defined(__APPLE__) && defined(__aarch64__)
    return static_cast<u64>(static_cast<ucontext_t*>(context)->uc_mcontext->__ss.__pc);
#else
    return 0;
#endif
}

void OnFault(int sig, siginfo_t* info, void* context) {
    if (sig == SIGABRT) {
        std::fprintf(stderr, "dread: %s\n", strsignal(sig));  // no faulting address: abort() was called
    } else {
        const u64 address = reinterpret_cast<u64>(info ? info->si_addr : nullptr);
        const u64 pc = InstructionOf(context);
        std::fprintf(stderr, "dread: %s at address 0x%llx%s, instruction 0x%llx%s\n", strsignal(sig),
                     static_cast<unsigned long long>(address), Where(address),
                     static_cast<unsigned long long>(pc), Where(pc));
    }
    void* frames[64];
    const int n = backtrace(frames, 64);
    std::fprintf(stderr, "dread: return addresses (llvm-symbolizer --obj=<the executable> -f -C <addresses>):");
    for (int i = 0; i < n; ++i)
        std::fprintf(stderr, " %p", frames[i]);
    std::fprintf(stderr, "\n");
    std::fflush(stderr);
    // Die of the same signal, as without the handler (the exit status says which).
    signal(sig, SIG_DFL);
    raise(sig);
}

__attribute__((constructor(100))) void InstallFaultHandler() {
    void* warm[1];
    backtrace(warm, 1);  // loads the unwinder now, not inside the handler
    stack_t ss = {};
    ss.ss_sp = g_alt_stack;
    ss.ss_size = sizeof(g_alt_stack);
    sigaltstack(&ss, nullptr);
    struct sigaction sa = {};
    sa.sa_sigaction = OnFault;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_RESETHAND;
    sigemptyset(&sa.sa_mask);
    for (int sig : kSignals)
        sigaction(sig, &sa, nullptr);
}

}  // namespace

#endif
