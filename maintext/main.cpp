// maintext.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <armadillo>
#include <stack>
#include <map>
#include <fstream>
#include <sstream>
#include<io.h>

using namespace arma;
using namespace std;

#include<iostream>
#include<algorithm>
#include<stack>
#include<stdlib.h>
#include<vector>
#include<string>


using namespace std;
string format(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '-') {
			if (i == 0) {
				str.insert(0, 1, '0');
			}
			else if (str[i - 1] == '(') {
				str.insert(i, 1, '0');
			}
		}
		if (str[i] >= 'a' && str[i] <= 'z') {
			throw - 1;
		}
		if (str[i] >= 'A' && str[i] <= 'Z') {
			throw - 1;
		}
	}
	return str;
}
int prior(char c) {
	if (c == '+' || c == '-') {
		return 1;
	}
	else if (c == '*' || c == '/') {
		return 2;
	}
	else {
		return 0;
	}
}


template <class T, class T1>
void print(map<T, T1> m) {
	for (auto i = m.begin(); i != m.end(); i++) {
		cout << i->first << ":" << i->second << endl;
	}
	cout << endl;
}
vector<string> hz(string str) {
	vector<string> vs;
	stack<char> st;
	for (int i = 0; i < str.length(); i++) {
		string tmp = "";
		switch (str[i]) {
		case '+':
		case '-':
		case '*':
		case '/':
			if (st.empty() || st.top() == '(') {
				st.push(str[i]);
			}
			else {
				while (!st.empty() && prior(st.top()) >= prior(str[i])) {
					tmp += st.top();
					vs.push_back(tmp);
					st.pop();
					tmp = "";
				}
				st.push(str[i]);
			}
			break;
		case '(':
			st.push(str[i]);
			break;
		case ')':
			while (st.top() != '(') {
				tmp += st.top();
				vs.push_back(tmp);
				st.pop();
				tmp = "";
			}
			st.pop();
			break;
		default:
			if ((str[i] >= '0' && str[i] <= '9')) {
				tmp += str[i];
				while (i + 1 < str.size() && str[i + 1] >= '0' && str[i + 1] <= '9' || str[i + 1] == '.')
				{
					tmp += str[i + 1];//若是连续数字
					++i;
				}
				vs.push_back(tmp);
			}
		}
	}
	while (!st.empty()) {
		string tmp = "";
		tmp += st.top();
		vs.push_back(tmp);
		st.pop();
	}
	return vs;
}
double result(vector<string> bh) {
	stack<double> st;
	double num, op1, op2;
	for (int i = 0; i < bh.size(); i++) {
		string tmp = bh[i];
		if (tmp[0] >= '0' && tmp[0] <= '9') {
			num = atof(tmp.c_str());
			st.push(num);
		}
		else if (bh[i] == "+")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 + op2);
		}
		else if (bh[i] == "-")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 - op2);
		}
		else if (bh[i] == "*")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 * op2);
		}
		else if (bh[i] == "/")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 / op2);
		}
	}
	return st.top();
}
double solve(string str) {
	str = format(str);
	vector<string> bh = hz(str);
	double k = result(bh);
	return k;
}

vector<string> Stringsplit(string str, const char split)
{
	vector<string> res;
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	while (getline(iss, token, split))	// 以split为分隔符
	{
		if (token != "" && token != " ") {
			res.push_back(token);
		}
	}
	return res;
}

