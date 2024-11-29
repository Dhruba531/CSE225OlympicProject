#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Define a structure to hold a record (athlete details)
struct Record {
    string athleteID;
    string name;
    string gender;
    string type;
    string countryCode;
    string countryName;
    string nativeLanguage;
    string eventType;
    string eventName;
    string dob;  // Date of birth
    string medalID;
    string medalType;
    string medalRank;
    string eventDate;
    string opponentName;
    string game;
    string startTime;
    string endTime;
    string venue;
    string stadium;

    // Function to convert a Record into a CSV line
    string toCSV() const {
        stringstream ss;
        ss << athleteID << "," << name << "," << gender << "," << type << ","
           << countryCode << "," << countryName << "," << nativeLanguage << ","
           << eventType << "," << eventName << "," << dob << ","
           << medalID << "," << medalType << "," << medalRank << ","
           << eventDate << "," << opponentName << "," << game << ","
           << startTime << "," << endTime << "," << venue << "," << stadium;
        return ss.str();
    }
};

// Helper function to trim leading and trailing spaces
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Helper function to convert a string to lowercase
string toLower(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to read records from the CSV file
vector<Record> readCSV(const string &filename) {
    vector<Record> records;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Record record;
        getline(ss, record.athleteID, ',');
        getline(ss, record.name, ',');
        getline(ss, record.gender, ',');
        getline(ss, record.type, ',');
        getline(ss, record.countryCode, ',');
        getline(ss, record.countryName, ',');
        getline(ss, record.nativeLanguage, ',');
        getline(ss, record.eventType, ',');
        getline(ss, record.eventName, ',');
        getline(ss, record.dob, ',');
        getline(ss, record.medalID, ',');
        getline(ss, record.medalType, ',');
        getline(ss, record.medalRank, ',');
        getline(ss, record.eventDate, ',');
        getline(ss, record.opponentName, ',');
        getline(ss, record.game, ',');
        getline(ss, record.startTime, ',');
        getline(ss, record.endTime, ',');
        getline(ss, record.venue, ',');
        getline(ss, record.stadium);

        records.push_back(record);
    }
    return records;
}

// Function to write records to the CSV file
void writeCSV(const string &filename, const vector<Record> &records) {
    ofstream file(filename);
    for (const auto &record : records) {
        file << record.toCSV() << endl;
    }
}

// Function to display a record
void displayRecord(const Record &record) {
    cout << record.athleteID << ", " << record.name << ", " << record.gender << ", "
         << record.type << ", " << record.countryCode << ", " << record.countryName << ", "
         << record.nativeLanguage << ", " << record.eventType << ", " << record.eventName << ", "
         << record.dob << ", " << record.medalID << ", " << record.medalType << ", "
         << record.medalRank << ", " << record.eventDate << ", " << record.opponentName << ", "
         << record.game << ", " << record.startTime << ", " << record.endTime << ", "
         << record.venue << ", " << record.stadium << endl;
}

// Function to view records
void viewRecords(const vector<Record> &records) {
    int numRecords;
    cout << "How many records would you like to see? ";
    cin >> numRecords;
    for (int i = 0; i < numRecords && i < records.size(); i++) {
        displayRecord(records[i]);
    }
}

