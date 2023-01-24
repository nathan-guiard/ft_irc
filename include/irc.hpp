/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:40 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 13:28:01 by nguiard          ###   ########.fr       */
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

using namespace std;
class Client;

#define MAX_CONNECTIONS	16

typedef	map<int, Client>	client_map;

typedef struct con_data {
	int fd_epoll;
	int	fd_socket;
	struct sockaddr_in	soc_addr;
	struct epoll_event	event_socket;
	struct epoll_event	events[MAX_CONNECTIONS];
}	con_data;

/*	Connections	*/
bool		new_connection(int fd_epoll, int fd_socket);
bool		deconnection(con_data &data, int fd);
con_data	init_connection_data(int port);

#endif