#ifndef _DIRFILELIST_
#define _DIRFILELIST_

#include "odrive.h"
#include <list>
#include <string>

using namespace std;

/** Retreive the list of all files in a directory
 *  \param dirName name of the directory to scan
 *  \param outputList output list of file names found in this directory
 *  \return true if an error occured
 */

bool getDirectoryFileList(const string &dirName, list<string> & outputList);

/**
 * Check if the file already exists
 * @param od : Odrive object where check the file.
 * @param fileName : name of the file to search.
 */
bool fileAlreadyExists(const ODrive &od, const string &fileName);

/*
 * Return all files game writes in following server_.
 * @param: server : Is the current player a server or client ?
 */
list<string> Files(bool server);

#endif
