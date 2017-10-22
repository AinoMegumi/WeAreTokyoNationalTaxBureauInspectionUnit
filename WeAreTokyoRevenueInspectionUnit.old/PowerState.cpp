#include "PowerState.h"
#include <stdexcept>

PowerState::PowerState() {
	if (0 == GetSystemPowerStatus(this->power)) throw std::runtime_error("�d����Ԃ̎擾�Ɏ��s���܂����B");
}

void PowerState::Update() {
	if (0 == GetSystemPowerStatus(this->power)) throw std::runtime_error("�d����Ԃ̎擾�Ɏ��s���܂����B");
}

bool PowerState::ACPowerIsOnline() {
	return this->GetACPowerState() == ACPowerState::online;
}

bool PowerState::BatteryExists() {
	return this->GetBatteryState() == BatteryState::not_exist;
}

ACPowerState PowerState::GetACPowerState() {
	switch (this->power->ACLineStatus) {
		case AC_LINE_OFFLINE: return ACPowerState::offline;
		case AC_LINE_ONLINE: return ACPowerState::online;
		case AC_LINE_BACKUP_POWER: return ACPowerState::backup;
		default: throw std::runtime_error("AC�d���̏�Ԃ��s���ł�");
	}
}

BatteryState PowerState::GetBatteryState() {
	switch (this->power->BatteryFlag) {
		case BATTERY_FLAG_HIGH: return BatteryState::high;
		case BATTERY_FLAG_LOW: return BatteryState::low;
		case BATTERY_FLAG_CRITICAL: return BatteryState::critical;
		case BATTERY_FLAG_CHARGING: return BatteryState::charging;
		case BATTERY_FLAG_NO_BATTERY: return BatteryState::not_exist;
		default: throw std::runtime_error("�o�b�e���[�̏�Ԃ��s���ł�");
	}
}

int PowerState::GetBatteryRemain() {
	if (!this->BatteryExists()) return -1;
	return this->power->BatteryLifePercent;
}
