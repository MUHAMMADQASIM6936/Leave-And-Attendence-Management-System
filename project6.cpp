#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include<unordered_map>
#include<unordered_set>
using namespace std;
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// Base class representing a Person with common attributes for all users
class Person
{
protected:
    int id;         // Unique identifier for a person (e.g., employee ID)
    string name;    // Name of the person

public:
    // Default constructor initializing id to 0 and name to an empty string
    Person() : id(0), name("") {}

    // Parameterized constructor to initialize id and name
    Person(int id, string name) : id(id), name(name) {}

    // Setters to modify id and name
    void set_id(int id) { this->id = id; }
    void set_name(string name) { this->name = name; }

    // Getters to retrieve id and name
    int get_id() const { return id; }
    string get_name() const { return name; }

    // Pure virtual function: This forces derived classes (like Employee) 
    // to implement their own menu function
    virtual void menu() = 0;

};

// Derived class representing an Employee, which is a type of Person
class Employee : public Person
{
protected:
    int casual_leave_balance;  // Casual leave balance for the employee
    int earned_leave_balance;  // Earned leave balance for the employee

public:
    // Default constructor initializing leave balances to default values
    Employee() : Person(), casual_leave_balance(15), earned_leave_balance(21) {}

    // Parameterized constructor to initialize id, name, and leave balances
    Employee(int id, string name) : Person(id, name), casual_leave_balance(15), earned_leave_balance(21) {}

    // Getter functions to retrieve leave balances
    int get_casual_leave_balance() const { return casual_leave_balance; }
    int get_earned_leave_balance() const { return earned_leave_balance; }

    // Setter functions to update leave balances
    void update_casual_leave_balance(int new_balance) { casual_leave_balance = new_balance; }
    void update_earned_leave_balance(int new_balance) { earned_leave_balance = new_balance; };

    virtual void menu()     {
        cout << "\n--- Employee Menu ---\n";
        cout << "1. View Attendance\n";
        cout << "2. Apply for Leave\n";
        cout << "3. Request Attendance\n";
        cout << "4. View Leave Status\n";
        cout << "5. Exit\n";
    }
};

// EmployeeManager class to manage a collection of employees
class EmployeeManager
{
private:
    vector<Employee> employees;  // A vector to store the list of employees

public:
    // Function to load employee data from a file ("employees.txt")
    void load_employees()
    {
        ifstream file("employees.txt");  // Open file for reading
        if (file.is_open())  // Check if the file is open
        {
            string line;
            // Read each line from the file
            while (getline(file, line))
            {
                stringstream ss(line);  // Use stringstream to parse each line
                int id;  // Employee ID
                string name;  // Employee Name
                ss >> id;  // Read ID from line
                ss.ignore(1, ',');  // Ignore the comma separator
                getline(ss, name);  // Read the name
                employees.push_back(Employee(id, name));  // Add the employee to the vector
            }
            file.close();  // Close the file after reading
        }
    }

    // Function to save a new employee to the file ("employees.txt")
    void save_employee(const Employee& emp)
    {
        ofstream file("employees.txt", ios::app);  // Open file in append mode
        if (file.is_open())  // Check if the file is open
        {
            // Write employee ID and name to the file
            file << emp.get_id() << "," << emp.get_name() << endl;
            file.close();  // Close the file after writing
        }
    }

    // Function to register a new employee
    void register_employee()
    {
        int id;
        string name;
        cout << "Enter Employee ID: ";
        cin >> id;  // Input employee ID
        cin.ignore();  // Ignore the newline character left in the buffer
        cout << "Enter Employee Name: ";
        getline(cin, name);  // Input employee name

        // Check if employee with the same ID already exists
        if (find_employee(id))
        {
            cout << "Employee with ID " << id << " already exists.\n";
            return;
        }

        // Create a new employee and add it to the list and the file
        Employee new_employee(id, name);
        employees.push_back(new_employee);
        save_employee(new_employee);
        cout << "Employee " << name << " registered successfully.\n";
    }

    // Function to find an employee by ID
    Employee* find_employee(int id)
    {
        for (auto& emp : employees)  // Iterate through the employee list
        {
            if (emp.get_id() == id)  // Check if employee ID matches
                return &emp;  // Return the employee if found
        }
        return nullptr;  // Return nullptr if no employee is found
    }

    // Getter to retrieve the list of employees
    vector<Employee>& get_employees() { return employees; }
};

