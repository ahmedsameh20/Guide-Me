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
#include <stack>
#include <conio.h>
#include <numeric>
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
        adj_list[to_lower(city2)].emplace_back(to_lower(city1), make_pair(transport_type, price));
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



    void update(string source, string destination) 
    {
        // To count all ways from source to destination
        int count = 0;
        for (const auto& neighbor : adj_list[source]) 
        {
            if (neighbor.first == destination) 
            {
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
            int num;
            int price;
            while (!done) 
            {
                cout << "Enter trasportation number to update it:-\n";
                cin >> num;
                t = 1;
                if (num <= 0 || num > count) 
                {
                    cout << "Invalid choice\n";
                }
                else {
                    cout << "Enter the transport_type: ";
                    cin.ignore(); // Ignore any remaining newline characters
                    getline(cin, newTrans);
                    while (newTrans.empty() || std::all_of(newTrans.begin(), newTrans.end(), ::isdigit)) {
                        if (newTrans.empty()) {
                            cout << "Transport type cannot be empty. Please enter a valid transport type: ";
                        }
                        else {
                            cout << "Transport type cannot be a number. Please enter a valid transport type: ";
                        }
                        getline(cin, newTrans);
                    }
                    cout << "Enter the transport_price: ";
                    while (!(cin >> price) || price <= 0) {
                        cout << "Invalid input. Please enter a valid positive integer for the price: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    for (auto& neighbor : adj_list[source]) 
                    {
                        if (neighbor.first == destination) 
                        {
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

        vector<int> appearances;// to collect the indeces where the wanted destination appears
        int ind = 0;
        for (const auto& neighbor : adj_list[source]) {
            if (neighbor.first == destination) {
                count++;
                appearances.push_back(ind);
            }

            ind++;
        }
        if (count == 0)
            cout << "No direct path from " << source << " to " << destination << endl;
        else {
            int t = 1;
            cout << "All transportation from " << source << " to " << destination << " are:- \n";
            for (const auto& neighbor : adj_list[source]) {
                if (neighbor.first == destination) {
                    cout << t++ << "- " << neighbor.second.first << " " << neighbor.second.second << endl;
                }
            }
            bool done = false;
            int num;
            while (!done) {
                cout << "Enter transportation number to delete it:-\n";
                cin >> num;
                t = 1;
                if (num <= 0 || num > count) {
                    cout << "Invalid choice\n";
                }
                else {
                    for (int appearance : appearances) {
                        if (t == num) {
                            adj_list[source].erase(adj_list[source].begin() + appearance);
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
    void dfs(const string& start_city, unordered_set<string>& visited) {
        string lowercase_start_city = to_lower(start_city);
        if (adj_list.find(lowercase_start_city) == adj_list.end()) {
            cout << "Error: The entered city does not exist in the graph." << endl;
            return;
        }

        stack<string> s;
        s.push(lowercase_start_city);

        while (!s.empty()) {
            string current_city = s.top();
            s.pop();

            if (visited.find(current_city) == visited.end()) {
                cout << current_city << " -> ";
                visited.insert(current_city);

                // Sort neighbors alphabetically to ensure consistent traversal order
                sort(adj_list[current_city].begin(), adj_list[current_city].end());

                // Push neighbors onto stack in reverse order to maintain correct traversal order
                for (auto it = adj_list[current_city].rbegin(); it != adj_list[current_city].rend(); ++it) {
                    if (visited.find(it->first) == visited.end()) {
                        s.push(it->first);
                    }
                }
            }
        }
        cout << "End" << endl;
    }


    void bfs(const string& start_city) {
        string lowercase_start_city = to_lower(start_city);
        if (adj_list.find(lowercase_start_city) == adj_list.end()) {
            cout << "Error: The entered city does not exist in the graph." << endl;
            return;
        }

        unordered_set<string> visited;
        queue<string> q;
        q.push(lowercase_start_city);

        while (!q.empty()) {
            string current_city = q.front();
            q.pop();

            if (visited.find(current_city) == visited.end()) {
                cout << current_city << " -> ";
                visited.insert(current_city);

                for (const auto& neighbor : adj_list[current_city]) {
                    if (visited.find(neighbor.first) == visited.end()) {
                        q.push(neighbor.first);
                    }
                }
            }
        }
        cout << "End" << endl;
    }
    bool is_complete() {
        for (const auto& city_entry : adj_list) {
            unordered_set<string> connected_cities;
            for (const auto& connection : city_entry.second) {
                connected_cities.insert(connection.first);
            }

            if (connected_cities.size() != adj_list.size() - 1) {
                return false;
            }
        }
        return true;
    }
    // Updated DFS function to find paths
    void dfs2(const string& current_city, const string& destination, const int budget, unordered_set<string>& visited,
        vector<string>& path, vector<vector<string>>& paths, int& total_cost, const Graph& adj_list) {
        visited.insert(current_city);
        path.push_back(current_city);

        if (current_city == destination) {
            paths.push_back(path);
            total_cost += calculate_total_cost(path, adj_list);
        }
        else {
            for (const auto& neighbor : adj_list.adj_list.at(current_city)) {
                if (visited.find(neighbor.first) == visited.end()) {
                    dfs2(neighbor.first, destination, budget, visited, path, paths, total_cost, adj_list);
                }
            }
        }

        visited.erase(current_city);
        path.pop_back();
    }
    // Function to generate all possible routes by combining transportation types
    void generate_routes(int index, const vector<vector<string>>& transport_types, vector<string> current_route, vector<vector<string>>& routes) {
        if (index == transport_types.size()) {
            routes.push_back(current_route);
            return;
        }

        for (const auto& transport_type : transport_types[index]) {
            current_route.push_back(transport_type);
            generate_routes(index + 1, transport_types, current_route, routes);
            current_route.pop_back();
        }
    }

    int calculate_total_cost(const vector<string>& path, const Graph& adj_list) 
    {
        int total_cost = 0;
        for (size_t i = 0; i < path.size() - 1; i++) 
        {
            string current_city = path[i];
            string next_city = path[i + 1];

            // Find the edge between the current city and the next city
            for (const auto& neighbor : adj_list.adj_list.at(current_city)) 
            {
                if (neighbor.first == next_city) 
                {
                    total_cost += neighbor.second.second;
                    break;
                }
            }
        }
        return total_cost;
    }
    void find_routes(const string& source, const string& destination, const int budget, const Graph& adj_list) 
    {
        vector<vector<string>> paths;
        vector<string> path;
        unordered_set<string> visited;
        int total_cost = 0;

        dfs2(source, destination, budget, visited, path, paths, total_cost, adj_list);

        if (paths.empty()) 
        {
            cout << "No routes found within the specified budget." << endl;
        }
        else 
        {
            cout << "The source is: " << source << endl;
            cout << "The destination is: " << destination << endl;
            cout << "Your budget: " << budget << endl;

            // Print all unique paths
            unordered_set<string> printed_paths;
            for (const auto& p : paths) 
            {
                string path_string;
                int current_cost = calculate_total_cost(p, adj_list);
                vector<vector<string>> transport_types; // Store transport types for each edge
                // Check if the path exceeds the budget
                if (current_cost <= budget) 
                {
                    path_string = source;
                    for (size_t i = 0; i < p.size() - 1; i++)
                    {
                        string current_city = p[i];
                        string next_city = p[i + 1];
                        // Get all possible transportation types for the edge
                        vector<string> edge_transport_types;
                        for (const auto& neighbor : adj_list.adj_list.at(current_city))
                        {
                            if (neighbor.first == next_city) 
                            {
                                edge_transport_types.push_back(neighbor.second.first);
                            }
                        }

                        // Store the transportation types for the edge
                        transport_types.push_back(edge_transport_types);
                    }

                    // Generate all possible routes by combining transportation types
                    vector<vector<string>> routes;
                    generate_routes(0, transport_types, {}, routes);

                    for (const auto& route : routes) {
                        path_string = source;
                        int route_cost = 0; // Total cost for this route
                        for (size_t i = 0; i < p.size() - 1; i++) {
                            string current_city = p[i];
                            string next_city = p[i + 1];
                            // Find the transportation cost for this segment
                            for (const auto& neighbor : adj_list.adj_list.at(current_city)) 
                            {
                                if (neighbor.first == next_city && neighbor.second.first == route[i]) 
                                {
                                    route_cost += neighbor.second.second;
                                    break;
                                }
                            }
                            path_string += " (" + route[i] + ") -> " + p[i + 1];
                        }
                        if (route_cost <= budget) {
                            if (printed_paths.find(path_string) == printed_paths.end()) {
                                printed_paths.insert(path_string);
                                cout << "Route: " << path_string << endl;
                                cout << "Total cost: " << route_cost << endl;
                            }
                        }
                    }
                }
            }
        }
    }

};

void traverGraph(Graph transportation_graph) {
    string start_city;
    cout << "Enter the starting city: ";
    cin.ignore(); // Ignore any remaining newline characters
    getline(cin, start_city);
    while (start_city.empty() || std::all_of(start_city.begin(), start_city.end(), ::isdigit)) {
        if (start_city.empty()) {
            cout << "start_city cannot be empty. Please enter a valid start_city: ";
        }
        else {
            cout << "start_city cannot be a number. Please enter a valid start_city: ";
        }
        getline(cin, start_city);
    }
    start_city = to_lower(start_city);

    string algorithm_choice;
    while (true) {
        cout << "Choose an algorithm (BFS or DFS): ";
        getline(cin, algorithm_choice);

        // Check if the input is empty or consists of only digits
        if (algorithm_choice.empty() || std::all_of(algorithm_choice.begin(), algorithm_choice.end(), ::isdigit)) {
            if (algorithm_choice.empty()) {
                cout << "Algorithm cannot be empty. Please enter a valid algorithm: ";
            }
            else {
                cout << "Algorithm cannot be a number. Please enter a valid algorithm: ";
            }
        }
        else {
            // Convert the input to lowercase for case-insensitive comparison
            transform(algorithm_choice.begin(), algorithm_choice.end(), algorithm_choice.begin(), ::tolower);

            // Check if the input is either "bfs" or "dfs"
            if (algorithm_choice == "bfs") {
                transportation_graph.bfs(start_city);
                cout << endl;
                string next_action2;
                cout << "Choose the next algorithm to run (DFS) or enter any other key to exit: ";
                getline(cin, next_action2);

                // Convert the input to lowercase for case-insensitive comparison
                transform(next_action2.begin(), next_action2.end(), next_action2.begin(), ::tolower);

                if (next_action2 == "dfs") {
                    unordered_set<string> visited;
                    transportation_graph.dfs(start_city, visited);
                    cout << endl;
                    break;
                }
                else {
                    break; // Exit the loop if the user chooses to exit
                } 
            }
            else if (algorithm_choice == "dfs") {
                unordered_set<string> visited;
                transportation_graph.dfs(start_city, visited);
                cout << endl;

                // After running DFS, prompt the user for further action
                string next_action;
                cout << "Choose the next algorithm to run (BFS) or enter any other key to exit: ";
                getline(cin, next_action);

                // Convert the input to lowercase for case-insensitive comparison
                transform(next_action.begin(), next_action.end(), next_action.begin(), ::tolower);

                if (next_action == "bfs") {
                    transportation_graph.bfs(start_city);
                    cout << endl;
                    break;
                }
                else {
                    break; // Exit the loop if the user chooses to exit
                }
            }
            else {
                cout << "Invalid choice. Please enter either 'BFS' or 'DFS'." << endl;
                continue; // Restart the loop to ask for input again
            }
        }
    }

}

void updateGraph(Graph& transportation_graph) {
    string source, destination;
    cout << "Enter the source city: ";
    cin.ignore(); // Ignore any remaining newline characters
    getline(cin, source);
    while (source.empty() || std::all_of(source.begin(), source.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Source cannot be empty. Please enter a valid source: ";
        }
        else {
            cout << "Source cannot be a number. Please enter a valid source: ";
        }
        getline(cin, source);
    }
    cout << "Enter the destination city: ";
    getline(cin, destination);
    while (destination.empty() || std::all_of(destination.begin(), destination.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Destination cannot be empty. Please enter a valid destination: ";
        }
        else {
            cout << "Destination cannot be a number. Please enter a valid destination: ";
        }
        getline(cin, destination);
    }
    transportation_graph.update(source, destination);
}

void addEdgeToGraph(Graph& transportation_graph) {
    int count = 0;
    string source, destination;
    cout << "Enter the source city: ";
    cin.ignore(); // Ignore any remaining newline characters
    getline(cin, source);
    while (source.empty() || std::all_of(source.begin(), source.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Source cannot be empty. Please enter a valid source: ";
        }
        else {
            cout << "Source cannot be a number. Please enter a valid source: ";
        }
        getline(cin, source);
    }
    cout << "Enter the destination city: ";
    getline(cin, destination);
    while (destination.empty() || std::all_of(destination.begin(), destination.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Destination cannot be empty. Please enter a valid destination: ";
        }
        else {
            cout << "Destination cannot be a number. Please enter a valid destination: ";
        }
        getline(cin, destination);
    }
    string trans;
    cout << "Enter the transport_type: ";
    getline(cin, trans);
    while (trans.empty() || std::all_of(trans.begin(), trans.end(), ::isdigit)) {
        if (trans.empty()) {
            cout << "Transport type cannot be empty. Please enter a valid transport type: ";
        }
        else {
            cout << "Transport type cannot be a number. Please enter a valid transport type: ";
        }
        getline(cin, trans);
    }

    int price;
    cout << "Enter the transport_price: ";
    while (!(cin >> price) || price <= 0) {
        cout << "Invalid input. Please enter a valid positive integer for the price: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    transportation_graph.add_edge(source, destination, trans, price);
}



void deleteAnEdge(Graph& transportation_graph) {
    string source, destination;
    cout << "Enter the source city: ";
    cin.ignore(); // Ignore any remaining newline characters
    getline(cin, source);
    while (source.empty() || std::all_of(source.begin(), source.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Source cannot be empty. Please enter a valid source: ";
        }
        else {
            cout << "Source cannot be a number. Please enter a valid source: ";
        }
        getline(cin, source);
    }
    cout << "Enter the destination city: ";
    getline(cin, destination);
    while (destination.empty() || std::all_of(destination.begin(), destination.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Destination cannot be empty. Please enter a valid destination: ";
        }
        else {
            cout << "Destination cannot be a number. Please enter a valid destination: ";
        }
        getline(cin, destination);
    }
    transportation_graph.Delete(source, destination);
}

void route(Graph& transportation_graph, Graph& graph) {
    string source, destination;
    int budget;
    cout << "Enter the source city: ";
    cin.ignore(); // Ignore any remaining newline characters
    getline(cin, source);
    while (source.empty() || std::all_of(source.begin(), source.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Source cannot be empty. Please enter a valid source: ";
        }
        else {
            cout << "Source cannot be a number. Please enter a valid source: ";
        }
        getline(cin, source);
    }
    cout << "Enter the destination city: ";
    getline(cin, destination);
    while (destination.empty() || std::all_of(destination.begin(), destination.end(), ::isdigit)) {
        if (source.empty()) {
            cout << "Destination cannot be empty. Please enter a valid destination: ";
        }
        else {
            cout << "Destination cannot be a number. Please enter a valid destination: ";
        }
        getline(cin, destination);
    }
    int count = 0;
    for (const auto& neighbor : graph.adj_list)
    {
        if (neighbor.first == destination)
        {
            count++;
        }
    }
    if (count == 0)
        cout << "No direct path from " << source << " to " << destination << endl;
    cout << "Enter your budget: ";
    while (!(cin >> budget) || budget <= 0) {
        cout << "Invalid input. Please enter a valid positive integer for the budget: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    transportation_graph.find_routes(source, destination, budget, transportation_graph);
}

bool isLoggedIn()
{
    string username, password;
    string un, pw; // comparison strings

    cout << "Enter your username: ";
    cin.ignore(); // Ignore any remaining newline characters
    getline(cin, username);
    while (username.empty() || std::all_of(username.begin(), username.end(), ::isdigit)) {
        if (username.empty()) {
            cout << "Username cannot be empty. Please enter a valid username: ";
        }
        else {
            cout << "Username cannot be a number. Please enter a valid username: ";
        }
        getline(cin, username);
    }
    cout << "Enter your password: ";
    char ch;
    password = "";
    while ((ch = _getch()) != '\r') { // Loop until Enter is pressed
        if (ch == '\b') { // Handle backspace
            if (!password.empty()) {
                cout << "\b \b"; // Move cursor back, erase character, move cursor back again
                password.pop_back(); // Remove last character from password
            }
        }
        else {
            cout << '*'; // Print asterisk
            password += ch; // Append character to password
        }
    }
    cout << endl;

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

    // Input validation loop
    while (true) {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << endl;
        cout << "Your choice: ";
        cin >> choice;

        if (cin.fail()) { // Check if input is not an integer
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
        else if (choice != 1 && choice != 2) { // Check if input is not 1 or 2
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }
        else {
            break; // Valid input, exit the loop
        }
    }

    if (choice == 1)
    {
        string username, password;

        cout << "Registration: " << endl;
        cout << endl;
        cout << "Select your username: ";
        cin.ignore(); // Ignore any remaining newline characters
        getline(cin, username);
        while (username.empty() || std::all_of(username.begin(), username.end(), ::isdigit)) {
            if (username.empty()) {
                cout << "Username cannot be empty. Please enter a valid username: ";
            }
            else {
                cout << "Username cannot be a number. Please enter a valid username: ";
            }
            getline(cin, username);
        }
        cout << "Select your password: ";
        char ch;
        password = "";
        while ((ch = _getch()) != '\r') { // Loop until Enter is pressed
            if (ch == '\b') { // Handle backspace
                if (!password.empty()) {
                    cout << "\b \b"; // Move cursor back, erase character, move cursor back again
                    password.pop_back(); // Remove last character from password
                }
            }
            else {
                cout << '*'; // Print asterisk
                password += ch; // Append character to password
            }
        }
        cout << endl;

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
    vector <string> isprinted;

    ofstream outdata(filename, ios::trunc);
    if (!outdata.is_open()) {
        cout << "Error: Unable to open file: " << filename << endl;
        return;
    }

    for (const auto& city_entry : graph.adj_list) {
        for (const auto& connection : city_entry.second) {
            if (find(isprinted.begin(), isprinted.end(), connection.first) != isprinted.end()) {
                continue;
            }
            outdata << city_entry.first << " - " << connection.first << " " << connection.second.first << " " << connection.second.second << endl;


        }
        isprinted.push_back(city_entry.first);
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
        cout << "Enter 5 to check if the transportation map is complete:-\n";
        cout << "Enter 6 to find routes between two cities:-\n";
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
        else if (c == 5)
        {
            if (transportation_graph.is_complete()) {
                cout << "The transportation map is complete." << endl;
            }
            else {
                cout << "The transportation map is not complete." << endl;
            }
        }
        else if (c == 6)
        {
            route(transportation_graph, transportation_graph);
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
