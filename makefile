all: CMA-server CMA-client Teacher.o Classroom.o Assignment.o Student.o

CC = g++

CMA-server: server.o Teacher.h 
	$(CC) -o CMA-server server.o Teacher.o Classroom.o Assignment.o Student.o

CMA-client: client.o 
	$(CC) -o CMA-client client.o

server.o: server.cpp server_interface.cpp Saving.cpp header_stack.h
	$(CC) -c server.cpp server_interface.cpp Saving.cpp header_stack.h

client.o: client.cpp client_interface.cpp
	$(CC) -c client.cpp client_interface.cpp

Classroom.o: Classroom.h Classroom.cpp header_stack.h
	$(CC) -c Classroom.h Classroom.cpp header_stack.h

Student.o: Student.h Student.cpp header_stack.h
	$(CC) -c Student.h Student.cpp header_stack.h

Teacher.o: Teacher.h Teacher.cpp header_stack.h
	$(CC) -c Teacher.h Teacher.cpp header_stack.h

Assignment.o: Assignment.h Assignment.cpp header_stack.h
	$(CC) -c Assignment.h Assignment.cpp header_stack.h