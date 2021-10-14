#include "ListManager.h"
#include "TodoList.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <limits>

ListManager::ListManager (std::string dir, std::string file, std::string title): m_dir{dir}, m_activeList{file, title}
{
    for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            std::string file{entry.path()};
            std::string title{file.substr(8, file.length())};
            title.erase(title.end()-4, title.end());

            m_fileArr.push_back(file);
            m_titleArr.push_back(title);
        }

        m_activeIndex = 0;
        for (int i{0}; i < m_fileArr.size(); ++i)
        {
            if (m_fileArr[i].find("todo.dat") != std::string::npos)
            {
                m_activeIndex = i;
                break;
            } 
        }
}

void ListManager::newList()
{
    std::cout << "New List Name: ";
    std::string title;
    std::cin >> title;
    std::string path {m_dir + '/' + title + ".dat"};

    m_fileArr.push_back(path);
    m_titleArr.push_back(title);
    setActiveList(m_fileArr.size() - 1);
}

void ListManager::removeList()
{
    printNumbered();
    int num {0};

    while(true)
    {
        std::cout << "Select a list to delete: ";
        std::cin >> num;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Item not found\n";
            continue;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
        if (num < 1 || num > m_titleArr.size())
        {
            std::cout << "Out of Range\n";
            continue;
        }
        else break;
    }

    int remove{num-1};
    std::ifstream inf{m_fileArr[remove]};
    inf.close();
    std::filesystem::remove(m_fileArr[remove]);
    m_fileArr[remove] = "";
    m_titleArr[remove] = "";
    purgeArr();
    if (remove == m_activeIndex) selectList(true);
    else selectList();

}

void ListManager::printNumbered()
{
    int printedNum {1};
    std::cout << "\n**********\n";
    for (int i{0}; i < m_titleArr.size(); ++i)
    {
        if (m_titleArr[i] != "") std::cout << '[' << printedNum++ << ']' << ' ' << m_titleArr[i] << '\n';
    }
    std::cout << "**********\n\n";

}

void ListManager::selectList(bool deleteMode = false)
{
    printNumbered();
    int num {0};

    while(num < 1 || num > m_titleArr.size())
    {
        std::cout << "Select a list to open: ";
        std::cin >> num;
        std::cin.clear();
        std::cin.ignore(128, '\n');
    }

    if (deleteMode)
    {
        setActiveList(num - 1, deleteMode);
        m_activeList.print(); 
    }
    else
    {
        setActiveList(num - 1);
        m_activeList.print();
    }
}

void ListManager::setActiveList (int index, bool deleteMode = false)
{
    if (!deleteMode) m_activeList.saveToFile();
    m_activeIndex = index;
    m_activeList = TodoList{m_fileArr[m_activeIndex], m_titleArr[m_activeIndex]};
}

void ListManager::purgeArr()
{
    for (int i{0}; i < m_fileArr.size(); ++i)
    {
        if (m_fileArr[i] == "")
        {
            m_fileArr.erase(m_fileArr.begin() + i);
            m_titleArr.erase(m_titleArr.begin() + i);
        }
    }
}