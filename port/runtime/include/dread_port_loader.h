#pragma once

// The data loader: maps the game's main NSO, read from the user's own dump at run time, at its
// original addresses, so that every g_unk_<addr> symbol tools/port_link.py defines as an absolute
// address (the game's .rodata, .data and .bss) points at the game's bytes. Nothing of the game is
// in git or in the executable: the bytes come from orig/<version>/exefs/main when the port starts.
//
// Why the original addresses: 5,467 data symbols are referenced PC-relatively by the compiled
// game code (the visibility("hidden") declarations, R_X86_64_PC32), so the port's code must sit
// within 2 GB of every one of them, and they span 0x71015af379 to 0x7172a112e8 (rodata to the end
// of the 1.82 GB .bss). Mapping the image where the game linked it keeps every absolute symbol and
// every pointer inside .data valid; the port's executable is linked just below or above it.
//
// What the loader changes in the game's bytes: the dynamic relocations, as the console's loader
// applies them, except that a pointer into the game's .text (a vtable slot, a callback; 128,576 of
// the 145,813 relative relocations of 2.1.0) is translated to the port's x86-64 code for that
// function: the function table tools/port_link.py generates (dread_port_functions, every
// address the link defines, sorted). A text address with no entry gets a trap thunk that reports
// it (dread_port_missing_function), like the link's own traps. Imports (symbol relocations: the
// PLT and GOT, __cxa_pure_virtual in vtables, the C++ ABI's type-info vtables) resolve by name
// against the generated import table (the runtime's nn:: functions and the host's C and C++ ABI
// names); an unresolved one gets a thunk to dread_port_missing_import with its name.
//
// The core (dread_port_load_image) is freestanding: no allocation, no standard library, the host's
// services passed in. That keeps it testable without the rest of the port
// (tools/test_port_loader.py builds it into a DLL with nothing but clang and lld and runs it on
// the dump), and the runtime's host layer (runtime/src/image.cpp) small. The C++ ABI must be the
// Itanium one (Linux, macOS, MinGW on Windows): the vtables in the game's .data are used as they
// are, and the 71 vtables nothing defines in the link are the game's own.

#include "types.h"

extern "C" {

// One function the port defines, by its original address. Sorted by address.
struct DreadPortFunction {
    u64 address;
    void* code;
};

// One name an import can resolve to.
struct DreadPortImport {
    const char* name;
    void* code;
};

// What the host provides. All but log are required.
struct DreadPortHost {
    // Reserve and commit `size` bytes at exactly `address`, readable and writable, zero-filled.
    // Returns the address, or null if the range is not free.
    void* (*map_fixed)(u64 address, u64 size);
    // Make [address, address + size) read-only. Returns false on failure.
    bool (*protect_read_only)(void* address, u64 size);
    // Readable, writable and executable memory for `size` bytes of thunks, anywhere.
    void* (*map_executable)(u64 size);
    // Make thunk memory executable and no longer writable (may be a no-op).
    bool (*seal_executable)(void* address, u64 size);
    // One line of progress or error, no newline. Optional.
    void (*log)(const char* line);
};

struct DreadPortLoadOptions {
    u64 base;                    // where the game linked main: 0x7100000000
    const u8* expected_build_id;  // 0x20 bytes, the NSO header's module ID; null skips the check
    const DreadPortFunction* functions;
    u64 function_count;
    const DreadPortImport* imports;
    u64 import_count;
    // The addresses the link defines as data symbols (g_unk_<addr>): each must fall inside the
    // mapped .rodata, .data or .bss, or the load fails. May be empty.
    const u64* data_references;
    u64 data_reference_count;
    // Where the trap thunks jump: (address) for a text address with no function, (name) for an
    // import with no definition. Both receive their argument in the first integer register of
    // either x86-64 convention (rdi and rcx are both loaded).
    void* missing_function;
    void* missing_import;
};

enum DreadPortLoadStatus : s32 {
    DreadPortLoad_Ok = 0,
    DreadPortLoad_NotNso,           // bad magic or a segment outside the file
    DreadPortLoad_WrongBuild,       // module ID differs from expected_build_id
    DreadPortLoad_BadSegment,       // a segment does not decompress to its size, or overlaps
    DreadPortLoad_BadModule,        // no MOD0 header or no dynamic section
    DreadPortLoad_MapFailed,        // the image's range is not free
    DreadPortLoad_BadRelocation,    // a relocation type or target the loader does not know
    DreadPortLoad_BadTable,         // the function table is not sorted
    DreadPortLoad_ThunkMapFailed,
    DreadPortLoad_DataOutsideImage, // a data reference outside .rodata, .data and .bss
    DreadPortLoad_ProtectFailed,
};

struct DreadPortImageInfo {
    u64 base;
    u64 text_end, rodata_begin, rodata_end, data_begin, data_end, bss_end;  // offsets from base
    u8 build_id[0x20];
    u64 relocations;          // all applied
    u64 code_pointers;        // relative relocations into .text
    u64 code_translated;      // of those, found in the function table
    u64 imports;              // symbol relocations
    u64 imports_resolved;
    u64 thunks;               // trap thunks written
    u64 data_references_checked;
    u64 first_bad_value;      // the offending relocation type, address or offset when a load fails
};

// Maps `nso` (the whole file) and applies its relocations. On failure nothing stays mapped only
// if the failure precedes map_fixed; after it the range stays reserved (the process is expected
// to stop).
DreadPortLoadStatus dread_port_load_image(const u8* nso, u64 nso_size, const DreadPortLoadOptions* options,
                                          const DreadPortHost* host, DreadPortImageInfo* info);

const char* dread_port_load_status_text(DreadPortLoadStatus status);

}  // extern "C"
