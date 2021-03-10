/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_spi.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : SPI驱动文件。
其他	   : 无
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/17 左忠凯创建
***************************************************************/
#include "bsp_spi.h"

#define EXAMPLE_ECSPI_MASTER_BASEADDR 	ECSPI3
#define EXAMPLE_ECSPI_DEALY_COUNT 		1000000U
#define ECSPI_MASTER_CLK_FREQ 			(60000000) /* SPI时钟源为60M */
#define TRANSFER_SIZE 					64U         /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 				500000U /*! Transfer baudrate - 500k */


static inline void ECSPI_SoftwareReset(ECSPI_Type *base)
{
    /* Disables the block and resets the internal logic with the exception of the ECSPI control register */
    base->CONREG &= ~ECSPI_CONREG_EN_MASK;
    /* Software reset can not reset the control register, so clear the control register manually */
    base->CONREG = 0x0U;
}

void ECSPI_SetChannelConfig(ECSPI_Type *base, ecspi_channel_source_t channel, const ecspi_channel_config_t *config)
{
    switch (channel)
    {
        case kECSPI_Channel0:
            base->CONREG |= ECSPI_CONREG_CHANNEL_MODE(config->channelMode);
            base->CONFIGREG |=
                (ECSPI_CONFIGREG_SCLK_CTL(config->clockInactiveState) |
                 ECSPI_CONFIGREG_DATA_CTL(config->dataLineInactiveState) |
                 ECSPI_CONFIGREG_SS_POL(config->chipSlectActiveState) | ECSPI_CONFIGREG_SS_CTL(config->waveForm) |
                 ECSPI_CONFIGREG_SCLK_POL(config->polarity) | ECSPI_CONFIGREG_SCLK_PHA(config->phase));
            break;

        case kECSPI_Channel1:
            base->CONREG |= ECSPI_CONREG_CHANNEL_MODE(config->channelMode) << 1;
            base->CONFIGREG |=
                ((ECSPI_CONFIGREG_SCLK_CTL(config->clockInactiveState) << 1) |
                 (ECSPI_CONFIGREG_DATA_CTL(config->dataLineInactiveState) << 1) |
                 (ECSPI_CONFIGREG_SS_POL(config->chipSlectActiveState) << 1) |
                 (ECSPI_CONFIGREG_SS_CTL(config->waveForm) << 1) | (ECSPI_CONFIGREG_SCLK_POL(config->polarity) << 1) |
                 (ECSPI_CONFIGREG_SCLK_PHA(config->phase) << 1));
            break;

        case kECSPI_Channel2:
            base->CONREG |= ECSPI_CONREG_CHANNEL_MODE(config->channelMode) << 2;
            base->CONFIGREG |=
                ((ECSPI_CONFIGREG_SCLK_CTL(config->clockInactiveState) << 2) |
                 (ECSPI_CONFIGREG_DATA_CTL(config->dataLineInactiveState) << 2) |
                 (ECSPI_CONFIGREG_SS_POL(config->chipSlectActiveState) << 2) |
                 (ECSPI_CONFIGREG_SS_CTL(config->waveForm) << 2) | (ECSPI_CONFIGREG_SCLK_POL(config->polarity) << 2) |
                 (ECSPI_CONFIGREG_SCLK_PHA(config->phase) << 2));
            break;

        case kECSPI_Channel3:
            base->CONREG |= ECSPI_CONREG_CHANNEL_MODE(config->channelMode) << 3;
            base->CONFIGREG |=
                ((ECSPI_CONFIGREG_SCLK_CTL(config->clockInactiveState) << 3) |
                 (ECSPI_CONFIGREG_DATA_CTL(config->dataLineInactiveState) << 3) |
                 (ECSPI_CONFIGREG_SS_POL(config->chipSlectActiveState) << 3) |
                 (ECSPI_CONFIGREG_SS_CTL(config->waveForm) << 3) | (ECSPI_CONFIGREG_SCLK_POL(config->polarity) << 3) |
                 (ECSPI_CONFIGREG_SCLK_PHA(config->phase) << 3));
            break;

        default:
            break;
    }
}

void ECSPI_SetBaudRate(ECSPI_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz)
{

    uint8_t bestPreDividerValue = 0U, preDividerValue = 0U;
    uint8_t bestPostDividerValue = 0U, postDividerValue = 0U;
    uint32_t realBaudrate = 0U;
    uint32_t diff = 0xFFFFFFFFU;
    uint32_t min_diff = 0xFFFFFFFFU;

    for (preDividerValue = 0; (preDividerValue < 16) && diff; preDividerValue++)
    {
        for (postDividerValue = 0; (postDividerValue < 16) && diff; postDividerValue++)
        {
            realBaudrate = (srcClock_Hz / (preDividerValue + 1)) >> postDividerValue;
            if (realBaudrate > baudRate_Bps)
            {
                diff = realBaudrate - baudRate_Bps;
                if (diff < min_diff)
                {
                    min_diff = diff;
                    bestPreDividerValue = preDividerValue;
                    bestPostDividerValue = postDividerValue;
                }
            }
            else
            {
                diff = baudRate_Bps - realBaudrate;
                if (diff < min_diff)
                {
                    min_diff = diff;
                    bestPreDividerValue = preDividerValue;
                    bestPostDividerValue = postDividerValue;
                }
            }
        }
    }

    base->CONREG |= ECSPI_CONREG_PRE_DIVIDER(bestPreDividerValue) | ECSPI_CONREG_POST_DIVIDER(bestPostDividerValue);
}