// Function to search for a record (by Athlete ID or Name)
void searchRecord(const vector<Record> &records) {
    cout << "Search by: (1) Athlete ID, (2) Athlete Name: ";
    int searchChoice;
    cin >> searchChoice;

    string query;
    bool found = false;

    switch (searchChoice) {
        case 1:
            cout << "Enter Athlete ID: ";
            cin >> query;
            for (const auto &record : records) {
                if (record.athleteID == query) {
                    displayRecord(record);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Record not found!" << endl;
            }
            break;
        case 2:
            cout << "Enter Athlete Name: ";
            cin.ignore();
            getline(cin, query);
            for (const auto &record : records) {
                if (record.name == query) {
                    displayRecord(record);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Record not found!" << endl;
            }
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
    }
}

// Function to add a new record
void addRecord(vector<Record> &records) {
    Record newRecord;
    cout << "Enter Athlete ID: ";
    cin >> newRecord.athleteID;
    cout << "Enter Athlete Name: ";
    cin >> newRecord.name;
    cout << "Enter Gender: ";
    cin >> newRecord.gender;
    cout << "Enter Type: ";
    cin >> newRecord.type;
    cout << "Enter Country Code: ";
    cin >> newRecord.countryCode;
    cout << "Enter Country Name: ";
    cin >> newRecord.countryName;
    cout << "Enter Native Language: ";
    cin >> newRecord.nativeLanguage;
    cout << "Enter Event Type: ";
    cin >> newRecord.eventType;
    cout << "Enter Event Name: ";
    cin >> newRecord.eventName;
    cout << "Enter Date of Birth: ";
    cin >> newRecord.dob;
    cout << "Enter Medal ID: ";
    cin >> newRecord.medalID;
    cout << "Enter Medal Type: ";
    cin >> newRecord.medalType;
    cout << "Enter Medal Rank: ";
    cin >> newRecord.medalRank;
    cout << "Enter Event Date: ";
    cin >> newRecord.eventDate;
    cout << "Enter Opponent Name: ";
    cin >> newRecord.opponentName;
    cout << "Enter Game Type: ";
    cin >> newRecord.game;
    cout << "Enter Start Time: ";
    cin >> newRecord.startTime;
    cout << "Enter End Time: ";
    cin >> newRecord.endTime;
    cout << "Enter Venue: ";
    cin >> newRecord.venue;
    cout << "Enter Stadium: ";
    cin >> newRecord.stadium;

    records.push_back(newRecord);
    cout << "New record added successfully!" << endl;
}

// Function to update an existing record
void updateRecord(vector<Record> &records) {
    cout << "Enter Athlete ID to update: ";
    string athleteID;
    cin >> athleteID;

    auto it = find_if(records.begin(), records.end(), [&](const Record &record) {
        return record.athleteID == athleteID;
    });

    if (it != records.end()) {
        cout << "Record found! Current record details:" << endl;
        displayRecord(*it);

        cout << "Enter the field to update (1) Athlete Name, (2) Medal Type: ";
        int field;
        cin >> field;

        switch (field) {
            case 1:
                cout << "Enter new Athlete Name: ";
                cin >> it->name;
                break;
            case 2:
                cout << "Enter new Medal Type: ";
                cin >> it->medalType;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
        cout << "Record updated!" << endl;
    } else {
        cout << "Record not found!" << endl;
    }
}

// Function to delete a record
void deleteRecord(vector<Record> &records) {
    cout << "Enter Athlete ID to delete: ";
    string athleteID;
    cin >> athleteID;

    auto it = remove_if(records.begin(), records.end(), [&](const Record &record) {
        return record.athleteID == athleteID;
    });

    if (it != records.end()) {
        records.erase(it, records.end());
        cout << "Record deleted!" << endl;
    } else {
        cout << "Record not found!" << endl;
    }
}

******// Here is the problem Function to search medal statistics by country
void searchStatisticsByCountry(vector<Record> &records) {
    cout << "Enter the country name to search for medal statistics: ";
    string countryName;
    cin.ignore();  // To ignore any previous newline character
    getline(cin, countryName);

    // Trim and convert the input to lowercase for case-insensitive comparison
    countryName = trim(countryName);  // Ensure no leading/trailing spaces
    string lowerCountryName = toLower(countryName);  // Convert to lowercase

    // Debugging: Print out the trimmed and lowercased input country name
    cout << "Searching for country (trimmed and lowercased): " << lowerCountryName << endl;

    // Maps to store medal counts
    unordered_map<string, int> goldMedals;
    unordered_map<string, int> silverMedals;
    unordered_map<string, int> bronzeMedals;

    // Iterate over the records to count medals
    for (const auto &record : records) {
        // Trim and convert the medal type to lowercase for comparison
        string medalType = trim(record.medalType);  // Ensure no newlines or spaces
        string lowerRecordCountry = toLower(trim(record.countryName));  // Ensure no spaces and lowercase comparison

        // Check if the country matches (case-insensitive)
        if (lowerRecordCountry == lowerCountryName) {
            if (medalType == "Gold Medal") {
                goldMedals[record.countryName]++;
            } else if (medalType == "Silver Medal") {
                silverMedals[record.countryName]++;
            } else if (medalType == "Bronze Medal") {
                bronzeMedals[record.countryName]++;
            }
        }
    }

    // Output the medal counts for the country
    cout << "Medals for " << countryName << ":" << endl;
    cout << "Gold: " << goldMedals[countryName] << endl;
    cout << "Silver: " << silverMedals[countryName] << endl;
    cout << "Bronze: " << bronzeMedals[countryName] << endl;
}






// Main function
int main() {
    vector<Record> records = readCSV("Data1.csv");
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. View Records\n";
        cout << "2. Search Record\n";
        cout << "3. Add New Record\n";
        cout << "4. Update Record\n";
        cout << "5. Delete Record\n";
        cout << "6. Search Medal Statistics by Country\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewRecords(records);
                break;
            case 2:
                searchRecord(records);
                break;
            case 3:
                addRecord(records);
                break;
            case 4:
                updateRecord(records);
                break;
            case 5:
                deleteRecord(records);
                break;
            case 6:
                searchStatisticsByCountry(records);
                break;
            case 7:
                writeCSV("Data1.csv", records);
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 7);

    return 0;
}
