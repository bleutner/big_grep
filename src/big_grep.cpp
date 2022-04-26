/* big_grep
 * greps large textfiles with multiple patterns based on regex matches 
 *
 * Copyright (C) 2021 Benjamin Leutner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unordered_set>
#include <boost/regex.hpp>

//g++  big_grep.cpp -o big_grep -lboost_regex
std::string VERSION="1.2.1";
void help () {
	std::cerr << "USAGE: big_grep [args] -p <patternfile> -i <inputfile> " << std::endl << std::endl
			<< "DESCRIPTION:" << std::endl
			<< "  Matches every pattern of <patternfile> against every line in <inputfile>." << std::endl
			<< "  This is useful for large numbers of patterns, which exceed the normal 'GNU grep' " << std::endl
			<< "  input limits. Inclusive (default) or exclusive matching modes are possible." << std::endl << std::endl
			<< "ARGUMENTS:" << std::endl
			<< "  -i [filename]: target input file" << std::endl
			<< "  -p [filename]: file with patterns to match" << std::endl
			<< "  -o [filename]: output file (optional)" << std::endl
			<< "  -v: <flag> inverse mode, i.e. exclude matches (optional)" << std::endl
			<< "  -r: <string> perl-style regex which is used to extract a substring from <inputfile>, " << std::endl
			<< "      which is then searched for in <patternfile> (optional)" << std::endl
			<< "  -t: <flag> test regex on first line of <inputfile> and exit" << std::endl << std::endl
			<< "  -h: <flag> print help and exit" << std::endl
			<< "  -V: <flag> print program version" << std::endl << std::endl
			<< "COPYRIGHT:" << std::endl
            << "  Copyright 2021 Benjamin Leutner" << std::endl
            << "  This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl << std::endl;
}

void missing_arg(std::string arg) {
	std::cerr << "ERROR: Missing mandatory argument: " << arg << std::endl;
	exit(1);
}

int doit(const std::string &pfile, const std::string &infile, const std::string &outfile,  std::string &mode, std::string &regex , bool &testRegex) {

	std::ifstream input(infile);
	std::ifstream patterns(pfile);
	std::string line;
	const bool toFile = outfile.length() > 0;
	bool include = mode == "include";

	const bool doregex = regex.length() > 0;
	const boost::regex expr{regex};

	// Output
	std::ofstream os;
	if (toFile) {
		os.open (outfile, std::ios::out);
	}

	// Import pattern from file
	std::unordered_set<std::string> patlist;
	if (patterns.is_open()) {
		while(std::getline(patterns, line))
		{
			patlist.insert(line);
		}
		patterns.close();
	}

	if (testRegex) {
		if (!doregex) {
			std::cerr<< "Regex (-r) must be specified in order to test it" << std::endl;
		}
		std::getline(input, line);
		boost::smatch matches;
		if ( boost::regex_search(line, matches, expr) ) {
			std::cout << matches[0] << std::endl;
			exit(0);
		} else {
			std::cerr << "regex did not match anything" << std::endl;
			std::cerr << "regex was: " << regex << std::endl;
			std::cerr << "matched against: " << line << std::endl;
			exit(1);
		}
	}

	if (input.is_open()) {
		// Read line by line
		while(std::getline(input, line)) {
			std::string key;
			if (doregex) {
				boost::smatch matches;
				if ( boost::regex_search(line, matches, expr) ) {
					key=matches[0];
				}
			} else {
				key=line;
			}
			bool matched = ( patlist.find(key) != patlist.end() );

			// Output
			if (( include && matched ) || ( (!include) && (!matched) )) {
				if (toFile) {
					os << line << std::endl;
				} else {
					std::cout << line << std::endl;
				}
			}

		} // line loop
		input.close();
	}
	if(toFile) {
		os.close();
	}
	return 0;
}

int main(int argc, char* argv[]) {
	std::string mode="include";
	std::string infile, pfile, outfile, regex;
	bool testRegex=false;

	if (argc <= 1){
		std::cerr<< "ERROR: No arguments specified"<<std::endl<<std::endl;
		help();
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];

		if (arg ==  "-v") {
			mode = "exclude";
			continue;
		}
		if (arg ==  "-h") {
			help();
			exit(0);
		}
		if (arg ==  "-i" ) {
			infile=argv[i+1];
			i++;
			continue;
		}
		if (arg ==  "-p") {
			pfile=argv[i+1];
			i++;
			continue;
		}
		if (arg ==  "-o") {
			outfile=argv[i+1];
			i++;
			continue;
		}
		if (arg ==  "-r") {
			regex=argv[i+1];
			i++;
			continue;
		}
		if (arg ==  "-t") {
			testRegex=true;
			continue;
		}
		if (arg ==  "-V") {
			std::cout << "big_grep v" << VERSION << std::endl;
			exit(0);
		}
		std::cerr<< "ERROR: Unknown argument "<<i<< ": "<< argv[i] << std::endl << std::endl;
		help();
		exit(1);
	}

	if (infile.length() == 0 ) {
		missing_arg("-i <inputfile>");
	}

	if (pfile.length() == 0 ) {
		missing_arg("-p <patternfile>");
	}

	doit(pfile, infile, outfile, mode, regex, testRegex) ;
	return 0;
}
