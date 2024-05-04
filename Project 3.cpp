// Author: Beatriz Ceschin Antunes
// Class: CMPSC 121
// Project 3
// Purpose: Program searches for parts in a txt file, then allows user to add new parts to the file. It also writes out the updated inventory information into a new file "updated_parts.txt". 
// Sources of logic assistance: N/A

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// defining parallel vectors that will be used to store info from parts.txt
vector<string> part_number;
vector<char> part_class;
vector<int> part_ohb;
vector<double> part_cost;

// function prototypes
bool get_data(vector<string>& part_number, vector<char>& part_class, vector<int>& part_ohb, vector<double>& part_cost);
int bin_search(string key, const vector<string>& part_number);
string get_target();
void get_more_data(char& class_in, int& part_ohb_in, double& part_cost_in);
void insert_data(vector<string>& part_number, vector<char>& part_class, vector<int>& part_ohb, vector<double>& part_cost, string part_in, char class_in, int part_ohb_in, double part_cost_in);
void display(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb, const vector <double>& part_cost, int finder);
void sort(vector<string>& part_number, vector<char>& part_class, vector<int>& part_ohb, vector<double>& part_cost);
void print_stats(int searches, int good, int bad);
void put_data(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb, const vector <double>& part_cost);

// swaps two items in a vector
template <class CType>
void swapper(CType& a, CType& b)
{
    CType temp;
    temp = a;
    a = b;
    b = temp;
}

int main()
{
    // defining variables for counting
    int searches = 0; // Number of searches
    int good = 0;     // Number of successful searches (part is found)
    int bad = 0;      // Number of unsuccessful searches (part isn't found)

    // variable for user to choose whether they'd like to do additional search
    char anotherSearch;

    // Reads data from file and fills vectors
    get_data(part_number, part_class, part_ohb, part_cost);
    // sorts data 
    sort(part_number, part_class, part_ohb, part_cost);

    do
    {
        // stores the part number being searched in file
        string target_part_number = get_target();

        // does binary search for part number 
        int finder = bin_search(target_part_number, part_number);

        // if part is found
        if (finder != -1)
        {
            // displays information about part
            display(part_number, part_class, part_ohb, part_cost, finder);
            // increases count of successful searches
            good++;
        }
        // if part is not found
        else
        {
            // variables used to store data for new part
            char class_in;
            int part_ohb_in;
            double part_cost_in;

            // increases count of unsuccessful searches (part was not found)
            bad++;

            // gets confirmation from user if they want to add new part
            char addPart;
            cout << "Add this part? (Y or N)\n";
            cin >> addPart;

            // if they want to add new part
            if (addPart == 'Y' || addPart == 'y')
            {
                // asks for data for new part
                get_more_data(class_in, part_ohb_in, part_cost_in);
                // adds new data to vectors
                insert_data(part_number, part_class, part_ohb, part_cost, target_part_number, class_in, part_ohb_in, part_cost_in);
                // resorts list of parts
                sort(part_number, part_class, part_ohb, part_cost);
                // displays confirmation message to user
                cout << "Part number " << target_part_number << " added to inventory." << endl;
            }
        }
        // increases count of overall searches
        searches++;

        // asks user if they'd like to continue
        cout << "Would you like to do another search? (Y or N)\n";
        cin >> anotherSearch;

    } while (anotherSearch == 'Y' || anotherSearch == 'y');

    // outputs data into new txt file
    put_data(part_number, part_class, part_ohb, part_cost);
    // displays number of successful, unsuccessful and overall searches to user
    print_stats(searches, good, bad);

    return 0;
}

// fills vectors with data from parts.txt
bool get_data(vector<string>& part_number, vector<char>& part_class, vector<int>& part_ohb, vector<double>& part_cost)
{
    // defining input file stream object
    ifstream file("C:/CMPSC121/homework/module 15/Project 3/parts.txt");

    if (!file.is_open())
    { // tests if file was opened successfully
        cout << "Error opening file!" << endl;
        return false;
    }

    // string to be used when reading data from parts.txt
    string line;

    // Read data from file into vectors
    while (getline(file, line))
    {
        istringstream iss(line);

        // variables for data from file
        string number;
        char cls;
        int ohb;
        double cost;

        // fills vectors
        iss >> number >> cls >> ohb >> cost;
        part_number.push_back(number);
        part_class.push_back(cls);
        part_ohb.push_back(ohb);
        part_cost.push_back(cost);
    }

    // closes file
    file.close();

    return true;
}

