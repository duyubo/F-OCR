#include<iostream>
#include<list>
#include<vector>
#include<algorithm>
#include<map>
#include"logical_relation_and_scriptchanging.h"
//#define ERROR 0.05
#define Prod 80
#define Add 81
#define Minus 82
#define Less 85
#define Larger 86 
#define Sim 87
#define  Fan_slash 94
#define Vert 93
#define Sqrt 105
#define Int 108
#define Oin 109
#define Sum 110
#define Wide_hat 116
#define Comma 118//,
#define Point 120
#define Rightarrow 130
#define Iff 131
#define Breve  132
#define Black_slash 134
#define Circle 137
#define Equal 144//--
#define Geq 145
#define Leq 146
#define Approx 147
#define Equiv 148//---
#define Percent 150
#define Semicolon 151//;
#define Colon 152//:
#define Lim 153
#define Pm 154
#define Mp 155
#define Div 156
#define Frac 157
#define Over_line 158
#define Under_line 159

#define Wide_tilde 160
#define Dot 161
#define Over_Rightarrow 162
//#define Cdot

namespace Logical
{

	/*
	a-z;A-Z;other chars' normalized size
	*/
	//deal with all the char before this to avoid errors
	std::vector<inf> set;
	std::vector<tree> tree_set;
	std::vector<double> char_size;//its a set of coefficient of the size(height)
	//the tree_set should not be empty

	std::vector<inf> set_initiation;


	std::vector<std::string> table;



	//read file
	inline bool load_file(std::vector<std::string> &table)
	{
		FILE* fp;
        if (fopen_s(&fp,"C:/Users/lenovo/Documents/Visual Studio 2015/Projects/Win32Project1/Win32Project1/char_latex.txt", "r"))
		{///Users/pengshenglin/QtProjects/formulaToLaTex/char_latex.txt
		
			return false;
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			int fsize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			char *buffer = new char[fsize];
			fread(buffer, 1, fsize, fp);
			char *bufferEnd = buffer + fsize;
			fclose(fp);
			char *p = buffer;
			while (p < bufferEnd)
			{
				std::string s = "";
				while (p < bufferEnd)
				{
					switch (*p)
					{
					case '\r':
					case '\n':
						goto EndReadOneString;
					default:
						s += *p++;
					}
				}
			EndReadOneString:
				p++;
				if (s.length())
					table.push_back(s);
			}
		}

		return true;
	};


	bool load_file()
	{
		return load_file(table);
	}
	//output a string;

	inline void change(std::vector<inf> &set, const std::vector<std::string> &table, std::string &result) {
		if (set.size() != 0) {
			sort(set.begin(), set.end());
			change_char(set);
            sort(set.begin(),set.end());
			inf cha = set[0];
			set.erase(set.begin());
			std::vector<tree> tree_set;
			tree tree_zhongjian;
			tree_zhongjian.order = cha.order;
			initiation_struct(tree_zhongjian);
			tree_set.push_back(tree_zhongjian);
			search(cha, set, tree_set);

			change_string(0, tree_set, result, table);
		}
		else {
			result += "√ª”– ‰»Î";
		}
	}

	void change(std::vector<inf>& set, std::string & result)
	{
		change(set, table, result);
	}