void ECSPI_MasterInit(ECSPI_Type *base, const ecspi_master_config_t *config, uint32_t srcClock_Hz)
{
    /* Reset control register to default value */
    ECSPI_SoftwareReset(base);
    /* Config CONREG register */
    base->CONREG = ECSPI_CONREG_BURST_LENGTH(config->burstLength - 1) | ECSPI_CONREG_SMC(1) | ECSPI_CONREG_EN(1);
    /* Config CONFIGREG register */
    ECSPI_SetChannelConfig(base, config->channel, &config->channelConfig);
    /* Config DMAREG register */
    base->DMAREG |=
        ECSPI_DMAREG_TX_THRESHOLD(config->txFifoThreshold) | ECSPI_DMAREG_RX_THRESHOLD(config->rxFifoThreshold);
    /* Config PERIODREG register */
    base->PERIODREG |= ECSPI_PERIODREG_CSRC(config->samplePeriodClock) |
                       ECSPI_PERIODREG_SAMPLE_PERIOD(config->samplePeriod) |
                       ECSPI_PERIODREG_CSD_CTL(config->chipSelectDelay);
    /* Set baud rate */
    ECSPI_SetBaudRate(base, config->baudRate_Bps, srcClock_Hz);
}

static void ECSPI_GetDefaultChannelConfig(ecspi_channel_config_t *config)
{
    config->channelMode = kECSPI_Slave;                              /*!< ECSPI peripheral operates in slave mode.*/
    config->clockInactiveState = kECSPI_ClockInactiveStateLow;       /*!< Clock line (SCLK) inactive state */
    config->dataLineInactiveState = kECSPI_DataLineInactiveStateLow; /*!< Data line (MOSI&MISO) inactive state */
    config->chipSlectActiveState = kECSPI_ChipSelectActiveStateLow;  /*!< Chip select(SS) line active state */
    config->waveForm = kECSPI_WaveFormSingle;                        /*!< ECSPI SS wave form */
    config->polarity = kECSPI_PolarityActiveHigh;                    /*!< Clock polarity */
    config->phase = kECSPI_ClockPhaseFirstEdge;                      /*!< clock phase */
}

void ECSPI_MasterGetDefaultConfig(ecspi_master_config_t *config)
{
    config->channel = kECSPI_Channel0;
    config->burstLength = 8;
    config->samplePeriodClock = kECSPI_spiClock;
    config->baudRate_Bps = 500000;
    config->chipSelectDelay = 0;
    config->samplePeriod = 0;
    config->txFifoThreshold = 1;
    config->rxFifoThreshold = 0;
    /* Default configuration of channel */
    ECSPI_GetDefaultChannelConfig(&config->channelConfig);
    /*!< ECSPI peripheral operates in slave mode.*/
    config->channelConfig.channelMode = kECSPI_Master;
}

/*
 * @description	: 初始化SPI
 * @param		: 无
 * @return 		: 无
 */
void spi3_init(void)
{
	/* 1、IO初始化 
 	 * ECSPI3_SSO 	-> UART2_TXD
 	 * ECSPI3_SCLK 	-> UART2_RXD
 	 * ECSPI3_MISO 	-> UART2_RTS
 	 * ECSPI3_MOSI	-> UART2_CTS
 	 */
	IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_ECSPI3_SS0, 0);
	IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
	IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
	IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);

	/* 2、配置SPI SS0 IO属性	
	 *bit 16: 1 HYS打开
	 *bit [15:14]: 10 默认100K上拉
	 *bit [13]: 1 pull功能
	 *bit [12]: 1 pull/keeper使能 
	 *bit [11]: 0 关闭开路输出
 	 *bit [7:6]: 10 速度100Mhz
 	 *bit [5:3]: 110 驱动能力为R0/6
	 *bit [0]: 1 高转换率
 	 */
	IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL,0x1B0B1);

	/* 配置SPI   SCLK MISO MOSI IO属性	
	 *bit 16: 0 HYS关闭
	 *bit [15:14]: 00 默认100K下拉
	 *bit [13]: 0 keeper功能
	 *bit [12]: 1 pull/keeper使能 
	 *bit [11]: 0 关闭开路输出
 	 *bit [7:6]: 10 速度100Mhz
 	 *bit [5:3]: 110 驱动能力为R0/6
	 *bit [0]: 1 高转换率
 	 */
	IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA,0x10B1);
	IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL,0x10B1);
	IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA,0x10B1);

	/* 3、配置SPI3 */
    ecspi_master_config_t masterConfig;
    ecspi_transfer_t masterXfer;

    /* Master config:
     * masterConfig.channel = kECSPI_Channel0;
     * masterConfig.burstLength = 8;
     * masterConfig.samplePeriodClock = kECSPI_spiClock;
     * masterConfig.baudRate_Bps = TRANSFER_BAUDRATE;
     * masterConfig.chipSelectDelay = 0;
     * masterConfig.samplePeriod = 0;
     * masterConfig.txFifoThreshold = 1;
     * masterConfig.rxFifoThreshold = 0;
     */
    ECSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = TRANSFER_BAUDRATE;


}