struct AttendanceRequest
{
    int employee_id;
    string employee_name;
};
struct AttendanceRecord
{
    int employee_id;
    string date;
    string status;
    int hours_worked; // Track the hours worked on a given day
};
// Class to handle file operations for loading and saving attendance records
class AttendanceFileHandler
{
public:
    // Function to load attendance records from a file ("attendance.txt")
    vector<AttendanceRecord> load_attendance()
    {
        vector<AttendanceRecord> records;  // Vector to store attendance records
        ifstream file("attendance.txt");   // Open the file for reading
        if (file.is_open())  // Check if the file is open
        {
            string line;
            // Read each line of the file
            while (getline(file, line))
            {
                stringstream ss(line);  // Use stringstream to parse the line
                AttendanceRecord record;
                getline(ss, record.date, ',');  // Read the date from the line
                ss >> record.employee_id;  // Read the employee ID
                ss.ignore(1, ',');  // Ignore the comma separator
                getline(ss, record.status, ',');  // Read the attendance status (Present/Absent)
                ss >> record.hours_worked;  // Read the number of hours worked
                records.push_back(record);  // Add the record to the vector
            }
            file.close();  // Close the file after reading
        }
        return records;  // Return the list of attendance records
    }

    // Function to save an attendance record to the file ("attendance.txt")
    void save_attendance(const AttendanceRecord& record)
    {
        ofstream file("attendance.txt", ios::app);  // Open the file in append mode
        if (file.is_open())  // Check if the file is open
        {
            // Write the attendance record to the file (date, employee ID, status, hours worked)
            file << record.date << "," << record.employee_id << "," << record.status << "," << record.hours_worked << endl;
            file.close();  // Close the file after writing
        }
    }
};
class IAttendanceManager {
public:
    virtual vector<AttendanceRecord> load_attendance_data() = 0;
    virtual void request_attendance(EmployeeManager& emp_manager, int employee_id) = 0;
    virtual void view_attendance(int employee_id) = 0;
    virtual void mark_attendance(int employee_id) = 0;
    virtual void generate_attendance_report(int employee_id) = 0;
    virtual void view_pending_requests() = 0;
  
};

// Class to manage attendance-related operations (mark attendance, view attendance, etc.)
class AttendanceManager :public IAttendanceManager
{
private:
    AttendanceFileHandler file_handler;  // Instance of AttendanceFileHandler for file operations
    vector<AttendanceRequest> pending_requests;  // List of pending attendance requests

public:
    // Function to load attendance data from file using AttendanceFileHandler
    vector<AttendanceRecord> load_attendance_data() 
    {
        return file_handler.load_attendance();  // Return the list of attendance records
    }

    // Function to submit a request for marking attendance for a specific employee
    void request_attendance(EmployeeManager& emp_manager, int employee_id) 
    {
        Employee* employee = emp_manager.find_employee(employee_id);  // Find the employee by ID
        if (!employee)  
        {
            cout << "No employee found with ID: " << employee_id << endl;
            return;
        }

        // Check if the attendance request for this employee has already been submitted
        auto it = find_if(pending_requests.begin(), pending_requests.end(),
            [employee_id](const AttendanceRequest& req) { return req.employee_id == employee_id; });
        if (it == pending_requests.end())  // If not found, add the request
        {
            pending_requests.push_back({ employee_id, employee->get_name() });  // Add request to pending list
            cout << "Attendance request submitted for " << employee->get_name() << " (ID: " << employee_id << ").\n";
        }
        else
        {
            cout << "Attendance request already submitted.\n";  // If already exists, notify user
        }
    }

    // Function to view all pending attendance requests
    void view_pending_requests() 
    {
        if (pending_requests.empty())  // If no pending requests
        {
            cout << "No pending attendance requests.\n";
            return;
        }
        cout << "Pending Attendance Requests:\n";
        // Print the details of all pending requests
        for (const auto& req : pending_requests)
        {
            cout << "ID: " << req.employee_id << ", Name: " << req.employee_name << endl;
        }
    }

    void show_attendance_details(int employee_id)
    {
        vector<AttendanceRecord> records = file_handler.load_attendance(); // Load attendance records
        bool found = false;

        cout << "\n--- Attendance Details for Employee ID: " << employee_id << " ---\n";

        for (const auto& record : records)
        {
            if (record.employee_id == employee_id)
            {
                cout << "Date: " << record.date << ", Status: " << record.status
                    << ", Hours Worked: " << record.hours_worked << "\n";
                found = true;
            }
        }

        if (!found)
            cout << "No attendance records found for Employee ID: " << employee_id << ".\n";
    }

    void generate_attendance_report(int employee_id)
    {
        vector<AttendanceRecord> records = file_handler.load_attendance(); // Load attendance records
        int total_hours = 0, days_present = 0;

        for (const auto& record : records)
        {
            if (record.employee_id == employee_id && record.status == "Present")
            {
                total_hours += record.hours_worked;
                days_present++;
            }
        }

        cout << "\n--- Attendance Report for Employee ID: " << employee_id << " ---\n";
        cout << "Total Days Present: " << days_present << "\n";
        cout << "Total Hours Worked: " << total_hours << "\n";
    }


