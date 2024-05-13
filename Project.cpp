#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <ctime>

using namespace std;

string fileName = "inventory.txt";

// Abstract class for Items
class Item
{
protected:
    string name;
    double price;
    string myTime;
    string myDate;

public:
    Item(const string &name, double price, const string &myTime, const string &myDate) : name(name), price(price), myTime(myTime), myDate(myDate) {}

    // Virtual method for displaying item details
    virtual void displayDetails() const = 0;

    // Getter for item name
    string getName() const
    {
        return name;
    }

    // Getter for item price
    double getPrice() const
    {
        return price;
    }

    string getTime() const
    {
        return myTime;
    }
};

// Concrete classes for different types of products
class ElectronicProduct : public Item
{
private:
    string brand;

public:
    ElectronicProduct(const string &name, double price, const string &brand, const string &myTime, const string &myDate) : Item(name, price, myTime, myDate), brand(brand) {}

    // Override method to display electronic product details
    void displayDetails() const override
    {
        cout << "\tElectronic Product: " << name << endl;
        cout << "\tBrand: " << brand << endl;
        cout << "\tPrice: Rs " << price << endl;
        cout << "\tTime: " << myTime << endl;
        cout << "\tDate: " << myDate << endl;
    }
};

class FrozenFoodProduct : public Item
{
private:
    string expiryDate;

public:
    FrozenFoodProduct(const string &name, double price, const string &expiryDate, const string &myTime, const string &myDate) : Item(name, price, myTime, myDate), expiryDate(expiryDate) {}

    // Override method to display frozen food product details
    void displayDetails() const override
    {
        cout << "\tFrozen Food Product: " << name << endl;
        cout << "\tExpiry Date: " << expiryDate << endl;
        cout << "\tPrice: Rs " << price << endl;
        cout << "\tTime: " << myTime << endl;
        cout << "\tDate: " << myDate << endl;
    }
};

class BeautyProduct : public Item
{
private:
    string manufacturer;

public:
    BeautyProduct(const string &name, double price, const string &manufacturer, const string &myTime, const string &myDate) : Item(name, price, myTime, myDate), manufacturer(manufacturer) {}

    // Override method to display beauty product details
    void displayDetails() const override
    {
        cout << "\tBeauty Product: " << name << endl;
        cout << "\tManufacturer: " << manufacturer << endl;
        cout << "\tPrice: Rs " << price << endl;
        cout << "\tTime: " << myTime << endl;
        cout << "\tDate: " << myDate << endl;
    }
};

// Inventory class to manage items
class Inventory
{
private:
    vector<Item *> items;

public:
    // Method to add items to the inventory

    void addItem(Item *item)
    {
        items.push_back(item);
    }

    // Method to display all items in the inventory

    void displayInventory() const
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            items[i]->displayDetails();
            cout << endl;
        }
    }

    // Method to delete an item from the inventory by name

    void deleteItem(const string &itemName)
    {
        vector<Item *>::iterator it; // Iterator for the vector

        for (it = items.begin(); it != items.end();)
        {
            if ((*it)->getName() == itemName)
            {
                delete *it;
                it = items.erase(it); // Update iterator after erasing
                cout << "Item '" << itemName << "' deleted from inventory." << endl;
                removeItemFromFile(itemName);
                return;
            }
            else
            {
                ++it; // Move to the next element if not deleting
            }
        }

        cout << "Item '" << itemName << "' not found in inventory." << endl;
    }

    // Method t0 remove the item from "inventory.txt"

    void removeItemFromFile(const string &itemName)
    {
        ifstream inputFile(fileName);
        if (!inputFile.is_open())
        {
            cout << "Error: Unable to open file: inventory.txt" << endl;
            return;
        }

        vector<string> lines;
        string line;
        while (getline(inputFile, line))
        {
            if (line.find(itemName) == string::npos)
            {
                lines.push_back(line);
            }
        }
        inputFile.close();

        ofstream outputFile(fileName);
        if (!outputFile.is_open())
        {
            cout << "Error: Unable to open file: inventory.txt" << endl;
            return;
        }

        for (const string &currentLine : lines)
        {
            outputFile << currentLine << endl;
        }
        outputFile.close();
    }

    // Method to search for specific Product

    Item *searchItem(const string &itemName)
    {
        vector<Item *>::iterator it; // Iterator for the vector

        for (it = items.begin(); it != items.end(); ++it)
        {
            if ((*it)->getName() == itemName)
            {
                return *it;
            }
        }
        cout << "Item '" << itemName << "' not found in inventory." << endl;
        return nullptr;
    }

    // Destructor to free memory for items

    ~Inventory()
    {
        // Using a simple for loop to delete items
        for (size_t i = 0; i < items.size(); ++i)
        {
            delete items[i];
        }
    }
};

// Global Function to fetch data from "inventory.txt"

