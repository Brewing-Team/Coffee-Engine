#include "FileDialog.h"
#include "TeaEngine/Core/Log.h"
#include <nfd.h>

namespace Tea {

    std::string FileDialog::OpenFile(const FileDialogArgs& args)
    {
        NFD_Init();

        nfdu8char_t *outPath = nullptr;

        /* nfdu8filteritem_t filters[2] = { { "Text Files", "txt" }, { "C++ Files", "cpp" } };
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 2; */

        nfdopendialogu8args_t nfdArgs = {0};
        nfdArgs.filterCount = args.Filters.size();
        nfdArgs.filterList = (nfdu8filteritem_t*)args.Filters.data();
        nfdArgs.defaultPath = args.DefaultPath.c_str();

        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &nfdArgs);
        if (result == NFD_OKAY)
        {
            return std::string(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            TEA_INFO("User pressed cancel.");
            return "";
        }
        else
        {
            TEA_ERROR("Error: %s\n", NFD_GetError());
            return "";
        }

        NFD_Quit();
    }

    std::string FileDialog::SaveFile(const FileDialogArgs& args)
    {
        NFD_Init();

        nfdchar_t *outPath = nullptr;

        nfdsavedialogu8args_t nfdArgs = {0};
        nfdArgs.filterCount = args.Filters.size();
        nfdArgs.filterList = (nfdu8filteritem_t*)args.Filters.data();
        nfdArgs.defaultPath = args.DefaultPath.c_str();
        nfdArgs.defaultName = args.DefaultName.c_str();

        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &nfdArgs);
        if (result == NFD_OKAY)
        {
            return std::string(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            TEA_INFO("User pressed cancel.");
            return "";
        }
        else
        {
            TEA_ERROR("Error: %s\n", NFD_GetError());
            return "";
        }

        NFD_Quit();
    }

}