    // Function to mark attendance for an employee
    void mark_attendance(int employee_id) 
    {
        // Search for the attendance request for the given employee ID
        auto it = find_if(pending_requests.begin(), pending_requests.end(),
            [employee_id](const AttendanceRequest& req) { return req.employee_id == employee_id; });
        if (it == pending_requests.end())  // If no pending request found
        {
            cout << "No pending attendance request found for Employee ID: " << employee_id << endl;
            return;
        }

        // Prompt the user to enter attendance details (date, status, hours worked)
        string date, status;
        int hours;
        cout << "Enter Date (yyyy-mm-dd): ";
        cin >> date;
        do
        {
            cout << "Enter Status (P/p for Present, A/a for Absent): ";
            cin >> status;
            // Ensure valid status input (either 'P' or 'A')
            if (status == "P" || status == "p")
            {
                status = "Present";
                break;
            }
            else if (status == "A" || status == "a")
            {
                status = "Absent";
                break;
            }
            else
            {
                cout << "Invalid input. Please enter P/p for Present or A/a for Absent.\n";
            }
        } while (true);

        cout << "Enter Hours Worked: ";
        cin >> hours;
        // Create an attendance record and save it to the file
        AttendanceRecord record{ employee_id, date, status, hours };
        file_handler.save_attendance(record);  // Save record using file handler
        pending_requests.erase(it);  // Remove the processed request from the pending list
        cout << "Attendance marked for Employee ID: " << employee_id << " on " << date << " as " << status << " for " << hours << " hours.\n";
    }

    // Function to view attendance records for a specific employee
    void view_attendance(int employee_id) 
    {
        // Load all attendance records from file
        vector<AttendanceRecord> records = file_handler.load_attendance();
        bool found = false;
        // Print the attendance records for the specified employee
        for (const auto& record : records)
        {
            if (record.employee_id == employee_id)  // If record matches the employee ID
            {
                cout << "Date: " << record.date << ", Status: " << record.status << ", Hours Worked: " << record.hours_worked << endl;
                found = true;
            }
        }
        if (!found)  // If no records are found for the employee
        {
            cout << "No attendance records found for Employee ID: " << employee_id << endl;
        }
    }
};


struct Leave
{
    int employee_id;
    string type;
    string start_date;
    string end_date;
    string reason;
    string status; // Pending or Approved
};
// Handles the loading and saving of leave data from/to a file
class LeaveFileHandler
{
public:
    // Method to load leaves from the "leaves.txt" file
    vector<Leave> load_leaves()
    {
        vector<Leave> leaves;
        ifstream file("leaves.txt"); // Open the file for reading
        if (file.is_open())  // Check if file is open
        {
            string line;
            // Read each line of the file
            while (getline(file, line))
            {
                stringstream ss(line); // Use stringstream to parse the line
                string temp;
                Leave leave; // Create a Leave object to store parsed data

                // Parse each field in the line and assign it to the Leave object
                getline(ss, temp, ','); // Employee ID
                leave.employee_id = stoi(temp);  // Convert string to integer
                getline(ss, leave.type, ',');    // Leave type (e.g., Casual, Earned)
                getline(ss, leave.start_date, ','); // Start date of the leave
                getline(ss, leave.end_date, ',');   // End date of the leave
                getline(ss, leave.reason, ',');  // Reason for the leave
                getline(ss, leave.status, ',');  // Status of the leave (e.g., Pending, Approved)

                leaves.push_back(leave); // Add the parsed leave object to the vector
            }
            file.close(); // Close the file after reading
        }
        return leaves; // Return the vector of loaded leaves
    }

    // Method to save a new leave record to the "leaves.txt" file
    void save_leave(const Leave& leave)
    {
        ofstream file("leaves.txt", ios::app); // Open file in append mode
        if (file.is_open())  // Check if file is open
        {
            // Write leave data to the file in CSV format
            file << leave.employee_id << "," << leave.type << "," << leave.start_date << ","
                << leave.end_date << "," << leave.reason << "," << leave.status << endl;
            file.close(); // Close the file after saving
        }
    }
};

// ILeaveManager interface
class ILeaveManager {
public:
    virtual void apply_leave(const Leave& leave) = 0;
    virtual void view_leaves(int employee_id) = 0;
    virtual void approve_leave_by_supervisor(int leave_id) = 0;
    virtual void approve_leave_by_director(int leave_id) = 0;
    virtual void show_pending_leaves() = 0;
    virtual void generate_leave_report(int employee_id, int casual_balance, int earned_balance) = 0;
    virtual vector <Leave> load_leaves() = 0;
 
};

// Manages the leave process, including applying, viewing, approving, and showing pending leaves
// Updated LeaveManager class to handle both Supervisor and Director approval
class LeaveManager : public ILeaveManager {
private:
    LeaveFileHandler file_handler;  // Instance to handle leave-related file operations
    AttendanceFileHandler attendance_file_handler;  // Instance to handle attendance-related file operations

public:

