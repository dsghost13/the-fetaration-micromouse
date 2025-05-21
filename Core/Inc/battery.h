#pragma once

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern ADC_HandleTypeDef hadc1;

	class Battery {
	public:
		float get_vdda() {
			uint16_t vrefint_raw = 0;

			ADC_ChannelConfTypeDef sConfig = {0};
			sConfig.Channel = ADC_CHANNEL_VREFINT;
			sConfig.Rank = ADC_REGULAR_RANK_1;
			sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);

			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			vrefint_raw = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);

			return (VREFINT * MAX_ADC) / (float)vrefint_raw;
		}

		float voltage() {
			uint16_t vbatt_raw = 0;
			float vdda = get_vdda();
			float vbatt_adc_voltage = 0.0f;

			ADC_ChannelConfTypeDef sConfig = {0};
			sConfig.Channel = ADC_CHANNEL_1;
			sConfig.Rank = ADC_REGULAR_RANK_1;
			sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);

			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			vbatt_raw = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);

			vbatt_adc_voltage = ((float)vbatt_raw / MAX_ADC) * vdda;
			return vbatt_adc_voltage;
		}

	};

#ifdef __cplusplus
}
#endif
