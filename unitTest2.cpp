#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <unordered_set>

class Directory;

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

Entry::Entry(std::string name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
}

Entry::~Entry(){
}

Directory* Entry::changeDir(Entry* dir, Directory* currentDir){
  std::cout<<"You cannot change directories into a file\n";
  return currentDir;
}

void Entry::setParent(Directory* par){
  parent = par;
}

class Directory:public Entry{
  public:
    Directory(std::string name);
    Directory* changeDir(Entry* dir, Directory* currentDir);
    std::unordered_set<Entry*> allEntries;
};

Directory::Directory(std::string name):Entry(name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
  parent = NULL;
  allEntries = {};
}


Directory* Directory::changeDir(Entry* dir, Directory* currentDir){
  currentDir = (Directory*)dir;
  return currentDir;
}

class File:public Entry{
  public:
    File(std::string name);
};

File::File(std::string name):Entry(name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
}

class FileSystem{
  public:
    Directory* currentDir;				//Pointer so that the directory constructor is not called
    FileSystem();
    ~FileSystem();
    bool addFile(std::string name);
    bool addDir(std::string name);
    bool delEntry(std::string name);
    bool cd(std::string name);
    void printDir();
};

FileSystem::FileSystem(){
  Directory* dir = new Directory("root");
  currentDir = dir;
}

FileSystem::~FileSystem(){
}

bool FileSystem::addFile(std::string name){
  File* newFile = new File(name);
  newFile->setParent(currentDir);
  if(currentDir->allEntries.size() == 0) {
    currentDir->allEntries.insert(newFile);
    return true;
  }
  else{
    for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
      Entry* entryP = *i;				//We need to access every element in the set so that we can get the name and make sure
      Entry thisEntry = *entryP;			//the file/directory name does not already exist in the current Dir
      if(thisEntry.fileName == name){
        std::cout<<"This file name already exist in this directory\n";
	return false;
      }
    }
    currentDir->allEntries.insert(newFile);
    return true;
  }
}

bool FileSystem::addDir(std::string name){
  Directory* newDir = new Directory(name);
  newDir->setParent(currentDir);
  if(currentDir->allEntries.size() == 0) {
    currentDir->allEntries.insert(newDir);
    return true;
  }
  else{
    for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
      Entry* entryP = *i;				//We need to access every element in the set so that we can get the name and make sure
      Entry thisEntry = *entryP;			//the file/directory name does not already exist in the current Dir
      if(thisEntry.fileName == name){
        std::cout<<"This dir already exist in this directory\n";
	return false;
      }
    }
    currentDir->allEntries.insert(newDir);
    return true;
  }
}

bool FileSystem::delEntry(std::string name){
  for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
    Entry* entryP = *i;				//We need to access every element in the set so that we can get the name and make sure
    Entry thisEntry = *entryP;			//the file/directory name does not already exist in the current Dir
    if(thisEntry.fileName == name){
      currentDir->allEntries.erase(*i);
      return true;
    }
  }
  std::cout<<"That name does not exist in this directory\n";
  return false;
}

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
    Entry* entryP = *i;				//We need to access every element in the set so that we can get the name and make sure
    if(entryP->fileName == name){
      currentDir = entryP->changeDir(*i, currentDir);
    }
  }
}

void FileSystem::printDir(){
  for (auto i = currentDir->allEntries.begin(); i != currentDir->allEntries.end(); i++){
    Entry* entryP = *i;
    std::cout<<entryP->fileName<<"\n";
  }
}

int main(){
  FileSystem fs = FileSystem();
  fs.addFile("fil1");
  fs.addFile("fil2");
  fs.addFile("fil3");
  fs.addDir("dir1");
  fs.addDir("dir2");
  fs.printDir();
  std::cout<<"--------------------\n";
  fs.delEntry("dir2");
  fs.printDir();
  std::cout<<"--------------------\n";
  fs.cd("fil1");
  fs.addDir("dir5");
  fs.addFile("fil1");
  fs.printDir();
}