    void apply_leave(const Leave& leave) {
         if (leave.type == "Casual") {
            int leave_duration = calculate_leave_duration(leave.start_date, leave.end_date);
           // Check if Casual leave exceeds the 4-day limit
            if (leave_duration > 4) {
                cout << "You cannot take more than 4 days of Casual leave at a time.\n";
                cout << "Please apply for Earned leave instead.\n";
                return;
            }
            else {
                // Approve Casual leave directly if within the limit
                cout << "Leave applied successfully. Approved directly.\n";
                Leave leave_copy = leave;
                leave_copy.status = "Approved";  
                file_handler.save_leave(leave_copy); 
            }
        }
        else if (leave.type == "Earned") {
            int leave_duration = calculate_leave_duration(leave.start_date, leave.end_date);
            // Check if Earned leave exceeds the 21-day limit
            if (leave_duration > 21) {
                cout << "You cannot take more than 21 days of Earned leave at a time.\n";
                return;
            }
            else {
                cout << "Supervisor and Director approval are required for this leave.\n";
                Leave leave_copy = leave;
                leave_copy.status = "Pending Supervisor Approval";  // Mark leave as pending
                file_handler.save_leave(leave_copy);  // Save leave details to file
                cout << "Leave applied successfully. Waiting for Supervisor approval.\n";
                //// Approve Earned leave if within the limit
                //cout << "Leave applied successfully.\n";
                //Leave leave_copy = leave;
                //leave_copy.status = "Approved";  // Mark the leave as approved
                //file_handler.save_leave(leave_copy);  // Save leave details to file
            }
        }
        else if (leave.type == "Unpaid") {
            cout << "Supervisor and Director approval are required for this leave.\n";
            Leave leave_copy = leave;
            leave_copy.status = "Pending Supervisor Approval";  // Mark leave as pending
            file_handler.save_leave(leave_copy);  // Save leave details to file
            cout << "Leave applied successfully. Waiting for Supervisor approval.\n";
        }
        else if (leave.type == "Official") {
            int leave_duration = calculate_days(leave.start_date, leave.end_date);
            vector<string> dates = generate_date_range(leave.start_date, leave.end_date);
           // Automatically mark attendance for official leave dates
            for (const auto& date : dates) {
                AttendanceRecord record{ leave.employee_id, date, "Present", 8 };
                attendance_file_handler.save_attendance(record);  // Save attendance record
            }
            // Save leave as approved
            Leave approved_leave = leave;
            approved_leave.status = "Approved";
            file_handler.save_leave(approved_leave);
            cout << "Official leave for " << leave_duration << " day(s) approved and credited with 8 hours/day.\n";
        }
        else {
            cout << "Unknown leave type. Leave not applied.\n";
        }
    }

