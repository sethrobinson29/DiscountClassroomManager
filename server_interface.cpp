#include "Teacher.h"
#include "Saving.cpp"

int login_attempts = 0;			
int cur_user = 0;									//keep track of userID currently logged in
int cur_course = 0;									//keep track of current classroom being edited
int courses_index = 0;								//keep index of current classroom
std::vector<int> cur_roster; 						//hold ids of students for current classroom
std::vector<std::string> cur_assignments; 			//hold names of assignments for current classroom
std::string cur_assignment;							//hold name of assignment for grade_loop

std::vector<classroom> courses;						//list of all courses
std::unordered_map<int, std::string> logins;		//map of userID and password all users with login credentials (CANNOT BE ACCESSED BY MENU)
std::unordered_map<int, student> all_students;		//map of studentIDs and student objects that hold student information

void check_login(int, package&);					//verifies user's login credentials agains logins
void exit_to_login(package&);						//returns user to login menu
void fill_structures_from_file();					//fills courses, logins, and all_students with data read from the files by calling functions from Saving.cpp

//admin menu functions
void create_teacher(package&);
void delete_teacher(package&);
void create_student(package&);							
void delete_student(package&);							
void print_teachers(std::vector<teacher> teachers);		

//teacher menu functions
void edit_classroom(package&);														//access classroom menu
void create_classroom(package&);
void remove_classroom(package&);

//classroom menu functions
void fill_classroom_data_structures(std::vector<int>&, classroom&);					//fills this->assignments and this->student grades from file, as well as current structures
void add_student(package&);
void edit_assignment(package&);														//change grade of single student for given assignment
void create_assignment(package&);
void add_grade(package&);															//looping function for create_assignment()
void delete_assignment(package&);
void print_roster(package&);
void print_grades(package&);
void print_course_grades(package&);
void prev_menu(package&);															//reset current values and returns to teacher menu


/*
	Server-side logic for Classroom Management Application
	sockfd is a socket descriptor
*/
int server_interface(int sockfd) {
  package pkg;
  ssize_t n, err;
  std::string recv_buf, send_buf, temp, first, last;
  bool end = false;

  fill_structures_from_file();

  again:
  while (!end) {
	if (n = read(sockfd, &pkg, sizeof(pkg)) > 0) {
    switch(pkg.code) {
    	case 11: {
        	check_login(sockfd, pkg);										//validate user's login credentials
    	} break;
		case 66: {															//end application
			std::cout << "SHUTDOWN CODE RECEIVED\n";
			pkg.code = 94;
			end = true;
		} break;
	  	case 101: {
			  create_teacher(pkg);
		} break;
		case 111: {
			delete_teacher(pkg);
		} break;
		case 121: {
			create_student(pkg);
		} break;
		case 131: {
			delete_student(pkg);
		} break;
		case 141: { 
			exit_to_login(pkg);
		} break;
		case 201: {
			edit_classroom(pkg);
		} break;
		case 211: {
			create_classroom(pkg);
		} break;
		case 221: {
			remove_classroom(pkg);
		} break;
	    case 301: {
	      add_student(pkg);
	    } break;
		case 311: {
			edit_assignment(pkg);
		} break;
		case 321: {
			create_assignment(pkg);
		} break;
		case 322: {
			add_grade(pkg);
		} break;
		case 331: {
			delete_assignment(pkg);
		} break;
		case 341: {
			print_roster(pkg);
		} break;
		case 351: {
			print_grades(pkg);
		} break;
		case 361: {
			prev_menu(pkg);
		} break;
		case 371: {
			print_course_grades(pkg);
		} break;
    }

    write(sockfd, &pkg, sizeof(pkg));
    memset(pkg.message, '\0', MAXLINE);				//clear message
  	
	} else if (n < 0 && err == EINTR) {
		goto again;
	} else if (n < 0){
	   	fprintf(stderr, "server_interface: read error");
		exit(1);
   	}
  }

  return 0;
}

//print list of teachers
void print_teachers(package &pkg, std::vector<teacher> teachers) {
	std::string str;
	for (auto i: teachers) {
		str += i.get_first_name() + "\n";
	}
	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
}

