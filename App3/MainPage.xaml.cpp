//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <pplawait.h>
#include <ppltasks.h>
using namespace concurrency;
using namespace Windows::Devices::Enumeration;

using namespace App3;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;
using namespace Windows::Devices::Gpio;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	isRequestInProgress = false;

	auto gpioController = GpioController::GetDefault();   // Gets the default general-purpose I/O (GPIO) controller for the system.
	if (gpioController != nullptr) {
		ledPin = gpioController->OpenPin(LEDPIN);
		ledPin->SetDriveMode(GpioPinDriveMode::Output);          // Sets the drive mode of the general-purpose I/O (GPIO) pin. The drive mode specifies whether the pin is configured as an input or an output, and determines how values are driven onto the pin.
		ledPin->Write(GpioPinValue::High);  // Drives the specified value onto the general purpose I/O (GPIO) pin according to the current drive mode for the pin.

		buttonPin = gpioController->OpenPin(BUTTONPIN);
		buttonPin->SetDriveMode(GpioPinDriveMode::InputPullUp);
		const Windows::Foundation::TimeSpan debounceTime = { BUTTONDEBOUNCETIME };
		buttonPin->DebounceTimeout = debounceTime;
		buttonPin->ValueChanged += ref new TypedEventHandler<GpioPin^, GpioPinValueChangedEventArgs^>(this, &App3::MainPage::ButtonPin_ValueChanged);
	}

	FetchNewQuotation();
}


void App3::MainPage::thebutton_click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FetchNewQuotation();
}

void App3::MainPage::ButtonPin_ValueChanged(Windows::Devices::Gpio::GpioPin ^sender, Windows::Devices::Gpio::GpioPinValueChangedEventArgs ^args) {
	FetchNewQuotation();
}

void App3::MainPage::FetchNewQuotation() {
	if (!isRequestInProgress) {
		OutputDebugStringA("FetchNewQuotation\n");

		isRequestInProgress = true;
		if (ledPin != nullptr) {
			ledPin->Write(GpioPinValue::Low);
		}

		auto uri = ref new Uri("https://favqs.com/api/qotd");
		httpClient = ref new HttpClient();

		IAsyncOperationWithProgress<Platform::String^, Windows::Web::Http::HttpProgress>^ httpOp = httpClient->GetStringAsync(uri);

		auto httpTask = create_task(httpOp);
		httpTask.then([this](Platform::String^ responseString) {
			OutputDebugStringA("FetchNewQuotation - httpTask\n");

			JsonObject^ httpResponse = ref new JsonObject();
			if (JsonObject::TryParse(responseString, &httpResponse)) {
				auto map = httpResponse->GetView();
				auto quote = map->Lookup("quote")->GetObject();

				String^ quoteText = quote->Lookup("body")->Stringify();
				String^ authorText = quote->Lookup("author")->Stringify();

				std::wstring newAuthor(authorText->Data());
				newAuthor = newAuthor.substr(1, newAuthor.length() - 2);
				authorText = ref new String(newAuthor.c_str());

				Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
					Windows::UI::Core::CoreDispatcherPriority::High,
					ref new Windows::UI::Core::DispatchedHandler([this, quoteText, authorText]()
				{
					OutputDebugStringA("FetchNewQuotation - UI Update\n");
					App3::MainPage::TheQuotation->Text = quoteText;
					App3::MainPage::TheAuthor->Text = authorText;
					OutputDebugStringA("FetchNewQuotation - UI Update End\n");
				}));
			}
			isRequestInProgress = false;

			if (ledPin != nullptr) {
				ledPin->Write(GpioPinValue::High);
			}

			OutputDebugStringA("FetchNewQuotation - httpTask End\n");
		});
	}
}