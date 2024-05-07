#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm> // for std::transform
#include <cctype>
#include <string>
#include <fstream> // reading and writing files 
#include <sstream>
using namespace std;

// Function to convert string to lowercase
string to_lower(const string& str) {
    string lower_str = str;
    transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

class Graph {
public:
    unordered_map<string, vector<pair<string, pair<string, int>>>> adj_list;

    // Function to convert string to lowercase
    string to_lower(const string& str) {
        string lower_str = str;
        transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
        return lower_str;
    }

    void add_edge(const string& city1, const string& city2, const string& transport_type, const int& price) {
        // Add bidirectional connection between city1 and city2
        adj_list[to_lower(city1)].emplace_back(to_lower(city2), make_pair(transport_type, price));
       // adj_list[to_lower(city2)].emplace_back(to_lower(city1), make_pair(transport_type, price));
    }

    // Function to build the graph data structure by parsing provided graph data file
    void build_graph_from_file(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to open file: " << filename << endl;
            return;
        }

        string line;
        int line_number = 0; // Track the line number for error reporting
        while (getline(file, line)) {
            line_number++;
            if (line.empty()) continue; // Skip empty lines

            // Split the line based on whitespace
            istringstream iss(line);
            vector<string> tokens;
            string token;
            while (iss >> token) {
                tokens.push_back(token);
            }

            // Check if there are enough tokens
            if (tokens.size() < 4) {
                //cout << "Error: Not enough information in line " << line_number << ": " << line << endl;
                continue;
            }

            // Extract city names
            string city1 = tokens[0];
            string city2 = tokens[2];

            // Extract transportation details
            for (size_t i = 3; i < tokens.size(); i += 2) {
                string transport_type = tokens[i];
                try {
                    int price = stoi(tokens[i + 1]);
                    add_edge(city1, city2, transport_type, price);
                }
                catch (const std::invalid_argument& e) {
                    cout << "Error: Invalid price in line " << line_number << ": " << line << endl;
                }
            }
        }

        file.close();
    }



    void update(string source, string destination) {
        // 3ashan count all ways from source to destination
        int count = 0;
        for (const auto& neighbor : adj_list[source]) {
            if (neighbor.first == destination) {
                count++;
            }
        }

        if (count == 0)
            cout << "No direct path from " << source << " to " << destination << endl;
        else {
            int t = 1;
            cout << "All trasportation from " << source << " to " << destination << " are:- \n";
            for (const auto& neighbor : adj_list[source]) {
                if (neighbor.first == destination) {
                    cout << t++ << "- " << neighbor.second.first << " " << neighbor.second.second << endl;
                }
            }
            bool done = false;
            string newTrans;
            int num, price;
            while (!done) {
                cout << "Enter trasportation number to update it:-\n";
                cin >> num;
                t = 1;
                if (num <= 0 || num > count) {
                    cout << "Invalid choice\n";
                }
                else {
                    cout << "Enter the new trasportation:-\n";
                    cin >> newTrans;
                    cout << "Enter the price:-\n";
                    cin >> price;
                    for (auto& neighbor : adj_list[source]) {
                        if (neighbor.first == destination) {
                            if (num == t) {
                                neighbor.second.first = newTrans;
                                neighbor.second.second = price;
                                cout << "Done.\n";
                                done = true;
                                break;
                            }
                            t++;
                        }
                    }
                }
            }
        }
    }
    void Delete(string source, string destination) {
        int count = 0;
        for (const auto& neighbor : adj_list[source]) {
            if (neighbor.first == destination) {
                count++;
            }
        }
        if (count == 0)
            cout << "No direct path from " << source << " to " << destination << endl;
        else {
            int t = 1;
            cout << "All trasportation from " << source << " to " << destination << "are:- \n";
            for (const auto& neighbor : adj_list[source]) {
                if (neighbor.first == destination) {
                    cout << t++ << "- " << neighbor.second.first << " " << neighbor.second.second << endl;
                }
            }
            bool done = false;
            string newTrans;
            int num;
            while (!done) {
                cout << "Enter trasportation number to delete it:-\n";
                cin >> num;
                t = 1;
                if (num <= 0 || num > count) {
                    cout << "Invalid choice\n";
                }
                else {
                    for (auto& neighbor : adj_list[source]) {
                        if (neighbor.first == destination) {
                            if (num == t) {
                                adj_list[source].erase(adj_list[source].begin() + (t - 1));
                                cout << "Done.\n";
                                done = true;
                                break;
                            }
                            t++;
                        }
                    }
                }
            }
        }
    }
    void bfs(const string& start_city) {
        string lowercase_city = to_lower(start_city);
        if (adj_list.find(lowercase_city) == adj_list.end()) {
            cout << "Error: The entered city does not exist in the graph." << endl;
            return;
        }

        unordered_set<string> visited;
        queue<string> q;
        q.push(start_city);

        while (!q.empty()) {
            string n = q.front();
            q.pop();

            // Output n to the terminal
            cout << n;

            // Traverse the connections of the current city
            for (const auto& v : adj_list[n]) {
                // Check if v is marked "unvisited"
                if (visited.find(v.first) == visited.end()) {
                    // Mark v as "visited"
                    visited.insert(v.first);
                    // Enqueue v on Q
                    q.push(v.first);
                }
            }
            if (!q.empty()) {
                cout << " -> ";
            }
        }
    }

    // Function to perform depth-first search traversal
    void dfs(const string& start_city, unordered_set<string>& visited) {
        cout << start_city;

        visited.insert(start_city);

        bool first_neighbor = true;
        for (const auto& neighbor : adj_list[start_city]) {
            if (visited.find(neighbor.first) == visited.end()) {
                cout << " -> ";
                dfs(neighbor.first, visited);
            }
        }
    }
};