    // Utility method to calculate the number of days between two dates
    int calculate_leave_duration(const string& start_date, const string& end_date) {
        int start_year, start_month, start_day;
        int end_year, end_month, end_day;
        // Parse the start and end dates in "YYYY-MM-DD" format
        sscanf_s(start_date.c_str(), "%d-%d-%d", &start_year, &start_month, &start_day);
        sscanf_s(end_date.c_str(), "%d-%d-%d", &end_year, &end_month, &end_day);
        // Simplified duration calculation assuming valid inputs
        int duration = (end_year - start_year) * 365 + (end_month - start_month) * 30 + (end_day - start_day);
        return duration;
    }
    // Utility method to calculate exact days between two dates
    int calculate_days(const string& start_date, const string& end_date) {
        tm start_tm = {}, end_tm = {};
        // Parse the start and end dates into tm structure
        sscanf_s(start_date.c_str(), "%d-%d-%d", &start_tm.tm_year, &start_tm.tm_mon, &start_tm.tm_mday);
        sscanf_s(end_date.c_str(), "%d-%d-%d", &end_tm.tm_year, &end_tm.tm_mon, &end_tm.tm_mday);
       // Adjust for tm structure specifics
        start_tm.tm_year -= 1900;
        end_tm.tm_year -= 1900;
        start_tm.tm_mon -= 1;
        end_tm.tm_mon -= 1;
        // Calculate time difference
        time_t start_time = mktime(&start_tm);
        time_t end_time = mktime(&end_tm);
        return (end_time - start_time) / (60 * 60 * 24) + 1;  // Convert seconds to days
    }
    // Utility method to generate date range between two dates
    vector<string> generate_date_range(const string& start_date, const string& end_date) {
        vector<string> dates;
        tm current_tm = {};
        // Parse start date into tm structure
        sscanf_s(start_date.c_str(), "%d-%d-%d", &current_tm.tm_year, &current_tm.tm_mon, &current_tm.tm_mday);
        current_tm.tm_year -= 1900;
        current_tm.tm_mon -= 1;
        time_t current_time = mktime(&current_tm);
        // Generate consecutive dates
        for (int i = 0; i < calculate_days(start_date, end_date); ++i) {
            tm temp_tm;
            localtime_s(&temp_tm, &current_time);  // Get current date
            char date[11];
            strftime(date, sizeof(date), "%Y-%m-%d", &temp_tm);  // Format the date
            dates.push_back(date);
            current_time += 24 * 60 * 60;  // Increment by 1 day
        }
        return dates;
    }
   // Method to view all leave records for a specific employee
    void view_leaves(int employee_id) {
        vector<Leave> leaves = file_handler.load_leaves();  // Load leave records from file
        bool found = false;
        // Display all leaves for the specified employee
        for (const auto& leave : leaves) {
            if (leave.employee_id == employee_id) {
                cout << "Type: " << leave.type << ", Start Date: " << leave.start_date
                    << ", End Date: " << leave.end_date << ", Reason: " << leave.reason
                    << ", Status: " << leave.status << endl;
                found = true;
            }
        }
        // Show a message if no leave records are found
        if (!found) {
            cout << "No leave records found for Employee ID: " << employee_id << endl;
        }
    }
    // Method to approve leave by Supervisor
    void approve_leave_by_supervisor(int leave_id)
    {        vector<Leave> leaves = file_handler.load_leaves(); // Load all leaves
        bool found = false; // Flag to check if the leave is found
        // Loop through leaves to find the pending leave for Supervisor approval
        for (auto& leave : leaves)
        {            if (leave_id == leave.employee_id && leave.status == "Pending Supervisor Approval")
            {
                leave.status = "Pending Director Approval"; // Change status to "Pending Director Approval" after Supervisor's approval
                found = true;
                break; // Stop once the leave is found and updated
            }
        }
        // If the leave is found and updated, save the changes to the file
        if (found)
        {   ofstream file("leaves.txt", ios::out); // Open the file for writing (overwrite mode)
            if (file.is_open())  // Check if file is open
            {               // Write all leaves to the file, including the updated leave status
                for (const auto& leave : leaves)
                {
                    file << leave.employee_id << "," << leave.type << "," << leave.start_date << ","
                        << leave.end_date << "," << leave.reason << "," << leave.status << endl;
                }
                file.close(); // Close the file after saving
                cout << "Leave approved by Supervisor. Now awaiting Director approval.\n";
            }
            else
            {
                cout << "Error: Could not open leave file to save changes.\n"; // Error message if file cannot be opened
            }
        }
        else
        {
            cout << "No leave found for Supervisor approval with ID: " << leave_id << endl;
        }
    }
    void approve_leave_by_director(int leave_id)
    {        vector<Leave> leaves = file_handler.load_leaves(); // Load all leaves
        bool found = false; // Flag to check if the leave is found
        // Loop through leaves to find the pending leave for Director approval
        for (auto& leave : leaves)
        {
            if (leave_id == leave.employee_id && leave.status == "Pending Director Approval")
            {
                leave.status = "Approved"; // Change status to "Approved" after Director's approval
                found = true;
                break; // Stop once the leave is found and updated
            }
        }
        // If the leave is found and updated, save the changes to the file
        if (found)
        {            ofstream file("leaves.txt", ios::out); // Open the file for writing (overwrite mode)
            if (file.is_open())  // Check if file is open
            {               // Write all leaves to the file, including the updated leave status
                for (const auto& leave : leaves)
                {
                    file << leave.employee_id << "," << leave.type << "," << leave.start_date << ","
                        << leave.end_date << "," << leave.reason << "," << leave.status << endl;
                }
                file.close(); // Close the file after saving
                cout << "Leave approved by Director.\n";
            }
            else
            {
                cout << "Error: Could not open leave file to save changes.\n"; // Error message if file cannot be opened
            }
        }
        else
        {
            cout << "No leave found for Director approval with ID: " << leave_id << endl;
        }
    }
    void show_pending_leaves()
    {
        vector<Leave> leaves = file_handler.load_leaves(); // Load all leaves
        cout << "Pending Leaves:\n";
        // Loop through the leaves and print those with a status of "Pending Supervisor Approval" or "Pending Director Approval"
        for (const auto& leave : leaves)
        {
            if (leave.status == "Pending Supervisor Approval" || leave.status == "Pending Director Approval")
            {
                cout << "Employee ID: " << leave.employee_id << ", Type: " << leave.type
                    << ", Reason: " << leave.reason << ", Status: " << leave.status << endl;
            }
        }
    }

