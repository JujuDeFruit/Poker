#include "dirfilelist.h"
#include "constantes_files_name.h"
#include "odrive.h"
#include <windows.h>

/** Retreive the list of all files in a directory
 *  \param dirName name of the directory to scan
 *  \param outputList output list of file names found in this directory
 *  \return true if an error occured
 */
bool getDirectoryFileList(const string &dirName, list<string> & outputList)
{
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  
  string dir=dirName+"\\*";
  hFind = FindFirstFile(dir.c_str(), &ffd);

  if ( hFind == INVALID_HANDLE_VALUE) return true;

  outputList.clear();
  do {
    if ( ! (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // This is not a directory
    {
      outputList.push_back(string(ffd.cFileName));
    }
  }
  while (FindNextFile(hFind, &ffd) != 0);
  
  bool error = (GetLastError() != ERROR_NO_MORE_FILES);

  FindClose(hFind);
  return error;
}

/**
 * Check if the file already exists
 * @param od : Odrive object where check the file.
 * @param fileName : name of the file to search.
 */
bool fileAlreadyExists(const ODrive &od, const string &fileName) {
	const string odDrivePath = od.getOdDrivePath();

	list<string> fileList;
	bool error = getDirectoryFileList(odDrivePath, fileList);

	for each (string file in fileList) {
		/* Does the file exist ? */
		if (fileName == file) return true;
	}
	return false;
}

/*
 * Return all files game writes in following server_.
 * @param: server : Is the current player a server or client ?
 */
list<string> Files(bool server) {
	list<string> fileList;
	if (server) {
		fileList.push_back(ConstFiles::INITFILE);
		fileList.push_back(ConstFiles::DECKFILESERVER);
		fileList.push_back(ConstFiles::RIVERFILESERVER);
		fileList.push_back(ConstFiles::TURNFILESERVER);
		fileList.push_back(ConstFiles::HANDFILESERVER);
		fileList.push_back(ConstFiles::HANDFILECLIENT);
		fileList.push_back(ConstFiles::ACTIONFILESERVER);
		fileList.push_back(ConstFiles::MONEYFILESERVER);
		fileList.push_back(ConstFiles::WINNERFILESERVER);
		fileList.push_back(ConstFiles::POTFILESERVER);
	}
	else if (!server) {
		fileList.push_back(ConstFiles::DECKFILECLIENT);
		fileList.push_back(ConstFiles::RIVERFILECLIENT);
		fileList.push_back(ConstFiles::TURNFILECLIENT);
		fileList.push_back(ConstFiles::ACTIONFILECLIENT);
		fileList.push_back(ConstFiles::MONEYFILECLIENT);
		fileList.push_back(ConstFiles::WINNERFILECLIENT);
		fileList.push_back(ConstFiles::POTFILECLIENT);
	}
	return fileList;
}