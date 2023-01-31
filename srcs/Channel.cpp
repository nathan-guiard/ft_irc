/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:42:47 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/31 16:12:37 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() { cerr << "USING THE WRONG CHANNEL CONSTRUCTOR" << endl; }

Channel::Channel(string name): _name(name) {}

Channel::Channel(const Channel &copy) {
	*this = copy;
}

Channel	&Channel::operator = (const Channel &copy) {
	this->_name = copy._name;
	return *this;
}

Channel::~Channel() {
	cout << "Deleting channel " << _name << endl;
}

string	Channel::get_name()	const {
	return _name;
}

bool	Channel::add_user(User *new_user, bool is_op) {
	user_set::iterator		it = _users.begin();
	user_set::iterator		ite = _users.end();
	pair<User *, bool>		user = make_pair(new_user, is_op);

	if (_users.find(user) == ite) {
		_users.insert(user);
		for (; it != ite; it++) {
			(*it).first->send_to(JOIN(user.first->get_nick(), user.first->get_user(),
							string("localhost"), _name));
		}

		user.first->send_to(RPL_NAMREPLY(user.first->get_nick(),
							user.first->get_user(),
							string("localhost"), _name));
		it = _users.begin();
		ite = _users.end();
		for (; it != ite; it++) {
			user_set::iterator	it2 = it;
			it2++;
			if ((*it).second && it2 == ite) {
				new_user->send_to(string("@") + (*it).first->get_nick()
					+ string("\r\n"));
				
			}
			else if ((*it).second) {
				new_user->send_to(string("@") + (*it).first->get_nick()
					+ string(" "));
			}
		}
		it = _users.begin();
		ite = _users.end();
		for (; it != ite; it++) {
			user_set::iterator	it2 = it;
			it2++;
			if (!(*it).second && it2 == ite) {
				new_user->send_to((*it).first->get_nick() + string("\r\n"));
				
			}
			else if (!(*it).second) {
				new_user->send_to((*it).first->get_nick() + string(" "));
			}
		}
		
		user.first->send_to(RPL_ENDOFNAMES(user.first->get_nick(),
							user.first->get_user(),
							string("localhost"), _name));
		return true;
	}
	return false;
}