void traverGraph(Graph transportation_graph) {
    string start_city;
    cout << "Enter the starting city: ";
    cin >> start_city;
    start_city = to_lower(start_city);

    string algorithm_choice;
    cout << "Choose an algorithm (BFS or DFS): ";
    cin >> algorithm_choice;

    if (algorithm_choice == "BFS") {
        transportation_graph.bfs(start_city);
        cout << endl;
    }
    else if (algorithm_choice == "DFS") {
        unordered_set<string> visited;
        transportation_graph.dfs(start_city, visited);
        cout << endl;
    }
    else {
        cout << "Invalid choice." << endl;
    }
}

void updateGraph(Graph& transportation_graph) {
    string source, destination;
    cout << "Enter the source city: ";
    cin >> source;
    cout << "Enter the destination city: ";
    cin >> destination;
    transportation_graph.update(source, destination);
}

void addEdgeToGraph(Graph& transportation_graph) {
    string source, destination;
    cout << "Enter the source city: ";
    cin >> source;
    cout << "Enter the destination city: ";
    cin >> destination;
    cout << "Enter the transport_type: ";
    string trans;
    cin >> trans;
    cout << "Enter the transport_price: ";
    int price;
    cin >> price;
    transportation_graph.add_edge(source, destination, trans, price);
}

void deleteAnEdge(Graph& transportation_graph) {
    string source, destination;
    cout << "Enter the source city: ";
    cin >> source;
    cout << "Enter the destination city: ";
    cin >> destination;
    transportation_graph.Delete(source, destination);
}

bool isLoggedIn()
{
    string username, password;
    string un, pw; // comparison strings

    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;

    ifstream read(username + ".txt"); // ifstream reads a file 
    getline(read, un); // reads the username 
    getline(read, pw); // reads the password

    if (un == username && pw == password) // if both un & username and pw & password are the same, true
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ourSystem() {
    int choice;

    cout << endl;
    cout << "\t####### Guide me application #######\n" << endl;
    cout << "\t\t\t\t Welcome!\n" << endl;
    cout << endl;
    cout << "-------------------------------" << endl;
    cout << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << endl;
    cout << "Your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        string username, password;

        cout << "Registration: " << endl;
        cout << endl;
        cout << "Select a username: ";
        cin >> username;
        cout << "Select a password: ";
        cin >> password;

        ofstream file; // ofstream creates a file
        file.open(username + ".txt");
        file << username << endl << password;
        file.close();
        cout << "Welcome " << username << "!" << endl;
    }
    else if (choice == 2) // if user chooses to login
    {
        bool status = isLoggedIn();

        if (!status) // if isLoggedIn() returns false
        {
            system("clear");
            cout << endl;
            cout << "Invalid login!" << endl;
            ourSystem();
            return;
        }
        else // if isLoggedIn() returns true, the dashboard is displayed
        {
            cout << "Successfully logged in!" << endl;
            cout << endl;

            int choiceTwo;

            cout << "Welcome back!" << endl;
            cout << endl;
            return;
        }
    }
}

void write_graph_to_file(const string& filename, Graph& graph) {
    ofstream outdata(filename, ios::trunc);
    if (!outdata.is_open()) {
        cout << "Error: Unable to open file: " << filename << endl;
        return;
    }

    for (const auto& city_entry : graph.adj_list) {
        for (const auto& connection : city_entry.second) {
            outdata << city_entry.first << " - " << connection.first << " " << connection.second.first << " " << connection.second.second << endl;
        }
    }

    outdata.close();
}

int main() {
    ourSystem();
    Graph transportation_graph;
    string filename = "TransportationMap.txt";
    transportation_graph.build_graph_from_file(filename);
    char choice = 'Y';
    while (choice == 'Y' || choice == 'y') {
        cout << "Enter 1 to traverse the transportation graph:-\n";
        cout << "Enter 2 to update the transportation graph:-\n";
        cout << "Enter 3 to Add an edge:-\n";
        cout << "Enter 4 to delete an edge:-\n";
        int c;
        cin >> c;
        if (c == 1) {
            traverGraph(transportation_graph);
        }
        else if (c == 2) {
            updateGraph(transportation_graph);
        }
        else if (c == 3) {
            addEdgeToGraph(transportation_graph);
        }
        else if (c == 4) {
            deleteAnEdge(transportation_graph);
        }
        else {
            cout << "Invalid choice.\n";
        }
        cout << "Do you want to continue (Y / N):- ";
        cin >> choice;
    }
    write_graph_to_file(filename, transportation_graph);
    return 0;
}
