#ifndef ODRIVE_H
#define ODRIVE_H
#include <string>

using namespace std;

class ODrive
{
    string odAgentPath_;
    string odDrivePath_;
    string redirString_;
	string odErrorPath_;
    bool debugMode_;
public:
    ODrive(string =".odrive-agent", string ="source\\repos\\ProjetInfo", bool debugMode=false);
    string getFullName(string file) {return odDrivePath_+'/'+file;}
    void sync(string dir);
    void refresh(string dir);
    void emptytrash();
    void mkDir(string dir);
    void delFile(string file);
    bool isDir(string dir);
    void waitForChange(string file);

	void writeInErrorLogFile(string);
	void writeInFile(string, string, ios_base::openmode);
	string getOdDrivePath() const { return odDrivePath_; };
};

#endif // ODRIVE_H
