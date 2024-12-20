#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <thread>
#include <chrono>
#include <conio.h>
#include <unordered_map>
#include <filesystem>
#include <set>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>


using namespace std;


// Struct for athlete records

struct Athlete {
    string athleteID, name, gender, type, countryName, nativeLanguage;
    string eventType, eventName, dob, medalID, medalType, medalRank;
    string opponentName, game, eventStartTime, eventEndTime, venue, stadium;
    Athlete* next;
    bool isChanged = false;
};

// Global variables

const string userDatabasePath = "users.dat";
set<int> changedIndices;

Athlete* head = nullptr; // Linked list head
unordered_map<string, string> userDatabase;



// Function prototypes

void animatedText();
void authenticationPage();
void registerUser();
bool loginUser();
string readPassword();
void mainMenu();
void loadFromCSV();
bool saveToCSV();
void viewRecords();
void searchAthlete();
void addAthlete();
void updateAthlete();
void deleteAthlete();
void showChangedRecords();
void insertAthlete(const Athlete& athlete);
Athlete* searchAthleteByID(const string& id);
void displayAthlete(const Athlete& athlete);
void deleteAllNodes();
void ensureDatabaseFileExists();
void saveUserToFile(const string& email, const string& password);
void updateCSVRecord(const string& id, const Athlete& updatedAthlete);
void displayThankYouMessage();
void displayGroupMates();
void showCountryStatistics();
void showTotalAthletes();
void showTopRankingCountriesAndAthletes();
void showPopularSportsAndMedalCounts();





// Animation helper

