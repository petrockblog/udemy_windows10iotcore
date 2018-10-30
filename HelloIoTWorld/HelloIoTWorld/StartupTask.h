#pragma once

#include "pch.h"

namespace HelloIoTWorld
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class StartupTask sealed : public Windows::ApplicationModel::Background::IBackgroundTask
    {
    public:
        virtual void Run(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance);

	private:
		const int LEDPIN = 18;
		const int BUTTONPIN = 17;
		const long BUTTONDEBOUNCETIME = 20 * 10000LL;  // equivalent to 20 ms

		Platform::Agile<Windows::ApplicationModel::Background::BackgroundTaskDeferral> bgTaskDeferral;
		Windows::Devices::Gpio::GpioPin ^ledPin;
		Windows::Devices::Gpio::GpioPin ^buttonPin;

		void ButtonPin_ValueChanged(Windows::Devices::Gpio::GpioPin ^sender, Windows::Devices::Gpio::GpioPinValueChangedEventArgs ^args);

    };
}
