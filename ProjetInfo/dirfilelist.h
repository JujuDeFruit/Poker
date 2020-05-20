#ifndef _DIRFILELIST_
#define _DIRFILELIST_

#include <list>
#include <string>

using namespace std;


/** Retreive the list of all files in a directory
 *  \param dirName name of the directory to scan
 *  \param outputList output list of file names found in this directory
 *  \return true if an error occured
 */

bool getDirectoryFileList(const string &dirName, list<string> & outputList);


#endif
