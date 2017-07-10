#include "FPSManager.h"
#include "Convert.h"
#include "DxLib.h"
#include "Ini.h"
#include <cmath>

constexpr FPSManager::FPSManager() : FPSManager(60.0f) {}

constexpr FPSManager::FPSManager(const float fps)
	: mStartTime(), mCount(), current_fps(), N(static_cast<int>(fps)), FPS(fps) {}

bool FPSManager::Update() {
	if (this->mCount == 0) { //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (this->mCount == this->N) { //60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		this->current_fps = 1000.f / ((t - this->mStartTime) / (float)this->N);
		this->mCount = 0;
		this->mStartTime = t;
	}
	this->mCount++;
	return true;
}

void FPSManager::Wait() {
	int tookTime = GetNowCount() - this->mStartTime;	//������������
	int waitTime = static_cast<int>((float)this->mCount * 1000.0f / this->FPS - (float)tookTime);	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}

constexpr float FPSManager::GetFPS() const {
	return this->current_fps;
}

/*
bool FPSManager::Update() {
	if (this->mCount == 0) { //1�t���[���ڂȂ玞�����L��
		this->mStartTime = std::chrono::steady_clock::now();
	}
	if (this->mCount == this->N) { //60�t���[���ڂȂ畽�ς��v�Z����
		auto t = std::chrono::steady_clock::now();
		this->current_fps = 1000.f / ((t - this->mStartTime) / this->N).count();
		this->mCount = 0;
		this->mStartTime = t;
	}
	this->mCount++;
	return true;
}

void FPSManager::Wait() {
	auto tookTime = std::chrono::steady_clock::now() - this->mStartTime;	//������������
	int waitTime = std::chrono::milliseconds(this->mCount * 1000 / this->FPS) - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		std::this_thread::sleep_until(waitTime);	//�ҋ@
	}
}
*/
