#pragma once

// nn::fs: the RomFS and save data. The boot path, in order: the system start-up fn_71001325d0
// (SetAllocator for the library's own memory, QueryMountRomCacheSize, MountRom("rom") over a cache
// it allocates), the shutdown fn_7100132730 (Unmount("rom")); then, reached through vtables, the
// engine's file object (fn_7101235090 opens, fn_71012351c0 sizes, fn_7101235224 and fn_7101235520
// read, fn_71012352b0 and fn_71012355b0 write, fn_710123517c flushes, fn_7101235138 closes), the
// sound banks (fn_7100100e74 reads rom:/packs/sounds/*.bfgrp whole), and the save system
// (fn_7100fc3510 mounts "savedata": EnsureSaveData, MountSaveData; fn_710002dedc to fn_710002eec8
// list, read, write, create and delete files and directories in it and CommitSaveData after each).
// On PC each mount is a host directory: "rom" the user's dump (orig/<version>/romfs, read-only),
// the save mount a directory of its own (DREAD_SAVE, default build/save); files.toc and the .pkg
// packs are read from the dump as they are.
//
// `main` imports (data/functions/): MountRom, QueryMountRomCacheSize, Unmount, SetAllocator,
// SetPriorityOnCurrentThread, OpenFile, CloseFile, ReadFile, WriteFile, FlushFile, GetFileSize,
// SetFileSize, CreateFile, DeleteFile, OpenDirectory, CloseDirectory, ReadDirectory,
// CreateDirectory, DeleteDirectory, EnsureSaveData, MountSaveData, CommitSaveData.
// All of them are declared below.

#include "nn/account.h"
#include "nn/nn_common.h"

namespace nn::fs {

// The game decodes a failure itself (fn_7101235090): module 2 in bits 0-8, the description from
// bit 9. It maps description 1 (path not found), 4000 to 4999 (data corrupted) and 7 (target
// locked) to its own codes, so the port returns those values where the console would.
constexpr u32 kModule = 2;
constexpr Result MakeResult(u32 description) { return Result{kModule | description << 9}; }
constexpr Result ResultPathNotFound() { return MakeResult(1); }
constexpr Result ResultPathAlreadyExists() { return MakeResult(2); }
constexpr Result ResultTargetLocked() { return MakeResult(7); }
constexpr Result ResultDirectoryNotEmpty() { return MakeResult(8); }
// The port's own: a host I/O error or a request the port refuses (a write to "rom", a path that
// leaves its mount). Description 6000 lies outside every range the game tests.
constexpr Result ResultHostFailure() { return MakeResult(6000); }

// An open file or directory as the game keeps it: one word, passed by value (fn_7101235090 keeps
// it at +8 of its file object and passes the word to every call).
struct FileHandle {
    void* handle;
};
struct DirectoryHandle {
    void* handle;
};
static_assert(sizeof(FileHandle) == 8 && sizeof(DirectoryHandle) == 8, "one word in the game's objects");

// OpenFile's mode: the engine's file object reads with 1 and the save writer (fn_710002eec8)
// opens with 6, write and append.
enum OpenMode : s32 {
    OpenMode_Read = 1,
    OpenMode_Write = 2,
    OpenMode_AllowAppend = 4,
};

// OpenDirectory's mode. The game always passes 3 (fn_710002e6e0, which then recurses into the
// directories it gets back), so both kinds; which bit is which is the SDK's convention, not
// something the game shows.
enum OpenDirectoryMode : s32 {
    OpenDirectoryMode_Directory = 1,
    OpenDirectoryMode_File = 2,
    OpenDirectoryMode_All = 3,
};

enum DirectoryEntryType : s8 {
    DirectoryEntryType_Directory = 0,
    DirectoryEntryType_File = 1,
};

// One entry of ReadDirectory: fn_710002e6e0 reads them 0x310 bytes apart into a buffer of 0x80,
// copies the name with strcpy from +0 and recurses when the byte at +0x304 is 0.
struct DirectoryEntry {
    char name[0x301];
    u8 reserved[3];
    DirectoryEntryType type;
    u8 reserved2[3];
    s64 file_size;
};
static_assert(sizeof(DirectoryEntry) == 0x310, "fn_710002e6e0 steps 0x310 bytes");
static_assert(__builtin_offsetof(DirectoryEntry, type) == 0x304, "fn_710002e6e0 tests +0x304");

// WriteFile's options: the save writer passes one on its stack; 1 asks for a flush.
struct WriteOption {
    s32 flags;
};
constexpr s32 WriteOptionFlag_Flush = 1;

// The library's own allocator (fn_71001325d0 passes the engine's fn_7100132b20 and fn_7100132b54).
// The port allocates on the host's heap and keeps the pair unused.
using AllocateFunction = void* (*)(u64 size);
using DeallocateFunction = void (*)(void* pointer, u64 size);
void SetAllocator(AllocateFunction allocate, DeallocateFunction deallocate);

Result QueryMountRomCacheSize(u64* out_size);
Result MountRom(const char* name, void* cache, u64 cache_size);
void Unmount(const char* name);

Result EnsureSaveData(const account::Uid& user);
Result MountSaveData(const char* name, const account::Uid& user);
Result CommitSaveData(const char* name);

// The loader threads lower their I/O priority (fn_7101236478). The PC has one priority.
enum Priority : s32 {};
void SetPriorityOnCurrentThread(Priority priority);

Result OpenFile(FileHandle* out, const char* path, s32 mode);
void CloseFile(FileHandle file);
Result ReadFile(u64* out_read, FileHandle file, s64 offset, void* buffer, u64 size);
Result WriteFile(FileHandle file, s64 offset, const void* buffer, u64 size, const WriteOption& option);
Result FlushFile(FileHandle file);
Result GetFileSize(s64* out_size, FileHandle file);
Result SetFileSize(FileHandle file, s64 size);
Result CreateFile(const char* path, s64 size);
Result DeleteFile(const char* path);

Result OpenDirectory(DirectoryHandle* out, const char* path, s32 mode);
void CloseDirectory(DirectoryHandle directory);
Result ReadDirectory(s64* out_count, DirectoryEntry* entries, DirectoryHandle directory, s64 max_entries);
Result CreateDirectory(const char* path);
Result DeleteDirectory(const char* path);

}  // namespace nn::fs
