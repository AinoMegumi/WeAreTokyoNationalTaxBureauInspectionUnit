#pragma once
#include "ApplicationDirectory.h"
#include <string>
#include <vector>

struct CaughtTax {
	CaughtTax(const std::string target_name, const std::string tax_name, const unsigned int payable_tax,
		const unsigned int already_paid, bool intended, const int evidence, const std::string map_path,
		const std::string item_path)
		:target_name(target_name), tax_name(tax_name), payable_tax(payable_tax), already_paid(already_paid),
		intended(intended), evidence(evidence), map_path(map_path), item_path(item_path) {}
	std::string target_name; // �E�Ŏ҂̖��O
	std::string tax_name; // �ŋ��̖��O
	std::string who_are_you;
	unsigned int payable_tax, // �{�ł̍��v
		already_paid; // �x�����ς�
	bool intended; // �Ӑ}�������̂�
	int evidence; // �E�ł̏؋��̐�
	std::string map_path;
	std::string item_path;
};

std::vector<CaughtTax> read_xml_criminal_investigation_case(ApplicationDirectory directory);
