#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>
#include <fstream>
#include <windows.h>
#include <Lmcons.h>
#ifndef DEBUG
#define DEBUG
#endif

namespace fs = std::filesystem;

//true if directory was created, false if error or directory already exists
bool createDirectory(const std::wstring& absPath);
//gets wstring absolute filepath of all file in given directory including files of subdirectories and so on
std::vector<std::wstring> getFilepaths(const std::wstring& absPath);
//true if the two given paths are contain non-identical files
bool filesDiffer(const std::wstring& absPath1, const std::wstring& absPath2);

//get filesize in bytes
std::size_t filesize(const std::wstring& absPath);
//hash file into a 64 bit number
uint64_t getFileHash(const std::wstring& absPath);

//get user account name on windows
std::wstring getUsername();
//get absolute path of Documents folder
std::wstring getDocumentsPath();
//get path relative to Documents
std::wstring getRelativeToDocuments(std::wstring absPath);
//get path relative to relDir
std::wstring getRelativeTo(std::wstring absPath, std::wstring relDir);
//Delete file
bool deleteFile(const std::wstring& absPath);

//given a file's path it creates all directories needed to create the file
void acquireDirectories(std::wstring absPath);