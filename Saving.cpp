#include "header_stack.h"
#include "Teacher.h"
#include <sys/stat.h>   //mkdir() AND stat()
#include <sys/types.h>   //mkdir()

//save classroom objects to file
void SaveCourses(std::vector<classroom> courses){
	std::string filepath = "Data Files/Courses.txt";
	//if no folder for a given course exists, make one
	for (auto i: courses) {
		std::string folderpath = "Data Files/" + std::to_string(i.get_course_num()) + "/";
		struct stat st;

		if (stat(folderpath.c_str(), &st) == 0) {
			std::cout << "FOLDER EXISTS\n";
		} else {												//try to make folder
			if (mkdir(folderpath.c_str(), 0777) < 0) {				//creation failed
				std::cout << "CREATION OF FOLDER FAILED\n";
			} else {											//created directory, now create files to hold data
				std::cout << "FOLDER CREATED\n";
				std::string assignments_tmp = folderpath + "Assignments.txt";
				std::string roster_tmp = folderpath + "Roster.txt";
				std::ofstream { assignments_tmp };							
				std::ofstream { roster_tmp };
			}
		}
	}

    std::ofstream fout;															//output file stream
	fout.open(filepath, std::fstream::out | std::fstream::trunc);				//open file for writing, overwrite existing data

	if(fout){ 																	//if creation was successful
  		for(int i=0;i < courses.size();i++){
        	fout<<courses[i].get_course_name() <<'\t'<< courses[i].get_course_num()<<'\t'<< courses[i].get_instructor() << std::endl;
    	}
  	} else {
		std::cout << "FILE FAILED TO OPEN\n";
	}
	std::cout << "courses saved\n";
	fout.close();
}

//read course information into file from data structure
void OpenCourses(std::vector<classroom> &courses){ 
  	std::ifstream fin;															
	std::string StringTemp;
	fin.open("Data Files/Courses.txt");

	if (fin) {
		while (std::getline(fin, StringTemp)) {
    		
			//tokenize string 
			std::stringstream stream(StringTemp);
			std::vector<std::string> tokens;
			std::string str;
			while(getline(stream, str, '\t')) {
				tokens.push_back(str);
			}
    		courses.push_back(classroom(std::stoi(tokens[1]), tokens[0], std::stoi(tokens[2])));			//create course
		}
	} else {
		std::cout << "FILE FAILED TO OPEN\n";
	}
	fin.close();
	std::cout << courses.size() << " COURSES READ\n";
}

//read roster for course from file into data structure
void OpenRoster(std::vector<int> *roster, classroom *course) {
	std::string path = "Data Files/" + std::to_string(course->get_course_num()) + "/Roster.txt";

	std::ifstream fin;
	fin.open(path, std::fstream::in);
	int id;

	if (fin) {																//file exists
		while (fin >> id) {
			roster->push_back(id);											//add to roster
			course->student_grades.insert(std::make_pair(id, 0));			//create overall course grades for each student
		}
	} else {
		std::cout << "FILE FAILED TO OPEN\n";
	}
	fin.close();
	std::cout << roster->size() << " sutdents READ\n";
}

//save roster for course
void SaveRoster(std::vector<int> &roster, classroom &course) {
	std::string path = "Data Files/" + std::to_string(course.get_course_num()) + "/Roster.txt";
	std::ofstream fout;
	fout.open(path, std::fstream::out | std::fstream::trunc);							//open file for writing, overwrite existing data

	if (fout) {																			//file exists
		for (auto i: roster) {
			fout << std::to_string(i) << std::endl;	
		}
	} else {
		std::cout << "FILE NOT OPENED\n";
  	}
	fout.close();
	std::cout << "Roster saved!\n";
}

