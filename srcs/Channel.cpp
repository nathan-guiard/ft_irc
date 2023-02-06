/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:42:47 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/06 19:34:52 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() { cerr << "USING THE WRONG CHANNEL CONSTRUCTOR" << endl; }

Channel::Channel(string name): _name(name), _users(), _banned(), _invited(),
							_invite_only(false), _moderated(false), _limit(-1) {
	cout << "Creating channel " << _name << endl;
}

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

	bool is_in_chan = false;

	for (; it != ite; it++) {
		if ((*it).first == new_user)
			is_in_chan = true;
	}
	it = _users.begin();
	ite = _users.end();
	if (!is_in_chan) {
		if (_add_user_verifications(new_user) == false)
			return false;

		_users.insert(user);
		for (; it != ite; it++) {
			(*it).first->send_to(JOIN(user.first->get_nick(), user.first->get_user(),
							string("localhost"), _name));
		}

		it = _users.begin();
		ite = _users.end();
		string sending;
		for (; it != ite; it++) {
			user_set::iterator	it2 = it;
			it2++;
			if ((*it).second && it2 == ite) {
				sending += string("@") + (*it).first->get_nick();
			}
			else if ((*it).second) {
				sending += string("@") + (*it).first->get_nick() + string(" ");
			}
		}
		it = _users.begin();
		ite = _users.end();
		for (; it != ite; it++) {
			user_set::iterator	it2 = it;
			it2++;
			if (!(*it).second && it2 == ite) {
				sending += (*it).first->get_nick();
			}
			else if (!(*it).second) {
				sending += (*it).first->get_nick() + string(" ");
			}
		}
		user.first->send_to(RPL_NAMREPLY(user.first->get_nick(),
							user.first->get_user(),
							string("localhost"), _name) + sending + "\r\n");
		
		user.first->send_to(RPL_ENDOFNAMES(user.first->get_nick(),
							user.first->get_user(),
							string("localhost"), _name));
		return true;
	}
	return false;
}

bool	Channel::broadcast(string const& str, User *excluded)
{
	user_set::iterator it = _users.begin();
	user_set::iterator ite = _users.end();

	for(; it != ite; it++) {
		if ((*it).first != excluded)
			(*it).first->send_to(str);
	}
	return true;
}

bool	Channel::rm_user(User *user) {
	user_set::iterator it = _users.find(make_pair(user, false));
	user_set::iterator ite = _users.end();
	
	if (it == ite) {
		it = _users.find(make_pair(user, true));
	}
	if (it == ite) {
		return false;
	}
	
	_users.erase(it);
	return true;
}


bool	Channel::has_user(User *user) {
	user_set::iterator it = _users.find(make_pair(user, false));
	user_set::iterator ite = _users.end();
	
	if (it == ite) {
		it = _users.find(make_pair(user, true));
	}
	if (it == ite) {
		return false;
	}
	return true;
}

bool	Channel::is_empty() const {
	return _users.empty();
}

bool	Channel::is_op(User *user)	const {
	return (!(_users.find(make_pair(user, true)) == _users.end()));
}

void	Channel::set_invite_only(bool status) {
	_invite_only = status;
}

void	Channel::set_moderated(bool status) {
	_moderated = status;
}

void	Channel::set_limit(size_t limit) {
	_limit = limit;
}

bool	Channel::invite(User *usr) {
	_invited.insert(usr);
	return true;
}

bool	Channel::ban(User *usr) {
	_banned.insert(usr);
	return true;
}

bool	Channel::unban(User *usr) {
	_banned.erase(usr);
	return true;
}

bool	Channel::_add_user_verifications(User *new_user) const {
	if (_limit != (size_t)-1 && _limit >= _users.size()) {
		new_user->send_to(ERR_CHANNELISFULL(new_user->get_nick(), _name));
		return false;
	}
	if (_invite_only) {
		if (_invited.find(new_user) == _invited.end()) {
			new_user->send_to(ERR_INVITEONLYCHAN(new_user->get_nick(), _name));
			return false;
		}
	}
	if (_banned.find(new_user) != _banned.end()) {
		new_user->send_to(ERR_BANNEDFROMCHAN(new_user->get_nick(), _name));
		return false;
	}
	return true;
}