#include "pch.h"
#include "StartupTask.h"

using namespace HelloIoTWorld;

using namespace Platform;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::Devices::Gpio;
using namespace Windows::Foundation;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	bgTaskDeferral = taskInstance->GetDeferral();

	auto gpioController = GpioController::GetDefault();   // Gets the default general-purpose I/O (GPIO) controller for the system.
	ledPin = gpioController->OpenPin(LEDPIN);           // Opens a connection to the specified general-purpose I/O (GPIO) pin in exclusive mode.
	buttonPin = gpioController->OpenPin(BUTTONPIN);
	
	const bool allPinsWereOpened = (ledPin != nullptr) && (buttonPin != nullptr);
	if (allPinsWereOpened) {
		ledPin->SetDriveMode(GpioPinDriveMode::Output);          // Sets the drive mode of the general-purpose I/O (GPIO) pin. The drive mode specifies whether the pin is configured as an input or an output, and determines how values are driven onto the pin.
		ledPin->Write(GpioPinValue::High);  // Drives the specified value onto the general purpose I/O (GPIO) pin according to the current drive mode for the pin.

		buttonPin->SetDriveMode(GpioPinDriveMode::InputPullUp);
		const Windows::Foundation::TimeSpan debounceTime = { BUTTONDEBOUNCETIME };  
		buttonPin->DebounceTimeout = debounceTime;
		buttonPin->ValueChanged += ref new TypedEventHandler<GpioPin^, GpioPinValueChangedEventArgs^>(this, &StartupTask::ButtonPin_ValueChanged);
	}
	else
	{
		bgTaskDeferral->Complete();
	}
}

void StartupTask::ButtonPin_ValueChanged(Windows::Devices::Gpio::GpioPin ^sender, Windows::Devices::Gpio::GpioPinValueChangedEventArgs ^args) {
	if (args->Edge == GpioPinEdge::FallingEdge) {
		const GpioPinValue currentValue = ledPin->Read();
		if (currentValue == GpioPinValue::High) {
			ledPin->Write(GpioPinValue::Low);
		}
		else
		{
			ledPin->Write(GpioPinValue::High);
		}
	}
}
