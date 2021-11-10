workspace "Mod_photo_crossbuild"
configurations {"Debug"}

includedirs "/home/ilgiz/NEWretkslp/retk2/"
includedirs "/home/ilgiz/NEWretkslp/apks_slp/"
includedirs "/home/ilgiz/KHADASCARDIMAGE/ROOTFS1/usr/include"
includedirs "/home/ilgiz/KHADASCARDIMAGE/ROOTFS1/usr/lib"
project "mod_photo"
kind "ConsoleApp"
language "C++"
files{"./*.hpp","./*cpp"}
libdirs{"libs","/home/ilgiz/NEWretkslp/libaarch64"}
links{"retk2","slp"}
