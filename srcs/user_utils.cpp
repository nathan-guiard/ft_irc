/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:32:30 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:42:12 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**
 * @brief	Finds the ID of the User with the FD
 * 
 * @details Since we have a map of type <ID, User>,
 * 			having a function that return the ID when we
 * 			only have the FD can be really helpfull to
 * 			find the needed User.
 * 
 * @example	users[fd_to_id(fd)] -> the User related to
 * 			the fd.
 * 
 * @param users	user_map of the all the users
 * @param fd 		The FD to search the User
 * @return	 		The matching ID of the FD
 */
int	fd_to_id(const user_map &users, int fd) {
	user_map::const_iterator	it = users.begin();
	user_map::const_iterator	ite = users.end();

	for (; it != ite; it++) {
		User current_User((*it).second);

		if (current_User.get_fd() == fd) {
			return current_User.get_id();
		}
	}
	return -1;
}