// Does a binary search for specific part number
int bin_search(string key, const vector<string>& part_number)
{
    // variables to calculate beginning, middle and end of list of values
    int first = 0,
        last = part_number.size() - 1,
        middle,
        position = -1; // position of search value
    bool found = false;

    while (!found && first <= last)
    {
        // calculates midpoint
        middle = (first + last) / 2;
        if (part_number[middle] == key) // if value is found
        {
            found = true;
            position = middle;
        }
        else if (part_number[middle] > key)
        { 
            last = middle - 1; // if in lower half
        }
        else
            first = middle + 1; // if in upper half
    }

    return position;
}

// Asks user for a part number to search for
string get_target()
{
    string part_number;

    // displays message to user and asks for part number
    cout << "What part number would you like to look for?\n";
    cin >> part_number;

    return part_number;
}

// asks for more information about new part number that will be added to inventory
void get_more_data(char& class_in, int& part_ohb_in, double& part_cost_in)
{
    // asks for class
    cout << "What class is this part number in?\n";
    cin >> class_in;

    // asks for on hand balance
    cout << "What is the on hand balance of this part?\n";
    cin >> part_ohb_in;

    // asks for part's cost
    cout << "What is the cost of this part?\n";
    cin >> part_cost_in;
}

// Inserts new part number data into vectors
void insert_data(vector<string>& part_number, vector<char>& part_class, vector<int>& part_ohb, vector<double>& part_cost, string part_in, char class_in, int part_ohb_in, double part_cost_in)
{
    part_number.push_back(part_in);
    part_class.push_back(class_in);
    part_ohb.push_back(part_ohb_in);
    part_cost.push_back(part_cost_in);
}

// Displays info for part number found in inventory
void display(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb, const vector <double>& part_cost, int finder)
{
    // displays information to user
    cout << "There are " << part_ohb[finder] << " of part number " << part_number[finder] << " in inventory." << endl;
    cout << "It is a class " << part_class[finder] << " part." << endl;
    cout << "The cost is $" << fixed << setprecision(2) << part_cost[finder] << "." << endl;
    cout << "The value of that inventory is $" << fixed << setprecision(2) << part_ohb[finder] * part_cost[finder] << "." << endl;
}

// uses shell short to sort vectors using their part number
void sort(vector<string>& part_number, vector<char>& part_class, vector<int>& part_ohb, vector<double>& part_cost)
{
    bool flag = true;
    int i, numLength = part_number.size();

    int d = numLength;
    while (flag || (d > 1)) // bool flag
    {
        flag = false; // resets flag to 0 to check for future swaps
        d = (d + 1) / 2;
        for (i = 0; i < (numLength - d); i++)
        {
            if (part_number[i + d] < part_number[i])
            {
                // Swap part_number elements
                swapper(part_number[i], part_number[i + d]);
                // Swap corresponding elements in other vectors
                swapper(part_class[i], part_class[i + d]);
                swapper(part_ohb[i], part_ohb[i + d]);
                swapper(part_cost[i], part_cost[i + d]);

                flag = true; // swapped
            }
        }
    }
}

// prints search stats to user
void print_stats(int searches, int good, int bad)
{
    // displays message to user
    cout << "There were " << searches << " searches performed today." << endl;
    cout << good << " of them were successful." << endl;
    cout << bad << " of them were not in the system." << endl;
    cout << "The file is now updated." << endl;
}

// writes out file with all updates of new added parts and sorting
void put_data(const vector<string>& part_number, const vector<char>& part_class, const vector<int>& part_ohb, const vector<double>& part_cost)
{
    // defining output file stream object
    ofstream fout;

    // opening output file
    fout.open("updated_parts.txt");

    // testing if open was successful
    if (!fout)
    {
        cout << "Output file open failure!";
    }

    // Writing data to file
    for (size_t i = 0; i < part_number.size(); i++)
    {
        fout << part_number[i] << "\t" << part_class[i] << "\t" << part_ohb[i] << "\t" << fixed << setprecision(2) << part_cost[i] << endl;
    }

    fout.close();
}

/*
Execution Sample:

What part number would you like to look for?
P-12385
There are 41 of part number P-12385 in inventory.
It is a class B part.
The cost is $27.33.
The value of that inventory is $1120.53.
Would you like to do another search? (Y or N)
y
What part number would you like to look for?
P-14518
There are 9 of part number P-14518 in inventory.
It is a class D part.
The cost is $5.56.
The value of that inventory is $50.04.
Would you like to do another search? (Y or N)
y
What part number would you like to look for?
P-12345
Add this part? (Y or N)
y
What class is this part number in?
B
What is the on hand balance of this part?
20
What is the cost of this part?
15.00
Part number P-12345 added to inventory.
Would you like to do another search? (Y or N)
y
What part number would you like to look for?
P-12345
There are 20 of part number P-12345 in inventory.
It is a class B part.
The cost is $15.00.
The value of that inventory is $300.00.
Would you like to do another search? (Y or N)
n
There were 4 searches performed today.
3 of them were successful.
1 of them were not in the system.
The file is now updated.

*/