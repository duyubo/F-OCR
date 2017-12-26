/*
* The University of Science and Technology Software Design 



*/


#pragma once

#define N1 1//integration etc.
#define N4 4//division (not include minus)
#define N2 2//radical
#define N0 0//other chars

namespace Logical
{
	struct tree {// record the char around the char.
		int order;
		int tree_up;
		int tree_right_up;
		int tree_right_down;
		int tree_down;
		int next;

	};


	//save inf of char while recognize the char 
	struct inf {// information of the char

		int order;
		int w;
		int l;
		int x;
		int y;
		int char_kind;

		inf() {};
		inf(int order, int w, int l, int x, int y)
			:order(order), w(w), l(l), x(x), y(y)
		{
			char_kind = 0;
		};

		bool operator<(const inf &a) {//sort with top and leftest
			if (((x - w / 2.0) < (a.x - a.w / 2.0)) || (((x - w / 2.0) == (a.x - a.w / 2.0)) && ((y - l / 2.0) < (a.y - a.l / 2.0))))
				return true;
			else
				return false;
		};
		bool operator==(const inf &a) {
			if (order == a.order&&x == a.x&&y == a.y)
				return true;
			else
				return false;
		};


	};
	void change(std::vector<inf> &set, std::string &result);
	bool load_file();



	void change_char(std::vector<inf> &set_initiation);
	void search(inf cha, std::vector<inf> &set, std::vector<tree> &tree_set);
	void initiation_struct(tree &tree_zhongjian);
	void change_string(int p,const std::vector<tree> &tree_set, std::string &result,const std::vector<std::string> &table);

}