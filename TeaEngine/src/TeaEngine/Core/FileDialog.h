#pragma once

#include <string>
#include <vector>
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
        static std::string OpenFile(const FileDialogArgs& args = {});
        static std::string SaveFile(const FileDialogArgs& args = {});
    };

}