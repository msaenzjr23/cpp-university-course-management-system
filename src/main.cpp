 #include "University.h"
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
 
 namespace {
 std::vector<std::string> split(const std::string &line, char delimiter) {
     std::vector<std::string> parts;
     std::stringstream ss(line);
     std::string item;
     while (std::getline(ss, item, delimiter)) {
         parts.push_back(item);
     }
     return parts;
 }
 
std::string trim(const std::string &text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::pair<bool, std::size_t> parseCapacity(const std::string &input) {
    try {
        auto value = static_cast<std::size_t>(std::stoul(input));
        if (value == 0) {
            return {false, 0};
        }
        return {true, value};
    } catch (...) {
        return {false, 0};
    }
}

bool promptCapacity(std::size_t &capacityOut) {
    std::string capacityInput;
    std::cout << "Capacity (must be positive): ";
    std::getline(std::cin, capacityInput);
    auto parsed = parseCapacity(trim(capacityInput));
    if (!parsed.first) {
        std::cout << "Invalid capacity. Please enter a positive number.\n";
        return false;
    }
    capacityOut = parsed.second;
    return true;
}
 
 void loadStudents(const std::string &path, University &uni) {
     std::ifstream file(path);
     if (!file) {
         std::cerr << "Could not open " << path << " for reading.\n";
         return;
     }
     std::string line;
     while (std::getline(file, line)) {
         if (line.empty()) {
             continue;
         }
         auto parts = split(line, ',');
         if (parts.size() >= 2) {
             uni.addStudent(trim(parts[0]), trim(parts[1]));
         }
     }
 }
 
void loadCourses(const std::string &path, University &uni) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open " << path << " for reading.\n";
        return;
     }
     std::string line;
     while (std::getline(file, line)) {
         if (line.empty()) {
             continue;
         }
        auto parts = split(line, ',');
        if (parts.size() >= 4) {
            auto parsed = parseCapacity(trim(parts[3]));
            if (!parsed.first) {
                std::cerr << "Skipping course with invalid capacity: " << line << "\n";
                continue;
            }
            uni.addCourse(trim(parts[0]), trim(parts[1]), trim(parts[2]), parsed.second);
        }
    }
}
 
