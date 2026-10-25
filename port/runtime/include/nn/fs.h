#pragma once

// nn::fs: the RomFS and save data. The boot path mounts the RomFS (MountRom, after
// QueryMountRomCacheSize for its cache and SetAllocator for the library's own memory), then
// opens, sizes, reads and closes files and directories from it; saves come later (EnsureSaveData,
// MountSaveData, CommitSaveData, the create/delete/flush calls). On PC the RomFS is the user's
// own dump as plain files (files.toc and the .pkg packs are parsed as before, docs/PORT_PLAN.md).
//
// `main` imports (data/functions/): MountRom, QueryMountRomCacheSize, Unmount, SetAllocator,
// SetPriorityOnCurrentThread, OpenFile, CloseFile, ReadFile, WriteFile, FlushFile, GetFileSize,
// SetFileSize, CreateFile, DeleteFile, OpenDirectory, CloseDirectory, ReadDirectory,
// CreateDirectory, DeleteDirectory, EnsureSaveData, MountSaveData, CommitSaveData.
// Nothing is declared until the first matched or lifted caller needs it (M1).

#include "nn/nn_common.h"

namespace nn::fs {}  // namespace nn::fs
