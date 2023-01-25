/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connections.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 13:17:09 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/25 15:45:02 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool	deconnection(con_data &data, int fd, user_map *users) {
	epoll_ctl(data.fd_epoll, EPOLL_CTL_DEL, fd, &data.event_socket);
	close(fd);

	int		id_of_disconnected = fd_to_id(*users, fd);
	User	disconnected_User;
	try { disconnected_User = users->at(id_of_disconnected); }
	catch (std::out_of_range) {
		cerr << "User identified by fd " << fd << " is not findable" << endl;
		return false;
	}

	users->erase(id_of_disconnected);
	cout << "User " << disconnected_User.get_id();
	cout << " disconnected." << endl;
	return true;
}

bool	new_connection(int fd_epoll, int fd_socket, user_map *users) {
	int 				fd_new_con;
	struct sockaddr_in	s_new_con;
	socklen_t			size_s_new_con = sizeof s_new_con;
	struct epoll_event	event_new_con;
	static int			id = 1;

	fd_new_con = accept(fd_socket,
		(sockaddr *) &s_new_con, &size_s_new_con);
	if (fd_new_con < 0) {
		perror("accept()");
		// check d'autres trucs
		exit(1);
	}

	User	new_User(id, fd_new_con);
	
	//recuperer son nom son nickname et 
	event_new_con.events = EPOLLIN | EPOLLRDHUP;
	event_new_con.data.fd = fd_new_con;
	epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_new_con, &event_new_con);
	users->insert(make_pair(id, new_User));
	id++;
	new_User.send_to(1, "Bienvenue sur le serveur :).");
	return true;
}

con_data	init_connection_data(int port) {
	con_data data;
	data.fd_epoll = epoll_create1(0); // A besoin d'etre close
	if (data.fd_epoll < 0) {
		cerr << "epoll_create1(): Error." << endl;
		exit(1);
	}
	data.fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (data.fd_socket < 0) {
		cerr << "socket(): Error." << endl;
		close(data.fd_epoll);
		exit(1);
	}

	data.soc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	data.soc_addr.sin_family = AF_INET;
	data.soc_addr.sin_port = htons(port);
	
	bind(data.fd_socket, (sockaddr *) &data.soc_addr, sizeof data.soc_addr);

	data.event_socket.events = EPOLLIN;
	data.event_socket.data.fd = data.fd_socket;

	if (epoll_ctl(data.fd_epoll, EPOLL_CTL_ADD, data.fd_socket, &data.event_socket)) {
		cerr << "epoll_ctl(): Error." << endl;
		close(data.fd_epoll);
		close(data.fd_socket);
		exit(1);
	}
	listen(data.fd_socket, MAX_CONNECTIONS);
	//marque la socket référencée par sockfd comme passive socket
    //c'est-à-dire en tant que socket qui sera utilisé pour accepter
	//requêtes de connexion en utilisant accept(2) 
	return data;
}