	void change_char(std::vector<inf> &set_initiation) {//deal with the char for the first time
		for (int i = 0; i < set_initiation.size(); i++) {  // avoid  error
			if (set_initiation[i].w*set_initiation[i].l <= 9) {
				set_initiation[i].order = Point;
			}
		}
		for(int i = 0; i < set_initiation.size(); i++){
			if (set_initiation[i].order == 44 || set_initiation[i].order == 18 || set_initiation[i].order == 19 || set_initiation[i].order == 1 || set_initiation[i].order == Vert||set_initiation[i].order==21) {
				int j = 0;
				for (; j < set_initiation.size(); j++) {
					if (set_initiation[j].order == Point && set_initiation[j].y< set_initiation[i].y - set_initiation[i].l / 2.0 && (set_initiation[j].x-set_initiation[j].w/2.0<set_initiation[i].x + set_initiation[i].w) && (set_initiation[j].x+set_initiation[j].w/2.0 > set_initiation[i].x - set_initiation[i].w)) {
						break;

					}
				}
				if (j == set_initiation.size()) {
					if (set_initiation[i].order == 44 || set_initiation[i].order == 18 || set_initiation[i].order == 19)
						set_initiation[i].order = 1;
				}
				else {
					if (set_initiation[i].order == 1 || set_initiation[i].order == Vert||set_initiation[i].order==21) {
						set_initiation[i].order = 18;
						set_initiation.erase(set_initiation.begin() + j);
					}
					else {
						if (set_initiation[i].order == 44) {
							set_initiation[i].order = 19;
							set_initiation.erase(set_initiation.begin() + j);
						
						}
						else {
							set_initiation.erase(set_initiation.begin() + j);
						}
					}
				}
			}

		 
		}
		for (int i = 0; i<set_initiation.size()-1;i++){
			if ((set_initiation[i].order == Minus) && set_initiation[i + 1].order == Minus) {// = 
				if ((set_initiation[i].w >= (set_initiation[i + 1].w)*0.9) && (set_initiation[i].w <= (set_initiation[i + 1].w*1.1)) && (set_initiation[i].x > set_initiation[i + 1].x - set_initiation[i + 1].w / 2) && (set_initiation[i].x < set_initiation[i + 1].x + set_initiation[i + 1].w / 2)) {
					set_initiation[i + 1].w = set_initiation[i].w;
					set_initiation[i + 1].l = set_initiation[i].l;
					set_initiation[i + 1].x = set_initiation[i].x;
				}
			}
			else {
				if (set_initiation[i].order == Sim&&set_initiation[i + 1].order == Sim) {// ~ ~
					if ((set_initiation[i].w >= (set_initiation[i + 1].w)*0.9) && (set_initiation[i].w <= (set_initiation[i + 1].w*1.1)) && (set_initiation[i].x > set_initiation[i + 1].x - set_initiation[i + 1].w / 2) && (set_initiation[i].x < set_initiation[i + 1].x + set_initiation[i + 1].w / 2)) {
						set_initiation[i + 1].w = set_initiation[i].w;
						set_initiation[i + 1].l = set_initiation[i].l;
						set_initiation[i + 1].x = set_initiation[i].x;
					}
				}
				else {
					if ((set_initiation[i].order == Less) || (set_initiation[i].order == Larger)) { // >=  <=
						for (int j = i + 1; (j != i - 3) && (j >= 0); j = j - 2) {
							if ((set_initiation[j].w > set_initiation[i].w*0.9) && (set_initiation[j].w < set_initiation[i].w*1.1) && (set_initiation[j].y > set_initiation[i].y)&&(set_initiation[j].x>set_initiation[i].x-set_initiation[i].w/2.0)&&(set_initiation[j].x<set_initiation[i].x+set_initiation[i].w/2.0)&&(set_initiation[j].order==Under_line||set_initiation[j].order==Minus||set_initiation[j].order==Black_slash||set_initiation[j].order==Fan_slash)) {
								if(set_initiation[i].order==Less)
									set_initiation[j].order=Black_slash;
								else
								    set_initiation[j].order=Fan_slash;
								set_initiation[j].x = set_initiation[i].x;
								set_initiation[j].w = set_initiation[i].w;
							}
						}
					}
					else {
						if (set_initiation[i].order == Point&&set_initiation[i + 1].order == Point) { // :
							if ((set_initiation[i].y < set_initiation[i + 1].y) && (set_initiation[i].x > set_initiation[i + 1].x - set_initiation[i + 1].w / 2.0) && (set_initiation[i].x < set_initiation[i + 1].x + set_initiation[i + 1].w / 2.0)) {// (set_initiation[i].x-set_initiation[i+1].w/2.0>set_initiation[i+1].x-set_initiation[i+1].w/2.0)   	
								set_initiation[i].x = set_initiation[i + 1].x;
								set_initiation[i].w = set_initiation[i + 1].w;

							}
						}
						else {
							if (set_initiation[i].order == Point) {  // ;
								for (int j = i + 1; (j != i - 3) && (j >= 0); j = j - 2) {
									if ((set_initiation[j].w > set_initiation[i].w*0.9) && (set_initiation[j].w < set_initiation[i].w*1.1) && (set_initiation[j].y < set_initiation[i].y) && (set_initiation[j].order == Comma)) {
										set_initiation[j].x = set_initiation[i].x;
										set_initiation[j].w = set_initiation[i].w;
									}
								}
							}
							else {
								if (set_initiation[i].order == Add) {// +- -+
									for (int j = i + 1; (j != i - 3) && (j >= 0); j = j - 2) {
										if ((set_initiation[j].w > set_initiation[i].w*0.9) && (set_initiation[j].w < set_initiation[i].w*1.1) && (set_initiation[j].order == Minus) && (set_initiation[i].x > set_initiation[j].x - set_initiation[j].x / 2.0) && (set_initiation[i].x < set_initiation[j].x + set_initiation[j].w / 2.0)) {
											set_initiation[j].x = set_initiation[i].x;
											set_initiation[j].w = set_initiation[i].w;
										}

									}

								}
								

							}

						}

					}

				}

			}

		}

		sort(set_initiation.begin(), set_initiation.end());

		for (int i = 0; i < set_initiation.size(); i++) {
			if ((set_initiation[i].order == Minus)) {
				if (i != (set_initiation.size() - 1)) {//if i==set_initiation.size()-1 the order of i dose not change
					int j = 0;
					for (; j < set_initiation.size(); j++) {//
						if ((set_initiation[j].y + set_initiation[j].l / 2.0 > set_initiation[i].y - set_initiation[i].w*1.5) && (set_initiation[j].y < set_initiation[i].y) && ((set_initiation[j].x  > (set_initiation[i].x - set_initiation[i].w / 2.0)) && (set_initiation[j].x <= (set_initiation[i].x + set_initiation[i].w / 2.0))))
							break;
					}
					if (j != set_initiation.size()) {
						if ((set_initiation[j].order == Point) && (j + 1 != set_initiation.size()) && (set_initiation[j + 1].order == Point)) { //  . - . 
							set_initiation[i].order = Div;
							set_initiation[i].l = (-set_initiation[j].y + set_initiation[j].l + set_initiation[j + 1].y);
							set_initiation.erase(set_initiation.begin() + j);
							set_initiation.erase(set_initiation.begin() + j);
						}
						else {
							j = 0;
							for (; j < set_initiation.size(); j++) {
								if ((set_initiation[j].y - set_initiation[j].l / 2.0 < set_initiation[i].y + set_initiation[i].w*1.5) && (set_initiation[j].y > set_initiation[i].y) && ((set_initiation[j].x > (set_initiation[i].x - set_initiation[i].w / 2)) && (set_initiation[j].x < (set_initiation[i].x + set_initiation[i].w / 2))))
									break;
							}
							if (j != set_initiation.size()) {//have up and down i may be fraction or =with up chars
								if ((set_initiation[j].x == set_initiation[i].x) && (set_initiation[j].w == set_initiation[i].w) && (set_initiation[j].y > set_initiation[i].y)) {// =with up chars
									set_initiation[i].order = Equal;
									//change size//
									set_initiation[i].l = set_initiation[j].y - set_initiation[i].y + set_initiation[i].l;
									set_initiation[i].y = (set_initiation[j].y + set_initiation[i].y) / 2;
									set_initiation.erase(set_initiation.begin() + j);
								}
								else {// fraction
									set_initiation[i].order = Frac;
									/******************************************************/

									int j = i - 1;
									for (; j>=0; j--) {
										if (set_initiation[j].x<set_initiation[i].x - set_initiation[i].w / 2.0)
											break;
										
									}
									if (j != i - 1 && set_initiation[j+1].x - set_initiation[j+1].w / 2.0<set_initiation[i].x - set_initiation[i].w / 2.0) {
										set_initiation[i].w = set_initiation[i].x + set_initiation[i].w / 2.0 - (set_initiation[j+1].x - set_initiation[j+1].w / 2.0);
										set_initiation[i].x = set_initiation[i].w / 2.0 + set_initiation[j+1].x - set_initiation[j+1].w / 2.0;
										set_initiation[j+1].w--;
									}

									set_initiation[i].char_kind = N4;
								}
							}
							else {//have not down part  down-
								set_initiation[i].order = Under_line;
								set_initiation[i].char_kind = N4;
								int j = i - 1;
								for (; j >= 0; j--) {
									if (set_initiation[j].x+set_initiation[j].w/2.0>set_initiation[i].x-set_initiation[i].w/2.0&&set_initiation[j].y<set_initiation[i].y&&set_initiation[j].y+set_initiation[j].w/2.0>set_initiation[i].y-set_initiation[i].w*2) {
										1;
									}
									else
										break;

								}

								if (j != i - 1 && set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0<set_initiation[i].x - set_initiation[i].w / 2.0) {
									set_initiation[i].w = set_initiation[i].x + set_initiation[i].w / 2.0 - (set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0);// ((set_initiation[i].x * 2) - (set_initiation[j + 1].x * 2 - set_initiation[j + 1].w));
									set_initiation[i].x = set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0 + set_initiation[i].w / 2.0;
								}
							}
						}
					}
					else {
						j = 0;
						for (; j < set_initiation.size(); j++) {
							if ((set_initiation[j].y - set_initiation[j].l / 2.0 < set_initiation[i].y + set_initiation[i].w*1.5) && (set_initiation[j].y > set_initiation[i].y) && ((set_initiation[j].x+set_initiation[j].w/2.5> (set_initiation[i].x - set_initiation[i].w / 2)) && (set_initiation[j].x -set_initiation[j].w/2.5 < (set_initiation[i].x + set_initiation[i].w / 2))))
								break;
						}
						if (j != set_initiation.size()) {
							if (set_initiation[j].order == Minus) {
								if ((j + 1 != set_initiation.size()) && (set_initiation[j + 1].order == Minus) && (set_initiation[j + 1].y > set_initiation[i].y) && (set_initiation[i].x == set_initiation[j + 1].x)) {//    ---  
									set_initiation[i].order = Equiv;
									//change size//
									set_initiation[i].l = (-set_initiation[i].y + set_initiation[j].l + set_initiation[j + 1].y);
									set_initiation[i].y = set_initiation[j].y;
									set_initiation.erase(set_initiation.begin() + j);
									set_initiation.erase(set_initiation.begin() + j);
								}
								else {//  =
									set_initiation[i].order = Equal;
									//change size//
									set_initiation[i].l = set_initiation[j].y - set_initiation[i].y + set_initiation[i].l;
									set_initiation[i].y = (set_initiation[i].y + set_initiation[j].y) / 2;
									set_initiation.erase(set_initiation.begin() + j);
								}

							}
							else {// up -
								set_initiation[i].order = Over_line;
								set_initiation[i].char_kind = N4;
								int j = i - 1;
								for (; j >= 0; j--) {
									if ((set_initiation[j].x+set_initiation[j].w/2.5>set_initiation[i].x-set_initiation[i].w/2.0&&set_initiation[j].y>set_initiation[i].y&&set_initiation[j].y-set_initiation[j].l/2.0<set_initiation[i].y+set_initiation[i].w*1.8)) {
										1;

									}
									else
										break;

								}

								if (j != i - 1 && set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0<set_initiation[i].x - set_initiation[i].w / 2.0) {
									set_initiation[i].w = set_initiation[i].x + set_initiation[i].w / 2.0 - (set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0);// ((set_initiation[i].x * 2) - (set_initiation[j + 1].x * 2 - set_initiation[j + 1].w));
									set_initiation[i].x = set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0 + set_initiation[i].w / 2.0;
								}
							}
						}
					}
				}
			}
			else {
				if ((set_initiation[i].order == Less) && (i != set_initiation.size() - 1)) {// <=
					if (set_initiation[i + 1].order == Black_slash||set_initiation[i+1].order==Under_line) {
						set_initiation[i].order = Leq;
						//change size//
						set_initiation[i].l = set_initiation[i + 1].y + set_initiation[i].l / 2 - set_initiation[i].y + set_initiation[i + 1].l / 2;//do not need change center
						set_initiation.erase(set_initiation.begin() + i + 1);
					}

				}
				else {
					if ((set_initiation[i].order == Larger) && (i != set_initiation.size() - 1)) {// >=
						if (set_initiation[i + 1].order == Fan_slash||set_initiation[i+1].order==Under_line) {
							set_initiation[i].order = Geq;
							//change size//
							set_initiation[i].l = set_initiation[i + 1].y + set_initiation[i].l / 2 - set_initiation[i].y + set_initiation[i + 1].l / 2;//do not need change center
							set_initiation.erase(set_initiation.begin() + i + 1);
						}
					}
					else {
						if ((set_initiation[i].order == Sim) ) {
							if ((i!=set_initiation.size()-1)&&(set_initiation[i + 1].order == Sim) && (set_initiation[i + 1].y > set_initiation[i].y) && (set_initiation[i + 1].x == set_initiation[i].x)) {// ~ ~
								set_initiation[i].order = Approx;
								//change size//
								set_initiation[i].l = set_initiation[i + 1].y - set_initiation[i].y + set_initiation[i].l;
								set_initiation[i].y = (set_initiation[i].y + set_initiation[i + 1].y) / 2;
								set_initiation.erase(set_initiation.begin() + i + 1);
							}
							else {
								for (int j = 0; j < set_initiation.size(); j++) {
									if ((set_initiation[j].x-set_initiation[j].w/2.5<set_initiation[i].x + set_initiation[i].w / 2.0&&set_initiation[j].x+set_initiation[j].w/2.5>set_initiation[i].x - set_initiation[i].w / 2.0)&&set_initiation[j].y>set_initiation[i].y&&set_initiation[j].y-set_initiation[j].w/2.0<set_initiation[i].y+set_initiation[i].w*1.5) {
										set_initiation[i].x = set_initiation[j].x;
										set_initiation[i].w = set_initiation[j].w;
										set_initiation[i].order = Wide_tilde;
										set_initiation[i].char_kind = N4;
									}
								
								}
							}
						}
						else {
							if (set_initiation[i].order == Point) {
								if ((i != set_initiation.size() - 1) && (set_initiation[i + 1].order == Point) && (set_initiation[i + 1].y > set_initiation[i].y)) { // :
									set_initiation[i].order = Colon;
									set_initiation[i].l = set_initiation[i + 1].y - set_initiation[i].y;
									set_initiation[i].y = (set_initiation[i].y + set_initiation[i + 1].y) / 2;
									set_initiation.erase(set_initiation.begin() + i + 1);
								}
								else {
									if ((i != set_initiation.size() - 1) && (set_initiation[i + 1].order == Comma)) {// ;
										set_initiation[i].order = Semicolon;
										set_initiation[i].l = set_initiation[i + 1].y - set_initiation[i].y;
										set_initiation[i].y = (set_initiation[i].y + set_initiation[i + 1].y) / 2;
										set_initiation.erase(set_initiation.begin() + i + 1);
									}
									else {
										for (int j = i - 1; j > 0; j--) {
											if((set_initiation[j].order==18||set_initiation[j].order==19||set_initiation[j].order==1||set_initiation[i].order==21)&&(set_initiation[i].y<set_initiation[j].y-set_initiation[j].l/2.0)&&(set_initiation[i].x+set_initiation[i].w/2.0<=set_initiation[j].x+set_initiation[j].w)){
										        if(set_initiation[j].order==1||set_initiation[j].order==21)
												    set_initiation[j].order=18;
											    set_initiation.erase(set_initiation.begin()+i);
												i--;
												break;
										  }
										}
									}

								}
							}
							else {
								if ((set_initiation[i].order == Circle) && (i < set_initiation.size() - 1) && (set_initiation[i + 1].order == Fan_slash) && (set_initiation[i + 2].order == Circle)) {// °£/°£
									set_initiation[i].order = Percent;
									set_initiation.erase(set_initiation.begin() + i + 1);
									set_initiation.erase(set_initiation.begin() + i + 1);

								}
								else {
									if ((set_initiation[i].order == 21||set_initiation[i].order==1||set_initiation[i].order==Vert)) {// lim
										for (int j = i + 1; j < set_initiation.size(); j++) {
											if ((set_initiation[j].x < set_initiation[i].x + set_initiation[i].l) && (set_initiation[j].y < set_initiation[i].y + set_initiation[i].l / 2.0) && (set_initiation[j].y > set_initiation[i].y - set_initiation[i].l / 2.0) && set_initiation[j].order == 18) {
												for (int m = j + 1; m < set_initiation.size(); m++) {
													if ((set_initiation[m].x < set_initiation[i].x + set_initiation[i].l*1.3) && (set_initiation[m].y < set_initiation[i].y + set_initiation[i].l / 2.0) && (set_initiation[m].y > set_initiation[i].y - set_initiation[i].l / 2.0) && set_initiation[m].order == 22) {
														set_initiation[i].order = Lim;
														set_initiation[i].char_kind = N1;
														set_initiation[i].w = set_initiation[m].x - set_initiation[i].x + set_initiation[i].w / 2 + set_initiation[m].w / 2;
														set_initiation[i].x = set_initiation[j].x;
														
														set_initiation.erase(set_initiation.begin() + j);	 
														set_initiation.erase(set_initiation.begin() + m - 1);
														break;
													}
												}

											}

										}

										if (set_initiation[i].order == Lim) {
											int j = i - 1;
											for (; j >= 0; j--) {
												if ((set_initiation[j].y - set_initiation[j].l / 2.0>set_initiation[i].y + set_initiation[i].l*(0.62)) || (set_initiation[j].y + set_initiation[j].l / 2.0 < set_initiation[i].y - set_initiation[i].l*(0.62))) {
													1;

												}
												else
													break;

											}

											if (j != i - 1 && set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0<set_initiation[i].x - set_initiation[i].w / 2.0) {
												set_initiation[i].w = ((set_initiation[i].x * 2) - (set_initiation[j + 1].x * 2 - set_initiation[j + 1].w));
											}
													 
												  
										}

									}
									else {
										if (set_initiation[i].order == Add) {
											if ((i + 1 != set_initiation.size()) && (set_initiation[i + 1].order == Minus) && (set_initiation[i + 1].x >set_initiation[i].x-set_initiation[i].w/2.0)&&(set_initiation[i + 1].x <set_initiation[i].x + set_initiation[i].w / 2.0)&&(set_initiation[i+1].y<set_initiation[i].y+set_initiation[i].l*0.65)) {//******* + -
												set_initiation[i].order = Pm;
												set_initiation[i].l = set_initiation[i + 1].y - set_initiation[i].y + (set_initiation[i + 1].l + set_initiation[i].l) / 2;
												set_initiation.erase(set_initiation.begin() + i + 1);
											}
											else {
												if ((i - 1 != -1) && (set_initiation[i - 1].order == Minus) && (set_initiation[i + 1].x >set_initiation[i].x - set_initiation[i].w / 2.0) && (set_initiation[i + 1].x <set_initiation[i].x + set_initiation[i].w / 2.0) && (set_initiation[i + 1].y>set_initiation[i].y - set_initiation[i].l*0.65)) {//****** - +
													set_initiation[i].order = Mp;
													set_initiation[i].l = set_initiation[i].y - set_initiation[i - 1].y + (set_initiation[i - 1].l + set_initiation[i].l) / 2;
													set_initiation.erase(set_initiation.begin() + i - 1);
													i--;

												}
											}
										}
										else {
											if (set_initiation[i].order == Rightarrow ) {
												set_initiation[i].char_kind = N4;
												int j = i - 1;
												for (; j >= 0; j--) {
													if ((set_initiation[j].y - set_initiation[j].l / 2.0>set_initiation[i].y ) &&set_initiation[j].x+set_initiation[j].w/3.0>set_initiation[i].x-set_initiation[i].w/2.0) {
														set_initiation[i].order = Over_Rightarrow;

													}
													else
														break;

												}

												if (j != i - 1 && set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0<set_initiation[i].x - set_initiation[i].w / 2.0) {
													set_initiation[i].w = set_initiation[i].x + set_initiation[i].w / 2.0 - (set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0);// ((set_initiation[i].x * 2) - (set_initiation[j + 1].x * 2 - set_initiation[j + 1].w));
													set_initiation[i].x = set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0 + set_initiation[i].w / 2.0;
												}



											}
											else {
												if ((set_initiation[i].order == Sum) || (set_initiation[i].order == Prod) || (set_initiation[i].order == Int) || (set_initiation[i].order == Oin)) {
													set_initiation[i].char_kind = N1;
													int j = i - 1;
													for (; j >= 0; j--) {
														if ((set_initiation[j].y - set_initiation[j].l / 2.0>set_initiation[i].y + set_initiation[i].l*(0.62)) || (set_initiation[j].y + set_initiation[j].l / 2.0 < set_initiation[i].y - set_initiation[i].l*(0.62))) {
															1;

														}
														else
															break;

													}

													if (j != i - 1 && set_initiation[j + 1].x - set_initiation[j + 1].w / 2.0<set_initiation[i].x - set_initiation[i].w / 2.0) {
														set_initiation[i].w = ((set_initiation[i].x * 2) - (set_initiation[j + 1].x * 2 - set_initiation[j + 1].w));
													}

												}
												else {
													if (set_initiation[i].order == Sqrt) {
														set_initiation[i].char_kind = N2;
													}
													else {
														if (set_initiation[i].order == Point) {
															if ((i - 1 >= 0) && (set_initiation[i - 1].order == 18) || (set_initiation[i - 1].order == 19)) {
																set_initiation.erase(set_initiation.begin() + i);
																i--;
															}
															else {
																set_initiation[i].char_kind = N4;
															}
														}
														else {
															if (set_initiation[i].order == Wide_hat) {
																for (int j = 0; j < set_initiation.size(); j++) {
																	if ((set_initiation[j].x-set_initiation[j].w/2.5<set_initiation[i].x + set_initiation[i].w / 2.0&&set_initiation[j].x+set_initiation[i].w/2.5>set_initiation[i].x - set_initiation[i].w / 2.0)&&set_initiation[j].y>set_initiation[i].y&&set_initiation[j].y - set_initiation[j].w / 2.0<set_initiation[i].y + set_initiation[i].w*1.5) {
																		set_initiation[i].x = set_initiation[j].x;
																		set_initiation[i].w = set_initiation[j].w;
																		set_initiation[i].char_kind = N4;
																	}

																}
															
															}
														
														
														}
													}
												}

											}

										}

									}

								}

							}

						}
					}

				}

			}

		}

	};

