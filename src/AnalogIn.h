/*
 * AnalogIn.h
 *
 *  Created on: 6 Sep 2018
 *      Author: David
 */

#ifndef SRC_HARDWARE_ANALOGIN_H_
#define SRC_HARDWARE_ANALOGIN_H_

#include <CoreIO.h>

typedef void (*AnalogInCallbackFunction)(CallbackParameter p, uint16_t reading) noexcept;

namespace AnalogIn
{
	// The number of bits that the ADCs return
#if SAMC21
	constexpr unsigned int AdcBits = 16;
#elif SAME5x
	constexpr unsigned int AdcBits = 16;
#endif

#ifdef RTOS
	// Initialise the analog input subsystem. Call this just once.
	// For the SAME5x we need 4 DMA channels. For the SAMC21 we need 1 DMA channel, or 2 if supporting the SDADC.
	void Init(DmaChannel dmaChan,
#if SAME5x
		DmaPriority txPriority,
#endif
		DmaPriority rxPriority) noexcept;

	// Shut down the analog system. making it safe to terminate the AnalogIn task
	void Exit() noexcept;

	// Enable analog input on a pin.
	// Readings will be taken and about every 'ticksPerCall' milliseconds the callback function will be called with the specified parameter and ADC reading.
	// Set ticksPerCall to 0 to get a callback on every reading.
	// Warning! there is nothing to stop you enabling a channel twice, in which case in the SAME51 configuration, it will be read twice in the sequence.
	bool EnableChannel(AdcInput adcin, AnalogInCallbackFunction fn, CallbackParameter param, uint32_t ticksPerCall, bool useAlternateAdc = false) noexcept;

	// Readings will be taken and about every 'ticksPerCall' milliseconds the callback function will be called with the specified parameter and ADC reading.
	// Set ticksPerCall to 0 to get a callback on every reading.
	bool SetCallback(AdcInput adcin, AnalogInCallbackFunction fn, CallbackParameter param, uint32_t ticksPerCall, bool useAlternateAdc = false) noexcept;

	// Return whether or not the channel is enabled
	bool IsChannelEnabled(AdcInput adcin, bool useAlternateAdc = false) noexcept;

	// Disable a previously-enabled channel
	void DisableChannel(AdcInput adcin, bool useAlternateAdc = false) noexcept;

	// Get the latest result from a channel. the channel must have been enabled first.
	uint16_t ReadChannel(AdcInput adcin) noexcept;

	// Get the number of conversions that were started
	void GetDebugInfo(uint32_t &convsStarted, uint32_t &convsCompleted, uint32_t &convTimeouts) noexcept;

#if SAME5x
	// Enable an on-chip MCU temperature sensor. We don't use this on the SAMC21 because that chip has a separate TSENS peripheral.
	bool EnableTemperatureSensor(unsigned int sensorNumber, AnalogInCallbackFunction fn, CallbackParameter param, uint32_t ticksPerCall, unsigned int adcnum) noexcept;
#endif

#if SAMC21
	void EnableTemperatureSensor(AnalogInCallbackFunction fn, CallbackParameter param, uint32_t ticksPerCall) noexcept;
#endif

	extern "C" [[noreturn]] void TaskLoop(void*) noexcept;

#else

	// Simple analog input functions, for projects that don't use RTOS e.g. bootloaders
# if SAMC21						// these are currently used only by SAMC21 projects
	void Init(Adc * device);
	void Disable(Adc * device);
	uint16_t ReadChannel(Adc * device, uint8_t channel);
# endif

#endif
}

#ifdef RTOS

// This function is for backwards compatibility with CoreNG
inline uint16_t AnalogInReadChannel(AdcInput adcin)
{
	return AnalogIn::ReadChannel(adcin);
}

// This function is for backwards compatibility with CoreNG
inline void AnalogInEnableChannel(AdcInput adcin, bool enable)
{
	if (enable)
	{
		if (!AnalogIn::IsChannelEnabled(adcin))
		{
			AnalogIn::EnableChannel(adcin, nullptr, CallbackParameter(), 1000);
		}
	}
	else
	{
		AnalogIn::DisableChannel(adcin);
	}
}

#endif

#endif /* SRC_HARDWARE_ANALOGIN_H_ */