/*
	Validate user's login credentials
*/
void check_login(int sockfd, package& pkg) {
  std::string pass = pkg.message, str;
  int userID = pkg.id;

    std::cout <<"USERID: " << pkg.id << " Password: " << pass << std::endl;
	if (login_attempts < 5) {
		login_attempts++; 

    	if (userID > 10000 && userID < 20000) {  //admin user
        	if (pass.compare(logins[pkg.id]) == 0) {   //valid admin (pass.compare(logins[pkg.id]) == 0)
          		std::cout << "\nadmin_menu()\n";

          		//create data to send back to user
          		str = "ADMIN\n";                  

          		pkg.code = 92;
          		pkg.id = 0;
				cur_user = userID;
        	} else {          //invalid admin
        		str = logins[userID] + " " + std::to_string(userID) + " INVALID ADMIN LOGIN ATTEMPT\n";
          		pkg.code = 91;                                                 
          		pkg.id = 0;
        	}
    	} else if (userID > 20000 && userID < 30000) {  //teacher user
        	std::cout << "\nteacher_menu()\n";

        	if (pass.compare(logins[userID]) != 0){
          		str = "INVALID PASSWORD\n";
          		pkg.code = 91;
        	} else {
          		str = "WELCOME\n";
				cur_user = userID;	
          		pkg.code = 93;  												//gives access to teacher menu
        	} 
        
    	} else if (userID > 30000 && userID < 40000) {  //student user
        	//TODO: create student login menu
    	} else {
			str = "USERID NOT FOUND\n";
			pkg.code = 91;
    	} 
	} else {
		str = "NUMBER LOGIN ATTEMPTS EXCEEDED: closing connection\n";
		pkg.code = 555;
	}

  	//copy result to pkg to be passed back to client
	memset(pkg.message, '\0', MAXLINE);
  	str.copy(pkg.message, str.length());

  //close connection if login attempts have been exceeded
  if (pkg.code == 555) {
	shutdown(sockfd, SHUT_RDWR);
  }
}

//returns to login menu
void exit_to_login(package &pkg) {
	std::string tmp = "return to login\n";
	pkg = package();

	tmp.copy(pkg.message, tmp.length());
	pkg.code = 91;
}

//creates teacher object from members of pkg and adds to prof_vector and instructors
void create_teacher(package &pkg) {
	std::string str;
	logins.insert( {pkg.id, pkg.password});
	SaveLogins(logins);

	str = "INSTRUCTOR CREATED\n";
	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
	pkg.code = 92;
}

//remove teacher from data structures TODO
void delete_teacher(package &pkg) {
	std::string str;
	
	if (logins.erase(pkg.id) == 1) {					//matching id was found
		str = std::to_string(pkg.id) + " successfully removed\n";
		SaveLogins(logins);
	} else {
		str = "Failed to find matching instructor\n";
	}

	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
	pkg.code = 92;
}

//inserts new student into global list
void create_student(package &pkg) {

	student tmp = student(pkg.firstName, pkg.lastName, pkg.id);
	all_students.insert(std::make_pair(tmp.get_student_id(), tmp));
	SaveStudents(all_students);

	pkg.code = 92;
}

//removes student from all associated lists 
void delete_student(package &pkg) {
	std::string str;
	
	if (all_students.erase(pkg.id) == 1) {
	for (int i = 0; i < cur_roster.size(); i++) {
		if (cur_roster[i] == pkg.id) {
			cur_roster.erase(cur_roster.begin() + i);
			break;
		}
	}
	} else {
		str = "Failed to find matching student\n";
	}

	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
	pkg.code = 92;
	SaveStudents(all_students);

}

/*
	Enter classroom menu and set variables to reflect current classroom 
*/
void edit_classroom(package &pkg) {
	std::string str;

	for (int i = 0; i < courses.size(); i ++) {
		if ((pkg.courseNumber == courses[i].get_course_num()) && (courses[i].get_instructor() == cur_user)) {			//course exists, proceed to classroom menu
			pkg.code = 95;	
			str = "Course found\n";

			//set current variables and fill data structures
			fill_classroom_data_structures(cur_roster, courses[i]);
			cur_course = pkg.courseNumber;														
			courses_index = i;
			break;
		}
	}
	//if no course exists, return to teacher menu
	if (pkg.code != 95) {
		pkg.code = 93;
		str = "INVALID COURSE NUM\n";
	}

	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
}

//adds classroom object to courses and saves to Data File
void create_classroom(package &pkg) {
  courses.push_back(classroom(pkg.courseNumber, pkg.message, cur_user));
  SaveCourses(courses);

  std::cout << "Course created!\n";
  pkg.code = 93;
}

//remove course from data structures
void remove_classroom(package &pkg) {
	std::vector<classroom>::iterator pos;
	std::string str;

	for (int i = 0; i < courses.size(); i++) {
		if (courses[i].get_course_num() == pkg.id) {
			  pos = courses.begin() + i;
		}
	}

	if (pos != courses.end()) {					//matching id was found
		courses.erase(pos);
		//TODO: SaveFiles()
		str = std::to_string(pkg.id) + " successfully removed\n";
	} else {
		str = "Failed to find matching course\n";
	}

	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
	pkg.code = 93;
}
/*
	Adds student IDs to corresonding course roster file
*/
void add_student(package& pkg) {
	std::string str;

	cur_roster.push_back(pkg.id);
	str = "cur_roster updated for " + std::to_string(cur_course) +"\n";
	std::cout << str;
	pkg.code = 95;
	SaveRoster(cur_roster, courses[courses_index]);

	memset(pkg.message, '\0', MAXLINE);	
	str.copy(pkg.message, str.length());
}