	void initiation_struct(tree &tree_zhongjian) {
		tree_zhongjian.tree_down = -1;
		tree_zhongjian.tree_up = -1;
		tree_zhongjian.tree_down = -1;
		tree_zhongjian.tree_right_up = -1;
		tree_zhongjian.tree_right_down = -1;
		tree_zhongjian.next = -1;

	};

	void search(inf cha, std::vector<inf> &set, std::vector<tree> &tree_set) {
		//set has been sorted
		//N4 N5 N6 is different from here

		double left_min = (double)cha.x - cha.w / 2.0;
		double right_max = (double)cha.x + cha.w / 2.0;

		int p = tree_set.size() - 1;
		if (cha.char_kind == N1) {//sum limitation multiplition integration(1,2)
			  //**************************** search the up and down chars of this special chars **************** 
			std::vector<inf> information[2];
			int i = 0;
			while (set.size() != 0) {
				if (cha.order != Int&&cha.order!=Oin) {
					if ((set[0].y + set[0].l / 2.0 < cha.y - cha.l*0.5) || (set[0].y - set[0].l / 2.0 > cha.y + cha.l*0.5)) {
						if (set[0].y + set[0].l / 2.0 < cha.y - cha.l*0.5) {
							information[0].push_back(set[0]);//save up chars
							set.erase(set.begin());
						}
						else {
							information[1].push_back(set[0]);//save down chars
							set.erase(set.begin());
						}
					}
					else {
						break;
					}
				}
				else {
					if (((set[0].y + set[0].l / 2.0 < cha.y - cha.l*0.15) || (set[0].y - set[0].l / 2.0 > cha.y + cha.l*0.15) )&& set[0].order != Over_line&&set[0].order != Wide_tilde&&set[0].order != Wide_hat&&set[0].order != Under_line) {
						if (set[0].y + set[0].l / 2.0 < cha.y - cha.l*0.15) {
							information[0].push_back(set[0]);//save up chars
							set.erase(set.begin());
						}
						else {
							information[1].push_back(set[0]);//save down chars
							set.erase(set.begin());
						}
					}
					else {
						break;
					}
				
				
				
				}
			}


			for (i = 0; i < 2; i++) {
				if (information[i].size() != 0) {
					tree tree_zhongjian;
					initiation_struct(tree_zhongjian);
					tree_zhongjian.order = information[i][0].order;
					tree_set.push_back(tree_zhongjian);
					inf zhongjian = information[i][0];
					information[i].erase(information[i].begin());
					if (i == 0)
						tree_set[p].tree_right_up = tree_set.size() - 1;
					else
						tree_set[p].tree_right_down = tree_set.size() - 1;
					search(zhongjian, information[i], tree_set);
				}
			}

		}


		if (cha.char_kind == N4) {
			if (cha.order == Under_line || cha.order == Over_line || cha.order == Wide_hat || cha.order ==Over_Rightarrow || cha.order == Dot||cha.order==Wide_tilde) {//!!!!!!!!!!!!! //if  up-  down-  up->  up~  up. up..
				//std::vector<tree>::iterator p_zhongjian=prev(tree_set.end());
				// cha=set[0];
				std::vector<inf> information;
				if (cha.order != Dot) {
					while (set.size() != 0) {
						if ((cha.x - cha.w / 2.0 < set[0].x+set[0].w/3.5) && (set[0].x-set[0].w/3.5 < cha.x + cha.w / 2.0)) {
						
							information.push_back(set[0]);
							set.erase(set.begin());
						}
						else
							break;

					}
				}
				else {
					while (set.size() != 0) {
						if ((set[0].x - set[0].w / 2.0 < cha.x) && (cha.x < set[0].x + set[0].w / 2.0)) {
				
							information.push_back(set[0]);
							set.erase(set.begin());
						}
						else
							break;
					}
				}
				if (information.size() != 0) {

					tree tree_zhongjian;
					initiation_struct(tree_zhongjian);
					tree_zhongjian.order = information[0].order;
					inf inf_zhongjian;
					inf_zhongjian = information[0];
					tree_set.push_back(tree_zhongjian);
					information.erase(information.begin());
					tree_set[p].tree_down = tree_set.size() - 1;
					search(inf_zhongjian, information, tree_set);

				}

			}

			else {//if division:only search up and down

				std::vector<inf> set_zhongjian;
				while (set.size() != 0) {
					if ((set[0].x >= (cha.x-cha.w/2.0) && set[0].x <= (cha.x+cha.w/2.0))) {
						set_zhongjian.push_back(set[0]);
						set.erase(set.begin());
					}
					else
						break;
				}
				std::vector<inf> information[2];
				for (std::vector<inf>::iterator it = set_zhongjian.begin(); it != set_zhongjian.end(); it++) {
					if (((*it).y + (*it).l / 2.0) < cha.y)//up
						information[0].push_back(*it);
					else                             //down
						information[1].push_back(*it);
				}
				for (int j = 0; j < 2; j++) {
					if (information[j].size() != 0) {
						sort(information[j].begin(), information[j].end());
						tree zhongjian;
						initiation_struct(zhongjian);
						inf inf_zhongjian = information[j][0];
						zhongjian.order = (information[j][0]).order;
						tree_set.push_back(zhongjian);
						information[j].erase(information[j].begin());
						if (j == 0)
							tree_set[p].tree_right_up = tree_set.size() - 1;
						else
							tree_set[p].tree_right_down = tree_set.size() - 1;
						search(inf_zhongjian, information[j], tree_set);
					}
				}

			}

		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (cha.order == Sqrt) {//!!!!!!!!!!!!!!!!!!  radical sign sqrt
			std::vector<inf> set_zhongjian;

			while (set.size() != 0) {//find all the chars in the radical sign
				if ((set[0].x >= left_min) && (set[0].x <= right_max) && (set[0].y <= cha.y + cha.l / 2.0)) {
					set_zhongjian.push_back(set[0]);
					set.erase(set.begin());
				}
				else
					break;
			}

			sort(set.begin(), set.end());
			if (set_zhongjian.size() != 0) {
				tree zhongjian;
				initiation_struct(zhongjian);
				zhongjian.order = set_zhongjian[0].order;
				tree_set.push_back(zhongjian);
				//set_zhongjian.erase(set_zhongjian.begin());
				tree_set[p].tree_down = tree_set.size() - 1;
				inf cha_zhongjian = set_zhongjian[0];
				set_zhongjian.erase(set_zhongjian.begin());
				///****************** we should make sure the p(pointer) points to the right position *************

				search(cha_zhongjian, set_zhongjian, tree_set);
			}
			//////////////


		}
		///*char_kind==N0          *******************************************************************************
		if (cha.char_kind == N0) { //we can also judge the right_up and right_down by the lowest and highest lines
			if (cha.order != Comma&&cha.order!=Point&&cha.order!=Equal&&cha.order!=Equiv&&cha.order!=121&&((cha.order<81||cha.order>92))&&(cha.order<94||cha.order>99)&&cha.order!=123&&cha.order!=125&&cha.order!=126&&cha.order!=134) {

				std::vector<inf> set_zhongjian;
				std::vector<inf> information[2];
				while (set.size() != 0) { 
					if ( (set[0].order != Over_line&&set[0].order != Under_line&&set[0].order != Wide_hat&&set[0].order!=Wide_tilde&&set[0].order!=Over_Rightarrow)) {
						if (set[0].order == Minus || set[0].order == Frac ||set[0].order == Equal||(set[0].order>=Add&&set[0].order<=92)||(set[0].order>=95&&set[0].order<=99)) {
							if ((set[0].y>= cha.y + cha.l*0.3) ||(set[0].y+set[0].l/2.0>cha.y+cha.l*0.8&&set[0].l<cha.l*1.6)) {//down
							    information[1].push_back(set[0]);
								set.erase(set.begin());
							}
							else {
								if( (set[0].y + set[0].l / 2.0 <= cha.y - cha.l*0.2)||(set[0].y-set[0].l/2.0<cha.y-cha.l*0.8&&set[0].l<cha.l*1.6)) {//up
									information[0].push_back(set[0]);
									set.erase(set.begin());
								}
							    else
								    break;
							}
							
						}
						else {
							if (set[0].order == Comma || set[0].order == Point) {
								if (set[0].y > cha.y + cha.l*0.7) {//down
									information[1].push_back(set[0]);
									set.erase(set.begin());
								}
								else {
									if (set[0].y<cha.y-cha.l*0.3) {
										information[0].push_back(set[0]);
										set.erase(set.begin());	
									}
								    else
									   break;
								}
								
							}
							else {
								if (set[0].order == 11 || set[0].order == 20 || set[0].order == 21 || set[0].order == 17||set[0].order==78||(set[0].order>=0&&set[0].order<=9)) {//bhkl0123456789
									if (set[0].y - set[0].l / 2.0 > cha.y - cha.l*0.33&&(set[0].l<cha.l*1.33)) {//down
										information[1].push_back(set[0]);
										set.erase(set.begin());

									}
									else {
										if (set[0].y+set[0].l/2.0<cha.y+cha.l*0.25) {//up
											information[0].push_back(set[0]);
											set.erase(set.begin());
										
										}
									    else
										    break;
									}
									
								}
								else{
									if (set[0].order!=Fan_slash&&set[0].order != Black_slash&&set[0].order != 56&&set[0].order!=49 && set[0].order != 61 && set[0].order != 25 && set[0].order != 26 && set[0].order != 15 && set[0].order != 16 && set[0].order != 19 && set[0].order != 34 && set[0].order != 49) {//!=pqgjyQ
										if (set[0].y - set[0].l / 2.0 > cha.y + cha.l*0.18||(set[0].y-set[0].l/2.0>cha.y&&set[0].l<=0.4*cha.l)||(((set[0].y+set[0].l/2.0>cha.y+cha.l*0.5+set[0].l*0.08)||set[0].y+set[0].l/2.0>cha.y+cha.l*0.6)&&set[0].l<=cha.l*1.08)) {//down
											information[1].push_back(set[0]);
											set.erase(set.begin());

										}
										else {
											if (set[0].y+set[0].l/2.0<cha.y+cha.l*0.1) {//up
												information[0].push_back(set[0]);
												set.erase(set.begin());

											}
											else {
												break;
											}
										}

									}
									else {
										if (set[0].order==Fan_slash||set[0].order==Black_slash||set[0].order==49||set[0].order==56||set[0].order==61||set[0].order == 25||set[0].order==26||set[0].order==15||set[0].order==16||set[0].order==19||set[0].order==34) {//pqgQj
											if (set[0].y + set[0].l / 2.0 > cha.y + cha.l*0.5+set[0].l*0.4||set[0].y-set[0].l/2.0>cha.y+cha.l*0.15 ||set[0].l<0.5*cha.l) {//down
												information[1].push_back(set[0]);
												set.erase(set.begin());

											}
											else {
												if (set[0].y+set[0].l/2.0<cha.y+cha.l*0.35) {//up
													information[0].push_back(set[0]);
													set.erase(set.begin());
												
												}
											    else
												   break;
										
											}
											
										
										}
										else {
											if (set[0].y - set[0].l / 2.0 > cha.y + cha.l*0.1 || set[0].y + set[0].l / 2.0 > cha.y) {
												set_zhongjian.push_back(set[0]);
												set.erase(set.begin());

											}
											else
												break;
										
										
										}
										    
									}
								}
							}
						}
					}
					else
						break;
				}
				/*
				std::vector<inf> information[2];
				*/
				for (std::vector<inf>::iterator it = set_zhongjian.begin(); it != set_zhongjian.end(); it++) {

					if ((*it).y +(*it).l*0.2 <= cha.y)//right_up //avoid error
						information[0].push_back(*it);

					else  //right_down
						information[1].push_back(*it);

				}
				

				for (int j = 0; j < 2; j++) {
					if (information[j].size() != 0) {
						sort(information[j].begin(), information[j].end());
						tree zhongjian;
						initiation_struct(zhongjian);
						zhongjian.order = information[j][0].order;
						inf inf_zhongjian = information[j][0];

						information[j].erase(information[j].begin());
						tree_set.push_back(zhongjian);

						if (j == 0) {
							tree_set[p].tree_right_up = tree_set.size() - 1;
						}
						else {
							tree_set[p].tree_right_down = tree_set.size() - 1;
						}
						search(inf_zhongjian, information[j], tree_set);
					}
				}
				//delete

			   //end
			}
			 

}
		if (set.size() != 0 && set[0].x > cha.x) {// link this char to the next char
			cha = set[0];
			set.erase(set.begin());
			tree tree_zhongjian;
			initiation_struct(tree_zhongjian);
			tree_zhongjian.order = cha.order;
			tree_set.push_back(tree_zhongjian);
			tree_set[p].next = tree_set.size() - 1;
			//set.erase(set.begin());
			search(cha, set, tree_set);
		}

	};

	void change_string(int p, const std::vector<tree> &tree_set, std::string &result, const std::vector<std::string> &table) {
		if (p != -1) {
			if (tree_set[p].order == Frac) {
				result += "\\frac {";//.append();
				change_string(tree_set[p].tree_right_up, tree_set, result, table);
				result += "}{";
				//result.append("}{");
				change_string(tree_set[p].tree_right_down, tree_set, result, table);
				result += "}";
				//result.append("}");
			}
			else {
				if (tree_set[p].order == Sqrt) {
					result += "\\sqrt {";//.append();
					change_string(tree_set[p].tree_down, tree_set, result, table);
					result += "}";//.append();

				}
				else {
					if (tree_set[p].order == Over_line || tree_set[p].order == Under_line || tree_set[p].order == Over_Rightarrow || tree_set[p].order == Wide_tilde||tree_set[p].order==Wide_hat) {
						result += table[tree_set[p].order];
						result += "{";//.append("{");
						change_string(tree_set[p].tree_down, tree_set, result, table);
						result += "}";//.append("}");  
					}
					else {
						result += table[tree_set[p].order];
						if (tree_set[p].tree_right_up != -1) {
							if ((tree_set[p].tree_right_up == Dot && tree_set[tree_set[p].tree_right_up].next == -1)) {
								result += "\\Dot ";//.append();
								result.append(table[tree_set[p].order]);
							}
							else {
								result += "^{";//append();
								change_string(tree_set[p].tree_right_up, tree_set, result, table);
								result += "}";
								//result.append("");
							}
						}
						if (tree_set[p].tree_right_down != -1) {
							result += "_{";//.append("_{");
							change_string(tree_set[p].tree_right_down, tree_set, result, table);
							result += "}";//.append("}"); 
						}

					}
				}
			}

			if (tree_set[p].next != -1 && p != tree_set.size() - 1) {
				change_string(tree_set[p].next, tree_set, result, table);
			}
		}
	};

}
