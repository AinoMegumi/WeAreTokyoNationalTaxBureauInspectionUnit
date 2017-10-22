#pragma once
class FPSManager {
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float current_fps;             //fps
	int N;//���ς����T���v����
	float FPS;	//�ݒ肵��FPS
public:
	constexpr FPSManager();
	constexpr FPSManager(const float fps);
	bool Update();
	constexpr float GetFPS() const;
	void Wait();
};
