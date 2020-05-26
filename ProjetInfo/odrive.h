#ifndef ODRIVE_H
#define ODRIVE_H
#include <string>
#include <vector>

using namespace std;

class ODrive
{
    string odAgentPath_;
    string odDrivePath_;
    string redirString_;
	string odErrorPath_;
    bool debugMode_;
public:
    ODrive(string =".odrive-agent", bool debugMode=false);
    string getFullName(string file) {return odDrivePath_+'/'+file;}
    void sync(string dir);
    void refresh(string dir);
    void emptytrash();
    void mkDir(string dir);
    void delFile(string file);
    bool isDir(string dir);
    void waitForChange(string file);

	void writeInErrorLogFile(string);							// Write in report log.
	void writeInFile(string, string, ios_base::openmode);		// Write a single line in a file and choose your openmode (erase and write or append ?).  
	void writeInFile(string, vector<string>);					// Write many lines in a file
	void deleteAllFiles();										// Delete all files in the mounted point.
	string getOdDrivePath() const { return odDrivePath_; };		// Return odrive path.
	vector<string> readFile(string file);						// Read a file and return a vector of string which represent all lines.
	void syncAll();												// Sync all files in the mounted point.
	void clearFiles();											// Clear all files except the __init__.txt one.
};

#endif // ODRIVE_H
