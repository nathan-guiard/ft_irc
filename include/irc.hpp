/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:40 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/21 19:00:47 by nguiard          ###   ########.fr       */
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
#include <set>

using namespace std;
class Client;

#define MAX_CONNECTIONS	16

typedef	set<Client>	client_set;

#endif