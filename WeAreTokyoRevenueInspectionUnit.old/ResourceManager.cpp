#include "ResourceManager.h"
#include <intrin.h>
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "psapi.lib")

static inline double change_four_digits(const double data) {
	int i = static_cast<int>((data + 0.005) * 100);
	return static_cast<double>(i) / 100;
}

CPUManagerA::CPUManagerA(const char* process_name) {
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	this->core = sys.dwNumberOfProcessors;
	if (ERROR_SUCCESS != PdhOpenQuery(NULL, 0, &this->hQuery)) throw std::runtime_error("Open Query Error");
	if (process_name == NULL) {
		this->GetTotalUsage = true;
		if (ERROR_SUCCESS != PdhAddCounter(this->hQuery, TEXT("\\Process(_Total#0)\\% Processor Time"), 0, &this->hCounter)) throw std::runtime_error("Add Counter Error");
		if (ERROR_SUCCESS != PdhCollectQueryData(this->hQuery)) throw std::runtime_error("Collect Query Data Error");
	}
	else {
		this->GetTotalUsage = false;
		std::string imagename = process_name;
		if (process_name[std::strlen(process_name) - 4] == '.') remove_extension(imagename);
		char* buf = new char[MAX_PATH];
		sprintf_s(buf, MAX_PATH, "\\Process(%s)\\% Processor Time", imagename.c_str());
		if (ERROR_SUCCESS != PdhAddCounterA(this->hQuery, buf, 0, &this->hCounter)) throw std::runtime_error("Add Counter Error");
		if (ERROR_SUCCESS != PdhGetRawCounterValue(this->hCounter, NULL, &this->rawValueStack)) throw std::runtime_error("Get Raw Counter Value Error");
	}
}

CPUManagerA::CPUManagerA(const std::string& process_name) {
	CPUManagerA(process_name.c_str());
}

CPUManagerA::~CPUManagerA() {
	PdhCloseQuery(this->hQuery);
}

void CPUManagerA::UpdateT() {
	if (ERROR_SUCCESS != PdhCollectQueryData(this->hQuery)) throw std::runtime_error("Collect Query Data Error");
	if (ERROR_SUCCESS != PdhGetFormattedCounterValue(this->hCounter, PDH_FMT_DOUBLE, NULL, &this->fmtValue)) throw std::runtime_error("Get Formatted Counter Value Error");
}

void CPUManagerA::UpdateP() {
	PDH_RAW_COUNTER rawValue;
	if (ERROR_SUCCESS != PdhCollectQueryData(this->hQuery)) throw std::runtime_error("Collect Query Data Error");
	if (ERROR_SUCCESS != PdhGetRawCounterValue(this->hCounter, NULL, &rawValue)) throw std::runtime_error("Get Raw Counter Value Error");
	if (ERROR_SUCCESS != PdhCalculateCounterFromRawValue(this->hCounter, PDH_FMT_DOUBLE, &rawValue, &this->rawValueStack, &this->fmtValue)) throw std::runtime_error("Calculate Counter From Raw Value Error");
	this->cpu_usage = this->fmtValue.doubleValue / this->core;
}

void CPUManagerA::Update() {
	this->GetTotalUsage ? this->UpdateT() : this->UpdateP();
}

CPUManagerA& CPUManagerA::operator = (const char* process_name) {
	return CPUManagerA(process_name);
}

CPUManagerA& CPUManagerA::operator = (const std::string& process_name) {
	return CPUManagerA(process_name.c_str());
}

CPUManagerA& CPUManagerA::operator = (const CPUManagerA& c) {
	this->hQuery = c.hQuery;
	this->hCounter = c.hCounter;
	this->rawValueStack = c.rawValueStack;
	this->fmtValue = c.fmtValue;
	this->core = c.core;
	this->cpu_usage = c.cpu_usage;
	this->GetTotalUsage = c.GetTotalUsage;
	return *this;
}

double CPUManagerA::get_usage() {
	return change_four_digits(this->cpu_usage);
}

void CPUManagerA::remove_extension(std::string &process_name) {
	process_name.erase(process_name.end() - 4, process_name.end());
}

MemoryManager::MemoryManager(const DWORD pid) {
	this->process_handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
}

MemoryManager::~MemoryManager() {
	CloseHandle(this->process_handle);
}

double MemoryManager::get_memory_usage() {
	return change_four_digits(static_cast<double>(this->memory.WorkingSetSize) / 1024);
}

void MemoryManager::Update() {
	GetProcessMemoryInfo(this->process_handle, &this->memory, sizeof(this->memory));
}

struct MemorySize {
	MemorySize() = default;
	MemorySize(DWORDLONG physical_memory, DWORDLONG pagefile, DWORDLONG virtual_memory);
	DWORDLONG physical_memory, // �����������̑傫��
		pagefile, // �y�[�W �t�@�C���̑傫��
		virtual_memory;
};

