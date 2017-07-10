#include <random>
#include <algorithm>

static auto rand_engine() {
	std::random_device rnd;
	std::vector<std::uint_least32_t> v(10);// �������p�x�N�^
	std::generate(v.begin(), v.end(), std::ref(rnd));// �x�N�^�̏�����
	std::seed_seq seed(v.begin(), v.end());
	return std::mt19937(seed);// �����G���W��
}

std::mt19937& make_mt() {
	static std::mt19937 mt = rand_engine();
	return mt;
}
