# The Banana
This is a 3D game engine I'm making. Many aspects of this engine are inspired by The Cherno's [Hazel](https://github.com/TheCherno/Hazel). It currently supports the following platforms:
- Windows
- MacOS X

**DISCLAIMER**: this doesnt work right now lmao

## TO BUILD
### PROJECT FILES
There is a Premake script included in the repository, with executables in the `vendor` folder. To generate projects, run the following commands:
- Windows: `vendor\premake5 vs2019`
- MacOS: `vendor/premake5-macosx gmake` OR `vendor/premake5-macosx xcode4`
- Linux: `vendor/premake5-linux gmake`