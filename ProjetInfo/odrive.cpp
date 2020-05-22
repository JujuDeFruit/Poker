#pragma message("SOLUTIONDIR == " SOLUTIONDIR)
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <list>
#include "odrive.h"
#include "sys/stat.h"
#include "dirfilelist.h"

#if defined(_WIN32) || defined(WIN32)
#define stat _stat
#pragma warning(suppress : 4996)
#endif

using namespace std;

/**
 * Constructs the ODrive object
 *
 * @param odAgentDir the path where the odriveagent is installed
 * @param odDriveDir the path of the mounting point of the drive
 * @param debugMode  output odrive information to the console
 */
ODrive::ODrive(string odAgentDir, bool debugMode)
{
	ostringstream osAgent, osDrive, osError;
	string logFile = "odrive_class.log";

#ifdef __unix__
	osAgent << getenv("HOME") << '/' << odAgentDir << "/bin/odrive";
	osDrive << getenv("HOME") << '/' << odDriveDir << "/Google Drive";
	redirString_ = " >> " + logFile + " 2>&1";
#elif defined(_WIN32) || defined(WIN32)
	osAgent << getenv("HOMEDRIVE") << getenv("HOMEPATH") << '\\' << odAgentDir << "\\bin\\odrive";
	osDrive << SOLUTIONDIR << "odrive_poker";
	osError << SOLUTIONDIR << "error.txt";
	redirString_ = " >> " + logFile + " 2>&1";
#endif

	odAgentPath_ = osAgent.str();
	odDrivePath_ = osDrive.str();
	odErrorPath_ = osError.str();

	debugMode_ = debugMode;
	if (debugMode) {
		redirString_ = "";
		cout << "Init the odrive" << endl;
		cout << odAgentPath_ << endl;
		cout << odDrivePath_ << endl;
	}
	else {
		ofstream logStream(logFile);
		logStream << "Init the odrive" << endl;
		logStream << odAgentPath_ << endl;
		logStream << odDrivePath_ << endl;
	}
}

#pragma region Basic methods

void ODrive::sync(string dir)
{
	// Wait for 1s
	this_thread::sleep_for(chrono::seconds(1));
	// Synchronize a file
	string fich = odDrivePath_ + '/' + dir + ".cloudf";
	if (ifstream(fich).good())
	{
		ostringstream ocmd;
		ocmd << odAgentPath_ << " sync \"" << fich << '"' << redirString_;
		system(ocmd.str().c_str());
	}
	else
	{
		fich = odDrivePath_ + '/' + dir + ".cloud";
		if (ifstream(fich).good())
		{
			ostringstream ocmd;
			ocmd << odAgentPath_ << " sync \"" << fich << '"' << redirString_;
			system(ocmd.str().c_str());
		}
	}
}


void ODrive::refresh(string dir = "")
{
	dir = dir == "" ? odDrivePath_ : dir;
	// Wait for 1s
	this_thread::sleep_for(chrono::seconds(1));
	// Send refresh command
	ostringstream ocmd;
	ocmd << odAgentPath_ << " refresh \"" << odDrivePath_ << '/' << dir << '"' << redirString_;
	string cmd = ocmd.str();
	system(ocmd.str().c_str());
}


void ODrive::emptytrash()
{
	string cmd = odAgentPath_ + " emptytrash" + redirString_;
	system(cmd.c_str());
}


void ODrive::mkDir(string dir)
{
	ostringstream ocmd;
	ocmd << "mkdir \"" << odDrivePath_ << '/' << dir << '"' << redirString_;
	system(ocmd.str().c_str());
}


void ODrive::delFile(string file)
{
	// Deletes the file
	ostringstream ocmd;
#ifdef __unix__
	ocmd << "rm \"" << odDrivePath_ << '/' << file << "\"" << redirString_;
#elif defined(_WIN32) || defined(WIN32)
	ocmd << "del \"" << odDrivePath_ << '/' << file << "\"" << redirString_;
#endif
	system(ocmd.str().c_str());

	// Extract parent dir
	string odDir;
	size_t pos = file.find_last_of("/\\");
	if (pos != string::npos) odDir = file.substr(0, pos);
	else odDir = "";
	// Refresh parent dir
	refresh(odDir);
	// Wait for 2s
	this_thread::sleep_for(chrono::seconds(2));
	// Empty the trash to delete the file in the drive
	emptytrash();
}