void animatedText(const string& text, int delay) {
    for (char ch : text) {
        cout << ch;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

// Authentication Page

void authenticationPage() {
    bool loggedIn = false;

    while (!loggedIn) {
        int choice;
        cout << "1. Login\n2. Register\nChoose an option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            animatedText("Invalid option. Please enter 1 or 2.\n", 10);
            continue;
        }

        switch (choice) {
            case 1:
                loggedIn = loginUser();
                break;
            case 2:
                registerUser();
                break;
            default:
                animatedText("Invalid option. Try again.\n", 10);
        }
    }
}


// Register User
void registerUser() {
    string email, password;
    while (true) {
        cout << "Enter your Email: ";
        cin >> email;

        // Check email is already registered ????
        if (userDatabase.find(email) != userDatabase.end()) {
            animatedText("Email already registered. Try logging in.\n", 10);
        } else {
            break;
        }
    }


    password = readPassword();


    userDatabase[email] = password;
    saveUserToFile(email, password);
    animatedText("Registration completed successfully. You may now proceed to log in.......\n", 10);

}
// Ensure database file exists
void ensureDatabaseFileExists() {
    ifstream file(userDatabasePath);
    if (!file) {
        ofstream newFile(userDatabasePath);
        if (newFile.is_open()) {
           // cout << "New user database successfully created at: " << userDatabasePath << endl;
        } else {
            cerr << "Error: Could not create database file at " << userDatabasePath << endl;
            exit(1);
        }
    }
    file.close();
}

// Function to load user data from a file

void loadUsersFromFile() {
    ifstream file(userDatabasePath);
    if (!file.is_open()) {
        animatedText("No existing user database found. Initializing a new one.\n", 10);
        return;
    }

    string email, password;
    while (getline(file, email) && getline(file, password)) {
        userDatabase[email] = password;
    }
    file.close();

      //animatedText("User data loaded successfully.\n", 50);
}



// Function to add a new user to the database file

void saveUserToFile(const string& email, const string& password) {
    ofstream file(userDatabasePath, ios::app);
    if (!file.is_open()) {
        animatedText("Error: Failed to save user data..\n", 15);
        return;
    }
    file << email << "\n" << password << "\n";
    file.close();
}




// Login User

bool loginUser() {
    string email, password;
    cout << "Enter your Email: ";
    cin >> email;
    password = readPassword();

    if (userDatabase.find(email) != userDatabase.end() && userDatabase[email] == password) {
        animatedText("Login successful!\n", 10);
        return true;
    } else {
        animatedText("Invalid email or password.\n", 10);
        return false;
    }
}

// Function to read password with masking

string readPassword() {
    string password;
    char ch;
    cout << "Enter Password: ";
    while ((ch = _getch()) != '\r') { // '\r' is Enter
        if (ch == '\b') {             // Handle backspace
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            cout << '*';
            password += ch;
        }
    }
    cout << "\n";
    return password;
}

// Helper function to count the number of rows in a CSV file

int countLinesInFile(const string& filePath) {
    ifstream file(filePath);
    int lineCount = 0;
    string line;
    while (getline(file, line)) {
        lineCount++;
    }
    return lineCount;
}


// Function to load data from a CSV file

void loadFromCSV() {
    ifstream file("athletes.csv");
    if (!file.is_open()) {
       animatedText("No CSV file found. Initializing a new file......\n\n", 15);
        return;
    }

    string line;
    while (getline(file, line)) {
        Athlete athlete;
        size_t pos = 0, index = 0;
        string fields[18];

        while ((pos = line.find(',')) != string::npos) {
            fields[index++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        fields[index] = line;

        athlete = {fields[0], fields[1], fields[2], fields[3], fields[4], fields[5],
                   fields[6], fields[7], fields[8], fields[9], fields[10], fields[11],
                   fields[12], fields[13], fields[14], fields[15], fields[16], fields[17], nullptr};
        insertAthlete(athlete);
    }
    file.close();
    //animatedText("Data loaded from CSV successfully.\n", 15);
}






// View all records


void viewRecords() {
    if (!head) {
        animatedText("No records available.\n", 15);
        return;
    }

    while (true) {
        cout << "1. View All Records\n2. View Paginated Records (5 per page)\n3. Show a specific record by index\n4. Back to Main Menu\nChoose: ";
        int choice;

        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            animatedText("Invalid option. Please input a valid number (1-4).\n", 15);
            continue;
        }

        switch (choice) {
            case 1: {
                Athlete* current = head;
                int totalRecords = 0;

                while (current) {
                    displayAthlete(*current);
                    current = current->next;
                    totalRecords++;
                }
                cout << "Total Records: " << totalRecords << "\n";
                return;
            }
            case 2: {
                Athlete* current = head;
                vector<Athlete*> pages;
                int totalRecords = 0;

                while (current) {
                    totalRecords++;
                    pages.push_back(current);
                    for (int i = 0; i < 5 && current; ++i) {
                        current = current->next;
                    }
                }

                int totalPages = pages.size();
                int currentPage = 0;

                while (true) {
                    // Display the current page
                    current = pages[currentPage];
                    cout << "\nPage " << (currentPage + 1) << " of " << totalPages << ":\n";
                    for (int i = 0; i < 5 && current; ++i) {
                        displayAthlete(*current);
                        current = current->next;
                    }

                    // Navigation options
                    char nav;
                    cout << "\n[N] Next Page, [P] Previous Page, [Q] Quit Viewing, [B] Back to Main Menu: ";
                    cin >> nav;
                    nav = tolower(nav);

                    if (nav == 'q') return; // Quit viewing
                    if (nav == 'b') return; // Back to main menu
                    if (nav == 'n') {
                        if (currentPage + 1 < totalPages) {
                            currentPage++; // Move to the next page
                        } else {
                            animatedText("You are on the last page.\n", 10);
                        }
                    } else if (nav == 'p') {
                        if (currentPage > 0) {
                            currentPage--; // Move to the previous page
                        } else {
                            animatedText("You are on the first page.\n", 10);
                        }
                    } else {
                        animatedText("Invalid input. Press 'N', 'P', 'Q', or 'B'.\n", 10);
                    }
                }
                break;
            }
            case 3: {
                cout << "Please enter the index of the record you wish to view: ";
                int recordIndex;
                if (!(cin >> recordIndex)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    animatedText("Invalid input. Please enter a valid index.\n", 15);
                    break;
                }

                Athlete* current = head;
                int count = 1;

                while (current && count < recordIndex) {
                    current = current->next;
                    count++;
                }

                if (current) {
                    displayAthlete(*current);
                } else {
                    animatedText("No record found at the specified index.\n", 15);
                }
                break;
            }
            case 4: {
                animatedText("Returning to main menu.....\n", 10);
                return;
            }
            default: {
                animatedText("Invalid option. Please input a valid number (1-4).\n", 15);
            }
        }
    }
}



// Function to read and display data directly from the CSV file
void readData() {
    ifstream file("athletes.csv");
    if (!file.is_open()) {
        animatedText("Error: Unable to open the CSV file.\n", 20);
        return;
    }

    string line;
    int index = 1;

    cout << "\n--- Reading Athlete Records from CSV ---\n";
    while (getline(file, line)) {
        cout << "Record " << index++ << ": " << line << "\n";
    }

    file.close();
    animatedText("\nCSV Reading Complete!\n", 20);
}







// Search for an athlete record


void searchAthlete() {
    if (!head) {
        animatedText("No records available.\n", 15);
        return;
    }

    while (true) {
        cout << "\n1. Unique Search (Athlete ID, Name, Medal ID)\n";
        cout << "2. Common Search (Gender, Type, Country Name, etc.)\n";
        cout << "3. Back to Main Menu\nChoose: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            animatedText("Invalid choice. Please input a valid number (1-3).\n", 15);
            continue;
        }

        if (choice == 3) {
            animatedText("Returning to main menu...\n", 10);
            return;
        }

        cout << "Please enter your search query: ";
        cin.ignore();
        string query;
        getline(cin, query);

        Athlete* current = head;
        bool found = false;

        while (current) {
            if ((choice == 1 && (current->athleteID == query || current->name == query || current->medalID == query)) ||
                (choice == 2 && (current->gender == query || current->type == query || current->countryName == query ||
                                 current->nativeLanguage == query || current->eventType == query || current->eventName == query ||
                                 current->dob == query || current->medalType == query || current->medalRank == query ||
                                 current->opponentName == query || current->game == query || current->eventStartTime == query ||
                                 current->eventEndTime == query || current->venue == query || current->stadium == query))) {
                displayAthlete(*current);
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            animatedText("No matching records found.\n", 15);
        }
    }
}




// Add a new record

void addAthlete() {
    Athlete newAthlete;

    cout << "Enter Athlete ID (Leave blank if not applicable): ";
    getline(cin, newAthlete.athleteID);

    cout << "Enter Name (Leave blank if not applicable): ";
    getline(cin, newAthlete.name);

    cout << "Enter Gender (Leave blank if not applicable): ";
    getline(cin, newAthlete.gender);

    cout << "Enter Type (Leave blank if not applicable): ";
    getline(cin, newAthlete.type);

    cout << "Enter Country Name (Leave blank if not applicable): ";
    getline(cin, newAthlete.countryName);

    cout << "Enter Native Language (Leave blank if not applicable): ";
    getline(cin, newAthlete.nativeLanguage);

    cout << "Enter Event Type (Leave blank if not applicable): ";
    getline(cin, newAthlete.eventType);

    cout << "Enter Event Name (Leave blank if not applicable): ";
    getline(cin, newAthlete.eventName);

    cout << "Enter Date of Birth (Leave blank if not applicable): ";
    getline(cin, newAthlete.dob);

    cout << "Enter Medal ID (Leave blank if not applicable): ";
    getline(cin, newAthlete.medalID);

    cout << "Enter Medal Type (Leave blank if not applicable): ";
    getline(cin, newAthlete.medalType);

    cout << "Enter Medal Rank (Leave blank if not applicable): ";
    getline(cin, newAthlete.medalRank);

    cout << "Enter Opponent Name (Leave blank if not applicable): ";
    getline(cin, newAthlete.opponentName);

    cout << "Enter Game (Leave blank if not applicable): ";
    getline(cin, newAthlete.game);

    cout << "Enter Event Start Time (Leave blank if not applicable): ";
    getline(cin, newAthlete.eventStartTime);

    cout << "Enter Event End Time (Leave blank if not applicable): ";
    getline(cin, newAthlete.eventEndTime);

    cout << "Enter Venue (Leave blank if not applicable): ";
    getline(cin, newAthlete.venue);

    cout << "Enter Stadium (Leave blank if not applicable): ";
    getline(cin, newAthlete.stadium);


    insertAthlete(newAthlete);

    // Save new record to the CSV
    ofstream file("athletes.csv", ios::app);
    if (file.is_open()) {
        file << newAthlete.athleteID << "," << newAthlete.name << "," << newAthlete.gender << ","
             << newAthlete.type << "," << newAthlete.countryName << "," << newAthlete.nativeLanguage << ","
             << newAthlete.eventType << "," << newAthlete.eventName << "," << newAthlete.dob << ","
             << newAthlete.medalID << "," << newAthlete.medalType << "," << newAthlete.medalRank << ","
             << newAthlete.opponentName << "," << newAthlete.game << "," << newAthlete.eventStartTime << ","
             << newAthlete.eventEndTime << "," << newAthlete.venue << "," << newAthlete.stadium << "\n";
        file.close();
        animatedText("New record added successfully.\n", 10);
    } else {
        animatedText("Error: Unable to save the record to CSV file.\n", 10);
    }
}




bool saveToCSV() {
    // Open the CSV file in truncate mode to overwrite it
    ofstream file("athletes.csv", ios::trunc);
    
    // Check if file opens successfully
    if (!file.is_open()) {
        animatedText("Error: Unable to open the CSV file for saving records.\n", 15);
        return false;
    }

    // Pointer to traverse the linked list
    Athlete* current = head;

    // Iterate through the linked list and save data to CSV
    while (current) {
        file << current->athleteID << "," 
             << current->name << "," 
             << current->gender << "," 
             << current->type << "," 
             << current->countryName << "," 
             << current->nativeLanguage << "," 
             << current->eventType << "," 
             << current->eventName << "," 
             << current->dob << "," 
             << current->medalID << "," 
             << current->medalType << "," 
             << current->medalRank << "," 
             << current->opponentName << "," 
             << current->game << "," 
             << current->eventStartTime << "," 
             << current->eventEndTime << "," 
             << current->venue << "," 
             << current->stadium << "\n";

        current = current->next; // Move to the next node
    }

    // Close the file
    file.close();

    animatedText("All records have been successfully saved to athletes.csv.\n", 10);
    return true; // Indicate success
}

// Update an existing record


void updateAthlete() {
    if (!head) {
        animatedText("No records available.\n", 10);
        return;
    }

    animatedText("Enter Athlete ID to update: ", 15);
    string id;
    cin >> id;
    cin.ignore();

    Athlete* athlete = searchAthleteByID(id);
    if (!athlete) {
        animatedText("No record found for the specified Athlete ID.\n", 25);
        return;
    }

    cout << "Updating record for Athlete ID: " << id << "\n";

    cout << "Enter new Name (current: " << athlete->name << "): ";
    getline(cin, athlete->name);

    cout << "Enter new Gender (current: " << athlete->gender << "): ";
    getline(cin, athlete->gender);

    cout << "Enter new Type (current: " << athlete->type << "): ";
    getline(cin, athlete->type);

    cout << "Enter new Country Name (current: " << athlete->countryName << "): ";
    getline(cin, athlete->countryName);

    cout << "Enter new Native Language (current: " << athlete->nativeLanguage << "): ";
    getline(cin, athlete->nativeLanguage);

    cout << "Enter new Event Type (current: " << athlete->eventType << "): ";
    getline(cin, athlete->eventType);

    cout << "Enter new Event Name (current: " << athlete->eventName << "): ";
    getline(cin, athlete->eventName);

    cout << "Enter new Date of Birth (current: " << athlete->dob << "): ";
    getline(cin, athlete->dob);

    cout << "Enter new Medal ID (current: " << athlete->medalID << "): ";
    getline(cin, athlete->medalID);

    cout << "Enter new Medal Type (current: " << athlete->medalType << "): ";
    getline(cin, athlete->medalType);

    cout << "Enter new Medal Rank (current: " << athlete->medalRank << "): ";
    getline(cin, athlete->medalRank);

    cout << "Enter new Opponent Name (current: " << athlete->opponentName << "): ";
    getline(cin, athlete->opponentName);

    cout << "Enter new Game (current: " << athlete->game << "): ";
    getline(cin, athlete->game);

    cout << "Enter new Event Start Time (current: " << athlete->eventStartTime << "): ";
    getline(cin, athlete->eventStartTime);

    cout << "Enter new Event End Time (current: " << athlete->eventEndTime << "): ";
    getline(cin, athlete->eventEndTime);

    cout << "Enter new Venue (current: " << athlete->venue << "): ";
    getline(cin, athlete->venue);

    cout << "Enter new Stadium (current: " << athlete->stadium << "): ";
    getline(cin, athlete->stadium);

    updateCSVRecord(id, *athlete);
    animatedText("\nRecord updated successfully.\n", 15);

    int currentIndex = countLinesInFile("athletes.csv");
    changedIndices.insert(currentIndex);
    animatedText("Record updated successfully and marked as changed.\n", 15);
}

//updateCSVRecord

void updateCSVRecord(const string& id, const Athlete& updatedAthlete) {
    ifstream inputFile("athletes.csv");
    if (!inputFile.is_open()) {
        animatedText("Error: Unable to open CSV file.\n", 15);
        return;
    }

    vector<string> lines;
    string line;

    while (getline(inputFile, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos && line.substr(0, pos) == id) {


            ostringstream updatedLine;
            updatedLine << updatedAthlete.athleteID << "," << updatedAthlete.name << "," << updatedAthlete.gender << ","
                        << updatedAthlete.type << "," << updatedAthlete.countryName << "," << updatedAthlete.nativeLanguage << ","
                        << updatedAthlete.eventType << "," << updatedAthlete.eventName << "," << updatedAthlete.dob << ","
                        << updatedAthlete.medalID << "," << updatedAthlete.medalType << "," << updatedAthlete.medalRank << ","
                        << updatedAthlete.opponentName << "," << updatedAthlete.game << "," << updatedAthlete.eventStartTime << ","
                        << updatedAthlete.eventEndTime << "," << updatedAthlete.venue << "," << updatedAthlete.stadium;
            lines.push_back(updatedLine.str());
        } else {
            lines.push_back(line);
        }
    }
    inputFile.close();

    // updated records back to the file

    ofstream outputFile("athletes.csv", ios::trunc);
    if (!outputFile.is_open()) {
        animatedText("Error: Unable to write to the CSV file.\n", 15);
        return;
    }

    for (const string& updatedLine : lines) {
        outputFile << updatedLine << "\n";
    }
    outputFile.close();
}


// Delete records

void deleteAthlete() {
    if (!head) {
        animatedText("No records available.\n", 25);
        return;
    }

    cout << "Delete by:\n1. Athlete ID\n2. Index Number\nChoose an option: ";
    int choice;
    cin >> choice;
    cin.ignore();

    string id;
    int index = 0;
    bool found = false;
    Athlete* deletedAthlete = nullptr; // store the deleted athlete's information


    if (choice == 1) {
        cout << "Enter Athlete ID to delete: ";
        cin >> id;
        cin.ignore();
    } else if (choice == 2) {
        cout << "Enter index number to delete: ";
        cin >> index;
        cin.ignore();
    } else {
        animatedText("Invalid choice. Returning to main menu...\n", 25);
        return;
    }
    // Update CSV File
    ifstream inputFile("athletes.csv");
    if (!inputFile.is_open()) {
        animatedText("Error: Unable to open CSV file.\n", 25);
        return;
    }

    vector<string> lines;
    string line;
    int currentIndex = 1;


    while (getline(inputFile, line)) {
        if ((choice == 1 && line.substr(0, line.find(',')) == id) ||
            (choice == 2 && currentIndex == index)) {
            found = true;

            stringstream ss(line);
            string fields[18];
            for (int i = 0; i < 18 && getline(ss, fields[i], ','); i++);

            deletedAthlete = new Athlete{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5],
                                         fields[6], fields[7], fields[8], fields[9], fields[10], fields[11],
                                         fields[12], fields[13], fields[14], fields[15], fields[16], fields[17], nullptr};
        } else {
            lines.push_back(line);
        }
        currentIndex++;
    }
    inputFile.close();

    if (!found) {
        animatedText("No matching record found.\n", 15);
        return;
    }

    // Write updated data back to the CSV

    ofstream outputFile("athletes.csv", ios::trunc);
    if (!outputFile.is_open()) {
        animatedText("Error: Unable to write to CSV file.\n", 25);
        return;
    }

    for (const string& remainingLine : lines) {
        outputFile << remainingLine << "\n";
    }
    outputFile.close();


    // Remove from linked list

    Athlete* current = head;
    Athlete* prev = nullptr;
    currentIndex = 1;

    while (current) {
        if ((choice == 1 && current->athleteID == id) ||
            (choice == 2 && currentIndex == index)) {
            if (!prev) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
        currentIndex++;
    }

   // Display Deleted Record Information

    if (deletedAthlete) {
        animatedText("The following record was deleted:\n", 25);
        displayAthlete(*deletedAthlete);
        delete deletedAthlete;
    }

    animatedText("Record deleted successfully.\n", 25);
}


// Show changed records

void showChangedRecords() {
    if (changedIndices.empty()) {
        animatedText("No changes have been made to the records.\n", 25);
        return;
    }

    ifstream inputFile("athletes.csv");
    if (!inputFile.is_open()) {
        animatedText("Error: Unable to open CSV file.\n", 25);
        return;
    }

    string line;
    int currentIndex = 1;

    cout << "Changed Records:\n";
    while (getline(inputFile, line)) {
        if (changedIndices.count(currentIndex)) {
            cout << "Index " << currentIndex << ": " << line << "\n";
        }
        currentIndex++;
    }
    inputFile.close();
}


//insert a new node
void insertAthlete(const Athlete& athlete) {
    Athlete* newNode = new Athlete(athlete);
    if (!head) {
        head = newNode;
    } else {
        Athlete* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

Athlete* searchAthleteByID(const string& id) {
    Athlete* current = head;
    while (current) {
        if (current->athleteID == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;

}

// Delete all nodes in the linked list
void deleteAllNodes() {
    while (head) {
        Athlete* temp = head;
        head = head->next;
        delete temp;
    }
}


// Display athlete record

void displayAthlete(const Athlete& athlete) {

    cout << "-----------------------------------\n";
    cout << "-----------------------------------\n";

    animatedText("\nAthlete ID: " + athlete.athleteID +
                 "\nName: " + athlete.name +
                 "\nGender: " + athlete.gender +
                 "\nType: " + athlete.type +
                 "\nCountry Name: " + athlete.countryName +
                 "\nNative Language: " + athlete.nativeLanguage +
                 "\nEvent Type: " + athlete.eventType +
                 "\nEvent Name: " + athlete.eventName +
                 "\nDate of Birth: " + athlete.dob +
                 "\nMedal ID: " + athlete.medalID +
                 "\nMedal Type: " + athlete.medalType +
                 "\nMedal Rank: " + athlete.medalRank +
                 "\nOpponent Name: " + athlete.opponentName +
                 "\nGame: " + athlete.game +
                 "\nEvent Start Time: " + athlete.eventStartTime +
                 "\nEvent End Time: " + athlete.eventEndTime +
                 "\nVenue: " + athlete.venue +
                 "\nStadium: " + athlete.stadium + "\n", 0);

    cout << "-----------------------------------\n";
    cout << "-----------------------------------\n";
}

// Function to show country statistics
void showCountryStatistics() {
    if (!head) {
        animatedText("No records available to generate statistics.\n", 15);
        return;
    }

    string inputCountry;
    cout << "Enter the country name to view statistics: ";
    getline(cin, inputCountry);

    // Convert input to lowercase for case-insensitive comparison
    transform(inputCountry.begin(), inputCountry.end(), inputCountry.begin(), ::tolower);

    int totalAthletes = 0, goldMedals = 0, silverMedals = 0, bronzeMedals = 0;

    Athlete* current = head;

    // Traverse the linked list to gather statistics
    while (current) {
        string currentCountry = current->countryName;
        // Convert current record's country name to lowercase
        transform(currentCountry.begin(), currentCountry.end(), currentCountry.begin(), ::tolower);

        if (currentCountry == inputCountry) {
            totalAthletes++;

            // Count medals based on medal type
            if (current->medalType == "Gold Medal") 
                goldMedals++;
            else if (current->medalType == "Silver Medal") 
                silverMedals++;
            else if (current->medalType == "Bronze Medal") 
                bronzeMedals++;
        }
        current = current->next;
    }

    // Display the statistics
    if (totalAthletes > 0) {
        cout << "\n--- Country Statistics ---\n";
        cout << "Country Name: " << inputCountry << "\n";
        cout << "Total Athletes: " << totalAthletes << "\n";
        cout << "Gold Medals: " << goldMedals << "\n";
        cout << "Silver Medals: " << silverMedals << "\n";
        cout << "Bronze Medals: " << bronzeMedals << "\n";
    } else {
        animatedText("No records found for the specified country.\n", 15);
    }
}

// Function to display the total number of athletes participated
void showTotalAthletes() {
    if (!head) {
        animatedText("No records available.\n", 15);
        return;
    }

    set<string> uniqueAthletes; // Set to track unique athlete names
    Athlete* current = head;

    while (current) {
        if (!current->name.empty()) {
            uniqueAthletes.insert(current->name);
        }
        current = current->next;
    }

    animatedText("\n--- Olympic Participation Statistics ---\n", 25);
    cout << "Total Athletes Participated in the Olympics: " << uniqueAthletes.size() << "\n";
}

// Function to display top 5 countries and top-performing athletes
void showTopRankingCountriesAndAthletes() {
    if (!head) {
        animatedText("No records available to generate rankings.\n", 15);
        return;
    }

    // Maps to store country and athlete medal counts
    unordered_map<string, int> countryMedalCount;
    unordered_map<string, int> athleteMedalCount;

    Athlete* current = head;

    // Traverse linked list to count medals
    while (current) {
        if (!current->countryName.empty()) {
            countryMedalCount[current->countryName]++;
        }
        if (!current->name.empty()) {
            athleteMedalCount[current->name]++;
        }
        current = current->next;
    }

    // Sort countries based on total medal count
    vector<pair<string, int>> countryList(countryMedalCount.begin(), countryMedalCount.end());
    sort(countryList.begin(), countryList.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    // Sort athletes based on total medal count
    vector<pair<string, int>> athleteList(athleteMedalCount.begin(), athleteMedalCount.end());
    sort(athleteList.begin(), athleteList.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    // Display the top 5 countries
    animatedText("\n--- Top 5 Ranking Countries Based on Medal Count ---\n", 25);
    int rank = 1;
    for (const auto& country : countryList) {
        cout << rank << ". " << country.first << " - Total Medals: " << country.second << "\n";
        if (rank++ >= 5) break;
    }

    // Display the top 5 athletes
    animatedText("\n--- Top 5 Performing Athletes Based on Medal Count ---\n", 25);
    rank = 1;
    for (const auto& athlete : athleteList) {
        cout << rank << ". " << athlete.first << " - Total Medals: " << athlete.second << "\n";
        if (rank++ >= 5) break;
    }
}





// Function to display group mates'
void displayGroupMates() {
    // Group mate names
    vector<string> groupMates = {
        "Group Member : Rafiur Rahman Mashrafi,ID: 2221971042",
        "Group Member : Dhruba Saha           ,ID: 2232537042",
        "Group Member : Md. Tanvir Rahman     ,ID: 2211462042",
        "Group Member : Foysal Mahamud        ,ID: 2231078042 "
    };

    for (int i = 0; i < groupMates.size(); ++i) {
        animatedText(groupMates[i] + "\n", 50);



}
}

void showPopularSportsAndMedalCounts() 
{
    if (!head) {
        animatedText("No records available to analyze Olympic sports.\n", 15);
        return;
    }

    unordered_map<string, int> sportMedalCount;
    unordered_map<string, unordered_map<string, int>> eventMedalCount;

    Athlete* current = head;
    while (current) {
        if (!current->eventType.empty()) {
            sportMedalCount[current->eventType]++;
            if (!current->eventName.empty()) {
                eventMedalCount[current->eventType][current->eventName]++;
            }
        }
        current = current->next;
    }

    vector<pair<string, int>> sortedSports(sportMedalCount.begin(), sportMedalCount.end());
    sort(sortedSports.begin(), sortedSports.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    animatedText("\n--- Most Popular Olympic Sports (by Medal Count) ---\n", 25);
    for (const auto& sport : sortedSports) {
        cout << sport.first << " - Total Medals: " << sport.second << "\n";
    }

    animatedText("\n--- Event-Specific Medal Counts ---\n", 25);
    for (const auto& sport : sortedSports) {
        cout << "\nSport: " << sport.first << "\n";
        const auto& events = eventMedalCount[sport.first];
        for (const auto& event : events) {
            cout << "  Event: " << event.first << " - Medals: " << event.second << "\n";
        }
    }
}





void mainMenu() {
    while (true) {
        animatedText("\n--- Paris 2024 Olympic Summer Games ---\n", 50);
        animatedText("1. View Records\n", 25);
        animatedText("2. Search for a Record\n", 25);
        animatedText("3. Add a New Record\n", 25);
        animatedText("4. Update an Existing Record\n", 25);
        animatedText("5. Delete a Record\n", 25);
        animatedText("6. Show Records After Change\n", 25);
        animatedText("7. Show Country Statistics\n", 25);
        animatedText("8. Show Total Athletes Participated\n", 25);
        animatedText("9. Show Top Ranking Countries and Athletes\n", 25);
        animatedText("10. Show Most Popular Sports and Event-Specific Medal Counts\n", 25);
        animatedText("11. Exit\n", 25);
        animatedText("Enter your choice: ", 25);

        int choice;

        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            animatedText("Invalid choice. Please input a number.\n", 50);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Switch case for menu options
        switch (choice) {
            case 1: 
                viewRecords(); 
                break;
            case 2: 
                searchAthlete(); 
                break;
            case 3: 
                addAthlete(); 
                break;
            case 4: 
                updateAthlete(); 
                break;
            case 5: 
                deleteAthlete(); 
                break;
            case 6: 
                showChangedRecords(); 
                break;
            case 7: 
                showCountryStatistics(); 
                break;
            case 8: 
                showTotalAthletes(); 
                break;
            case 9: 
                showTopRankingCountriesAndAthletes(); 
                break;
            case 10: 
                showPopularSportsAndMedalCounts(); 
                break;
            case 11:
                animatedText("Exiting the system...\n", 50);
                displayGroupMates();
                return;
            default:
                animatedText("Invalid choice. Please input the correct option.\n", 50);
        }
    }
}



// Main function
int main() {

    loadFromCSV();

    animatedText("------Welcome to Olympic Athlete Tracking System-------\n\n",50);

    ensureDatabaseFileExists();

    loadUsersFromFile();

    animatedText("!Welcome to the Authentication System!\n", 10);

    authenticationPage();

    animatedText("Proceeding to the main system...\n", 10);

    mainMenu();

    //saveToCSV();

    deleteAllNodes();

    return 0;
}
