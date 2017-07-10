#pragma once
#include "ApplicationDirectory.h"
void launch(ApplicationDirectory directory);
int title(ApplicationDirectory directory, const int string_size);
#include "TaxDataRead.h"
#include "Graphic.h"
CaughtTax choose_play_criminal_investigation_case(Key &key, String str, ApplicationDirectory directory);

struct CriminalInvestigationCase {
	CriminalInvestigationCase(const std::vector<CaughtTax> list);
	std::vector<CaughtTax> income_tax; // ������
	std::vector<CaughtTax> corporate_tax; // �@�l��
	std::vector<CaughtTax> inheritance_tax; // ������
};
