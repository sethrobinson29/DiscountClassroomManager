#include "Classroom.h"

// classroom functions
void classroom::set_course_name(std::string str) {
	this->course_name = str;
}
std::string classroom::get_course_name( ) {
	return this->course_name;
}

void classroom::set_instructor(int id) {
	this->instructor = id;
}
int classroom::get_instructor() { 
	return this->instructor; 
}

void classroom::print_roster() {
	for (int i = 0; i < this->roster.size(); i++) {
		std::cout << this->roster[i].get_first_name() << " " << this->roster[i].get_last_name() << std::endl;
	}
}

//name of assignement, student id, grade of assignment
void classroom::create_assignment(std::string &s, std::vector<int> roster) {
		std::unordered_map<int, int> grades;
		for (auto i: roster) {
			grades.insert(std::make_pair(i, 0));
		}
		this->assignments.insert({{s, grades}});
		std::cout << s << " added to assignments\n";
}

void classroom::delete_assignment(std::string s) {
	//remove grade from course totals
	for (auto i: this->student_grades) {
		i.second -= this->assignments[s][i.first];
	}
	this->assignments.erase(s);
}

//name of assignement, student id, grade of assignment
void classroom::add_grade(std::string &s, int id, int grade) {
	std::cout << "assignment " + s + "!\n";
	this->assignments[s][id] = grade;
	this->student_grades[id] += grade;
}

void classroom::print_assignments() {
	for (auto i: this->get_assignments()) {
		std::cout << i.first << std::endl;
		for (auto j: i.second) {
			std::cout << "\t" << j.first << " : " << j.second << std::endl;
		}
	}
}