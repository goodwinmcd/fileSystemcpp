#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <unordered_set>

class Entry{
  public:
    std::chrono::time_point<std::chrono::system_clock> dateCreated;
    std::string fileName;
    Entry(std::string name);
    ~Entry();
};

Entry::Entry(std::string name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
}

Entry::~Entry(){
}

class Directory:public Entry{
  public:
    Directory(std::string name);
    void setParent(Directory* par);
    Directory* changeDir(Directory* dir, Directory* currentDir);
    std::unordered_set<Entry*> allEntries;
    Directory* parent;
};

Directory::Directory(std::string name):Entry(name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
}

void Directory::setParent(Directory* par){
  parent = par;
}

Directory* Directory::changeDir(Directory* dir, Directory* currentDir){
  currentDir = dir;
  return currentDir;
}

class File:public Entry{
  public:
    File(std::string name);
    void setParent(Directory* par);
    void changeDir(Directory* dir, Directory* currentDir);
    Directory* parent;
};

File::File(std::string name):Entry(name){
  fileName = name;
    dateCreated = std::chrono::system_clock::now();
}

void File::setParent(Directory* par){
  parent = par;
}

void File::changeDir(Directory* dir, Directory* currentDir){
  std::cout<<"You cannot change directories into a file\n";
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
};

FileSystem::FileSystem(){
  Directory dir = Directory("root");
  currentDir = &dir;
}

FileSystem::~FileSystem(){
}

bool FileSystem::addFile(std::string name){
  File newFile = File(name);
  newFile.setParent(currentDir);
  if(currentDir->allEntries.size() == 0) {
    currentDir->allEntries.insert(&newFile);
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
    currentDir->allEntries.insert(&newFile);
    return true;
  }
}

bool FileSystem::addDir(std::string name){
  Directory newDir = Directory(name);
  newDir.setParent(currentDir);
  if(currentDir->allEntries.size() == 0) {
    currentDir->allEntries.insert(&newDir);
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
    currentDir->allEntries.insert(&newDir);
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
    Entry thisEntry = *entryP;			//the file/directory name does not already exist in the current Dir
    if(thisEntry.fileName == name){
      currentDir = thisEntry.changeDir(*i, &currentDir);
    }
  }
}

int main(){
}
