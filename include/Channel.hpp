/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:16:05 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 18:11:16 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_IRC
#define CHANNEL_IRC

#include "irc.hpp"

class Channel {
public:
	Channel();
	Channel(string name, int id);
	Channel(const Channel &copy);
	~Channel();

	Channel &operator = (const Channel &copy);

	string	get_name()	const;

	bool	add_user(int id);
	bool	put_op(int caller, int target);
	bool	sudo_put_op(int target);

private:
	string		_name;
	set<int>	_operators;
	set<int>	_users;
};

#endif