void printMenu() {
    std::cout << "\n==== University Course Management ====\n";
    std::cout << "1) Add student\n";
    std::cout << "2) Update student name\n";
    std::cout << "3) Remove student\n";
    std::cout << "4) Add course\n";
    std::cout << "5) Update course\n";
    std::cout << "6) Remove course\n";
    std::cout << "7) Enroll student in course\n";
    std::cout << "8) Drop student from course\n";
    std::cout << "9) List courses for student\n";
    std::cout << "10) List students in course\n";
    std::cout << "11) Show waitlist for course\n";
    std::cout << "12) Search courses by instructor\n";
    std::cout << "13) Show activity log\n";
    std::cout << "0) Exit\n";
    std::cout << "Select an option: ";
}
 
 void listCourses(const University &uni) {
     std::string id;
     std::cout << "Student ID: ";
     std::getline(std::cin, id);
     auto courses = uni.listCoursesForStudent(trim(id));
     if (courses.empty()) {
         std::cout << "No courses found for that student.\n";
         return;
     }
     std::cout << "Courses:\n";
     for (const auto &code : courses) {
         std::cout << "- " << code << "\n";
     }
 }
 
 void listStudents(const University &uni) {
     std::string code;
     std::cout << "Course code: ";
     std::getline(std::cin, code);
     auto students = uni.listStudentsInCourse(trim(code));
     if (students.empty()) {
         std::cout << "No students enrolled for that course.\n";
         return;
     }
     std::cout << "Students:\n";
     for (const auto &id : students) {
         std::cout << "- " << id << "\n";
     }
 }
 
 void showWaitlist(const University &uni) {
     std::string code;
     std::cout << "Course code: ";
     std::getline(std::cin, code);
     auto waitlist = uni.listWaitlistForCourse(trim(code));
     if (waitlist.empty()) {
         std::cout << "No one is waitlisted for that course.\n";
         return;
     }
     std::cout << "Waitlist:\n";
     for (const auto &id : waitlist) {
         std::cout << "- " << id << "\n";
     }
 }
 
 void searchByInstructor(const University &uni) {
     std::string query;
     std::cout << "Instructor name (partial accepted): ";
     std::getline(std::cin, query);
     auto matches = uni.searchCoursesByInstructor(trim(query));
     if (matches.empty()) {
         std::cout << "No courses matched.\n";
         return;
     }
     std::cout << "Matches:\n";
     for (const auto &entry : matches) {
         std::cout << "- " << entry.first << ": " << entry.second << "\n";
     }
 }
 
 void printActivityLog(const University &uni) {
     auto log = uni.getActivityLog();
     if (log.empty()) {
         std::cout << "No activity yet.\n";
         return;
     }
     for (const auto &line : log) {
         std::cout << line << "\n";
     }
 }
 }
 
 int main() {
     University uni;
     loadStudents("data/students.txt", uni);
     loadCourses("data/courses.txt", uni);
 
     std::cout << "Demo data loaded from data/students.txt and data/courses.txt.\n";
     std::cout << "Use the menu to manage students and courses.\n";
 
     bool running = true;
     while (running) {
         printMenu();
        std::string choice;
        std::getline(std::cin, choice);
        choice = trim(choice);

        if (choice == "1") {
            std::string id, name;
             std::cout << "New student ID: ";
             std::getline(std::cin, id);
             std::cout << "Full name: ";
             std::getline(std::cin, name);
             if (uni.addStudent(trim(id), trim(name))) {
                 std::cout << "Student added.\n";
             } else {
                 std::cout << "Student already exists.\n";
             }
        } else if (choice == "2") {
            std::string id, name;
            std::cout << "Student ID: ";
            std::getline(std::cin, id);
            std::cout << "New full name: ";
            std::getline(std::cin, name);
            if (uni.updateStudentName(trim(id), trim(name))) {
                std::cout << "Student updated.\n";
            } else {
                std::cout << "Student not found.\n";
            }
        } else if (choice == "3") {
            std::string id;
            std::cout << "Student ID to remove: ";
            std::getline(std::cin, id);
            if (uni.removeStudent(trim(id))) {
                std::cout << "Student removed.\n";
            } else {
                std::cout << "Student not found.\n";
            }
        } else if (choice == "4") {
            std::string code, title, instructor;
            std::cout << "Course code: ";
            std::getline(std::cin, code);
            std::cout << "Title: ";
            std::getline(std::cin, title);
            std::cout << "Instructor: ";
            std::getline(std::cin, instructor);
            std::size_t capacity;
            if (!promptCapacity(capacity)) {
                continue;
            }
            if (uni.addCourse(trim(code), trim(title), trim(instructor), capacity)) {
                std::cout << "Course added.\n";
            } else {
                std::cout << "Course already exists.\n";
            }
        } else if (choice == "5") {
            std::string code, title, instructor;
            std::cout << "Course code: ";
            std::getline(std::cin, code);
            std::cout << "New title: ";
            std::getline(std::cin, title);
            std::cout << "New instructor: ";
            std::getline(std::cin, instructor);
            std::size_t capacity;
            if (!promptCapacity(capacity)) {
                continue;
            }
            if (uni.updateCourse(trim(code), trim(title), trim(instructor), capacity)) {
                std::cout << "Course updated.\n";
            } else {
                std::cout << "Course not found.\n";
            }
        } else if (choice == "6") {
            std::string code;
            std::cout << "Course code to remove: ";
            std::getline(std::cin, code);
            if (uni.removeCourse(trim(code))) {
                std::cout << "Course removed.\n";
            } else {
                std::cout << "Course not found.\n";
            }
        } else if (choice == "7") {
            std::string id, code;
            std::cout << "Student ID: ";
            std::getline(std::cin, id);
            std::cout << "Course code: ";
            std::getline(std::cin, code);
             if (uni.enrollStudentInCourse(trim(id), trim(code))) {
                 std::cout << "Enroll action completed.\n";
             } else {
                 std::cout << "Enroll failed (check IDs or duplicate enrollment).\n";
             }
        } else if (choice == "8") {
            std::string id, code;
            std::cout << "Student ID: ";
            std::getline(std::cin, id);
            std::cout << "Course code: ";
            std::getline(std::cin, code);
             if (uni.dropStudentFromCourse(trim(id), trim(code))) {
                 std::cout << "Drop action completed.\n";
             } else {
                 std::cout << "Drop failed (not enrolled or waitlisted).\n";
             }
        } else if (choice == "9") {
            listCourses(uni);
        } else if (choice == "10") {
            listStudents(uni);
        } else if (choice == "11") {
            showWaitlist(uni);
        } else if (choice == "12") {
            searchByInstructor(uni);
        } else if (choice == "13") {
            printActivityLog(uni);
         } else if (choice == "0") {
            running = false;
         } else {
            std::cout << "Unknown option.\n";
         }
     }
 
     std::cout << "Goodbye.\n";
     return 0;
 }
