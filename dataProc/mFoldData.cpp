#include<iostream>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <string>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/linear_congruential.hpp>
using namespace boost;
using namespace std;
typedef unordered_map<string, unordered_map<string, int>> schemaMap;
void loadSchema(char *filename, schemaMap &schema){
	//FILE *file = fopen(filename, "r");
	string line;
	vector<string> strs;
	ifstream fin(filename);
	while (getline(fin, line)){
		split(strs, line, is_any_of("\t"));
		int cnt = atoi(strs[1].c_str());
		for (int i = 0; i < cnt; i++){
			schema[strs[0]][strs[2 + i]] = i;
		}
	}
	cout << "size of schema " << schema.size() << endl;
}
string getMatched(const char *head, regex expression, string aaa){ 
	cmatch what;
	if (regex_match(head, what, expression) && what.size() >= 1){
		return string(what[1]);
	}
	//else
		//cout << aaa << " "<<head << endl;
	return "";
}
string getmid(string head){
	regex expression("<http://(?:[a-zA-Z_.0-9]+/)+m[.]([0-9a-z_]+)>");
	
	return getMatched(head.c_str(), expression, "mid");
}
string getrel(string rel){
	regex expression("<http://(?:[a-zA-Z_.0-9]+/)+([a-zA-Z_.0-9]+)>");
	return getMatched(rel.c_str(), expression, "rel");
}
void genMFoldData(int i, const vector<vector<string> > &vv, vector<vector<string> > &vv2, vector<string>& v){
	int sz = vv.size();
	if (i == sz) {
		vv2.push_back(v); 
		return;
	}
	for (int j = 0; j < vv[i].size(); j++){
		v.push_back(vv[i][j]);
		genMFoldData(i + 1, vv, vv2, v);
		v.pop_back();
	}
	return;
}
void writeData(FILE *out, string cvt, vector<vector<string> > &vv){
	for (int i = 0; i < vv.size(); i++){
		fprintf(out, "%s", cvt.c_str());
		for (int j = 0; j < vv[i].size(); j++){
			fprintf(out, "\t%s", vv[i][j].c_str());
		}
		fprintf(out, "\n");
	}
}
void sampleDataFromCvts(string cvt_path, string output, schemaMap &schema, boost::minstd_rand &rng, const string mediator_mid,int num){
	//FILE *file = fopen(cvt_path.c_str(), "r");
	FILE *out = fopen(output.c_str(), "w");
	string line;
	vector<string> strs;
	ifstream fin(cvt_path);
	int len = schema[mediator_mid].size();
	uniform_01<boost::minstd_rand> pick(rng);
	while (getline(fin, line)){
		split(strs, line, is_any_of("\t"));
		string cvt = strs[0];
		if (pick() > 1000.0 / num) continue;
		
		vector<vector<string> > vv(len, vector<string>());
		//cout << "mediator mid: "<<mediator_mid << " " << schema[mediator_mid].size() << endl;
		//cout << "vv size: " << vv.size() << endl;
		for (int i = 1; i +3 <strs.size(); i += 4){
			string head = getmid(strs[i]);
			if (head != cvt ||  head=="") continue;
			string rel = getrel(strs[i + 1]);
			if (rel == "") continue;
			string tail = getmid(strs[i + 2]);
			if (tail == "")continue;
			if (schema[mediator_mid].count(rel)>0)
				vv[schema[mediator_mid][rel]].push_back(tail);
		}

		/*cout << "----------------" << endl;
		for (int k = 0; k < vv.size(); k++){
			for (int l = 0; l < vv[k].size(); l++)
				cout << vv[k][l] << " ";
			cout << endl;
		}*/
		//cout << endl;
		vector <string> v;
		vector<vector<string> > vv2;
		genMFoldData(0, vv, vv2, v);
		
		if (!vv2.empty())
			writeData(out, cvt, vv2);
	}
	fclose(out);
}
void randomPickData(char *mediator_list_path, string cvt_superdir,string outdir, schemaMap &schema){
	//FILE *file = fopen(mediator_list_path, "r");
	string line;
	vector<string> strs;
	ifstream fin(mediator_list_path);
	boost::minstd_rand rng(time(0));
	while (getline(fin, line)){
		split(strs, line, is_any_of("\t"));
		int cnt = atoi(strs[2].c_str());
		string mid = strs[0];
		string id = strs[1].substr(1);
		if (cnt >= 10000){
			sampleDataFromCvts(cvt_superdir + "/" + id+".cvt", outdir + "/" + id+".cvt", schema, rng, mid,cnt);
		}
	}
}
int main(int argc, char **argv){
	char *mediator_list_path = argv[1];
	string cvt_superdir = string(argv[2]);
	string outdir = string(argv[3]);
	char *schema_path = argv[4];
	schemaMap schema;
	loadSchema(schema_path, schema);
	randomPickData(mediator_list_path, cvt_superdir, outdir, schema);
}