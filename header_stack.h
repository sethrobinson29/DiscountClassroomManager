/*
	Collection of includes and package structure defintion for Classroom Management Application
*/

#ifndef HEADER_STACK_H_
#define HEADER_STACK_H_

#include <iostream>
#include <fstream>
#include <sstream>			//stringstream
#include <bits/stdc++.h>	//stringstream
#include <cctype>
#include <unordered_map>
#include <iterator>			//iterator
#include <algorithm>		//for_each
#include <vector>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string.h>			//memset()
#include <string>			//to_string()

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#define SERV_PORT 5342      //server port
#define SA struct sockaddr  //socket address structure
#define LISTENQ 1024        //bind() backlog
#define MAXLINE 4096
#define BUF_SIZE 8192

//structure to hold data that will be passed across a socket
struct package {
  int code, id, courseNumber, index;
  char message[MAXLINE];
  char firstName[50], lastName[50], password[50];
};

#endif