    void show_leave_details(int employee_id, int casual_balance, int earned_balance) {
        vector<Leave> leaves = file_handler.load_leaves(); // Load leave records
        bool found = false;

        cout << "\n--- Leave Details for Employee ID: " << employee_id << " ---\n";
        for (const auto& leave : leaves)
        {
            if (leave.employee_id == employee_id)
            {
                cout << "Type: " << leave.type << ", Start Date: " << leave.start_date
                    << ", End Date: " << leave.end_date << ", Reason: " << leave.reason
                    << ", Status: " << leave.status << "\n";
                found = true;
            }
        }

        if (!found)
            cout << "No leave records found for Employee ID: " << employee_id << ".\n";

        cout << "\n--- Leave Balances ---\n";
        cout << "Casual Leave Balance: " << casual_balance << "\n";
        cout << "Earned Leave Balance: " << earned_balance << "\n";
    }
    void generate_leave_report(int employee_id, int casual_balance, int earned_balance)
    {
        vector<Leave> leaves = file_handler.load_leaves(); // Load leave records
        int total_casual_used = 0, total_earned_used = 0; // Track used leaves for each type
        int total_days; // Variable to calculate days for each leave

        cout << "\n--- Leave Report for Employee ID: " << employee_id << " ---\n";

        for (const auto& leave : leaves)
        {
            if (leave.employee_id == employee_id)
            {
                cout << "Type: " << leave.type << ", Start Date: " << leave.start_date
                    << ", End Date: " << leave.end_date << ", Status: " << leave.status << "\n";

                // Count only approved leaves
                if (leave.status == "Approved")
                {
                    total_days = calculate_days(leave.start_date, leave.end_date);
                    if (leave.type == "Casual")
                        total_casual_used += total_days;
                    else if (leave.type == "Earned")
                        total_earned_used += total_days;
                }
            }
        }
        // Calculate remaining balances
        int remaining_casual = casual_balance - total_casual_used;
        int remaining_earned = earned_balance - total_earned_used;
        cout << "\n--- Leave Summary ---\n";
        cout << "Total Casual Leaves Used: " << total_casual_used << "\n";
        cout << "Total Earned Leaves Used: " << total_earned_used << "\n";
        cout << "Remaining Casual Leaves: " << remaining_casual << "\n";
        cout << "Remaining Earned Leaves: " << remaining_earned << "\n";
    }
    // Method to load all leaves (returns a vector of Leave objects)
    vector<Leave> load_leaves()
    {
        return file_handler.load_leaves(); // Load and return leaves from file
    }
};

class ReportGenerator
{
private:
    // References to AttendanceManager and LeaveManager to access attendance and leave data
    IAttendanceManager& attendance_manager;
    ILeaveManager& leave_manager;

public:
    // Constructor to initialize ReportGenerator with references to the AttendanceManager and LeaveManager
    ReportGenerator(IAttendanceManager& att_manager, ILeaveManager& leave_mgr)
        : attendance_manager(att_manager), leave_manager(leave_mgr) {
    }

    // Method to generate the attendance report for employees with less than 80% attendance
    void generate_attendance_report()
    {
        // Load all attendance records
        vector<AttendanceRecord> records = attendance_manager.load_attendance_data();

        // Assuming a 40-hour workweek for 5 working days
        int total_working_hours = 40 * 5;

        cout << "\n--- Attendance Report (Less than 80% attendance) ---\n";

        // Map to store the total hours worked by each employee
        unordered_map<int, int> hours_worked_by_employee;

        // Loop through all the attendance records and calculate the total hours worked by each employee
        for (const auto& record : records)
        {
            hours_worked_by_employee[record.employee_id] += record.hours_worked;
        }

        // Loop through each employee's total hours worked to check if it's less than 80% of the expected hours
        for (const auto& employee_hours : hours_worked_by_employee)
        {
            int employee_id = employee_hours.first;
            int hours_worked = employee_hours.second;

            // 80% of the total expected working hours in a week
            int eighty_percent_of_week_hours = total_working_hours * 0.8;

            // If hours worked are less than 80% of the expected hours, print the employee's information
            if (hours_worked < eighty_percent_of_week_hours)
            {
                cout << "Employee ID: " << employee_id
                    << ", Total Hours Worked: " << hours_worked
                    << " (Less than 80% of expected hours)\n";
            }
        }
    }

    // Method to generate a leave report showing all pending leave requests
    void generate_leave_report()
    {
        // Load all leave records
        vector<Leave> leaves = leave_manager.load_leaves();

        cout << "\n--- Leave Report ---\n";

        // Loop through all leaves and show those that are pending
        for (const auto& leave : leaves)
        {
            if (leave.status == "Pending")
            {
                cout << "Employee ID: " << leave.employee_id << ", Type: " << leave.type << ", Reason: " << leave.reason << endl;
            }
        }
    }

