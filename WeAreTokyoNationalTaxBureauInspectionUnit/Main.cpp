#include "D:/Users/AinoMegumi/BitBucket/Kamioda Games/CommonSourceCode/Split.hpp"
#include <Windows.h>
#include "KgWinException.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(CmdShow);
	try {
		if (std::strlen(lpCmdLine) == 0) {

		}
		else {
			const std::vector<std::string> Args = SplitA(lpCmdLine, ' ');
		}
	}
	catch (const std::exception& er) {
		return MessageBoxA(NULL, er.what(), "�������ŋǍ��@���ł�", MB_ICONERROR | MB_OK);
	}
	catch (const KgWinException& kex) {
		return kex.GraphErrorMessageOnMessageBox("�������ŋǍ��@���ł�");
	}
}
