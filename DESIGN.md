# Design Notes

## Goals
- Keep responsibilities clear between domain objects (Student, Course) and coordination logic (University).
- Demonstrate required STL containers in realistic roles rather than contrived usage.
- Make the program easy to extend (e.g., persistence, extra queries) without rewriting the core enrollment logic.

## Architecture
- **Student**: owns ID, name, and two enrollment collections. `std::set` prevents duplicate course codes and offers fast lookup. `std::list` preserves the order the student enrolled, which is helpful for reports.
- **Course**: owns code, title, instructor, capacity, enrolled students, and a waitlist. `std::set` keeps a unique roster. `std::queue` models FIFO waitlists. Helpers handle promotion/removal from the waitlist.
- **University**: orchestrates operations with `std::map` registries for students and courses. It coordinates updates across objects (adding/removing students from rosters, promoting waitlisted students) and records actions in an `std::deque` activity log (fast front/back operations and trimming via pop_front). Search helpers return `std::pair` values to demonstrate paired associations.

## Data Flow
- Startup: `main.cpp` loads demo students/courses from `data/` files and populates the `University` maps.
- Enrollment: `University::enrollStudentInCourse` rejects duplicates (roster or waitlist). If capacity is open, the student joins the course roster and their personal sets/lists; otherwise they enter the waitlist queue. Actions are logged.
- Drop: `University::dropStudentFromCourse` removes from roster or waitlist. Dropping a rostered student triggers a waitlist promotion if available. Removing a student from the system also cleans them from every waitlist and fills vacancies via promotions.
- Removal: Dropping a course purges it from each student's enrollment list to keep student state consistent.

## STL Container Usage
- `std::map`: primary registries of students and courses keyed by ID/code; provides ordered lookup and search iteration.
- `std::set`: rosters and each student's enrolled courses; enforces uniqueness and quick membership tests.
- `std::list`: stores a student's enrollment order so reports reflect join order even though `std::set` is used for membership.
- `std::queue`: waitlists for oversubscribed courses; enables FIFO promotions.
- `std::deque`: activity log enabling cheap append/trim at both ends; capped to a rolling window.
- `std::pair`: search helpers (e.g., instructor search returns code/title pairs) and waitlist operations return success/value pairs.

## Error Handling & Edge Cases
- Duplicate add operations return `false` and log nothing disruptive.
- Enrollment checks for both roster and waitlist duplicates before acting.
- Dropping a student cleans them from every course roster and waitlist to avoid stale references.
- Course capacity updates are accepted even if capacity drops below the current roster; the roster is left intact while future adds respect the new limit.
- CLI entry validation rejects non-positive capacity values to avoid unusable courses; file loading tolerates blank lines and ignores malformed records with too few fields.

## Extensibility
- Persistence: Because domain objects expose clean getters/setters, a serializer (JSON/CSV) can be layered without changing core logic.
- Queries: Additional searches (e.g., by course title) can iterate the `std::map` registries similarly to the instructor search helper.
- UI: The CLI is isolated in `main.cpp`; swapping to a GUI or web API would reuse the University class wholesale.

## Testing Notes
- `make run` provides an interactive menu for quick smoke testing.
- `make test` runs assertion-based coverage over enrollment/waitlist promotions, updates/removals, and duplicate handling.
- A sample scripted run that exercises enrollment, waitlisting, and promotions is captured in `logs/demo_output.txt`.
