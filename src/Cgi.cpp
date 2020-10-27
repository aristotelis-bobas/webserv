/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Cgi.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/17 19:27:46 by abobas        #+#    #+#                 */
/*   Updated: 2020/10/27 02:29:09 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include <iostream>

Cgi::Cgi(Data &data) : data(data)
{
	setEnvironment();
	redirectOutput();
	executeScript();
}

void Cgi::setConfigEnv()
{
	for (auto object : data.config["http"]["cgi"]["cgi_params"].object_items())
	{
		std::string insert;
		insert += object.first;
		insert += '=';
		insert += object.second.string_value();
		memory.push_back(std::move(insert));
		env.push_back(memory.back().c_str());
	}
}

void Cgi::setRequestEnv()
{
	std::string method("REQUEST_METHOD=");
	method += data.request.getMethod();
	memory.push_back(std::move(method));
	env.push_back(memory.back().c_str());

	std::string uri("REQUEST_URI=");
	uri += data.request.getPath();
	memory.push_back(std::move(uri));
	env.push_back(memory.back().c_str());

	std::string query("QUERY_STRING=");
	if (data.request.getQueryString().empty())
		return;
	query += data.request.getQueryString();
	memory.push_back(std::move(query));
	env.push_back(memory.back().c_str());
}

void Cgi::setServerEnv()
{
	std::string name("SERVER_NAME=");
	name += data.server["name"].string_value();
	memory.push_back(std::move(name));
	env.push_back(memory.back().c_str());

	std::string port("SERVER_PORT=");
	port += std::to_string(static_cast<int>(data.server["listen"].number_value()));
	memory.push_back(std::move(port));
	env.push_back(memory.back().c_str());
}

void Cgi::setPathEnv()
{
	std::string script("SCRIPT_NAME=");
	size_t pos = data.path.find_last_of("/");
	if (pos == std::string::npos)
		pos = -1;
	script += data.path.substr(pos + 1);
	memory.push_back(std::move(script));
	env.push_back(memory.back().c_str());
}

void Cgi::setEnvironment()
{
	setConfigEnv();
	setRequestEnv();
	setServerEnv();
	setPathEnv();
	env.push_back(NULL);
	std::cout << "--------ENV---------" << std::endl;
	for (auto n : env)
		std::cout << n << std::endl;
	std::cout << "--------------------" << std::endl;
}

void Cgi::redirectOutput()
{
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe()");
		data.response.sendInternalError();
		return;
	}
	if ((restore_fd = dup(1)) < 0)
	{
		perror("dup()");
		data.response.sendInternalError();
		return;
	}
	if ((dup2(pipe_fd[1], 1)) < 0)
	{
		perror("dup2()");
		data.response.sendInternalError();
		return;
	}
}

void Cgi::resetOutput()
{
	if ((dup2(restore_fd, 1)) < 0)
	{
		perror("dup2()");
		data.response.sendInternalError();
		return;
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void Cgi::executeScript()
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		data.response.sendInternalError();
		return;
	}
	else if (pid == 0)
		childProcess();
	else
		mainProcess();
}

void Cgi::childProcess()
{
	std::vector<char *> argv;
	argv.push_back(const_cast<char *>(data.path.c_str()));
	argv.push_back(NULL);

	if (execve(data.path.c_str(), argv.data(), const_cast<char **>(env.data())) < 0)
	{
		perror("execve()");
		data.response.sendInternalError();
		return;
	}
}

std::string Cgi::convertOutput()
{
	char buf[257];
	std::string buffer;

	while (1)
	{
		int ret = read(pipe_fd[0], buf, 256);
		buf[ret] = '\0';
		buffer += buf;
		if (ret < 256)
			break;
	}
	return buffer;
}

void Cgi::mainProcess()
{
	int status;

	while (1)
	{
		wait(&status);
		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			data.response.addHeader("content-type", "text/html");
			data.response.sendData(convertOutput());
			resetOutput();
			return;
		}
	}
}
