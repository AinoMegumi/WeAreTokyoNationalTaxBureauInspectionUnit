#pragma once
#include "ApplicationDirectory.h"
#include <array>
#include <utility>
constexpr char* config_filepath = ".\\System\\config.ini";

class Key {
private:
	ApplicationDirectory directory;
	std::array<char, 256> buffer;
	int up_key, down_key, left_key, right_key, choose_key, cancel_key, screen_shot_key;
	void load_config(); // �R���t�B�O�̓ǂݍ���(config.ini�̃��[�h)
public:
	Key() = default;
	Key(ApplicationDirectory directory);
	void buffer_format(); // �z�񏉊���
	void update(); // �L�[��Ԃ̍Ď擾
	bool up();
	bool down();
	bool left();
	bool right();
	bool choose();
	bool cancel();
	bool screen_shot();
};
