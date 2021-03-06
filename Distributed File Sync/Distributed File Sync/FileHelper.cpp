#include "FileHelper.h"

bool createDirectory(const std::wstring& path) {
	fs::path p = path;
	p.make_preferred(); //make slashes the correct way
	//true if directory is successfully created
	if (fs::create_directory(p)) {
		return true;
	}
	else {
		std::wcout << "Directory already exists or error creating directory: " << path << std::endl;
		return false;
	}
}

//returns absolute paths
std::vector<std::wstring> getFilepaths(const std::wstring& absPath) {
	std::vector<std::wstring> filepaths;
	//for each object in directory
	for (fs::directory_entry entry : fs::directory_iterator(absPath)){
		fs::path p = entry.path();
		p.make_preferred();
		//if it's a directory then handle it recursively
		if (fs::is_directory(p)) {
			for (std::wstring filepath : getFilepaths(p.wstring())) {
				filepaths.push_back(filepath);
			}
		}
		else {
			filepaths.push_back(p.wstring());
		}
	}
	return filepaths;
}

bool filesDiffer(const std::wstring& absPath1, const std::wstring& absPath2) {
	return getFileHash(absPath1) != getFileHash(absPath2);
}

std::size_t filesize(const std::wstring& absPath) { //POSSIBLY CHANGE TO use fs::file_size
	std::ifstream file;
	file.open(absPath);
	std::size_t length = 0;
	if (file) {
		// get length of file:
		file.seekg(0, file.end);
		length = file.tellg();
	}
	file.close();

	return length;
}

bool deleteFile(const std::wstring& absPath) {
	return std::filesystem::remove(absPath);
}



//hash is a takes into account absolute path, file size, and file contents
uint64_t getFileHash(const std::wstring& absPath) {
	std::vector<uint64_t> multiplands{2,7,17,29,41,53,67,79,97};
	uint64_t modBase = UINT64_MAX - 1000;
	int i = 0;

	//hash file length
	uint64_t hash = filesize(absPath);

	//hash path
	for(char c : getRelativeToDocuments(absPath))
		hash = (hash + c) % modBase;

	//hash contents
	std::ifstream file;
	file.open(absPath);

	if (file) {
		char c;
		file.get(c);
		while (file.good()) {
			hash = (hash + (int)c * i) % modBase;
			file.get(c);
			i = (i == multiplands.size()) ? 0 : i + 1;
		}
	}

	file.close();

	return hash;
}

std::wstring getUsername()
{
	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;
	//if user name successfully retreived
	if (GetUserName((TCHAR*)name, &size)) {
		std::wstring username(name, wcslen(name));
		return username;
	}
	else {
		std::cout << "couldn't retreive name" << std::endl;
	}

	return std::wstring();
}

//example C:\Users\Tanner\Documents
std::wstring getDocumentsPath() {
	std::wstring absPath = L"C:\\Users\\";
	absPath += getUsername();
	absPath += L"\\Documents";
	return absPath;
}

//assumes the path given is in the Documents directory or its subdirectories
std::wstring getRelativeToDocuments(std::wstring absPath) {
	absPath.erase(absPath.begin(), absPath.begin() + getDocumentsPath().size());
	return absPath;
}

//assumes the path given is in the relDir directory or its subdirectories
std::wstring getRelativeTo(std::wstring absPath, std::wstring relDir) {
	absPath.erase(absPath.begin(), absPath.begin() + relDir.size());
	return absPath;
}

void acquireDirectories(std::wstring absPath) {
	fs::path p = absPath;
	p.make_preferred();
	//create_directories attempts to create all missing directories in a path
	if (!fs::create_directories(p.parent_path())) {
		std::wcout << "Directory already exists or error creating directory: " << absPath << std::endl;
	}
}

