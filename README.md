# University Course Management System

Console-based C++17 program that manages students, courses, enrollment, waitlists, and activity logs using STL containers (map, set, list, queue, deque, pair).

## Build & Run
- Requirements: `g++` or `clang++` with C++17 support, `make`.
- Build: `make`
- Run: `make run`
- Clean: `make clean`
- Tests: `make test` (assertion-based sanity suite)

The app loads demo data from `data/students.txt` and `data/courses.txt` at startup. Edit those files or use the menu to add more records.

## Menu Actions
- Add/update/remove students
- Add/update/remove courses
- Enroll/drop students (auto waitlist + promotions)
- List a student's courses (preserves enroll order)
- List course rosters and waitlists
- Search courses by instructor (uses `std::pair` results)
- View the activity log backed by `std::deque`

## Data File Format
- `data/students.txt`: `student_id,full name`
- `data/courses.txt`: `course_code,title,instructor,capacity`

## Project Layout
- `include/` – headers for Student, Course, and University
- `src/` – implementations and `main.cpp` menu driver
- `data/` – sample input records
- `logs/` – sample output (generated in `logs/demo_output.txt`)
- `Makefile` – build helpers

See `DESIGN.md` for STL/container choices and design notes.

**Contribution Breakdown**

- Michael Saenz: Core CLI wiring and menu UX; implemented enrollment/drop flows with waitlisting and promotion logic; added validation for capacities and duplicate handling; produced demo data and sample log run.

- Wenning Wan: Designed and implemented Student/Course/University classes and STL container usage; built test suite (`make test`) covering enrollment edge cases; authored design notes and structured the project/Makefile.
