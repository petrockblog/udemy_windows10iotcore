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
		const int UNIT_MS = 250;
		void SignalDot(Windows::Devices::Gpio::GpioPin^ pin);
		void SignalDash(Windows::Devices::Gpio::GpioPin^ pin);
		void SignalLetterSeparation(Windows::Devices::Gpio::GpioPin^ pin);
		void SignalWordSeparation(Windows::Devices::Gpio::GpioPin^ pin);

    };
}
