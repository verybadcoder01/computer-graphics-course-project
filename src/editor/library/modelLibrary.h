#pragma once

#include <filesystem>
#include <string>
#include <vector>

enum class LibraryEntryKind { Part, Nozzle };

struct LibraryEntry {
    std::string name;
    std::filesystem::path objPath;
    std::filesystem::path texturePath;
    LibraryEntryKind kind = LibraryEntryKind::Part;
};

// root/parts/*.obj are plain parts, root/nozzles/*.obj become particle emitters; a texture is a
// same-stem .png/.jpg next to the .obj
class ModelLibrary {
   public:
    explicit ModelLibrary(std::filesystem::path root = "assets/models");
    virtual ~ModelLibrary() = default;

    void scan();
    const std::vector<LibraryEntry> &getEntries() const noexcept { return entries; }
    const std::filesystem::path &getRoot() const noexcept { return root; }

   protected:
    std::filesystem::path root;
    std::vector<LibraryEntry> entries;
    void scanFolder(const std::filesystem::path &folder, LibraryEntryKind kind);
};
