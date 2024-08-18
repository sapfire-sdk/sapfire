#!/bin/sh
cd `dirname $0`
cp /Users/jakrillis/tmp/sapfire/loader/loader_ios/build/Sapfire.dylib ./bin/ios/Sapfire.dylib
rm -rf Sapfire\ Helper.app
cp -r /Users/jakrillis/Library/Developer/Xcode/DerivedData/Sapfire_Helper-eojylhlhpfgucjaupttaqyhqcbmp/Build/Products/Debug-iphoneos/Sapfire\ Helper.app .
g++ main.cpp -arch arm64 -std=c++17 -isysroot `xcrun --show-sdk-path --sdk iphoneos` -dynamiclib -o SapfireLauncher.dylib

python3 pkg.py

scp "./sapfireloader-test-arm64.deb" root@three.local:/var/mobile/Documents
ssh root@three.local dpkg -i "/var/mobile/Documents/sapfireloader-test-arm64.deb" "&&" killall GeometryJump #";" uicache