map<string, double> init_Parameters(string path) {
	map<string, string> first_read;
	map<string, double> res;
	vector<string> count;
	ifstream in(path+"\\input_Parameters.inp");
	string line;
	if (in) // 有该文件
	{
		getline(in, line);
		while (getline(in, line))
		{
			//cout << line << endl;
			vector<string> s = Stringsplit(line, ' ');
			s[1] = Stringsplit(s[1], '[')[0];
			first_read[s[0]] = s[1];
			//cout << s[0] << "\t" << s[1] << endl;
			try {
				//cout <<  << endl;
				res[s[0]] = stod(s[1]);
			}
			catch (exception e) {
				//e.what();
				count.push_back(s[0]);
			}
		}
	}
	else // 没有该文件
	{
		cout << "无法打开" << path << "\\input_Parameters.inp" << endl;
		return res;
	}
	map<string,string> no_sovle;
	for (int i = count.size() - 1; i >= 0; i--) {
		string need_count = first_read[count[i]];
		//cout << need_count << endl;
		map<string, double>::iterator it = res.begin();
		while (it != res.end())
		{
			//cout << it->first << " " << it->second << endl;

			int where = need_count.find(it->first);
			//cout << where;
			while (where != -1) {
				need_count.replace(where, it->first.size(), to_string(it->second));
				where = need_count.find(it->first);
			}
			it++;
		}
		//cout << need_count << endl << endl;
		try {
			res[count[i]] = solve(need_count);
		}
		catch (int s) {
			no_sovle[count[i]] = need_count;
		}

	}
	int time = no_sovle.size()*2+1,little=0;
	while (1)
	{
		
		for (auto i = no_sovle.begin(); i != no_sovle.end();i++) {
			string need_count = i->second;
			map<string, double>::iterator it = res.begin();
			while (it != res.end())
			{
				//cout << it->first << " " << it->second << endl;

				int where = need_count.find(it->first);
				//cout << where;
				while (where != -1) {
					need_count.replace(where, it->first.size(), to_string(it->second));
					where = need_count.find(it->first);
				}
				it++;
			}
			try {
				res[i->first] = solve(need_count);
			}
			catch (int s) {
				cout << need_count << endl;
				break;
			}
			no_sovle.erase(i);
		}
		if (no_sovle.empty()) {
			break;
		}
		if (little>time) {
			cout << "迭代次数过多 请检查是否有为定义的变量 无法计算的表达式如下" << endl;
			for (auto i = no_sovle.begin(); i != no_sovle.end(); i++) {
				cout << i->first << "\t" << i->second << endl;
			}
			break;
		}
		little++;
	}
	return res;
}
string& trim(string& s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

int split(string s) {
	int res = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ',') {
			res++;
		}
	}
	return res;
}
class Matt {
	vector<double> val;
	int x, y;
public:
	Matt() {
		val = {};
		x = -1;
		y = -1;
	}
	Matt(vector<double> v, int x1, int y1) {
		this->val = v;
		this->x = x1;
		this->y = y1;
	}
	/*科学计数法显示*/
	void show(string t) {
		int n;
		cout << t << endl;
		for (int i = 0; i < this->val.size(); i++) {
			printf("\t%e", this->val[i]);
			if ((i + 1) % this->x == 0) {
				cout << endl;
			}
		}
		cout << endl;
	}
	/*小数位数显示*/
	void showbynum(string t) {
		int n;
		cout << "输入要显示的小数点位数";
		cin >> n;
		string format = "\t%.";
		format = format + to_string(n);
		format = format + "lf\t";
		cout << t << endl;
		for (int i = 0; i < this->val.size(); i++) {
			printf(format.c_str(), this->val[i]);
			if ((i + 1) % this->x == 0) {
				cout << endl;
			}
		}
		cout << endl;
	}
	void showlittle(string t) {
		cout << t << endl;
		cout << "x:" << x << " y:" << y << endl;
	}
};
string limit(string s) {
	string res = "";
	for (char i : s) {
		if (i != ' ') {
			res += i;
		}
	}
	return res;
}
vector<string> split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
map<string, string> init_cfg(string path,map<string,Matt>& mtm) {

	map<string, string> res;
	ifstream in(path + "\\Configure.cfg");
	string line, value;
	if (!in) {
		cout << "无法打开" << path << "\\Configure.cfg" << endl;
		return res;
	}
	while (getline(in, line)) {
		getline(in, value);
		line = trim(line);
		value = trim(value);
		vector<string> r = split(value,",");
		vector<double> n;
		try {
			for (string i : r) {
				//cout << i << endl;
				n.push_back(stod(i));
			}
			//mtm[line] = 
			Matt m(n,n.size(),1);
			mtm[line] = m;
		}
		catch (exception s) {
			n.clear();
			try {
				vector<string> r1 = split(value, " ");
				for (string i1 : r1) {
					//cout << i1 << endl;
					n.push_back(stod(i1));
				}
				Matt m(n, n.size(), 1);
				mtm[line] = m;
			}
			catch (exception s) {
				res[line] = value;
			}
			
		}
		//res[trim(line)] = trim(value);
	}
	return res;
}