vector<Item *> readItemsFromFile(const string &filename)
{
    vector<Item *> items;

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return items;
    }

    string type;
    string name;
    double price;
    string additionalData;
    string myTime;
    string myDate;

    while (file >> type >> name >> price >> additionalData >> myTime >> myDate)
    {
        if (type == "Electronic:")
        {
            items.push_back(new ElectronicProduct(name, price, additionalData, myTime, myDate));
        }
        else if (type == "FrozenFood:")
        {
            items.push_back(new FrozenFoodProduct(name, price, additionalData, myTime, myDate));
        }
        else if (type == "Beauty:")
        {
            items.push_back(new FrozenFoodProduct(name, price, additionalData, myTime, myDate));
        }
    }

    file.close();

    return items;
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    vector<Item *> items = readItemsFromFile(fileName);

    Inventory inventory;

    for (Item *item : items)
    {
        inventory.addItem(item);
    }

    // Display inventory
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "===> Initial Inventory:" << endl;
    inventory.displayInventory();
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

    while (true)
    {
        // Get current time
        time_t currentTime = time(nullptr);
        tm *localTime = localtime(&currentTime);
        // Variables to store time and date
        char timeBuffer[9];  // Buffer to hold time (HH:MM:SS)
        char dateBuffer[11]; // Buffer to hold date (DD/MM/YYYY)
        strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
        strftime(dateBuffer, sizeof(dateBuffer), "%d/%m/%Y", localTime);
        string myTime(timeBuffer);
        string myDate(dateBuffer);

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "\n-->Menu Options:" << endl;
        cout << "\t1. Add Item" << endl;
        cout << "\t2. Delete Item" << endl;
        cout << "\t3. Display Inventory" << endl;
        cout << "\t4. Search specific Product" << endl;
        cout << "\t5. Find total number of products" << endl;
        cout << "\t6. Exit" << endl;
        cout << "Enter your choice: ";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

        int choice;
        cin >> choice;

        string itemName;
        string name, type, additionalData;
        double price;
        switch (choice)
        {
        case 1:
        {
            // Add item

            cout << "Enter item name: ";
            cin.ignore(); // Ignore newline character
            getline(cin, name);

            cout << "Enter item type (Electronic/FrozenFood/Beauty): ";
            getline(cin, type);

            cout << "Enter item price: ";
            cin >> price;

            if (type == "Electronic")
            {
                cout << "Enter brand: ";
                cin.ignore(); // Ignore newline character
                getline(cin, additionalData);
                inventory.addItem(new ElectronicProduct(name, price, additionalData, myTime, myDate));
            }
            else if (type == "FrozenFood")
            {
                cout << "Enter expiry myDate (DD/MM/YYYY): ";
                cin.ignore(); // Ignore newline character
                getline(cin, additionalData);
                inventory.addItem(new FrozenFoodProduct(name, price, additionalData, myTime, myDate));
            }
            else if (type == "Beauty")
            {
                cout << "Enter brand: ";
                cin.ignore(); // Ignore newline character
                getline(cin, additionalData);
                inventory.addItem(new BeautyProduct(name, price, additionalData, myTime, myDate));
            }
            else
            {
                cout << "Invalid item type!" << endl;
            }

            // Append new entry into the inventory file

            ofstream file(fileName, ios::app);

            if (file.is_open())
            {

                file << type << ":     " << name << "     " << price << "     " << additionalData << "     " << myTime << "     " << myDate << endl;
                file.close();
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "Data added successfully to invnentory.txt.\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "Error: Unable to open file for writing!\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            }

            break;
        }
        case 2:
        {
            // Delete item
            cout << "Enter item name to delete: ";
            cin.ignore(); // Ignore newline character
            getline(cin, itemName);

            inventory.deleteItem(itemName);
            break;
        }
        case 3:
            // Display inventory
            cout << "Inventory:" << endl;
            inventory.displayInventory();
            break;

        case 4:
        { // Search Item
            cout << "Enter item name to search: ";
            cin.ignore(); // Ignore newline character
            getline(cin, itemName);

            Item *it = inventory.searchItem(itemName);
            if (it != nullptr)
            {
                it->displayDetails();
                cout << endl;
            }

            break;
        }

        case 5:
        { // Total number of products
            int electronic_counter = 0;
            int frozenFood_counter = 0;
            int beauty_counter = 0;
            int total = 0;
            ifstream file(fileName);
            if (!file.is_open())
            {
                cout << "Error opening file: " << fileName << endl;
            }

            while (file >> type >> name >> price >> additionalData >> myTime >> myDate)
            {
                if (type == "Electronic:")
                {
                    electronic_counter++;
                }
                else if (type == "FrozenFood:")
                {
                    frozenFood_counter++;
                }
                else if (type == "Beauty:")
                {
                    beauty_counter++;
                }
            }
            total = electronic_counter + frozenFood_counter + beauty_counter;
            cout << "Total number of products: " << total << endl;
            cout << "Total number of electronic products: " << electronic_counter << endl;
            cout << "Total number of frozen food products: " << frozenFood_counter << endl;
            cout << "Total number of beauty products: " << beauty_counter << endl;

            break;
        }
        case 6:
            // Exit the program
            cout << "Exiting..." << endl;
            return 0;

        default:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Invalid choice! Please enter a valid option." << endl;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        }
    }

    return 0;
}