class MemoryInfo {
private:
	MEMORYSTATUSEX mem;
public:
	MemoryInfo();
	void Update();
	MemorySize total, available;
	int physical_memory_usage;
};

#include <string>
#include <stdexcept>
#define PRINT_BYTE 1
#define PRINT_KB 1024
#define PRINT_MB PRINT_KB * 1024
#define PRINT_GB PRINT_MB * 1024

LPSTR get_error_message() {
	LPSTR lpMessageBuffer = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		LANG_USER_DEFAULT,
		(LPSTR)&lpMessageBuffer,
		0,
		NULL);
	return lpMessageBuffer;
}

MemorySize::MemorySize(DWORDLONG physical_memory, DWORDLONG pagefile, DWORDLONG virtual_memory)
	: physical_memory(physical_memory / 1024), pagefile(pagefile / 1024), virtual_memory(virtual_memory / 1024) {}

MemoryInfo::MemoryInfo() {
	this->mem.dwLength = sizeof(this->mem);
	if (0 == GlobalMemoryStatusEx(&this->mem)) throw std::runtime_error(get_error_message());
	this->total = MemorySize(this->mem.ullTotalPhys, this->mem.ullTotalPageFile, this->mem.ullTotalVirtual);
	this->available = MemorySize(this->mem.ullAvailPhys, this->mem.ullAvailPageFile, this->mem.ullAvailVirtual);
	this->physical_memory_usage = this->mem.dwMemoryLoad;
}

void MemoryInfo::Update() {
	if (0 == GlobalMemoryStatusEx(&this->mem)) throw std::runtime_error(get_error_message());
	this->total = MemorySize(this->mem.ullTotalPhys, this->mem.ullTotalPageFile, this->mem.ullTotalVirtual);
	this->available = MemorySize(this->mem.ullAvailPhys, this->mem.ullAvailPageFile, this->mem.ullAvailVirtual);
	this->physical_memory_usage = this->mem.dwMemoryLoad;
}

static CPUManagerA total = CPUManagerA();
static CPUManagerA current = CPUManagerA("WeAreTokyoRevenueInspectionUnit");
static MemoryInfo memory = MemoryInfo();

#include "Common.h"

// �v���Z�X��ی삷�邽�߂ɍs���ی쏈���B���ɃE�C���X�΍�\�t�g���N���b�V������ƁA�R���s���[�^�[���댯�ɎN����邱�ƂɂȂ�̂ŕK�v
void process_control() {
	total.Update();
	current.Update();
	memory.Update();
	if (total.get_usage() > 95.0f) {
		// ���̃Q�[�����\�����Ă���ꍇ�͋�����~
		if (current.get_usage() > 30.0f) throw std::runtime_error(
			"���̃v���Z�X����уI�y���[�e�B���O�V�X�e���̕ی�̂��߁A�����I�ɏI������܂����B\n"
			"�{�v���Z�X�ȊO��CPU�g�p���������Ȃ��Ă���v���Z�X������ꍇ�A���Y�v���Z�X����~�܂���CPU�g�p�����������Ă���ēx���y���݂��������B"
		);
		// ���̃Q�[�����\�����Ă��Ȃ�����CPU�g�p���������ꍇ�A������܂őҋ@
		else {
			MessageBoxA(
				NULL,
				"�Q�[���̓��삪�s����ɂȂ�̂�h�����߁A�ꎞ�I�ɃQ�[�����~���܂��B\n"
				"PCSP���ݒ肳��Ă���ꍇ�A���̊Ԃ̓J�E���g����~����܂��B\n"
				"CPU�g�p����75���ȉ��܂ŉ�����܂�����ĂэĊJ����܂��B",
				"�������ŋǍ��@���ł�",
				MB_OK
			);
			while(loop() && total.get_usage() > 75.0f) {}
			MessageBoxA(
				NULL,
				"�Q�[�����ĊJ���܂��B",
				"�������ŋǍ��@���ł�",
				MB_OK
			);
		}
	}
	// ���������SGB�ȉ���PC�ł�90���ȏ�̎g�p���A����ȊO��1.5GB�ȏ�̋󂫂��Ȃ���ԂɂȂ������A�v���Z�X�ی�̂��ߋ����I�ɃQ�[�����I��������
	if ((static_cast<double>(memory.total.physical_memory) / 1024 < 4096.0f && static_cast<double>(memory.physical_memory_usage < 90))
		|| (static_cast<double>(memory.total.physical_memory) / 1024 > 4096.0f && static_cast<double>(memory.available.physical_memory) / 1024 < 1536.0f)
		) throw std::runtime_error(
		"���̃v���Z�X����уI�y���[�e�B���O�V�X�e���̕ی�̂��߁A�����I�ɏI������܂����B\n"
		"�{�v���Z�X�ȊO�Ƀ������𑽂�����Ă���v���Z�X������ꍇ�A���Y�v���Z�X����~�܂��̓������g�p�����������Ă���ēx���y���݂��������B"
	);
}
