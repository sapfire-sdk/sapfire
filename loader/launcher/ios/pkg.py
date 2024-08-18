import sys, os
from shutil import copyfile, rmtree
from distutils.dir_util import copy_tree
print(sys.argv)

out_dir = os.getcwd()

os.makedirs(out_dir, exist_ok=True)
os.chdir(out_dir)
os.makedirs("tmp/Library/MobileSubstrate/DynamicLibraries/", exist_ok=True)
os.makedirs("tmp/Applications/", exist_ok=True)
os.makedirs("tmp/DEBIAN/", exist_ok=True)

open("tmp/DEBIAN/control", "w").write("""Name: Sapfire Launcher
Architecture: iphoneos-arm
Depends: com.cokepokes.libnotifications (>= 0.2-2)
Description: Modding suite for Geometry Dash (test package!!)
Maintainer: camila314
Package: com.camila314.sapfire-test
Priority: optional
Section: Tweaks
Version: 0.1.0
""")

copy_tree("Sapfire Helper.app/", "tmp/Applications/Sapfire Helper.app/")
copyfile("/Users/jakrillis/tmp/sapfire/loader/loader_ios/build/Sapfire.dylib", "bin/ios/Sapfire.dylib")
os.system("ldid -S SapfireLauncher.dylib")
os.system("ldid -S bin/ios/Sapfire.dylib")
copyfile("SapfireLauncher.dylib", "tmp/Library/MobileSubstrate/DynamicLibraries/SapfireLauncher.dylib")
copyfile("bin/ios/Sapfire.dylib", "tmp/Library/MobileSubstrate/DynamicLibraries/Sapfire.dylib")

open("tmp/Library/MobileSubstrate/DynamicLibraries/SapfireLauncher.plist", "w").write("""{ Filter = { Bundles = ( "com.robtop.geometryjump" ); }; }""")
os.system("dpkg-deb --build tmp sapfireloader-test-arm64.deb")
rmtree("tmp")
