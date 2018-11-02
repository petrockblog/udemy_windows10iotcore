#include "pch.h"
#include "StartupTask.h"

using namespace HelloIoTWorld;

using namespace Platform;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::Devices::Gpio;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	auto gpioController = GpioController::GetDefault();   // Gets the default general-purpose I/O (GPIO) controller for the system.
	GpioPin ^pin = gpioController->OpenPin(18);           // Opens a connection to the specified general-purpose I/O (GPIO) pin in exclusive mode.
	pin->SetDriveMode(GpioPinDriveMode::Output);          // Sets the drive mode of the general-purpose I/O (GPIO) pin. The drive mode specifies whether the pin is configured as an input or an output, and determines how values are driven onto the pin.

	const int UNIT_MS = 250;
	while (true) {
		SignalDot(pin);
		SignalDot(pin);
		SignalDot(pin);

		SignalLetterSeparation(pin);

		SignalDash(pin);
		SignalDash(pin);
		SignalDash(pin);

		SignalLetterSeparation(pin);

		SignalDot(pin);
		SignalDot(pin);
		SignalDot(pin);

		SignalWordSeparation(pin);
	}
}

void StartupTask::SignalDot(GpioPin^ pin) {
	pin->Write(GpioPinValue::High);
	Sleep(UNIT_MS);
	pin->Write(GpioPinValue::Low);
	Sleep(UNIT_MS);
}

void StartupTask::SignalDash(GpioPin^ pin) {
	pin->Write(GpioPinValue::High);
	Sleep(3 * UNIT_MS);
	pin->Write(GpioPinValue::Low);
	Sleep(UNIT_MS);
}

void StartupTask::SignalLetterSeparation(GpioPin^ pin) {
	Sleep(2* UNIT_MS);
}

void StartupTask::SignalWordSeparation(GpioPin^ pin) {
	Sleep(6 * UNIT_MS);
}
