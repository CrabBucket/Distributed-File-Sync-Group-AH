#pragma once
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include "FileHelper.h"
#include <map>
#include <vector>
#include <mutex>
#include "fileChangePacket.h"
#include <SFML/Network.hpp>

//thanks thomas
#ifndef DIRECTORY_MONITOR
#define DIRECTORY_MONITOR

extern std::vector<fileChangeData> fileChangeBuf;

void RefreshDirectory(LPTSTR);
void RefreshTree(LPTSTR);
void WatchDirectory(LPTSTR, std::mutex&);
std::vector<fileChangeData> getDirectoryChanges(LPTSTR, std::map<std::wstring, uint64_t>&);
std::vector<fileChangeData> getDirectoryChanges(std::map<std::wstring, uint64_t>&, std::map<std::wstring, uint64_t>&);
void printChanges(std::vector<fileChangeData>);
std::map<std::wstring, uint64_t> CreateFileHashes(const std::wstring dirPath);

#endif
