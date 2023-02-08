/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connections.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 13:17:09 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/08 16:11:49 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool	deconnection(con_data &data, int fd) {
	epoll_ctl(data.fd_epoll, EPOLL_CTL_DEL, fd, &data.event_socket);
	close(fd);

	int		id_of_disconnected = fd_to_id(fd);
	User	*disconnected_User;
	try { disconnected_User = g_users.at(id_of_disconnected); }
	catch (std::out_of_range) {
		cerr << "User identified by fd " << fd << " is not findable" << endl;
		return false;
	}
	g_users.erase(id_of_disconnected);

	delete disconnected_User;
	
	vector<int>::iterator	it = g_open_fd.begin();
	vector<int>::iterator	ite = g_open_fd.end();
	for (; it != ite; it++) {
		if (*it == fd) {
			g_open_fd.erase(it);
			break;
		}
	}
	
	cout << PRINT_LOG << "User " << id_of_disconnected;
	cout << " disconnected." << PRINT_RESET << endl;
	return true;
}

bool	new_connection(int fd_epoll, int fd_socket) {
	int 				fd_new_con;
	struct sockaddr_in	s_new_con;
	socklen_t			size_s_new_con = sizeof s_new_con;
	struct epoll_event	event_new_con;
	static int			id = 1;

	fd_new_con = accept(fd_socket,
		(sockaddr *) &s_new_con, &size_s_new_con);
	if (fd_new_con < 0)
		closeAndFreeAll("Accept error", 7);

	User	*new_User = new User(id, fd_new_con);
	g_users.insert(make_pair<int, User *>(id, new_User));
	g_open_fd.push_back(fd_new_con);
	event_new_con.events = EPOLLIN | EPOLLRDHUP;
	event_new_con.data.fd = fd_new_con;

	fcntl(fd_new_con, F_SETFL, O_NONBLOCK);

	if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_new_con, &event_new_con) == -1)
		closeAndFreeAll("epoll_ctl error", 7);
	id++;
	
	
	return true;
}

con_data	init_connection_data(int port) {
	con_data data;
	g_fd_epoll = -1;
	g_fd_socket = -1;
	
	data.fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (data.fd_socket < 0)
		closeFdsExit("Socket error", 1);
	g_fd_socket = data.fd_socket;
	data.soc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	data.soc_addr.sin_family = AF_INET;
	data.soc_addr.sin_port = htons(port);
	
	if (bind(data.fd_socket, (sockaddr *) &data.soc_addr,
		sizeof data.soc_addr) == -1)
		closeFdsExit("Bind error", 2);
	if (listen(data.fd_socket, MAX_CONNECTIONS) == -1)
		closeFdsExit("Listen error", 3);
	
	data.fd_epoll = epoll_create1(0); // A besoin d'etre close
	if (data.fd_epoll < 0)
		closeFdsExit("epollcreate1: listen_sock", 4);
	g_fd_epoll = data.fd_epoll;
	data.event_socket.events = EPOLLIN;
	data.event_socket.data.fd = data.fd_socket;

	if (epoll_ctl(data.fd_epoll, EPOLL_CTL_ADD, data.fd_socket, &data.event_socket))
		closeFdsExit("epoll_ctl: listen_sock", 5);
	//marque la socket référencée par sockfd comme passive socket
    //c'est-à-dire en tant que socket qui sera utilisé pour accepter
	//requêtes de connexion en utilisant accept(2) 
	return data;
}

void	check_dead_channels() {
	channel_map::iterator			it = g_channels.begin();
	channel_map::iterator			ite = g_channels.end();
	Channel							*current;
	vector<channel_map::iterator>	vec;

	for (; it != ite; it++) {
		current = (*it).second;
		if (current && current->is_empty()) {
			vec.push_back(it);
		}
	}

	size_t size = vec.size();

	for (size_t i = 0; i < size; i++) {
		current = (*(vec[i])).second;
		g_channels.erase(vec[i]);
		delete current;
	}
}