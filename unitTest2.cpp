/*************************************************************************
*Implementation of a simple in memory file system.  It uses a base class *
*called Entry and two derived classes called Directory and File.  A third*
*class called FileSystem implements the files and directories to actually*
*be used by a user.							 *
*@author Goodwin Mcdonald						 *
*@date 6/30/2017							 *
*************************************************************************/

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <unordered_set>

class Directory;							//Declare Directory class so we can reference it in Entry

/*******************************************************************
*Base class called Entry.  Declares all variables that will be     *
*shared by derived classed Directory and File			   *
*******************************************************************/
class Entry{
  public:
    std::chrono::time_point<std::chrono::system_clock> dateCreated;
    std::string fileName;
    Directory* parent;
    Entry(std::string name);
    ~Entry();
    virtual Directory* changeDir(Entry* dir, Directory* currentDir);
    void setParent(Directory* par);
};

/**
  Constructor for Entry class
  @paramn name The name of the file that the user will see
*/
Entry::Entry(std::string name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
}

/**
  Deconstructor
*/
Entry::~Entry(){
}

/**
  Virtual method for Entry.  Made for the specific reason if someone
  tries to change directory into a file. 
*/
Directory* Entry::changeDir(Entry* dir, Directory* currentDir){
  std::cout<<"You cannot change directories into a file\n";
  return currentDir;
}

/**
  It sets the pointer to the parent directory for the Entry object
  @param par The parent direcotry
*/
void Entry::setParent(Directory* par){
  parent = par;
}

/*****************************************************************
*This class is derived from Entry.  The only difference between  *
*this class and a file is that directory contains a unordered_set*
*of all files and directories that it contains			 *
*****************************************************************/
class Directory:public Entry{
  public:
    Directory(std::string name);
    Directory* changeDir(Entry* dir, Directory* currentDir);
    std::unordered_set<Entry*> allEntries;
};

/*
  Constructor for the Directory
  @param name The name of the directory as seen by the user
*/
Directory::Directory(std::string name):Entry(name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
  parent = NULL;
  allEntries = {};
}

/*
  A method to navigate the file system
  @param dir* The directory that is trying to be changed to 
  @param dir* pointer to current directory
  @ret currentDir Returns pointer to the new direcotry that was accessed
*/
Directory* Directory::changeDir(Entry* dir, Directory* currentDir){
  currentDir = (Directory*)dir;						//dir could be a File or a Directory. Must type cast it 
  return currentDir;
}

/************************************************************
*A class representing a file.  Could get rid of class all   *
*together and just use Entry object to represent file but   *
*I believe by defining it as a class it makes it easier to  *
*understand the code.					    *
************************************************************/
class File:public Entry{
  public:
    File(std::string name);
};

/**
  Constructory for File class
  @param name Name of the file that will be seen by the user
*/
File::File(std::string name):Entry(name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
}

/****************************************************************
*The File system class defines the methods to actually use the  *
*Entry class's derived classes.  				*
****************************************************************/
class FileSystem{
  public:
    Directory* currentDir;						//Pointer so that the directory constructor is not called
    FileSystem();
    ~FileSystem();
    bool addFile(std::string name);
    bool addDir(std::string name);
    bool delEntry(std::string name);
    bool cd(std::string name);
    void printDir();
};

/**
  Constructor initializes a root directory and sets the current dir to it
*/
FileSystem::FileSystem(){
  Directory* dir = new Directory("root");
  currentDir = dir;
}

FileSystem::~FileSystem(){
}

/**
  Method to add a file to the current dir.  The method checks if the file name already
  exist in the directory.  If the name does not exist or the directory is empty then
  the file is created
  @param name The name of the new file
  @ret bool Lets the user know if the operation was successful or not
*/
bool FileSystem::addFile(std::string name){
  if(currentDir->allEntries.size() == 0) {
    File* newFile = new File(name);
    newFile->setParent(currentDir);
    currentDir->allEntries.insert(newFile);
    return true;
  }
  else{
    for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
      Entry* entryP = *i;						//We need to access every element in the set so that we can get the name and make sure
      Entry thisEntry = *entryP;					//the file/directory name does not already exist in the current Dir
      if(thisEntry.fileName == name){
        std::cout<<"This file name already exist in this directory\n";
	return false;
      }
    }
    File* newFile = new File(name);
    newFile->setParent(currentDir);
    currentDir->allEntries.insert(newFile);
    return true;
  }
}

/**
  Method to create a new directory in the current directory.  The method checks whether the
  name exist in the directory or not.  If the the name does not exist or the directory is empty
  then the directory is created and stored.  
  @param name Name of the new directory
  @ret bool Lets the user know if the operation was a success
*/
bool FileSystem::addDir(std::string name){
  if(currentDir->allEntries.size() == 0) {
    Directory* newDir = new Directory(name);
    newDir->setParent(currentDir);
    currentDir->allEntries.insert(newDir);
    return true;
  }
  else{
    for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
      Entry* entryP = *i;						//We need to access every element in the set so that we can get the name and make sure
      Entry thisEntry = *entryP;					//the file/directory name does not already exist in the current Dir
      if(thisEntry.fileName == name){
        std::cout<<"This dir name already exist in this directory\n";
	return false;
      }
    }
    Directory* newDir = new Directory(name);
    newDir->setParent(currentDir);
    currentDir->allEntries.insert(newDir);
    return true;
  }
}

/**
  A method that will delete a file or directory in the current directory
  @param name The name of the file/directory to be deleted
  @ret bool Whether an entry was successfully deleted
*/
bool FileSystem::delEntry(std::string name){
  for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
    Entry* entryP = *i;							//Make sure the entry name exist in the current dir
    if(entryP->fileName == name){
      currentDir->allEntries.erase(*i);
      return true;
    }
  }
  std::cout<<"That name does not exist in this directory\n";
  return false;
}

/*
  A method to navigate the file system.
  @param name The name of the directory to change to
  @ret bool Whether the operation was a success or not
*/
bool FileSystem::cd(std::string name){
  if(name == ".."){
    if(currentDir->parent == NULL){
      std::cout<<"This is the root dir\n";
      return false;
    }
    else{
      currentDir = currentDir->parent;
      return true;
    }
  }
  for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
    Entry* entryP = *i;							//Make sure the entry name exist in the current dir
    if(entryP->fileName == name){
      currentDir = entryP->changeDir(*i, currentDir);			//polymorphism is used to determine if the given name is a file or dir
    }
  }
}

/**
  A method to show all files and directories in current directory
*/
void FileSystem::printDir(){
  std::cout<<"The current dir is " << currentDir->fileName<<"\n";
  for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
    Entry* entryP = *i;
    std::cout<<entryP->fileName<<"\n";
  }
  std::cout<<"--------------------------\n";
}

int main(){
  FileSystem fs = FileSystem();
  fs.addFile("fil1");
  fs.addFile("fil2");
  fs.addFile("fil3");
  fs.addDir("dir1");
  fs.addDir("dir2");
  fs.printDir();
  fs.cd("dir2");
  fs.addDir("dir5");
  fs.addFile("fil1");
  fs.printDir();
  fs.cd("..");
  fs.printDir();
}