map<string, Matt> init_mat(string path) {
	vector<string> files = { "Cell_ocv","EL_CONDUCTIVITY",
		"int28_k_neg","int29_k_pos",
		"int2_interpolation2_dEeqdT_int1",
		"int31_ds_pos","int32_ds_neg",
		"int33_Ea_neg","int34_Ea_pos",
		"int36_dEeqdT_pos",
		"int37_Ea_dspos",
		"int38_Ea_dsneg",
		"int39_Du_a_pos",
		"int40_Du_a_neg",
		"int53_Graphite_Eeq_int1",
		"int8_NMC_Eeq_int1"};
	map<string, Matt> res;
	for (int i = 0; i < files.size();i++) {
		ifstream unuse_in(path + "\\"+files[i] + ".dat");
		if (!unuse_in) {
			cout << "无法打开" << path + "\\" + files[i] + ".dat" << endl;
			continue;
		}
		string line;
		getline(unuse_in,line);
		stringstream ss;
		ss << line; 
		double num;
		int l = 0,flag = 0;
		while (ss>>num) {
			l++;
		}
		if (l==1) {
			l = split(line)+1;
			if (l!=1) {
				flag = 1;
			}
		}
		unuse_in.close();
		//cout << l <<endl;
		ifstream in(path + "\\" + files[i] + ".dat");
		if (!in) {
			cout << "无法打开" << path + "\\" + files[i] + ".dat" << endl;
			continue;
		}
		int t = 0;
		vector<double> n;
		if (flag == 0) {
			while (in >> num) {
				n.push_back(num);
			}
		}
		else {
			//getline();
			string line;
			while (getline(in,line)) {
				//line.sp
				vector<string> t = split(line,",");
				for (string i : t) {
					n.push_back(stod(i));
				}
			}
		}
		
		Matt m(n,l,n.size()/l);
		res[files[i]] = m;
		//m.show(files[i]+":");
	}
	return res;
}
Matt init_RATE(string path) {
	ifstream unuse_in(path + "\\"  + "input_RATE_cal.inp");
	string line;
	getline(unuse_in, line);
	getline(unuse_in, line);
	stringstream ss;
	ss << line;
	double num;
	int l = 0, flag = 0;
	while (ss >> num) {
		l++;
	}
	if (l == 1) {
		l = split(line) + 1;
		if (l != 1) {
			flag = 1;
		}
	}
	unuse_in.close();
	//cout << l <<endl;
	ifstream in(path + "\\" + "input_RATE_cal.inp");
	if (!in) {
		cout << "无法打开" << path + "\\" + "input_RATE_cal.inp" << endl;
	}
	int t = 0;
	vector<double> n;
	getline(in, line);
	if (flag == 0) {
		while (in >> num) {
			n.push_back(num);
		}
	}
	else { 
		while (getline(in, line)) {
			//line.sp
			vector<string> t = split(line, ",");
			for (string i : t) {
				n.push_back(stod(i));
			}
		}
	}

	Matt m(n, l, n.size() / l);
	return m;
}

int main()
{
	/*vx : zjxyyds0307*/
	string path = "E:\\c++计算\\input";
	string str;
	map<string, double> param = init_Parameters(path);
	
	map<string, Matt> matmap = init_mat(path);
	map<string, string> cfg = init_cfg(path,matmap);
	matmap["input_RATE_cal"] = init_RATE(path);
	cout << "初始化完毕" << endl;
	cout << "请输入变量名:";
	while (1) {
		//cin >> str;
		//cin.clear();
		//
		getline(cin,str);
		//cout << "--" << str << "--" << endl;
		if (str == "" || str == " ") {
			cout << "请输入变量名:";
			continue;
		}
		int flag = 0;
		if (param.find(str)==param.end()) {
			flag--;
		}
		else {
			cout << str<<":" << param[str] << endl;
		}

		if (cfg.find(str) == cfg.end()) {
			flag--;
		}
		else {
			cout << str << ":" << cfg[str] << endl;
		}

		if (matmap.find(str) == matmap.end()) {
			flag--;
		}
		else {
			matmap[str].show(str);
		}

		if (flag==-3) {
			cout << "此变量值不存在 将显示模糊搜索结果" << endl;
			int t = 0;
			for (auto i = param.begin(); i != param.end(); i++) {
				if (i->first.find(str) != -1) {
					cout << i->first << ":" << i->second << endl;
					t++;
				}
			}
			for (auto i = cfg.begin(); i != cfg.end(); i++) {
				if (i->first.find(str) != -1) {
					cout << i->first << ":\t" << i->second << endl;
					t++;
				}
			}
			for (auto i = matmap.begin(); i != matmap.end(); i++) {
				if (i->first.find(str) != -1) {
					i->second.showlittle(i->first);
					t++;
				}
			}
			cout << "共查询出" << t << "项结果" << endl;
		}
		cout << "请输入变量名:";
	}
}

