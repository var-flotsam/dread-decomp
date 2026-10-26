// The port's entry point. At M0 it prints a banner and exits: the game sources compile
// (dread_game) but are not linked in until every function the boot path reaches has a source
// (M1, docs/PORT_PLAN.md). Until then the executable only proves that the runtime and the
// toolchain link.

#include <cstdio>

#include "dread_port.h"
#include "nn/os.h"

int main(int argc, char** argv) {
    dread_port_set_host_arguments(argc, argv);
    std::printf("dread port skeleton (M0): nn:: runtime stubs only, no game code linked.\n");
    std::printf("system tick: %lld (%lld Hz)\n", static_cast<long long>(nn::os::GetSystemTick().value),
                static_cast<long long>(nn::os::SystemTickFrequency));
    return 0;
}
