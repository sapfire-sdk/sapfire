#include <Windows.h>
#include <iostream>
#include <array>
#include <filesystem>

std::filesystem::path workingDir;
std::filesystem::path sapfireDir;
std::filesystem::path updatesDir;
std::filesystem::path resourcesDir;

void showError(std::string const& error) {
    MessageBoxA(nullptr, error.c_str(), "Error Loading Sapfire", MB_ICONERROR);
}

bool waitForFile(std::filesystem::path const& path) {
    if (!path.has_filename())
        return false;

    int delay = 10;
    int maxDelayAttempts = 20;
    HANDLE hFile;
    while ((hFile = CreateFileA(path.string().c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_SHARING_VIOLATION) {
            Sleep(delay);
            // the delay would raise and go up to about 1 second, after which it will start a 20 second countdown
            if (delay < 1024) {
                delay *= 2;
            } else {
                maxDelayAttempts--;
                // delay too long, failed too many times, just give up now
                if (maxDelayAttempts == 0) {
                    hFile = NULL;
                    break;
                }
            }
        } else {
            break;
        }
    }
    if (hFile) {
        CloseHandle(hFile);
    } else {
        showError("Unable to update Sapfire: " + path.filename().string() + " is open by another process.");
        return false;
    }
    return true;
}

bool updateFile(std::string const& name) {
    std::error_code error;
    if (!std::filesystem::exists(updatesDir / name, error) || error)
        return true;
    if (!waitForFile(workingDir / name))
        return false;

    std::filesystem::rename(updatesDir / name, workingDir / name, error);
    if (error) {
        showError("Unable to update Sapfire: Unable to move " + name + " - " + error.message());
        return false;
    }
    return true;
}

void removePath(std::filesystem::path const& path) {
    std::error_code error;
    if (!std::filesystem::exists(path, error) || error)
        return;
    if (path.has_filename() && !waitForFile(path))
        return;

    if (std::filesystem::is_directory(path) && !std::filesystem::is_empty(path))
        std::filesystem::remove_all(path, error);
    std::filesystem::remove(path, error);
    if (error) {
        if (path.has_filename())
            showError("Unable to update Sapfire: Unable to remove " + path.filename().string() + " - " + error.message());
        else
            showError("Unable to update Sapfire: Unable to remove " + path.string() + " - " + error.message());
        return;
    }
}

void updateResources() {
    std::error_code error;
    if (!std::filesystem::exists(updatesDir / "resources", error) || error)
        return;

    std::filesystem::remove_all(resourcesDir / "sapfire.loader", error);
    if (error) {
        showError("Unable to update Sapfire resources:" + error.message());
        return;
    }

    std::filesystem::rename(updatesDir / "resources", resourcesDir / "sapfire.loader", error);
    if (error) {
        showError("Unable to update Sapfire resources: " + error.message());
        return;
    }
}

int main(int argc, char* argv[]) {
    workingDir = std::filesystem::current_path();
    sapfireDir = workingDir / "sapfire";
    updatesDir = sapfireDir / "update";
    resourcesDir = sapfireDir / "resources";

    if (std::filesystem::exists(workingDir / "SapfireBootstrapper.dll"))
        removePath(workingDir / "SapfireBootstrapper.dll");

    if (std::filesystem::exists(sapfireDir) && std::filesystem::exists(updatesDir)) {
        bool updateSuccess = true;
        updateSuccess &= updateFile("XInput1_4.dll");
        updateSuccess &= updateFile("Sapfire.dll");
        updateSuccess &= updateFile("Sapfire.pdb");
        updateResources();
        // if couldnt update the files, dont delete the updates folder
        if (updateSuccess)
            removePath(updatesDir);
    }

    if (argc < 2)
        return 0;

    if (!waitForFile(workingDir / argv[1])) {
        showError("There was an error restarting GD. Please, restart the game manually.");
        return 0;
    }

    // restart gd using the provided path
    ShellExecuteA(NULL, "open", (workingDir / argv[1]).string().c_str(), "", workingDir.string().c_str(), TRUE);
    return 0;
}
