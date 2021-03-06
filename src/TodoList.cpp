#include "TodoList.h"


TodoList::TodoList(std::string file, std::string title)
{
    //  TodoList Constructor, takes the path and title of a todo .dat file
    name = title;
    setFile(file);
    fileToArr();
    purgeArr();
}

void TodoList::setFile(std::string file)
{
    //  Startup function that can probably be eliminated, see where used

    //todo (maybe): set name of TodoList also
    filePath = file;
    fileToArr();
}

void TodoList::fileToArr()
{
    //  Looks at TodoList.filePath, takes file contents and populates TodoList.todoArr with them
    std::ifstream inf{filePath};
    int listLength {};
    std::string temp;
    while (inf)
    {
        std::getline(inf, temp);
        ++listLength;
    }

    todoArr.resize(listLength);
    inf.close();
    inf.open(filePath);

    int counter {0};
    while (inf && counter < listLength - 1)
    {
        std::getline(inf, temp);
        todoArr[counter] = temp;
        ++counter;
    }
}

void TodoList::purgeArr()
{
    //  Removes any TodoList.todoArr elements that are ""
    for(int i{0}; i < todoArr.size(); ++i)
    {
        if (todoArr[i] == "")
        {
            todoArr.erase(todoArr.begin() + i);
        }
    }
}

void TodoList::print() const
{
    //  Prints out formated version of TodoList
    std::cout << "\n\t***************\n\n\tTo-Do: "<< name << "\n\t--------------\n";
    for (auto i : todoArr)
    {
        if (i != "") std::cout << "\t- " << i << '\n';
    }
    std::cout << "\n\t***************\n\n";
}

void TodoList::printNumbered() 
{
    //  Prints out numbered version of TodoList
    purgeArr();
    std::cout << "\n\t***************\n";
    for (int i{0}; i < todoArr.size(); ++i)
    {
        std::cout << "\t[" << i+1 << "] " << todoArr[i] << '\n';
    }
    std::cout << "\t***************\n\n";
}

void TodoList::addFromUser()
{
    //  Gets user input for new list entry and adds to TodoList.todoArr
    std::cout << "Add to list: ";
    std::string newItem{};
    std::getline(std::cin >> std::ws, newItem);
    std::cin.clear();

    todoArr.push_back(newItem);
}

void TodoList::addItem (std::string item)
{
    todoArr.push_back(item);
}

void TodoList::removeItemFromUser()
{
    //  Gets user input and removes selected TodoList.todoArr entry
    int num {};

    while(true)
    {
        std::cout << "Select number to remove: ";
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
        
        if (num < 1 || num > todoArr.size())
        {
            std::cout << "Item not found.\n";
            continue;
        }
        else 
        {
            break;
        }

    }

    todoArr[num-1] = "";
    purgeArr();
}

void TodoList::removeItem (int index)
{
    if (index >= 0 && index < todoArr.size())
    {
        todoArr[index] = "";
        purgeArr();
    }
    else
    {
        std::cout << "Invalid Parameters\n";
    }
}

void TodoList::saveToFile()
{
    //  Saves TodoList.todoArr to TodoList.filePath
    std::ofstream outf{filePath, std::ios::trunc};
    for (std::string item : todoArr)
    {
        if (item != "") outf << item << '\n';
    } 
}