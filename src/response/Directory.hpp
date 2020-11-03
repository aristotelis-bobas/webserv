/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Directory.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/28 20:37:12 by abobas        #+#    #+#                 */
/*   Updated: 2020/10/28 20:44:19 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Data.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/**
* @brief Handles directory listings and index files.
*/
class Directory
{
public:
	Directory(Data &data);

private:
	Data data;

	void handleDirListing();
	void writeDirTitle(std::string &data);
	void writeDirFile(std::string &data, std::string &&file);
	void handleDirIndex();
};