/*
	Class to hold information about courses
*/

#ifndef Classroom_h
#define Classroom_h

#include "header_stack.h"
#include "Assignment.h"
#include "Student.h"

class classroom{
private:
		std::string course_name;
		int course_num, instructor;
		std::vector<student> roster;
		
public:
	std::unordered_map<std::string, std::unordered_map<int, int>> assignments;        //assignment name is key, list of students and grades is value
	std::unordered_map<int, int> student_grades;                            	//stores overall grade; student name is key, grade is value
	classroom(){
		this->course_name = "Network Programming";
		this->course_num = 3013;
	}
	classroom(int num, std::string name, int profID) {
		this->course_num = num; this->course_name = name; 
		this->instructor = profID;
	}
		
	void set_course_name(std::string);
	std::string get_course_name();
	int get_course_num() { return course_num; }
	int get_instructor();
	void set_instructor(int);

	std::unordered_map<std::string, std::unordered_map<int, int>> get_assignments() { return this->assignments; }		
	std::vector<student> get_roster() { return this->roster; }
		
	void print_roster();
	void create_roster();
	void add_to_roster();
	void remove_from_roster(int);
	void print_assignments();
	void create_assignment(std::string&, std::vector<int>);
	void delete_assignment(std::string);

	void add_grade(std::string&, int, int);
	void calc_student_average();
};

#endif 
