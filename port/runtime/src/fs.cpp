// nn::fs for the port: each mount is a host directory. "rom" is the user's dump
// (dread_port_orig_directory()/romfs, read-only); the save mount is DREAD_SAVE (default build/save),
// written in place, so CommitSaveData has nothing left to do. Files are std::FILE streams behind a
// lock each (ReadFile and WriteFile take an offset, so every call seeks); directories are listed
// whole when opened. Every stub names the boot-path function that depends on it in a TODO.

#include "nn/fs.h"

#include "dread_port.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <mutex>
#include <new>
#include <string>
#include <system_error>
#include <vector>

namespace nn::fs {

namespace {

namespace host = std::filesystem;

struct Mount {
    std::string name;
    host::path root;
    bool writable;
};

std::mutex g_mounts_lock;
std::vector<Mount> g_mounts;

void AddMount(const char* name, host::path root, bool writable) {
    std::lock_guard<std::mutex> hold(g_mounts_lock);
    for (Mount& m : g_mounts)
        if (m.name == name) {
            m = Mount{name, std::move(root), writable};
            return;
        }
    g_mounts.push_back(Mount{name, std::move(root), writable});
}

// "name:/a/b" to the host path under the mount's root. False for an unknown mount, a path without
// a mount name, or one with a ".." component (it would leave the mount).
bool Resolve(const char* path, host::path* out, bool* writable) {
    const char* colon = std::strchr(path, ':');
    if (!colon)
        return false;
    const std::string name(path, colon - path);
    std::lock_guard<std::mutex> hold(g_mounts_lock);
    for (const Mount& m : g_mounts) {
        if (m.name != name)
            continue;
        host::path result = m.root;
        const char* p = colon + 1;
        while (*p) {
            while (*p == '/')
                ++p;
            const char* end = p;
            while (*end && *end != '/')
                ++end;
            if (end == p)
                break;
            const std::string part(p, end - p);
            if (part == "..")
                return false;
            if (part != ".")
                result /= part;
            p = end;
        }
        *out = std::move(result);
        *writable = m.writable;
        return true;
    }
    return false;
}

const char* SaveDirectory() {
    const char* save = std::getenv("DREAD_SAVE");
    return save && *save ? save : "build/save";
}

bool Seek(std::FILE* f, s64 offset) {
#if defined(_WIN32)
    return _fseeki64(f, offset, SEEK_SET) == 0;
#else
    return fseeko(f, offset, SEEK_SET) == 0;
#endif
}

struct HostFile {
    std::mutex lock;
    std::FILE* stream;
    host::path path;
    s32 mode;
};

HostFile* File(FileHandle handle) { return static_cast<HostFile*>(handle.handle); }

struct HostDirectory {
    std::vector<DirectoryEntry> entries;
    u64 next;
};

s64 SizeOf(HostFile* f) {
    std::error_code ec;
    std::fflush(f->stream);
    const auto size = host::file_size(f->path, ec);
    return ec ? -1 : s64(size);
}

}  // namespace

void SetAllocator(AllocateFunction, DeallocateFunction) {
    // TODO(M1): fn_71001325d0 hands the library the engine's allocator first thing. The port's
    // nn::fs allocates on the host heap, so the pair is not used.
}

Result QueryMountRomCacheSize(u64* out_size) {
    // TODO(M1): fn_71001325d0 allocates this much and mounts only if the allocation succeeds, so
    // the answer must not be zero. The port needs no cache; one page keeps the allocation small.
    *out_size = 0x1000;
    return ResultSuccess();
}

Result MountRom(const char* name, void*, u64) {
    // TODO(M1): fn_71001325d0 mounts "rom" over the cache; every asset path the game opens
    // starts with "rom:/" (fn_7100100e74: rom:/packs/sounds/...).
    AddMount(name, host::path(dread_port_orig_directory()) / "romfs", false);
    return ResultSuccess();
}

void Unmount(const char* name) {
    // TODO(M1): the shutdown fn_7100132730 ("rom") and the save system fn_7100fc3510 ("save").
    std::lock_guard<std::mutex> hold(g_mounts_lock);
    for (auto it = g_mounts.begin(); it != g_mounts.end(); ++it)
        if (it->name == name) {
            g_mounts.erase(it);
            return;
        }
}

Result EnsureSaveData(const account::Uid&) {
    // TODO(M1): fn_7100fc3510 before mounting the save. One local user: one save directory.
    std::error_code ec;
    host::create_directories(SaveDirectory(), ec);
    return ec ? ResultHostFailure() : ResultSuccess();
}

Result MountSaveData(const char* name, const account::Uid&) {
    // TODO(M1): fn_7100fc3510 mounts the save under the name its global holds ("savedata", the
    // name the save calls commit).
    AddMount(name, host::path(SaveDirectory()), true);
    return ResultSuccess();
}

Result CommitSaveData(const char*) {
    // TODO(M1): fn_710002dedc, fn_710002e224, fn_710002eec8 commit after every change. The port
    // writes in place, so a crash between a write and its commit keeps the write (the console
    // would roll it back).
    return ResultSuccess();
}

void SetPriorityOnCurrentThread(Priority) {
    // TODO(M1): the loader thread fn_7101236478. Nothing on PC.
}

Result OpenFile(FileHandle* out, const char* path, s32 mode) {
    // TODO(M1): the engine's file object (fn_7101235090, mode 1), the sound banks (fn_7100100e74),
    // the save reader fn_710002ed98 (1) and writer fn_710002eec8 (6). A missing file is
    // ResultPathNotFound, which fn_7101235090 turns into its own 0x81000001.
    host::path p;
    bool writable;
    if (!Resolve(path, &p, &writable))
        return ResultPathNotFound();
    const bool write = (mode & OpenMode_Write) != 0;
    if (write && !writable)
        return ResultHostFailure();
    std::error_code ec;
    if (!host::is_regular_file(p, ec))
        return ResultPathNotFound();
    std::FILE* stream = std::fopen(p.string().c_str(), write ? "r+b" : "rb");
    if (!stream)
        return ResultTargetLocked();
    auto* f = new (std::nothrow) HostFile;
    if (!f) {
        std::fclose(stream);
        return ResultHostFailure();
    }
    f->stream = stream;
    f->path = std::move(p);
    f->mode = mode;
    out->handle = f;
    return ResultSuccess();
}

void CloseFile(FileHandle file) {
    // TODO(M1): fn_7101235138 and every reader above.
    HostFile* f = File(file);
    std::fclose(f->stream);
    delete f;
}

Result ReadFile(u64* out_read, FileHandle file, s64 offset, void* buffer, u64 size) {
    // TODO(M1): fn_7101235224 and fn_7101235520 read at the object's position and advance it by
    // what was read; a short read at the end is success with the count.
    HostFile* f = File(file);
    std::lock_guard<std::mutex> hold(f->lock);
    if (offset < 0 || !Seek(f->stream, offset))
        return ResultHostFailure();
    const u64 n = std::fread(buffer, 1, size, f->stream);
    if (n < size && std::ferror(f->stream)) {
        std::clearerr(f->stream);
        return ResultHostFailure();
    }
    *out_read = n;
    return ResultSuccess();
}

Result WriteFile(FileHandle file, s64 offset, const void* buffer, u64 size, const WriteOption& option) {
    // TODO(M1): fn_71012352b0, fn_71012355b0 and the save writer fn_710002eec8. Writing past the
    // end needs OpenMode_AllowAppend, as on the console.
    HostFile* f = File(file);
    std::lock_guard<std::mutex> hold(f->lock);
    if (!(f->mode & OpenMode_Write) || offset < 0)
        return ResultHostFailure();
    if (!(f->mode & OpenMode_AllowAppend) && offset + s64(size) > SizeOf(f))
        return ResultHostFailure();
    if (!Seek(f->stream, offset) || std::fwrite(buffer, 1, size, f->stream) != size)
        return ResultHostFailure();
    if ((option.flags & WriteOptionFlag_Flush) && std::fflush(f->stream) != 0)
        return ResultHostFailure();
    return ResultSuccess();
}

Result FlushFile(FileHandle file) {
    // TODO(M1): fn_710123517c.
    HostFile* f = File(file);
    std::lock_guard<std::mutex> hold(f->lock);
    return std::fflush(f->stream) == 0 ? ResultSuccess() : ResultHostFailure();
}

Result GetFileSize(s64* out_size, FileHandle file) {
    // TODO(M1): fn_71012351c0 (aborts on failure), the sound banks and the save reader, which
    // allocate this much and read it whole.
    HostFile* f = File(file);
    std::lock_guard<std::mutex> hold(f->lock);
    const s64 size = SizeOf(f);
    if (size < 0)
        return ResultHostFailure();
    *out_size = size;
    return ResultSuccess();
}

Result SetFileSize(FileHandle file, s64 size) {
    // TODO(M1): the save writer fn_710002eec8 truncates to what it wrote. The stream is closed
    // around the resize, which the host may refuse on an open file.
    HostFile* f = File(file);
    std::lock_guard<std::mutex> hold(f->lock);
    if (!(f->mode & OpenMode_Write) || size < 0)
        return ResultHostFailure();
    std::fclose(f->stream);
    std::error_code ec;
    host::resize_file(f->path, u64(size), ec);
    f->stream = std::fopen(f->path.string().c_str(), "r+b");
    if (!f->stream)
        __builtin_trap();  // the file existed a moment ago: nothing sensible to return to the game
    return ec ? ResultHostFailure() : ResultSuccess();
}

Result CreateFile(const char* path, s64 size) {
    // TODO(M1): the save writer fn_710002eec8 creates the file at its final size before writing.
    host::path p;
    bool writable;
    if (!Resolve(path, &p, &writable))
        return ResultPathNotFound();
    if (!writable || size < 0)
        return ResultHostFailure();
    std::error_code ec;
    if (host::exists(p, ec))
        return ResultPathAlreadyExists();
    if (!host::is_directory(p.parent_path(), ec))
        return ResultPathNotFound();
    std::FILE* stream = std::fopen(p.string().c_str(), "wb");
    if (!stream)
        return ResultHostFailure();
    std::fclose(stream);
    host::resize_file(p, u64(size), ec);
    return ec ? ResultHostFailure() : ResultSuccess();
}

Result DeleteFile(const char* path) {
    // TODO(M1): fn_710002e224 and fn_710002f210 (deleting saves).
    host::path p;
    bool writable;
    if (!Resolve(path, &p, &writable))
        return ResultPathNotFound();
    if (!writable)
        return ResultHostFailure();
    std::error_code ec;
    if (!host::is_regular_file(p, ec))
        return ResultPathNotFound();
    return host::remove(p, ec) && !ec ? ResultSuccess() : ResultHostFailure();
}

Result OpenDirectory(DirectoryHandle* out, const char* path, s32 mode) {
    // TODO(M1): fn_710002e168 and the recursive lister fn_710002e6e0 (mode 3, both kinds).
    host::path p;
    bool writable;
    if (!Resolve(path, &p, &writable))
        return ResultPathNotFound();
    std::error_code ec;
    if (!host::is_directory(p, ec))
        return ResultPathNotFound();
    auto* d = new (std::nothrow) HostDirectory;
    if (!d)
        return ResultHostFailure();
    d->next = 0;
    for (host::directory_iterator it(p, ec), end; !ec && it != end; it.increment(ec)) {
        const bool is_dir = it->is_directory(ec);
        if (!(mode & (is_dir ? OpenDirectoryMode_Directory : OpenDirectoryMode_File)))
            continue;
        const std::string name = it->path().filename().string();
        if (name.size() >= sizeof(DirectoryEntry::name))
            continue;  // no console path component is this long
        DirectoryEntry e{};
        std::memcpy(e.name, name.c_str(), name.size() + 1);
        e.type = is_dir ? DirectoryEntryType_Directory : DirectoryEntryType_File;
        e.file_size = is_dir ? 0 : s64(it->file_size(ec));
        d->entries.push_back(e);
    }
    if (ec) {
        delete d;
        return ResultHostFailure();
    }
    out->handle = d;
    return ResultSuccess();
}

void CloseDirectory(DirectoryHandle directory) {
    // TODO(M1): fn_710002e168 and fn_710002e6e0.
    delete static_cast<HostDirectory*>(directory.handle);
}

Result ReadDirectory(s64* out_count, DirectoryEntry* entries, DirectoryHandle directory, s64 max_entries) {
    // TODO(M1): fn_710002e6e0 reads up to 0x80 entries once per directory.
    auto* d = static_cast<HostDirectory*>(directory.handle);
    s64 n = 0;
    while (n < max_entries && d->next < d->entries.size())
        entries[n++] = d->entries[d->next++];
    *out_count = n;
    return ResultSuccess();
}

Result CreateDirectory(const char* path) {
    // TODO(M1): fn_710002dedc and fn_710002eec8 (the save's directories).
    host::path p;
    bool writable;
    if (!Resolve(path, &p, &writable))
        return ResultPathNotFound();
    if (!writable)
        return ResultHostFailure();
    std::error_code ec;
    if (host::exists(p, ec))
        return ResultPathAlreadyExists();
    if (!host::is_directory(p.parent_path(), ec))
        return ResultPathNotFound();
    return host::create_directory(p, ec) && !ec ? ResultSuccess() : ResultHostFailure();
}

Result DeleteDirectory(const char* path) {
    // TODO(M1): fn_710002e224, after deleting the files it listed.
    host::path p;
    bool writable;
    if (!Resolve(path, &p, &writable))
        return ResultPathNotFound();
    if (!writable)
        return ResultHostFailure();
    std::error_code ec;
    if (!host::is_directory(p, ec))
        return ResultPathNotFound();
    if (!host::is_empty(p, ec))
        return ResultDirectoryNotEmpty();
    return host::remove(p, ec) && !ec ? ResultSuccess() : ResultHostFailure();
}

}  // namespace nn::fs
