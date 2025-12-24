 #include "University.h"
 
 #include <algorithm>
 #include <chrono>
 #include <cctype>
 #include <iomanip>
 #include <sstream>
 #include <utility>
 
 namespace {
 std::string toLower(std::string value) {
     std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
     return value;
 }
 }
 
 bool University::addStudent(const std::string &id, const std::string &name) {
     if (studentExists(id)) {
         return false;
     }
     students.emplace(id, Student{id, name});
     logActivity("Added student " + id + " (" + name + ")");
     return true;
 }
 
 bool University::updateStudentName(const std::string &id, const std::string &newName) {
     auto it = students.find(id);
     if (it == students.end()) {
         return false;
     }
     it->second.setName(newName);
     logActivity("Updated student " + id + " name to " + newName);
     return true;
 }
 
 bool University::removeStudent(const std::string &id) {
     auto it = students.find(id);
     if (it == students.end()) {
         return false;
     }
 
     for (const auto &courseCode : it->second.getCourses()) {
         auto courseIt = courses.find(courseCode);
         if (courseIt != courses.end()) {
             if (courseIt->second.removeStudent(id)) {
                 auto nextUp = courseIt->second.popFromWaitlist();
                 if (nextUp.first) {
                     courseIt->second.addStudent(nextUp.second);
                     auto studentIt = students.find(nextUp.second);
                     if (studentIt != students.end()) {
                         studentIt->second.enrollInCourse(courseCode);
                     }
                     logActivity(nextUp.second + " moved from waitlist into " + courseCode + " after " + id + " left");
                 }
             }
         }
     }
 
     cleanStudentFromWaitlists(id);
     logActivity("Removed student " + id + " (" + it->second.getName() + ")");
     students.erase(it);
     return true;
 }
 
 bool University::addCourse(const std::string &code, const std::string &title, const std::string &instructor, std::size_t capacity) {
     if (courseExists(code)) {
         return false;
     }
     courses.emplace(code, Course{code, title, instructor, capacity});
     logActivity("Added course " + code + " (" + title + ")");
     return true;
 }
 
 bool University::updateCourse(const std::string &code, const std::string &title, const std::string &instructor, std::size_t capacity) {
     auto it = courses.find(code);
     if (it == courses.end()) {
         return false;
     }
     it->second.setTitle(title);
     it->second.setInstructor(instructor);
     it->second.setCapacity(capacity);
     logActivity("Updated course " + code + " info");
     return true;
 }
 
 bool University::removeCourse(const std::string &code) {
     auto it = courses.find(code);
     if (it == courses.end()) {
         return false;
     }
 
     for (const auto &studentId : it->second.getEnrolledStudents()) {
         auto studentIt = students.find(studentId);
         if (studentIt != students.end()) {
             studentIt->second.dropCourse(code);
         }
     }
 
     logActivity("Removed course " + code + " (" + it->second.getTitle() + ")");
     courses.erase(it);
     return true;
 }
 
 bool University::enrollStudentInCourse(const std::string &studentId, const std::string &courseCode) {
     auto studentIt = students.find(studentId);
     auto courseIt = courses.find(courseCode);
     if (studentIt == students.end() || courseIt == courses.end()) {
         return false;
     }
 
     Course &course = courseIt->second;
     Student &student = studentIt->second;
 
     if (course.isEnrolled(studentId)) {
         return false;
     }
     if (course.isInWaitlist(studentId)) {
         return false;
     }
 
     if (course.addStudent(studentId)) {
         student.enrollInCourse(courseCode);
         logActivity("Enrolled " + studentId + " in " + courseCode);
     } else {
         course.addToWaitlist(studentId);
         logActivity("Added " + studentId + " to waitlist for " + courseCode);
     }
     return true;
 }
 
 bool University::dropStudentFromCourse(const std::string &studentId, const std::string &courseCode) {
     auto studentIt = students.find(studentId);
     auto courseIt = courses.find(courseCode);
     if (studentIt == students.end() || courseIt == courses.end()) {
         return false;
     }
 
     Course &course = courseIt->second;
     Student &student = studentIt->second;
 
     bool wasEnrolled = course.removeStudent(studentId);
     bool removedFromWaitlist = false;
     if (!wasEnrolled) {
         removedFromWaitlist = course.removeFromWaitlist(studentId);
     }
 
     if (!wasEnrolled && !removedFromWaitlist) {
         return false;
     }
 
     if (wasEnrolled) {
         student.dropCourse(courseCode);
         auto promoted = course.popFromWaitlist();
         if (promoted.first) {
             course.addStudent(promoted.second);
             auto promotedIt = students.find(promoted.second);
             if (promotedIt != students.end()) {
                 promotedIt->second.enrollInCourse(courseCode);
             }
             logActivity(promoted.second + " promoted from waitlist into " + courseCode);
         }
         logActivity("Dropped " + studentId + " from " + courseCode);
     } else if (removedFromWaitlist) {
         logActivity("Removed " + studentId + " from waitlist for " + courseCode);
     }
 
     return true;
 }
 
 std::vector<std::string> University::listCoursesForStudent(const std::string &studentId) const {
     auto it = students.find(studentId);
     if (it == students.end()) {
         return {};
     }
 
     std::vector<std::string> result;
     for (const auto &courseCode : it->second.getCourseOrder()) {
         result.push_back(courseCode);
     }
     return result;
 }
 
 std::vector<std::string> University::listStudentsInCourse(const std::string &courseCode) const {
     auto it = courses.find(courseCode);
     if (it == courses.end()) {
         return {};
     }
 
     const auto &enrolled = it->second.getEnrolledStudents();
     return {enrolled.begin(), enrolled.end()};
 }
 
 std::vector<std::string> University::listWaitlistForCourse(const std::string &courseCode) const {
     auto it = courses.find(courseCode);
     if (it == courses.end()) {
         return {};
     }
 
     std::vector<std::string> result;
     std::queue<std::string> copy = it->second.getWaitlist();
     while (!copy.empty()) {
         result.push_back(copy.front());
         copy.pop();
     }
     return result;
 }
 
 std::pair<bool, Student> University::getStudent(const std::string &id) const {
     auto it = students.find(id);
     if (it == students.end()) {
         return {false, Student{}};
     }
     return {true, it->second};
 }
 
 std::pair<bool, Course> University::getCourse(const std::string &code) const {
     auto it = courses.find(code);
     if (it == courses.end()) {
         return {false, Course{}};
     }
     return {true, it->second};
 }
 
 std::vector<std::pair<std::string, std::string>> University::searchCoursesByInstructor(const std::string &instructorQuery) const {
     std::vector<std::pair<std::string, std::string>> matches;
     auto needle = toLower(instructorQuery);
     for (const auto &entry : courses) {
         if (toLower(entry.second.getInstructor()).find(needle) != std::string::npos) {
             matches.emplace_back(entry.first, entry.second.getTitle());
         }
     }
     return matches;
 }
 
 std::deque<std::string> University::getActivityLog() const {
     return activityLog;
 }
 
 void University::logActivity(const std::string &entry) {
     std::ostringstream oss;
     oss << "[" << currentTimestamp() << "] " << entry;
     activityLog.push_back(oss.str());
     if (activityLog.size() > logLimit) {
         activityLog.pop_front();
     }
 }
 
 std::string University::currentTimestamp() {
     using std::chrono::system_clock;
     auto now = system_clock::now();
     auto inTimeT = system_clock::to_time_t(now);
     std::tm buf{};
 #if defined(_WIN32)
     localtime_s(&buf, &inTimeT);
 #else
     localtime_r(&inTimeT, &buf);
 #endif
     std::ostringstream oss;
     oss << std::put_time(&buf, "%F %T");
     return oss.str();
 }
 
 bool University::studentExists(const std::string &id) const {
     return students.find(id) != students.end();
 }
 
 bool University::courseExists(const std::string &code) const {
     return courses.find(code) != courses.end();
 }
 
 void University::cleanStudentFromWaitlists(const std::string &studentId) {
     for (auto &entry : courses) {
         entry.second.removeFromWaitlist(studentId);
     }
 }
