#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cerrno>
#include <cstring>

using namespace std;

#ifdef _WIN32
const string HOSTS_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
#elif __linux__

string getHostsPath() {
    ifstream file("/proc/version");
    string line;
    getline(file, line);
    file.close();

    if (line.find("Microsoft") != string::npos || line.find("microsoft") != string::npos) {
        cout << "This application can not run in WSL\n";
        // but we can still give them normal linux path. if they want ? 
        string answer = "y";
        cout << "Do you want to continue with /etc/hosts? (Y/n)\n";
        cin >> answer;
        if (toupper(answer[0]) != 'Y') {
            exit(0);
        }
        return "/etc/hosts"; //return "/mnt/c/Windows/System32/drivers/etc/hosts";
    } else {
        // This is native Linux
        return "/etc/hosts";
    }
}

const string HOSTS_PATH = getHostsPath();
#else
#error "Unknown operating system"
#endif


string ltrim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    return (start == string::npos) ? "" : s.substr(start);
}

vector<pair<string, string>> readFile() {
  ifstream file(HOSTS_PATH);
    cout << "Reading file...\n" << HOSTS_PATH << endl;
    if (!file) {
        cout << "Error opening file" << endl;
        return {};
    }

    cout << left << setw(20) << "| IP" << setw(30) << "| URL |" << endl;
    cout << "|---------------------|---------------------------------------|\n";

    string line;
    vector<pair<string, string>> entries;

    while (getline(file, line)) {
        line = ltrim(line); // Trim leading spaces

        // Skip if the line is empty
        if (line.empty()) continue;

        // Check if the first character of the line is a digit
        if (isdigit(line[0])) {
            stringstream ss(line);
            string ip, url;

            if (ss >> ip >> url) {
                entries.push_back(make_pair(ip, url));
                cout << "| " << left << setw(18) << ip << "| " << left << setw(28) << url << " |\n";
            }
        }
    }

    file.close();
    return entries;
}

bool isValidUrl(const string& url) {
    
    // check it only contains url safe characters
    for (char c : url) {
        if (!isalnum(c) && c != '.' && c != '-' && c != '_') return false;
    }
    return url.find(' ') == string::npos;
}

bool isValidIp(string ip) {
    stringstream ss(ip);
    string part;
    int count = 0;

    while (getline(ss, part, '.')) {
        if (part.empty()) return false;

        for (char c : part) {
            if (!isdigit(c)) return false;
        }

        int num = stoi(part);
        if (num < 0 || num > 255) return false;

        count++;
    }

    return count == 4;
}

void writeFile(const vector<pair<string, string>>& entries) {
    ofstream file(HOSTS_PATH, ios::trunc);

    cout << "Writing to file...\n" << HOSTS_PATH << endl;

    if (!file) {
        cout << "Error opening file: " << strerror(errno) << endl;
        return;
    }

    for (const auto& entry : entries) {
        file << entry.first << " " << entry.second << "\n";
        if (file.fail()) {
            cout << "Error writing to file: " << strerror(errno) << endl;
            file.close();
            return;
        }
    }

    file.close();
    if (file.fail()) {
        cout << "Error closing file: " << strerror(errno) << endl;
    } else {
        cout << "File written successfully\n";
    }
}

void lineForm(vector<pair<string, string>>& entries, size_t index = 0) {
    string ip, url;
    do {
        cout << "Enter the new IP:\n";
        cin >> ip;
    } while (!isValidIp(ip));

    do {
      cout << "Enter the new URL:\n";
      cin >> url;
    } while (!isValidUrl(url));

    if (index == 0) {
      entries[index] = make_pair(ip, url);
    } else {
      entries.push_back(make_pair(ip, url));
    }    
    cout << "Entry added successfully\n";
    writeFile(entries);
}

void showMenu(vector<pair<string, string>>& entries) {
    while (true) {
        cout << "\nPlease choose an option:\n";
        cout << "1. Add a new entry\n";
        cout << "2. Edit an entry\n";
        cout << "3. Exit\n";

        int option;
        cin >> option;

        if (option == 1) {
            lineForm(entries);
  
        } else if (option == 2) {
            cout << "Enter the index of the entry you want to edit (0-" << entries.size()-1 << "):\n";
            size_t index;
            cin >> index;
            lineForm(entries, index);
        } else if (option == 3) {
            break;
        }
    }
}


int main() {
    vector<pair<string, string>> entries;

    while(true) {
        entries = readFile();

        showMenu(entries);

        cout << "\nDo you want to continue? (Y/N)\n";
        char c;
        cin >> c;
        if (toupper(c) != 'Y') {
            break;
        }
    }
    
    return 0;
}