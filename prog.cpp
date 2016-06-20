#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main()
{
	string str, command;
	vector<string> commands;
	getline(cin,str);
	command = "";
	for(int i = 0;;i++)
	{
		if(str[i] == '\0')
		{
			commands.push_back(command);
			command = "";
			break;
		}
		if(str[i] != '|')
			command += str[i];
		else
		{
			commands.push_back(command);
			command = "";
			i++;
		}
	}
	int fd[2];
	for(int i = 0; i < commands.size(); i++)
	{
		if(i == 0)
		{
			pipe(fd);
			close(STDOUT_FILENO);
			dup2(fd[1],STDOUT_FILENO);	
			if(!fork())
			{
				system(commands[i].c_str());
				return 0;
			}
			else
				continue;
		}
		else if(i+1 != commands.size())
		{
			close(STDIN_FILENO);
			dup2(fd[0],STDIN_FILENO);
			close(fd[0]);
			close(fd[1]);
			pipe(fd);
			close(STDOUT_FILENO);
			dup2(fd[1],STDOUT_FILENO);	
			if(!fork())
			{
				system(commands[i].c_str());
				return 0;
			}
			else
				continue;
		}
		else
		{
			close(STDIN_FILENO);
			dup2(fd[0],STDIN_FILENO);
			close(fd[0]);
			close(fd[1]);
			close(STDOUT_FILENO);
			int out = open("/home/box/result.out", O_RDWR | O_CREAT, 0666);
			dup2(out,STDOUT_FILENO);	
			system(commands[i].c_str());
		}
	}
}
