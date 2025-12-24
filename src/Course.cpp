 #include "Course.h"
 
 Course::Course(std::string code, std::string title, std::string instructor, std::size_t capacity)
     : code(std::move(code)), title(std::move(title)), instructor(std::move(instructor)), capacity(capacity) {}
 
 const std::string &Course::getCode() const {
     return code;
 }
 
 const std::string &Course::getTitle() const {
     return title;
 }
 
 const std::string &Course::getInstructor() const {
     return instructor;
 }
 
 std::size_t Course::getCapacity() const {
     return capacity;
 }
 
 std::size_t Course::enrolledCount() const {
     return enrolledStudentIds.size();
 }
 
 void Course::setTitle(const std::string &newTitle) {
     title = newTitle;
 }
 
 void Course::setInstructor(const std::string &newInstructor) {
     instructor = newInstructor;
 }
 
 void Course::setCapacity(std::size_t newCapacity) {
     capacity = newCapacity;
 }
 
 const std::set<std::string> &Course::getEnrolledStudents() const {
     return enrolledStudentIds;
 }
 
 const std::queue<std::string> &Course::getWaitlist() const {
     return waitlist;
 }
 
 bool Course::isEnrolled(const std::string &studentId) const {
     return enrolledStudentIds.find(studentId) != enrolledStudentIds.end();
 }
 
 bool Course::isInWaitlist(const std::string &studentId) const {
     std::queue<std::string> copy = waitlist;
     while (!copy.empty()) {
         if (copy.front() == studentId) {
             return true;
         }
         copy.pop();
     }
     return false;
 }
 
 bool Course::addStudent(const std::string &studentId) {
     if (enrolledStudentIds.size() >= capacity) {
         return false;
     }
 
     auto inserted = enrolledStudentIds.insert(studentId);
     return inserted.second;
 }
 
 bool Course::removeStudent(const std::string &studentId) {
     return enrolledStudentIds.erase(studentId) > 0;
 }
 
 void Course::addToWaitlist(const std::string &studentId) {
     waitlist.push(studentId);
 }
 
 bool Course::removeFromWaitlist(const std::string &studentId) {
     bool removed = false;
     std::queue<std::string> rebuilt;
     while (!waitlist.empty()) {
         auto current = waitlist.front();
         waitlist.pop();
         if (current == studentId) {
             removed = true;
             continue;
         }
         rebuilt.push(current);
     }
     waitlist.swap(rebuilt);
     return removed;
 }
 
 std::pair<bool, std::string> Course::popFromWaitlist() {
     if (waitlist.empty()) {
         return {false, ""};
     }
     std::string front = waitlist.front();
     waitlist.pop();
     return {true, front};
 }
