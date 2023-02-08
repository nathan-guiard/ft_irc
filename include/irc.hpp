/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:40 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/08 16:04:11 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	FT_IRC
#define	FT_IRC

#include <iostream>
#include <string>
#include <iosfwd>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <set>
#include <vector>
#include <algorithm>


#include "numeric_replies.hpp"

#define LOCALHOST       "127.0.0.1"
#define MAX_CONNECTIONS	16
#define	READ_SIZE		512
#define	PASSWORD		"JesuisBelle_42"

#define PRINT_LOG		"\033[3;1;31m"
#define PRINT_RESET		"\033[0m"

using namespace std;

class User;
class Channel;

typedef map<int, User *>		user_map;
typedef map<string, Channel *>	channel_map;

typedef struct con_data {
	int					fd_epoll;
	int					fd_socket;
	struct sockaddr_in	soc_addr;
	struct epoll_event	event_socket;
	struct epoll_event	events[MAX_CONNECTIONS];
}	con_data;

#include "Channel.hpp"
#include "User.hpp"

extern int			g_fd_epoll;
extern int			g_fd_socket;
extern vector<int>	g_open_fd;
extern user_map		g_users;
extern channel_map	g_channels;

/*	Connections	*/
bool				new_connection(int fd_epoll, int fd_socket);
bool				deconnection(con_data &data, int fd);
con_data			init_connection_data(int port);
vector<string>		get_command(int fd_user, int user_fd);
void				check_dead_channels();

/*	User		*/
int					fd_to_id(int fd);
User				*fd_to_user(int fd);
int					nick_to_id(string nick);
string				my_itoa(int nb);

/*	Parsing		*/
vector<string>		command_parsing(const string &str);

/*	Commands	*/
void	exec_commands(int user_id, int user_fd,
						const string &password, con_data &data);

/*	Pour itoa	*/
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm;
        stm << n ;
        return stm.str();
    }
}

/*ERROR HANDLING*/
void    closeFdsExit(const char *message, int errCode);
void	closeAndFreeAll(const char *message, int errCode);

#endif