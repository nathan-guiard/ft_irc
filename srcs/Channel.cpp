/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:42:47 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 18:18:00 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() { cerr << "USING THE WRONG CHANNEL CONSTRUCTOR" << endl; }

Channel::Channel(string name, int id): _name(name) {}

Channel::Channel(const Channel &copy) {
	*this = copy;
}

Channel	&Channel::operator = (const Channel &copy) {
	this->_name = copy._name;
	return *this;
}

Channel::~Channel() {}

string	Channel::get_name()	const {
	return _name;
}

bool	Channel::add_user(int id) {
	if (_users.insert(id).second) {
		cout << "User " << id << " added to " << _name << endl;
		return true;
	}
	else
		cout << "User already in " << _name << endl;
	return false;
}

/**
 * @brief	Puts a new operator on the channel.
 * 
 * @param caller	The id of the user that called the OPER
 * @param target	The target oth the OPER call
 * @return true 	The target has been added to the operators
 * @return false	The target has not been added to the operators
 */
/* bool	Channel::put_op(int caller, int target) {
	bool	caller_not_operator = _operators.find(caller) == _operators.end();
	bool	target_not_in_channel = _users.find(target) == _users.end();

	if (caller_not_operator) {
		// Code d'erreur?
		return false;
	}
	if (target_not_in_channel) {
		// Code d'erreur?
		return false;
	}
	_operators.insert(target);
	_users.erase(target);
	return true;
} */


/**
 * @brief			Puts a new operator no matter what
 * 
 * @param target	The User.ID of the new operator
 * @return true		No problems, the operator has been added
 * @return false	A 'Warning' occured
 */
bool	Channel::sudo_put_op(int target) {
	bool	is_already_op = _operators.find(target) != _operators.end();
	bool	is_not_a_current_user = _users.find(target) == _users.end();

	if (is_already_op) {
		cerr << "Warning: user " << target << " is already in operators list." << endl;
	}
	else if (is_not_a_current_user) {
		cerr << "Warning: user " << target << " is not a current user." << endl;
	}
	
}