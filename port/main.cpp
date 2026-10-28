// The port's entry point. With the game linked in (tools/port_link.py --executable, or
// DREAD_LINK_GAME), the data loader has already mapped the dump and run nninitStartup by the time
// main starts (runtime/src/image.cpp). main passes its arguments on, runs the game's static
// initialisers (main's .init_array, which the SDK runs before nnMain) and calls nnMain
// (src/engine/nnMain.cpp), which runs the system start-up and the game. Without the game it prints
// a banner: the runtime and the toolchain link.

#include <cstdio>

#include "dread_port.h"
#include "nn/os.h"

extern "C" void nnMain() __attribute__((weak));

int main(int argc, char** argv) {
    dread_port_set_host_arguments(argc, argv);
    if (nnMain) {
        dread_port_run_game_initialisers();
        nnMain();
        return 0;
    }
    std::printf("dread port: nn:: runtime only, no game code linked.\n");
    std::printf("system tick: %lld (%lld Hz)\n", static_cast<long long>(nn::os::GetSystemTick().value),
                static_cast<long long>(nn::os::SystemTickFrequency));
    return 0;
}
