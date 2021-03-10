#ifndef _BSP_BMP280_H
#define _BSP_BMP280_H
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_spi.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : SPI驱动头文件。
其他	   : 无
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/17 左忠凯创建
***************************************************************/
#include "imx6ul.h"

#define ECSPI_DUMMYDATA (0xFFFFFFFFU)

typedef enum _ecspi_channel_source
{
    kECSPI_Channel0 = 0x0U, /*!< Channel 0 is selectd */
    kECSPI_Channel1,        /*!< Channel 1 is selectd */
    kECSPI_Channel2,        /*!< Channel 2 is selectd */
    kECSPI_Channel3,        /*!< Channel 3 is selectd */
} ecspi_channel_source_t;
	
typedef enum _ecspi_master_slave_mode
{
    kECSPI_Slave = 0U, /*!< ECSPI peripheral operates in slave mode.*/
    kECSPI_Master,     /*!< ECSPI peripheral operates in master mode.*/
} ecspi_master_slave_mode_t;

/*! @brief ECSPI data line inactive state configuration. */
typedef enum _ecspi_data_line_inactive_state_t
{
    kECSPI_DataLineInactiveStateHigh = 0x0U, /*!< The data line inactive state stays high. */
    kECSPI_DataLineInactiveStateLow,         /*!< The data line inactive state stays low. */
} ecspi_data_line_inactive_state_t;

/*! @brief ECSPI clock inactive state configuration. */
typedef enum _ecspi_clock_inactive_state_t
{
    kECSPI_ClockInactiveStateLow = 0x0U, /*!< The SCLK inactive state stays low. */
    kECSPI_ClockInactiveStateHigh,       /*!< The SCLK inactive state stays high. */
} ecspi_clock_inactive_state_t;

/*! @brief ECSPI active state configuration.*/
typedef enum _ecspi_chip_select_active_state_t
{
    kECSPI_ChipSelectActiveStateLow = 0x0U, /*!< The SS signal line active stays low. */
    kECSPI_ChipSelectActiveStateHigh,       /*!< The SS signal line active stays high. */
} ecspi_chip_select_active_state_t;

/*! @brief ECSPI wave form configuration.*/
typedef enum _ecspi_wave_form_t
{
    kECSPI_WaveFormSingle = 0x0U, /*!< The wave form for signal burst */
    kECSPI_WaveFormMultiple,      /*!< The wave form for multiple burst */
} ecspi_wave_form_t;

typedef enum _ecspi_clock_polarity
{
    kECSPI_PolarityActiveHigh = 0x0U, /*!< Active-high ECSPI polarity high (idles low). */
    kECSPI_PolarityActiveLow,         /*!< Active-low ECSPI polarity low (idles high). */
} ecspi_clock_polarity_t;

/*! @brief ECSPI clock phase configuration. */
typedef enum _ecspi_clock_phase
{
    kECSPI_ClockPhaseFirstEdge =
        0x0U,                    /*!< First edge on SPSCK occurs at the middle of the first cycle of a data transfer. */
    kECSPI_ClockPhaseSecondEdge, /*!< First edge on SPSCK occurs at the start of the first cycle of a data transfer. */
} ecspi_clock_phase_t;
	

typedef enum _ecspi_sample_period_clock_source
{
    kECSPI_spiClock = 0x0U, /*!< The sample period clock source is SCLK. */
    kECSPI_lowFreqClock,    /*!< The sample seriod clock source is low_frequency reference clock(32.768 kHz). */
} ecspi_sample_period_clock_source_t;

enum _ecspi_flags
{
    kECSPI_TxfifoEmptyFlag = ECSPI_STATREG_TE_MASK,       /*!< Transmit FIFO buffer empty flag */
    kECSPI_TxFifoDataRequstFlag = ECSPI_STATREG_TDR_MASK, /*!< Transmit FIFO data requst flag */
    kECSPI_TxFifoFullFlag = ECSPI_STATREG_TF_MASK,        /*!< Transmit FIFO full flag */
    kECSPI_RxFifoReadyFlag = ECSPI_STATREG_RR_MASK,       /*!< Receiver FIFO ready flag */
    kECSPI_RxFifoDataRequstFlag = ECSPI_STATREG_RDR_MASK, /*!< Receiver FIFO data requst flag */
    kECSPI_RxFifoFullFlag = ECSPI_STATREG_RF_MASK,        /*!< Receiver FIFO full flag */
    kECSPI_RxFifoOverFlowFlag = ECSPI_STATREG_RO_MASK,    /*!< Receiver FIFO buffer overflow flag */
    kECSPI_TransferCompleteFlag = ECSPI_STATREG_TC_MASK,  /*!< Transfer complete flag */
};

enum _ecspi_status
{
    kStatus_ECSPI_Busy = MAKE_STATUS(kStatusGroup_ECSPI, 0),             /*!< ECSPI bus is busy */
    kStatus_ECSPI_Idle = MAKE_STATUS(kStatusGroup_ECSPI, 1),             /*!< ECSPI is idle */
    kStatus_ECSPI_Error = MAKE_STATUS(kStatusGroup_ECSPI, 2),            /*!< ECSPI  error */
    kStatus_ECSPI_HardwareOverFlow = MAKE_STATUS(kStatusGroup_ECSPI, 3), /*!< ECSPI  hardware overflow */
};

typedef struct _ecspi_transfer
{
    uint32_t *txData;               /*!< Send buffer */
    uint32_t *rxData;               /*!< Receive buffer */
    size_t dataSize;                /*!< Transfer bytes */
    ecspi_channel_source_t channel; /*!< ECSPI channel select */
} ecspi_transfer_t;	

typedef struct _ecspi_channel_config
{
    ecspi_master_slave_mode_t channelMode;                  /*!< Channel mode */
    ecspi_clock_inactive_state_t clockInactiveState;        /*!< Clock line (SCLK) inactive state */
    ecspi_data_line_inactive_state_t dataLineInactiveState; /*!< Data line (MOSI&MISO) inactive state */
    ecspi_chip_select_active_state_t chipSlectActiveState;  /*!< Chip select(SS) line active state */
    ecspi_wave_form_t waveForm;                             /*!< Wave form */
    ecspi_clock_polarity_t polarity;                        /*!< Clock polarity */
    ecspi_clock_phase_t phase;                              /*!< Clock phase */
} ecspi_channel_config_t;

typedef struct _ecspi_master_config
{
    ecspi_channel_source_t channel;                       /*!< Channel number */
    ecspi_channel_config_t channelConfig;                 /*!< Channel configuration */
    ecspi_sample_period_clock_source_t samplePeriodClock; /*!< Sample period clock source */

    uint8_t burstLength;     /*!< Burst length */
    uint8_t chipSelectDelay; /*!< SS delay time */
    uint16_t samplePeriod;   /*!< Sample period */
    uint8_t txFifoThreshold; /*!< TX Threshold */
    uint8_t rxFifoThreshold; /*!< RX Threshold */
    uint32_t baudRate_Bps;   /*!< ECSPI baud rate for master mode */
} ecspi_master_config_t;

/* 函数声明 */
void spi3_init(void)
;
unsigned char spi3_readwrite_byte(unsigned char TxData);
unsigned char bmp280_Read_Reg(unsigned char reg);

#endif