    // Method to generate a report of employees who were absent without an approved leave
    void generate_absent_without_approved_leave_report()
    {
        // Load all attendance records and leave records
        vector<AttendanceRecord> records = attendance_manager.load_attendance_data();
        vector<Leave> leaves = leave_manager.load_leaves();

        // Map to store approved leave dates for each employee
        unordered_map<int, unordered_set<string>> employee_approved_dates;

        // Populate the map with all approved leave dates
        for (const auto& leave : leaves)
        {
            if (leave.status == "Approved")
            {
                string start_date = leave.start_date;
                string end_date = leave.end_date;

                // Generate all dates in the leave range
                vector<string> approved_dates = generate_date_range(start_date, end_date);

                // Add each date to the employee's approved dates set
                for (const auto& date : approved_dates)
                {
                    employee_approved_dates[leave.employee_id].insert(date);
                }
            }
        }

        cout << "\n--- Report of Employees Who Were Absent Without Approved Leave ---\n";

        // Check attendance records for absences without approved leaves
        for (const auto& record : records)
        {
            if (record.status == "Absent")
            {
                int employee_id = record.employee_id;
                string date = record.date;
                
                // If the date is not in the employee's approved leave set, report it
                if (employee_approved_dates[employee_id].find(date) == employee_approved_dates[employee_id].end())
                {
                    cout << "Employee ID: " << employee_id
                        << ", Absent on: " << date
                        << " without approved leave.\n";
                }
            }
        }
    }
    vector<string> generate_date_range(const string& start_date, const string& end_date)
    {
        vector<string> dates;
        tm start_tm = {}, end_tm = {}, temp_tm = {};

        // Parse start and end dates into tm structure
        sscanf_s(start_date.c_str(), "%d-%d-%d", &start_tm.tm_year, &start_tm.tm_mon, &start_tm.tm_mday);
        sscanf_s(end_date.c_str(), "%d-%d-%d", &end_tm.tm_year, &end_tm.tm_mon, &end_tm.tm_mday);

        start_tm.tm_year -= 1900; // tm_year is years since 1900
        start_tm.tm_mon -= 1;     // tm_mon is 0-based
        end_tm.tm_year -= 1900;
        end_tm.tm_mon -= 1;

        time_t current_time = mktime(&start_tm);
        time_t end_time = mktime(&end_tm);

        // Generate all dates from start_date to end_date
        while (current_time <= end_time)
        {
            // Use localtime_s to get the broken-down time structure
            if (localtime_s(&temp_tm, &current_time) == 0)
            {
                char date[11];
                strftime(date, sizeof(date), "%Y-%m-%d", &temp_tm); // Format the date as yyyy-mm-dd
                dates.push_back(string(date));
            }

            current_time += 24 * 60 * 60; // Add one day in seconds
        }

        return dates;
    }


};

class SystemMenu
{
private:
    // Manager classes for handling various functionalities
    EmployeeManager employee_manager;
    IAttendanceManager& attendance_manager;
    ILeaveManager& leave_manager;
    ReportGenerator report_generator;


