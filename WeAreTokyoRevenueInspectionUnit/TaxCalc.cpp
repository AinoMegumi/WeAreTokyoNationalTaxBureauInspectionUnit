#include <chrono>
using day = std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type>;


class AdditionalTaxCalc {
private:
	/*long over_day, back_tax; // ���ߓ����ƒǒ��ې�
	bool within_time_limit; // �������\�����ǂ���*/
	day limit, paid;
	long back_tax;
	long delinquent_tax(); // ���ؐ�
	// ���Z��
	long under_reporting_additional_tax(); //�@�ߏ��\�����Z��
	long undeclared_additional_tax(); // ���\�����Z��
	long non_payment_additional_tax(); // �s�[�t���Z��
	long heavy_additional_tax(); // �d���Z��
public:
	AdditionalTaxCalc(day limit, day paid, long back_tax);
	long calc();
};

AdditionalTaxCalc::AdditionalTaxCalc(day limit, day paid, long back_tax)
	: limit(limit), paid(paid), back_tax(back_tax) {}

long AdditionalTaxCalc::under_reporting_additional_tax() {

}
