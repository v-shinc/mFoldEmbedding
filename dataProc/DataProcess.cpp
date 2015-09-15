#include<iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <time.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
using namespace boost;
using namespace std;
string getmid(const char *head){
	regex expression("<http://(?:[a-zA-Z_.0-9]+/)+m[.]([0-9a-z_]+)>");
	cmatch what;
	if (regex_match(head, what, expression) && what.size() >= 1){
		return string(what[1]);
	}
	else
		cout << head << endl;
	return "";
}
string replace(const char *tail){
	regex expression("\"([^\"]+)\"");
	cmatch what;
	if (regex_match(tail, what, expression) && what.size()>=1){
		return replace_all_copy(string(what[1]), "/", ".");
	}
	return "";
}
int main(int argc, char** argv){
	unordered_map<string, string> mid2id;
	int n;
	FILE *file, *outfile;
	char *mid2id_input, *mediator_input, *output;
	char head[500], rel[500], tail[1000], dot[10];
	vector<string> strs;
	mid2id_input = argv[1];
	mediator_input = argv[2];
	output = argv[3];
	file = fopen(mid2id_input, "r");
	ifstream fin(mid2id_input);
	string line;
	while (getline(fin, line)){
		split(strs, line, is_any_of("\t"));
		mid2id[getmid(strs[0].c_str())] = replace(strs[2].c_str());
	}
	fclose(file);
	outfile = fopen(output, "w");
	file = fopen(mediator_input, "r");
	while (fscanf(file, "%s\t%d", head, &n) != EOF){
		if (mid2id.count(head) == 0) cout << head << endl;
		else fprintf(outfile, "%s\t%s\t%d\n", head, mid2id[head].c_str(), n);
	}
	fclose(outfile);
}
