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

	while (true) {
		pin->Write(GpioPinValue::High);  // Drives the specified value onto the general purpose I/O (GPIO) pin according to the current drive mode for the pin.
		Sleep(500);                      // Suspends the execution of the current thread until the time-out interval elapses.
		pin->Write(GpioPinValue::Low);
		Sleep(500);
	}
}