bool ODrive::isDir(string dir)
{
	// Test place holder
	string fich = odDrivePath_ + '/' + dir + ".cloudf";
	bool b = ifstream(fich).good();

	if (!b) {
		// Test directory
		string fich = odDrivePath_ + '/' + dir;
		struct stat t_stat;
		b = stat(fich.c_str(), &t_stat) == 0;
	}

	return b;
}


void ODrive::waitForChange(string file)
{
	string odfile = odDrivePath_ + '/' + file;
	// Extract parent dir
	string odDir;
	size_t pos = file.find_last_of("/\\");
	if (pos != string::npos) odDir = file.substr(0, pos);
	else odDir = "";

	// Get initial time
	struct stat t_stat;
	stat(odfile.c_str(), &t_stat);
	time_t t1 = t_stat.st_mtime;

	// Scan time change
	bool changed = false;
	while (!changed)
	{
		// Refresh
		refresh(odDir);
		// Check if time is changed
		stat(odfile.c_str(), &t_stat);
		time_t t2 = t_stat.st_mtime;
		if (difftime(t1, t2) != 0) changed = true;
	}

}

#pragma endregion

#pragma region Added methods

/**
 * Method to report errors in log file without erasing previous errors.
 *
 * @param message : warning to append to log files.
 */
void ODrive::writeInErrorLogFile(string message) {
	ofstream logStream(odErrorPath_, ofstream::app);
	logStream << message << endl;
	logStream.close();
}

/**
 * Method to write a specific message in a specific file
 *
 * @param file : name of the file.
 * @param message : message to print in the file.
 * @param mode : opening mode (append or erase before).
 */
void ODrive::writeInFile(string file, string message, ios_base::openmode mode) {
	ODrive od;
	od.sync(file); // Create the file

	ofstream ofile(od.getFullName(file), mode);
	if (!ofile.is_open()) writeInErrorLogFile("Opening error file \"" + file + "\"");
	else if (message == "NULL") {
		ofile.close();
		return;
	}
	else {
		ofile << message << endl;
		if (ofile.bad()) writeInErrorLogFile("Writing error ! Message \"" + message + "\" not printed. ");
		else writeInErrorLogFile("Message : \"" + message + "\"\t\t\t | \tFile : \"" + file + "\".");
	}
	ofile.close();
}

/** Overload of writeInfIle method to write many messages.
 *
 * @param file : file name.
 * @param messages : list of messages to print.
 */
void ODrive::writeInFile(string file, vector<string> messages) {
	if (!messages.size()) return;

	ODrive od;
	od.sync(file); // Create the file

	ofstream ofile(od.getFullName(file), ofstream::app);
	if (!ofile.is_open()) writeInErrorLogFile("Opening error file \"" + file + "\"");
	else {
		for each(string message in messages) {
			ofile << message + "\n";
			if (ofile.bad()) writeInErrorLogFile("Writing error ! Message \"" + message + "\" not printed. ");
			else writeInErrorLogFile("Message : \"" + message + "\"\t\t\t | \tFile : \"" + file + "\".");
		}
	}
	ofile.close();
}

/**
 * Clear all the files in the drive when the game is over.
 */
void ODrive::clearAllFiles(){


	list<string> fileList;
	bool error = getDirectoryFileList(odDrivePath_, fileList);

	if (error) writeInErrorLogFile("An error occured when game closed.");
	else {
		fileList.sort();
		for each (string file in fileList) {
			writeInFile(file, "NULL", ofstream::out);
		}
	}
}

/**
 * Read a file and return a vector for each line of the file.
 * @param file : the name of the file to read.
 */
vector<string> ODrive::readFile(string file) {
	vector<string> fileContent;
	// If the comm files exists, prints contents
	if (ifstream(getFullName(file)).good())
	{
		ifstream ifile(getFullName(file));
		string text;
		while (!ifile.eof()) {
			ifile >> text;
			fileContent.push_back(text);
		}
		ifile.close();
	}
	else writeInErrorLogFile("Error while opening " + file + " file.");

	if (fileContent.size()) fileContent.pop_back();

	return fileContent;
}
#pragma endregion