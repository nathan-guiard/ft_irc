/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:00:21 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/26 17:07:53 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static string	read_connection_data(int fd_user);

/**
 * @brief 	get every command that the user sended.
 * 			stocks the command if no \n were given
 * 			(like get_next_line)
 * 
 * @param	fd_user fd of the given user	
 * @return	Every command that the user sent. Empty if none.
 */
vector<string>	get_command(int fd_user) {
	static string	command;
	string			line;
	vector<string>	res;
	bool			has_no_newline;
	size_t			index = string::npos;
	bool			whole_string_splitted = false;

	command.append(read_connection_data(fd_user));
	has_no_newline = command.find('\n') == string::npos;
	
	if (has_no_newline)
		return res;

	while (!whole_string_splitted) {
		bool should_not_push_back;
		command = string(command, index + 1);
		index = command.find('\n');
		line = string(command, 0, index);

		should_not_push_back = line.empty() || line.size() == 0
								|| command.find('\n') == string::npos;
		if (should_not_push_back)
			break;
		res.push_back(line);
		whole_string_splitted = line.empty() || line.size() == 0;
	}

	return res;
}

static string	read_connection_data(int fd_user) {
	char				buff[READ_SIZE + 1];
	int					bytes_read;
	string				res;	

	
	bzero(buff, READ_SIZE + 1);
	bytes_read = read(fd_user, buff, READ_SIZE);
	buff[bytes_read] = 0;
	while (bytes_read > 0) {
		res.append(buff);
		if (bytes_read < READ_SIZE)
			break;
		bytes_read = read(fd_user, buff, READ_SIZE);
		buff[bytes_read] = 0;
	}
	return res;
}