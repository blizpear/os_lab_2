//
// Created by blizpear on 9/27/20.
//
#include "File.h"
#include <iostream>



void move(const char *from,const char *to){
    rename(from, to);
}
void copy (std::filesystem::path what, std::filesystem::path to){
    try
    {
        fs::create_directories(to);
        fs::copy_file(what, to / what.filename(), fs::copy_options::overwrite_existing);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what();

    }
}

void delete_f(const char *file){
    remove(file);
}

unsigned long size(std::string dest){
    struct stat fileInfo;
    unsigned long size_tot=0;

    if(stat(dest.c_str(), &fileInfo)!=0){
        std::string err="Нет информации о файле "+dest;
        throw err;
    }

    if (S_ISDIR(fileInfo.st_mode)){
        DIR *dir=opendir(dest.c_str());
        if (dir == nullptr){
            std::string err="Папка"+ dest +"не открывается";
            throw err;
        }

        dirent *contentInfo;
        while ((contentInfo = readdir(dir))!=nullptr){
            if ((strcmp(contentInfo->d_name,".")!=0)&&(strcmp(contentInfo->d_name,"..")!=0)){
                std::string pathFile=dest+"/"+contentInfo->d_name;
                size_tot+=size(pathFile);
            }
        }
        closedir(dir);
        return size_tot;
    }
    else return fileInfo.st_size;
}

void show_all(char *dest){
    DIR *dir = opendir(dest);
    if (dir == nullptr) std::cout<<"проблема с путём";
    dirent *contentInfo;
    std::cout<<"В каталоге "<<dest<<":"<<std::endl;
    while ((contentInfo=readdir(dir))!= nullptr)
        std::cout<<contentInfo->d_name<<std::endl;
    closedir(dir);
}
std::string getPidInfo(std::ifstream &file, const int &offset)
{
    std::string buf;
    for (int i = 0; i < offset; i++)
    {
        buf.clear();
        file >> buf;
    }
    return buf;
}
char *getContent(char *str)
{
    char *buf = new char(64);
    char ch;
    int i = 0;

    do
    {
        ch = str[i];
        buf[i] = ch;
        ++i;
    } while (ch != '\\' && ch != '\0');

    buf[i] = '\0';

    return buf;
}
void procfs(){
    DIR *dir = opendir("/proc/");

    dirent *contentInfo;

    std::cout << std::setiosflags(std::ios::left);
    std::cout << std::setw(50) << "COMM";
    std::cout << std::setw(15) << "PID";
    std::cout << std::setw(15) << "STATE";
    std::cout << std::setw(15) << "PPID";
    std::cout << std::setw(15) << "NICE" << std::endl;

    while ((contentInfo = readdir(dir)) != nullptr)
    {
        char *pid = getContent(contentInfo->d_name);
        if (atoi(pid) != 0)
        {
            std::string pidPath = "/proc/";
            pidPath += pid;

            std::ifstream pidInfo(pidPath + "/stat", std::ios_base::in);
            if (!pidInfo.is_open())
            {
                std::string error = "Cannot open file " + pidPath;
                throw error;
            }

            std::cout << std::setw(50) << getPidInfo(pidInfo, 2);
            std::cout << std::setw(15) << pid;
            std::cout << std::setw(15) << getPidInfo(pidInfo, 1);
            std::cout << std::setw(15) << getPidInfo(pidInfo, 1);
            std::cout << std::setw(15) << getPidInfo(pidInfo, 15) << std::endl;
        }
        delete pid;
    }

    closedir(dir);

}
