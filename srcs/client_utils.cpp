/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:32:30 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:42:12 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**
 * @brief	Finds the ID of the client with the FD
 * 
 * @details Since we have a map of type <ID, Client>,
 * 			having a function that return the ID when we
 * 			only have the FD can be really helpfull to
 * 			find the needed client.
 * 
 * @example	clients[fd_to_id(fd)] -> the client related to
 * 			the fd.
 * 
 * @param clients	client_map of the all the clients
 * @param fd 		The FD to search the client
 * @return	 		The matching ID of the FD
 */
int	fd_to_id(const client_map &clients, int fd) {
	client_map::const_iterator	it = clients.begin();
	client_map::const_iterator	ite = clients.end();

	for (; it != ite; it++) {
		Client current_client((*it).second);

		if (current_client.get_fd() == fd) {
			return current_client.get_id();
		}
	}
	return -1;
}