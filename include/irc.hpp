/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:40 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/25 10:55:36 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	FT_IRC
#define	FT_IRC

#include <iostream>
#include <string>
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

#define MAX_CONNECTIONS	16
#define	READ_SIZE		16

using namespace std;

class User;

typedef map<int, User>	user_map;

typedef struct con_data {
	int fd_epoll;
	int	fd_socket;
	struct sockaddr_in	soc_addr;
	struct epoll_event	event_socket;
	struct epoll_event	events[MAX_CONNECTIONS];
}	con_data;

#include "User.hpp"

/*	Connections	*/
bool				new_connection(int fd_epoll, int fd_socket, user_map *users);
bool				deconnection(con_data &data, int fd, user_map *users);
con_data			init_connection_data(int port);
vector<string>		get_command(int fd_user);

/*	User		*/
int					fd_to_id(const user_map &users, int fd);

#endif