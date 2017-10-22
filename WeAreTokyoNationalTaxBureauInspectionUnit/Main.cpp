#include "Send.hpp"
#include "D:/Users/AinoMegumi/BitBucket/Kamioda Games/CommonSourceCode/Split.hpp"
#include <Windows.h>
#include "KgWinException.hpp"

static inline void SendExceptionMessageToService(const std::string GameTitle, const std::string ExceptionName, const std::string ExceptionMessage) {
	try {
		std::ostringstream ostr;
		Send snd(ostr, 300); // KgSvchost.exe�̗�O�̏o�̓X���b�h�ɓn��
		snd.InputData(GameTitle);
		snd.InputData(ExceptionName);
		snd.InputData(ExceptionMessage);
		snd.send(ostr);
	}
	catch (const std::exception& er) {
		MessageBoxA(NULL, 
			(std::string("�T�[�r�X�ւ̃G���[���b�Z�[�W�̑��M�Ɏ��s���܂����B\n���� : ") + er.what()).c_str(),
			"�������ŋǍ��@���ł�", MB_ICONERROR | MB_OK);
	}
}

std::vector<std::string> SplitString(const std::string Data, const std::string sep) {
	std::vector<std::string> Arr;
	size_t pos = 0;
	while (pos != std::string::npos) {
		size_t p = Data.find(sep, pos);
		if (p == std::string::npos) {
			Arr.push_back(Data.substr(pos));
			break;
		}
		else Arr.push_back(Data.substr(pos, p - pos));
		pos = p + sep.size();
	}
	return Arr;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(CmdShow);
	try {
		if (std::strlen(lpCmdLine) == 0) {

		}
		else {
			const std::vector<std::string> Args = SplitString(lpCmdLine, " ");
		}
	}
	catch (const std::exception& er) {
		SendExceptionMessageToService("WeAreTokyoNationalTaxBureauInspectionUnit", "std::exception", er.what());
		return MessageBoxA(NULL, er.what(), "�������ŋǍ��@���ł�", MB_ICONERROR | MB_OK);
	}
	catch (const KgWinException& kex) {
		SendExceptionMessageToService("WeAreTokyoNationalTaxBureauInspectionUnit", "KgWinException", kex.what());
		return kex.GraphErrorMessageOnMessageBox("�������ŋǍ��@���ł�");
	}
}
