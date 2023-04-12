/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 22:12:50 by mamellal          #+#    #+#             */
/*   Updated: 2023/04/11 01:51:05 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

void ParsConf::countserver(char *av)
{
	fstream file;
	std::string str;
	file.open(av);
	if(!file.good())
	{
		std::cout << "file does not exists"<< std::endl;
		exit (0);
	}
	while(getline(file, str))
	{    
		if(str == "server")
			count_server++;
		vec.push_back(str);
	}
	if(!count_server || vec[0] != "server"){
		std::cout << "There is no server"<< std::endl;
		exit (0);
		}
	brackets_errors();
}
void ParsConf::fill_server()
{
	t_server tmp_server;
	for(unsigned int j = 0; j < vec.size() ; j++)
	{
		if(vec[j] == "};")
		{
			servers.push_back(tmp_server);
			tmp_server.each_server.clear();
		}
		tmp_server.each_server.push_back(vec[j]);
	}
	fill_server_element();
}
void ParsConf::fill_server_element()
{
	std::vector<std::string> duplicate;
	std::vector<std::string> duplocation;
	std::string arr[4] = {"host", "listen", "max_client_body_size", "server_name"};
	std::string loc_arr[7] = {"allow_methods", "autoindex", "root", "upload_pass", "return", "cgi_pass", "index"};
	t_location tmp_location;
	for (unsigned int i = 0; i < servers.size() ; i++)
	{
		count_location = 0;
		closed = 0;
		for(unsigned int j = 0;j < servers[i].each_server.size() ;j++)
		{
			std::list <std::string> mylist = split_string(servers[i].each_server[j]);
			std::list<std::string>::iterator it;
			int index = check_validelem(mylist.front());
			if(index != -1)
			{
				if(index != 3 && mylist.size() != 2)
				{
					std::cout << "value of " << arr[index] << " not found or there is a lot of values"<<std::endl;
					exit(0);
				}
				if(index == 3 && mylist.size() < 2)
				{
					std::cout << "value of " << arr[index] << " not found"<<std::endl;
					exit(0);
				}
				duplicate.push_back(mylist.front());
				mylist.pop_front();
				if(index != 3)
					check_value(mylist.front());
				servers[i].server_map.insert(std::pair<std::string, std::list<string> >(arr[index], mylist));
			}
			else if(mylist.front() == "error_page")
			{
				if(mylist.size() != 3)
				{
					std::cout << "value of " <<arr[index] << " not found or there is a lot of values"<<std::endl;
					exit(0);
				}
					mylist.pop_front();
					duplicate.push_back(mylist.front());
					std::list<std::string>::iterator it = mylist.begin();
					it++;
					servers[i].error_page.insert(std::pair<int, std::string>(atoi(mylist.front().c_str()), *it));
			}
			else if(mylist.front() == "location")
			{
				if(mylist.size() != 2)
				{
					std::cout << "value of location not found or there is a lot of values"<<std::endl;
					exit(0);
				}
				count_location += 1;
				duplicate.push_back(mylist.front());
				mylist.pop_front();
				tmp_location.location_path = mylist.front();
				duplicate.push_back(tmp_location.location_path);
			}
			if(mylist.front() == "},")
			{
				closed += 1;
				check_duplcates(duplocation, 1);
				duplocation.clear();
				servers[i].locations.push_back(tmp_location);
				tmp_location.location_map.clear();
				tmp_location.location_path.clear();
			}
			int _index = check_validelem1(mylist.front());
			if(_index != -1)
			{
				duplocation.push_back(mylist.front());
				tmp_location.location_map.insert(std::pair<std::string, std::list<string> >(loc_arr[_index], mylist));
			}
		}
		check_duplcates(duplicate, 0);
		duplicate.clear();
		if(closed != count_location)
		{
			std::cout << "locations : syntax error" << std::endl;
			exit (0);
		}
	}
}
void ParsConf::check_value(std::string &value)
{
	for(unsigned int i = 0; i < value.size(); i++)
	{
		if(!isdigit(value[i]) && value[i] != '.')
		{
			std::cout << "value format is incorrect" << std::endl;
			exit (0);
		}
	}
}
void ParsConf::check_duplcates(std::vector<std::string> &duplicate, int check)
{
	for (std::vector<std::string>::size_type i = 0; i < duplicate.size(); ++i) {
	    for (std::vector<std::string>::size_type j = i + 1; j < duplicate.size(); ++j) {
	        if (duplicate[i] == duplicate[j] && duplicate[i] != "location") {
	            std::cout << "there is duplicate" << std::endl;
				exit(0);
	        }
	    }
	}
	if(!check)
		check_element(duplicate);
	else if(check)
	{
		std::vector<std::string>::iterator it;
		std::vector<std::string>::iterator it1;
		it = find(duplicate.begin(), duplicate.end(), "root");
		it1 = find(duplicate.begin(), duplicate.end(), "index");
		if(it == duplicate.end())
			throw std::out_of_range("there is no root in your location");
		if(it1 == duplicate.end())
			throw std::out_of_range("there is no index in your location");
		
	}
}

void ParsConf::check_element(std::vector<std::string> &vec)
{
	std::string arr[5] = {"host", "listen", "max_client_body_size", "server_name", "location"};
	int i = -1;
	while(++i < 5)
	{
		if (std::find(vec.begin(), vec.end(), arr[i]) == vec.end()){
			std::cout << "your element in your file not enough"<< std::endl;
			exit (0);
		}
	}
}
int ParsConf::check_validelem1(std::string str)
{
	std::string arr[7] = {"allow_methods", "autoindex", "root", "upload_pass", "return", "cgi_pass", "index"};
	int i = -1;
	while(++i < 7)
	{
		if(arr[i] == str)
			return (i);
	}
	return (-1);
}

int ParsConf::check_validelem(std::string str)
{
	std::string arr[4] = {"host", "listen", "max_client_body_size", "server_name"};
	unsigned int i = 0;
	while(i < arr->size())
	{
		if(arr[i] == str)
			return (i);
		i++;
	}
	return (-1);
}

std::list <std::string> ParsConf::split_string(std::string str)
{
	std::string buffer;
	std::list <std::string> mylist;
	for(unsigned int i = 0; i < str.size(); i++)
	{
		while(isspace(str[i]))
			i++;
		while(!isspace(str[i]) && str[i])
			buffer += str[i++];
		if(buffer != "{" && buffer != "\n")
		{
			if(buffer.size())
			{
				mylist.push_back(buffer);
				buffer.clear();
			}
		}
	}
	return (mylist);
}
void ParsConf::brackets_errors()
{
	unsigned int i = 0;
	int open = 0;
	while(i < vec.size())
	{
		if(vec[i] == "{" && !open)
			open = 1;
		else if(vec[i] == "{" && open)
		{
			std::cout << "there is problem in brackets "<< std::endl;
			exit (0);
		}
		if(vec[i] == "};" && open)
			open = 0;
		else if(vec[i] == "};" && !open)
		{
			std::cout <<"there is problem in brackets"<< std::endl;
			exit (0);
		}
		i++;
	}
}

void ParsConf::exec_cgi()
{
	char *env[2];
	// env[0] = "";
	int fd = open("lolo", O_CREAT | O_RDWR | O_TRUNC, 0777);
	char *arg[3];

	arg[0] = strdup("/Users/mamellal/Desktop/webs/php-cgi");
	arg[1] = strdup("/Users/mamellal/Desktop/webs/s.php");
	arg[2] = NULL;
	char *PATH_INFO = strdup(arg[0]);

	pid_t f = fork();

	if(f == 0)
	{
		dup2(fd, 1);
		close(fd);
		execve(arg[0], arg, NULL);
	}
	wait(NULL);
}

std::vector<t_location>::iterator ParsConf::getAll_locations(std::vector <t_server>::iterator it)
{
	return it->locations.begin();
}