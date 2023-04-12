#ifndef PARSING
#define PARSING

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#define ELEM 21

using namespace std;

typedef struct s_location
{
	std::string location_path;
	std::map<string, std::list<string> > location_map;
	std::list<std::string>& getElemetnBylocation(const std::string& element)
	{
		return location_map[element];
	}
}t_location;
typedef struct s_server
{
	std::vector <std::string> each_server;
	std::map<std::string, std::list<std::string> > server_map;
	std::map<int, std::string>  error_page;
	std::vector <t_location> locations;
} t_server;



class ParsConf
{
	private :
		int count_server;
		int closed;
		std::vector <std::string> vec;
	public :
	int count_location;
	std::vector <t_server> servers;
	ParsConf():count_server(0), count_location(0){}
	void countserver(char *av);
	void fill_server();
	void fill_server_element();
	void check_value(std::string &value);
	void check_duplcates(std::vector<std::string> &duplicate, int check);
	void check_element(std::vector<std::string> &vec);
	int check_validelem1(std::string str);
	int check_validelem(std::string str);
	std::list <std::string> split_string(std::string str);
	void brackets_errors();
	std::vector<t_server>& getServer();
	// std::map<std::string, std::list<std::string> >& GetLocationmap(std::map<std::string, std::list<std::string> >::iterator& it);
	std::vector<t_location>::iterator getAll_locations(std::vector <t_server>::iterator it);
	void exec_cgi();
};
#endif