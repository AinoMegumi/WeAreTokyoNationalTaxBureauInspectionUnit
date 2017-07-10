#include "Table.h"
#include <array>

constexpr int window_width = 720;
constexpr int window_height = 405;

int init() {
#ifndef _DEBUG
	SetWindowUserCloseEnableFlag(FALSE);
#endif
	SetMainWindowText("�L�[�R���t�B�O�ύX");
	SetOutApplicationLogValidFlag(FALSE);
	SetGraphMode(window_width, window_height, 16);
	ChangeWindowMode(TRUE);
	SetBackgroundColor(153, 217, 234);
	SetAlwaysRunFlag(TRUE);
	if (DxLib_Init() == -1 || SetMouseDispFlag(TRUE) == -1) return -1;
	SetTransColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	return 0;
}

bool all_zero(std::array<int, 256> buf, int &num) {
	for (size_t i = 0; i < buf.size(); i++) {
		if (buf[i] != 0) {
			num = static_cast<int>(i);
			return false;
		}
	}
	return true;
}

std::string change_num_to_string(int num) {
	for (auto& i : config_list) if (i.second == num) return i.first;
	throw std::runtime_error("���̃L�[�͎g���܂���B");
}

std::string change_english_to_japanese(std::string command_line) {
	if (command_line == "up") return "��";
	else if (command_line == "left") return "��";
	else if (command_line == "down") return "��";
	else if (command_line == "right") return "�E";
	else if (command_line == "choose") return "����";
	else if (command_line == "cancel") return "�L�����Z��";
	else if (command_line == "reload") return "�L�[���̍ēǍ���";
	else if (command_line == "screen") return "�X�N���[���V���b�g";
	else throw std::runtime_error("�����Ȉ����ł��B");
}

#include "Ini.h"

int key_reset() {
	Ini ini(".\\System\\system.ini");
	ini.edit("key", "up", std::to_string(KEY_INPUT_UP));
	ini.edit("key", "down", std::to_string(KEY_INPUT_DOWN));
	ini.edit("key", "left", std::to_string(KEY_INPUT_LEFT));
	ini.edit("key", "right", std::to_string(KEY_INPUT_RIGHT));
	ini.edit("key", "choose", std::to_string(KEY_INPUT_SPACE));
	ini.edit("key", "cancel", std::to_string(KEY_INPUT_B));
	ini.edit("key", "reload", std::to_string(KEY_INPUT_F11));
	ini.edit("key", "screen", std::to_string(KEY_INPUT_F12));
	return MessageBox(NULL, "�L�[�R���t�B�O�������ݒ�ɖ߂��܂����B", "�L�[�R���t�B�O�ύX", MB_OK);
}

void key_reset(const std::string cmdline) {
	if (cmdline.empty()) {
		if (
			MessageBox(
				NULL,
				"�L�[�R���t�B�O�������ݒ�ɖ߂��܂��B\n��낵���ł����H\n�������ݒ�ɖ߂����A�L�[�̕ύX���s���������́A�����`���[�́u�ݒ�ύX�v��I�����Ă��������B",
				"�L�[�R���t�B�O�ύX",
				MB_ICONWARNING | MB_YESNO) == IDYES) key_reset();
	}
	else if (
		MessageBox(
			NULL,
			"�L�[�R���t�B�O�������ݒ�ɖ߂��܂��B\n��낵���ł����H\n",
			"�L�[�R���t�B�O�ύX",
			MB_ICONWARNING | MB_YESNO) == IDYES) key_reset();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {
	if (std::strlen(lpCmdLine) == 0 || std::string(lpCmdLine) == "reset") {
		key_reset(lpCmdLine);
		return 0;
	}
	if (-1 == init()) return -1;
	try {
		char KeyStateBuf[256];
		std::array<int, 256> buf;
		buf.fill(0);
		std::string key;
		while (-1 != ProcessMessage()) {
			int num;
			// �L�[�R���t�B�O�ݒ蕔���X�^�[�g
			while (-1 != ProcessMessage() && all_zero(buf, num)) {
				ClearDrawScreen();
				DrawFormatString(0, 0, GetColor(0, 0, 0), "%s�̑�����ǂ̃L�[�Ɋ��蓖�Ă܂����H", change_english_to_japanese(lpCmdLine).c_str());
				ScreenFlip();
				buf.fill(0);
				while (ProcessMessage() == 0 && CheckHitKeyAll() == 0) {}
				GetHitKeyStateAll(KeyStateBuf);
				if (1 == KeyStateBuf[KEY_INPUT_F11]) {
					MessageBox(NULL, "���̃L�[�͌Œ�L�[�Ƃ��Ċ��蓖�Ă��Ă��܂��B", "�L�[�R���t�B�O�ύX", MB_OK);
					continue;
				}
				for (int i = 0; i < 256; i++) buf[i] = KeyStateBuf[i];
			}
			// �R���t�B�O�ύX�m�F
			try {
				key = change_num_to_string(num);
			}
			catch (std::exception &er) {
				if (MessageBox(NULL, er.what(), "�L�[�R���t�B�O�ύX", MB_ICONERROR | MB_RETRYCANCEL) == IDCANCEL) return -1;
				else continue;
			}
			char s[1024];
			sprintf_s(s, "%s�L�[��%s�L�[�Ɋ��蓖�Ă܂��B��낵���ł����H", change_english_to_japanese(lpCmdLine).c_str(), key.c_str());
			if (MessageBox(NULL, s, "�L�[�R���t�B�O�ύX", MB_YESNO) == IDYES) break;
		}
		// ��������
		Ini ini(".\\System\\system.ini");
		ini.edit("key", lpCmdLine, key);
		MessageBox(NULL, "�ύX�������܂����B", "�L�[�R���t�B�O�ύX", MB_OK);
		return DxLib_End();
	}
	catch (std::exception &er) {
		DxLib_End();
		return MessageBox(NULL, er.what(), "�L�[�R���t�B�O�ύX", MB_ICONERROR | MB_OK);
	}
}