//read assignments for course from file into data structure
void OpenAssignments(classroom *course, std::vector<int> roster) {
	std::string path = "Data Files/" + std::to_string(course->get_course_num()) + "/Assignments.txt";
	std::ifstream fin;
	std::string str;
	fin.open(path, std::fstream::in);
	
	if (fin) {
		int index = 0;
		std::string assignment_name;
		std::unordered_map<int, int> grades;

		while (std::getline(fin, str)) {								
			if (index == 0) {				//line is name of assignment
				assignment_name = str;
				course->create_assignment(assignment_name, roster);		//add to course object
				index++;
			} else if ((index > 0) && (index <= roster.size())) {			//line is touple of id and grade
				//tokenize string 
				std::stringstream stream(str);
				std::vector<std::string> tokens;
				std::string str;
				while(getline(stream, str, '\t')) {
					tokens.push_back(str);
				}

				grades.insert(std::make_pair(std::stoi(tokens[0]), std::stoi(tokens[1])));		//adds tokens to grades		
				index++;		
			} 
			
			if (index > roster.size()) {														//add grades and reset values to prepare for next assignment
				for (auto i: grades) {
					course->student_grades[i.first] += i.second;								//increase student's course grade
				}
				course->assignments[assignment_name] = grades;									//add grades to course object
				index = 0;
				grades.clear();
				assignment_name = "";
			}
		}
	} else {
		std::cout << "FILE NOT OPENED\n";
	}
	std::cout << std::to_string(course->assignments.size()) << " assignments added!\n";
}

//save assignments to file
void SaveAssignments(classroom *course) {
	std::string path = "Data Files/" + std::to_string(course->get_course_num()) + "/Assignments.txt";
	std::ofstream fout;
	std::string str;
	fout.open(path, std::fstream::out);
	
	if (fout) {
		int index = 0;

		for (auto i: course->assignments) {
			fout << i.first << std::endl;
			for (auto j: i.second) {
				fout << std::to_string(j.first) << "\t" << std::to_string(j.second) << std::endl;
			}
		}
	} else {
		std::cout << "FILE NOT OPENED\n";
	}
	std::cout << std::to_string(course->assignments.size()) << " assignments added!\n";
}

//saves login credentials to file
void SaveLogins(std::unordered_map<int, std::string> credentials) {
	std::ofstream fout;
	
	fout.open("Data Files/Logins.txt", std::fstream::out | std::fstream::trunc);	//open file for writing, overwrite existing data
  	if(fout){ 		//if creation was successful

		//write contents of map to file
  		std::for_each(credentials.begin(), credentials.end(),[&fout](std::pair<int, std::string> user) {
			fout << std::to_string(user.first) << "\t" << user.second << std:: endl;
		});
  	} else {  		//file was not opened
		std::cout << "FILE NOT OPENED\n";
  	}
	fout.close();
}

//read login credentials from file into data structure
void OpenLogins(std::unordered_map<int, std::string> &credentials) {
	std::ifstream fin("Data Files/Logins.txt"); //input file stream
	int id;
	std::string StringTemp, pass;

	if (fin) {
		while (std::getline(fin, StringTemp)) {
    		
				//tokenize string 
				std::stringstream stream(StringTemp);
				std::vector<std::string> tokens;
				std::string str;
				while(getline(stream, str, '\t')) {
					tokens.push_back(str);
				}

				id = std::stoi(tokens[0]);
				pass = tokens[1];

				//add to logins
				credentials.insert(std::make_pair(id, pass));
		}
	} else {
		std::cout << "FILE FAILED TO OPEN\n";
	}
	fin.close();
	std::cout << credentials.size() << " logins READ\n";
}

//save student objects to file
void SaveStudents(std::unordered_map<int, student> students) {
	std::ofstream fout;
	
	fout.open("Data Files/Students.txt", std::fstream::out | std::fstream::trunc);	//open file for writing, overwrite existing data
  	if(fout){ 				//if creation was successful

		//write contents of map to file
  		std::for_each(students.begin(), students.end(),[&fout](std::pair<int, student> user) {
			fout << user.second.get_first_name() << "\t" << user.second.get_last_name() << "\t" << user.first << std::endl;
		});
  	} else {  //file was not opened
		std::cout << "FILE NOT OPENED\n";
  	}
	fout.close();
}

//read student information from file into data structure
void OpenStudents(std::unordered_map<int, student> &students) {
	std::ifstream fin("Data Files/Students.txt"); 		//input file stream
	std::string StringTemp;

	if (fin) {
		while (std::getline(fin, StringTemp)) {
    		
				//tokenize string 
				std::stringstream stream(StringTemp);
				std::vector<std::string> tokens;
				std::string str;
				while(getline(stream, str, '\t')) {
					tokens.push_back(str);
				}
				//fill object to add to map
				student tmp = student(tokens[0], tokens[1], std::stoi(tokens[2]));

				//add to &students
				students.insert(std::make_pair(std::stoi(tokens[2]), tmp));
		}
	} else {
		std::cout << "FILE FAILED TO OPEN\n";
	}
	fin.close();
	std::cout << students.size() << " students READ\n";
}