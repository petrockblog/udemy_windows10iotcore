//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace App3
{
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		const int BUTTONPIN = 17;
		const int LEDPIN = 18;
		const long BUTTONDEBOUNCETIME = 20 * 10000LL; // equivalent to 20 ms

		Windows::Devices::Gpio::GpioPin ^ledPin;
		Windows::Devices::Gpio::GpioPin ^buttonPin;
		Windows::Web::Http::HttpClient^ httpClient;
		bool isRequestInProgress;

		void thebutton_click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonPin_ValueChanged(Windows::Devices::Gpio::GpioPin ^sender, Windows::Devices::Gpio::GpioPinValueChangedEventArgs ^args);
		void FetchNewQuotation();
	};
}