//uses pkg.courseNumber for grade value
void edit_assignment(package& pkg){

	for (auto i: courses[courses_index].assignments) {
		if ((i.first.compare(pkg.message)) == 0) {
			int tmp = i.second[pkg.id];
			i.second[pkg.id] = pkg.courseNumber;									//set grade for assignment
			courses[courses_index].student_grades.at(pkg.id) -= (tmp - pkg.courseNumber);	//change course grade by difference in grades
		}
		SaveAssignments(&courses[courses_index]);
	}
	pkg.code = 95;
}

//pkg.message will keep data for add_grade(pkg)
//creates assigment and adds to lists, begins loop for entering grades
void create_assignment(package& pkg){
		//code to enter ID gathering loop
		pkg.code = 322;
		std::string tmp = pkg.message;
		cur_assignment = tmp;
		courses[courses_index].create_assignment(tmp, cur_roster);
		cur_assignments.push_back(pkg.message);
		//sets id to start
		pkg.index = 0;
		pkg.id = cur_roster[0];
	
}
//adds grades to assignment, checks if more grades to add, returns to teacher menu if not
void add_grade(package& pkg) {
	int x;
	//add student grade to current assignment, which is held by pkg.courseNumber
	courses[courses_index].add_grade(cur_assignment, pkg.id, pkg.courseNumber);
	pkg.index++;

	//if entire roster has not had grade entered, repeat loop, otherwise, return to classroom menu
	if (pkg.index < cur_roster.size()) {
		pkg.id = cur_roster[pkg.index];
		x = 322;
	} else {
		SaveAssignments(&courses[courses_index]);
		x = 95;
		cur_assignment = "";
	}
	pkg.code = x;
}

//remove assignment
void delete_assignment(package& pkg){
	std::string str;
	for (int i = 0; i < cur_assignments.size(); i++) {
		if (cur_assignments[i] == pkg.message) {
			cur_assignments.erase(cur_assignments.begin() + i);
			courses[courses_index].delete_assignment(pkg.message);
			str = "Assignment deleted\n";
		}
	}

	if (str.size() == 0) {
		str = "ASSIGNMENT NOT FOUND\n";
	}

	pkg.code = 95;
	memset(pkg.message, '\0', MAXLINE);
	str.copy(pkg.message, str.length());
}

//fill pkg.message with list of students
void print_roster(package& pkg){
	std::string str;
	for (int i: cur_roster) {
		if (all_students.find(i) != all_students.end()) {		//is valid key 
			str += std::to_string(all_students[i].get_student_id()) + " " + all_students[i].get_first_name() + " " + all_students[i].get_last_name() + "\n";
		}
	}

	pkg.code = 95;
	memset(pkg.message, '\0', MAXLINE);
	str.copy(pkg.message, str.length());
}

//prints grades for an assignmnet
void print_grades(package& pkg){
	std::string str;
	
	for (auto j: courses[courses_index].get_assignments()) {
		if (j.first.compare(pkg.message)) {
			for (auto k: j.second) {
				str += std::to_string(k.first) + "\t" + std::to_string(k.second) + "\n";
			}
			pkg.code = 95;
			break;
			}
	}

	if (pkg.code != 95) {
		str = "ASSIGNMENT NOT FOUND\n";
		pkg.code = 95;
	}
			
	memset(pkg.message, '\0', MAXLINE);
	str.copy(pkg.message, str.length());
}

//print overall course averages for students
void print_course_grades(package &pkg) {
	std::string str;
	int num_assignments = courses[courses_index].assignments.size();
	std::cout << "NUM ASSIGNMENTS: " << std::to_string(num_assignments) << std::endl;
	for (auto i: courses[courses_index].student_grades) {
		int avg_grade = i.second / num_assignments;
		str += std::to_string(i.first) + "\t" + std::to_string(avg_grade) + "\n";
	}
	str += "\n";
	memset(pkg.message, '\0', MAXLINE);
	str.copy(pkg.message, str.length());
	pkg.code = 95;
}

//clear cur values and return to teacher menu
void prev_menu(package& pkg){
	pkg.code = 93;
	cur_course = 0;
	courses_index = 999;
	cur_roster.clear();
	cur_assignments.clear();
}

//reads data from files into various file structures
void fill_structures_from_file() {
	OpenCourses(courses);
	OpenLogins(logins);
	OpenStudents(all_students);
	std::cout <<"FILLED\n";
}

//fills course data structures when classroom is accessed
void fill_classroom_data_structures(std::vector<int> &roster, classroom &course) {
	OpenRoster(&roster, &course);
	OpenAssignments(&course, cur_roster);

}