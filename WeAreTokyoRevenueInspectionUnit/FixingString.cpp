#include"DxLib.h"
#include<string>

void start_criminal_investigation(const int x, const int y, const unsigned int color, const std::string& tax_name) {
	DrawString(x, y, "�������ŋǍ��@���ł��B", color);
	DrawFormatString(x, y, color, "%s�@�ᔽ�̌��^�ŁA�������܂��狭���������s���܂��B", tax_name.c_str());
}