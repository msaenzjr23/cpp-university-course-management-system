#include "University.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

void expect(bool condition, const std::string &message) {
    if (!condition) {
        std::cerr << "Test failed: " << message << "\n";
        std::exit(1);
    }
}

int main() {
    University uni;

    // Setup students and courses
    expect(uni.addStudent("S1", "One"), "add student S1");
    expect(uni.addStudent("S2", "Two"), "add student S2");
    expect(uni.addStudent("S3", "Three"), "add student S3");
    expect(!uni.addStudent("S1", "Duplicate"), "reject duplicate student");

    expect(uni.addCourse("C1", "Course One", "Prof A", 1), "add course C1");
    expect(!uni.addCourse("C1", "Course One", "Prof A", 1), "reject duplicate course");

    // Enrollment and waitlist behavior
    expect(uni.enrollStudentInCourse("S1", "C1"), "S1 enrolls C1");
    expect(uni.enrollStudentInCourse("S2", "C1"), "S2 waitlisted C1");
    expect(uni.enrollStudentInCourse("S3", "C1"), "S3 waitlisted C1");

    auto roster = uni.listStudentsInCourse("C1");
    expect(roster.size() == 1 && roster[0] == "S1", "roster has only S1");

    auto waitlist = uni.listWaitlistForCourse("C1");
    expect(waitlist.size() == 2 && waitlist[0] == "S2" && waitlist[1] == "S3", "waitlist order S2, S3");

    // Dropping promotes waitlist
    expect(uni.dropStudentFromCourse("S1", "C1"), "drop S1");
    roster = uni.listStudentsInCourse("C1");
    expect(roster.size() == 1 && roster[0] == "S2", "S2 promoted from waitlist");
    waitlist = uni.listWaitlistForCourse("C1");
    expect(waitlist.size() == 1 && waitlist[0] == "S3", "waitlist now only S3");

    // Removing a student cleans roster/waitlist and promotes next
    expect(uni.removeStudent("S2"), "remove S2");
    roster = uni.listStudentsInCourse("C1");
    expect(roster.size() == 1 && roster[0] == "S3", "S3 promoted after removing S2");

    // Update course and student data
    expect(uni.updateCourse("C1", "Course One Updated", "Prof B", 2), "update course C1");
    auto courseInfo = uni.getCourse("C1");
    expect(courseInfo.first && courseInfo.second.getInstructor() == "Prof B", "course instructor updated");

    expect(uni.updateStudentName("S3", "Three Updated"), "update student name");
    auto studentInfo = uni.getStudent("S3");
    expect(studentInfo.first && studentInfo.second.getName() == "Three Updated", "student name updated");

    // Remove course cleans enrollment
    expect(uni.removeCourse("C1"), "remove course C1");
    auto coursesForStudent = uni.listCoursesForStudent("S3");
    expect(coursesForStudent.empty(), "student courses cleared after course removal");

    std::cout << "All tests passed.\n";
    return 0;
}
