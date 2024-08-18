#include <Sapfire/DefaultInclude.hpp>

using namespace sapfire::prelude;

#include <Sapfire/loader/Dirs.hpp>
#include <UIKit/UIKit.h>
#include <iostream>
#include <sstream>
#include <Sapfire/utils/web.hpp>

bool utils::clipboard::write(std::string const& data) {
    [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:data.c_str()];
    return true;
}

std::string utils::clipboard::read() {
    return std::string([[UIPasteboard generalPasteboard].string UTF8String]);
}

void utils::web::openLinkInBrowser(std::string const& url) {
    [[UIApplication sharedApplication]
        openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

void sapfire_nslog(uintptr_t x) {
    NSLog(@"sapfire %lx", x);
}


std::filesystem::path dirs::getGameDir() {
    return std::filesystem::current_path();
}

std::filesystem::path dirs::getSaveDir() {
    return weaklyCanonical(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

bool sapfire::utils::permission::getPermissionStatus(Permission permission) {
    return true; // unimplemented
}

void sapfire::utils::permission::requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback) {
    callback(true); // unimplemented
}