    void reports_menu()
    {
        int choice;
        do
        {
            cout << "\n--- Reports Menu ---\n";
            cout << "1. Generate Attendance Report for Employee\n";
            cout << "2. Generate Leave Report for Employee\n";
            cout << "3. View Employees with Attendance Below 80%\n";
            cout << "4. View Employees with unapproved leaves\n";
            cout << "5. Exit Reports Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            clear_screen();

            switch (choice)
            {
            case 1:
            {
                int emp_id;
                cout << "Enter Employee ID for Attendance Report: ";
                cin >> emp_id;
                attendance_manager.generate_attendance_report(emp_id);
                break;
            }
            case 2:
            {
                int emp_id;

                cout << "Enter Employee ID for Leave Report: ";
                cin >> emp_id;
                Employee* employee = employee_manager.find_employee(emp_id);
                if (!employee)
                {
                    cout << "No employee found with ID: " << emp_id << ".\n";
                    break;
                }
                leave_manager.generate_leave_report(emp_id, employee->get_casual_leave_balance(), employee->get_earned_leave_balance());
                break;
            }
            case 3:
                report_generator.generate_attendance_report();  // View employees with attendance below 80%
                break;
            case 4:
                report_generator.generate_absent_without_approved_leave_report();  // View employees with unapproved leaves
                break;
            case 5:
                cout << "Exiting Reports Menu...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 5);
    }


    // Menu for employee-specific actions
    void employee_menu(int id)
    {
        int choice;
        do
        {
            cout << "\n--- Employee Menu ---\n";
            cout << "1. View Attendance\n";
            cout << "2. Apply for Leave\n";
            cout << "3. Request Attendance\n";
            cout << "4. View Leave Status\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            clear_screen();  // Clears the screen after each choice

            // Switch-case for handling employee actions
            switch (choice)
            {
            case 1:
                attendance_manager.view_attendance(id);  // View attendance of the employee
                break;
            case 2:
            {
                // Input leave details
                string type, start, end, reason;
                cout << "Enter Leave Type (Casual/Earned/Official/Unpaid): ";
                cin >> type;
                cout << "Enter Start Date (yyyy-mm-dd): ";
                cin >> start;
                cout << "Enter End Date (yyyy-mm-dd): ";
                cin >> end;
                cout << "Enter Reason: ";
                cin.ignore();  // Clears the input buffer
                getline(cin, reason);

                // Create Leave object and apply leave
                Leave leave{ id, type, start, end, reason, "Pending" };
                leave_manager.apply_leave(leave);
                break;
            }
            case 3:
                attendance_manager.request_attendance(employee_manager, id);  // Request attendance correction
                break;
            case 4:
                leave_manager.view_leaves(id);  // View leave status
                break;
            case 5:
                cout << "Exiting Employee Menu...\n";  // Exit the employee menu
                break;
            default:
                cout << "Invalid choice! Try again.\n";  // Handle invalid input
                break;
            }
        } while (choice != 5);  // Keep showing the menu until the user chooses to exit
    }

    // Menu for guard-specific actions
    void guard_menu()
    {
        int choice;
        do
        {
            cout << "\n--- Guard Menu ---\n";
            cout << "1. View Pending Attendance\n";
            cout << "2. Mark Attendance\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            clear_screen();  // Clears the screen after each choice

            // Switch-case for handling guard actions
            switch (choice)
            {
            case 1:
                attendance_manager.view_pending_requests();  // View all pending attendance requests
                break;
            case 2:
            {
                int emp_id;
                cout << "Enter Employee ID to mark attendance: ";
                cin >> emp_id;
                attendance_manager.mark_attendance(emp_id);  // Mark attendance for the given employee
                break;
            }
            case 3:
                cout << "Exiting Guard Menu...\n";  // Exit the guard menu
                break;
            default:
                cout << "Invalid choice! Try again.\n";  // Handle invalid input
                break;
            }
        } while (choice != 3);  // Keep showing the menu until the user chooses to exit
    }

    // Menu for supervisor-specific actions
    void supervisor_menu()
    {
        int choice;
        do
        {
            cout << "\n--- Supervisor Menu ---\n";
            cout << "1. Approve Leave\n";
            cout << "2. View Pending Leaves\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            clear_screen();  // Clears the screen after each choice

            // Switch-case for handling supervisor actions
            switch (choice)
            {
            case 1:
            {
                int leave_id;
                cout << "Enter Leave ID to approve: ";
                cin >> leave_id;
                leave_manager.approve_leave_by_supervisor(leave_id);  // Approve leave for the given leave ID
                break;
            }
            case 2:
                leave_manager.show_pending_leaves();  // View all pending leave requests
                break;
            case 3:
                cout << "Exiting Supervisor Menu...\n";  // Exit the supervisor menu
                break;
            default:
                cout << "Invalid choice! Try again.\n";  // Handle invalid input
                break;
            }
        } while (choice != 3);  // Keep showing the menu until the user chooses to exit
    }

    // Menu for director-specific actions
    void director_menu()
    {
        int choice;
        do
        {
            cout << "\n--- Director Menu ---\n";
            cout << "1. Approve Earned/Unpaid Leave\n";
            cout << "2. View Pending Leaves\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            clear_screen();  // Clears the screen after each choice

            // Switch-case for handling director actions
            switch (choice)
            {
            case 1:
            {
                int leave_id;
                cout << "Enter Leave ID to approve: ";
                cin >> leave_id;
                leave_manager.approve_leave_by_director(leave_id);  // Approve earned/unpaid leave for the given leave ID
                break;
            }
            case 2:
                leave_manager.show_pending_leaves();  // View all pending leave requests
                break;
            case 3:
                cout << "Exiting Director Menu...\n";  // Exit the director menu
                break;
            default:
                cout << "Invalid choice! Try again.\n";  // Handle invalid input
                break;
            }
        } while (choice != 3);  // Keep showing the menu until the user chooses to exit
    }

public:

    // Constructor to initialize the menu system and load employees
    SystemMenu(IAttendanceManager& att_manager, ILeaveManager& leave_mgr)
        : attendance_manager(att_manager), leave_manager(leave_mgr),
        report_generator(att_manager, leave_mgr)
    {
        employee_manager.load_employees();  // Load employees from file at startup
    }

    // Main menu for selecting roles (Employee, Guard, Supervisor, Director, etc.)
    void show_menu()
    {
        int choice;
        do
        {
            cout << "\n--- System Menu ---\n";
            cout << "1. Register Employee\n";
            cout << "2. Employee\n";
            cout << "3. Guard\n";
            cout << "4. Supervisor\n";
            cout << "5. Director\n";
            cout << "6. Reports\n";
            cout << "7. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            clear_screen();  // Clears the screen after each choice

            // Switch-case for handling different user roles
            switch (choice)
            {
            case 1:
                employee_manager.register_employee();  // Register a new employee
                break;
            case 2:
            {
                int id;
                cout << "Enter your Employee ID: ";
                cin >> id;
                employee_menu(id);  // Navigate to employee-specific actions
                break;
            }
            case 3:
                guard_menu();  // Navigate to guard-specific actions
                break;
            case 4:
                supervisor_menu();  // Navigate to supervisor-specific actions
                break;
            case 5:
                director_menu();  // Navigate to director-specific actions
                break;
            case 6:
                reports_menu();
                break;
            case 7:
                cout << "Exiting System...\n";  // Exit the system
                break;
            default:
                cout << "Invalid choice! Try again.\n";  // Handle invalid input
                break;
            }
        } while (choice != 7);  // Keep showing the menu until the user chooses to exit
    }
};

int main()
{
    AttendanceManager attendance_manager;
    LeaveManager leave_manager;
    SystemMenu menu(attendance_manager, leave_manager);
    menu.show_menu();
    return 0;
}


