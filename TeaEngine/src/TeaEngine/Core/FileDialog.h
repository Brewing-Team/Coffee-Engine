#pragma once

#include <string>
#include <vector>
#include <filesystem>
namespace Tea {

    struct FileDialogArgs {
        struct Filter {
            const char* Name;
            const char* Extension;
        };
        std::vector<Filter> Filters = {};
        std::string DefaultPath = "";
        std::string DefaultName = "";
    };

    class FileDialog {
    public:
        static const std::filesystem::path OpenFile(const FileDialogArgs& args = {});
        static const std::filesystem::path SaveFile(const FileDialogArgs& args = {});
    };

}