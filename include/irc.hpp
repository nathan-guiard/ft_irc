/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:40 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:37:46 by nguiard          ###   ########.fr       */
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

#define MAX_CONNECTIONS	16

using namespace std;

class Client;

typedef map<int, Client>	client_map;

typedef struct con_data {
	int fd_epoll;
	int	fd_socket;
	struct sockaddr_in	soc_addr;
	struct epoll_event	event_socket;
	struct epoll_event	events[MAX_CONNECTIONS];
}	con_data;

#include "Client.hpp"

/*	Connections	*/
bool		new_connection(int fd_epoll, int fd_socket, client_map *clients);
bool		deconnection(con_data &data, int fd, client_map *clients);
con_data	init_connection_data(int port);

/*	Client		*/
int			fd_to_id(const client_map &clients, int fd);

#endif