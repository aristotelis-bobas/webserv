/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Proxy.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/06 18:32:50 by abobas        #+#    #+#                 */
/*   Updated: 2020/10/23 19:20:36 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Proxy.hpp"

// debugging
#include <iostream>

static std::string lineTerminator = "\r\n";

Proxy::Proxy(Data &data) : data(data)
{
	setPath();
	if (createProxySocket())
		return;
	if (setProxyAddress())
		return;
	if (connectProxySocket())
		return;
	sendProxyRequest();
	receiveProxyResponse();
	sendProxyResponse();
}

void Proxy::setPath()
{
	if (data.path.size() == 0)
		data.path = "/";
}

int Proxy::createProxySocket()
{
	int new_socket;
	int enable = 1;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket == -1)
	{
		data.response.sendInternalError("socket() failed");
		return 1;
	}
	setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	proxy_socket = Socket("proxy", new_socket);
	return 0;
}

int Proxy::setProxyAddress()
{
	size_t pos = data.location["proxy_pass"].string_value().find(':');
	if (pos == std::string::npos)
		proxy_addr.sin_port = htons(80);
	else
	{
		proxy_addr.sin_port = htons(std::stoi(data.location["proxy_pass"].string_value().substr(pos + 1, std::string::npos)));
		std::cout << "proxy_addr.sin_port = " << std::stoi(data.location["proxy_pass"].string_value().substr(pos + 1, std::string::npos)) << std::endl;
	}
	host = data.location["proxy_pass"].string_value().substr(0, pos);
	std::cout << "HOST: " << host << std::endl;
	if ((inet_pton(AF_INET, host.c_str(), &proxy_addr.sin_addr)) <= 0)
	{
		data.response.sendInternalError("inet_pton() failed");
		return 1;
	}
	return 0;
}

int Proxy::connectProxySocket()
{
	if (connect(proxy_socket.getSocket(), reinterpret_cast<sockaddr *>(&this->proxy_addr), sizeof(proxy_addr)) < 0)
	{
		data.response.sendInternalError("connect() failed");
		return 1;
	}
	return 0;
}

void Proxy::sendProxyRequest()
{
	std::ostringstream oss;
	std::map<std::string, std::string> headers = data.request.getHeaders();

	oss << data.request.getMethod() << ' ' << data.path << ' ' << data.request.getVersion() << lineTerminator;
	oss << "host: " << host << lineTerminator;
	oss << "connection: close" << lineTerminator;
	for (auto header : headers)
	{
		if (header.first != "host" && header.first != "connection")
			oss << header.first << ": " << header.second << lineTerminator;
	}
	oss << lineTerminator;
	proxy_socket.sendData(oss.str());
}

void Proxy::receiveProxyResponse()
{
	raw_response = proxy_socket.receive();
}

void Proxy::sendProxyResponse()
{
	data.response.sendDataRaw(raw_response);
}