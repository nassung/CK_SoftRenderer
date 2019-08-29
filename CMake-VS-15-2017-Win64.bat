@echo off

@echo Generating visual studio 2017 64Bit solutions
PUSHD %~dp0

@echo Check exist Project folder
IF EXIST "Project" rd "Project" /s /q
@echo Make new Project folder
md Project
cd Project
@echo Run CMake Script
cmake -DBUILD_PLAYER=ON -G "Visual Studio 15 2017 Win64" ..\

@echo:
@echo Solution created in %~dp0\Project
@echo:
POPD
PAUSE
