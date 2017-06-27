#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <unordered_set>

class Directory{
  private:
    std::unordered_set<Directory*> allEntries;
  protected:
    std::chrono::time_point<std::chrono::system_clock> dateCreated;
    std::string fileName;
    Directory* parent;
  public:
    Directory(std::string name);
    Directory();
    ~Directory();
    void add(Directory name);
    bool del(std::string name);
    void printSet();
    bool rename(std::string name, std::string newName);
    void updateDate();
    void setParent(Directory* parent);
    void setName(std::string newName);
    std::string getName();
};

Directory::Directory(std::string name){
  fileName = name;
  dateCreated = std::chrono::system_clock::now();
  parent = NULL;
}

Directory::Directory(){
  
}

Directory::~Directory(){
}

void Directory::add(Directory name){
  if(allEntries.find(&name) != allEntries.end()) allEntries.insert(&name);
  else std::cout<<"That file name already exist in this directory\n";
}

bool Directory::del(std::string name){
  for(auto i = allEntries.begin(); i != allEntries.end(); i++){
    Directory* p = *i;
    Directory d = *p;
    if(d.getName() == name){
      allEntries.erase(*i);
      return true;
    }
  }
  return false;
}

void Directory::printSet(){
  for(auto i = allEntries.begin(); i != allEntries.end(); i++) std::cout<<*i << "\n";
}

/*******************************************
*Protected methods inherited by File class *
*******************************************/

void Directory::setParent(Directory* parent){
  parent = parent;
}

void Directory::setName(std::string newName){
  fileName = newName;
}

std::string Directory::getName(){
  return fileName;
}

bool Directory::rename(std::string name, std::string newName){
  for(auto i = allEntries.begin(); i != allEntries.end(); i++){
    Directory* p = *i;
    Directory d = *p;
    if(d.getName() == name){
      d.setName(newName);
      return true;
    }
  }
  return false;
}

class File:public Directory{

void add(Directory name){
  std::cout<<"You cannot add files to a file.  Only a directory. \n";
}

bool del(std::string name){
  std::cout<<"This is a file.  You cannot delete files from a file.\n";
  return false;
}

void printSet(){
  std::cout<<"This is a file.  There is nothing to print\n";
}

bool rename(std::string name, std::string newName){
  std::cout<<"This is a file.  You cannot do that.\n";
}

};

class FileSystem{
  public:
    FileSystem();
    ~FileSystem();
    void addDirectory(std::string name);
    void addFile(std::string name);
    void addDir(std::string name);
    void cd(std::string name);
  private:
    Directory currentDir; 
};

FileSystem::FileSystem(){
  Directory dir = Directory("root");
  dir.setParent(NULL);
  currentDir = dir;
}

void FileSystem::addDirectory(std::string name){
  Directory dir = Directory("name");
  dir.setParent(currentDir);
  currentDir.add(dir);
}

int main(){

}
